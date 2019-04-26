//=====================================================================
// OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
// Copyright 2018-2019 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of OpenGLCppWrapper.
//
// OpenGLCppWrapper is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// OpenGLCppWrapper is distributedin the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with OpenGLCppWrapper.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================
//
// This file is a derived work of https://github.com/glumpy/glumpy
//
// Copyright (c) 2009-2016 Nicolas P. Rougier. All rights reserved.
// Distributed under the (new) BSD License.
//=====================================================================

#ifndef OPENGLCPPWRAPPER_GLSHADER_HPP
#  define OPENGLCPPWRAPPER_GLSHADER_HPP

// *****************************************************************************
//! \file GLShaders.hpp holds the GLSL shader code, offers some macros
//! code for processing the code (like code inclusion) and allow to
//! compile code source.
// *****************************************************************************

#  include "IGLObject.hpp"
#  include <vector>
#  include <fstream>

namespace glwrap
{

// *****************************************************************************
//! \class GLShader GLShaders.hpp
//!
//! \brief holds the GLSL shader code, offers some macros
//! code for processing the code (like code inclusion) and allow to
//! compile code source.
// *****************************************************************************
class GLShader: public IGLObject<GLuint>
{
public:

  //----------------------------------------------------------------------------
  //! \brief Constructor. Do nothing.
  //----------------------------------------------------------------------------
  GLShader(std::string const& name, const GLenum target)
    : IGLObject(name)
  {
    m_target = target;
  }

  //----------------------------------------------------------------------------
  //! \brief Release GPU memory.
  //----------------------------------------------------------------------------
  virtual ~GLShader() override
  {
    destroy();
  }

  //----------------------------------------------------------------------------
  //! \brief Copy the shader code given as string into the class
  //! member variable. Note: the shader code compilation is delayed.
  //----------------------------------------------------------------------------
  bool fromString(std::string const& script)
  {
    throw_if_already_compiled();
    m_shader_code = script;
    DEBUG("From script '%s' %s: '%s'", cname(), type(), m_shader_code.c_str());
    return true;
  }

  //----------------------------------------------------------------------------
  //! \brief Read the whole shader code from the given ascii file.
  //! Note: the shader code compilation is delayed. The code is only
  //! store in the member variable.
  //! \param filename the path of the shader code.
  //! \return true if the whole file has been read, else return false.
  //! An error message is set and can be read through error().
  //----------------------------------------------------------------------------
  bool fromFile(std::string const& filename)
  {
    throw_if_already_compiled();
    bool loaded = load(filename, m_shader_code);
    DEBUG("From file '%s' %s: '%s'", cname(), type(), m_shader_code.c_str());
    if (false == loaded)
      {
        std::string msg = "Failed loading shader code '"
          + filename + "'";
        ERROR("%s", msg.c_str());
        m_error_msg += '\n' + msg;
      }
    return loaded;
  }

  //----------------------------------------------------------------------------
  //! \brief Check if the shader has been compiled with success by
  //! OpenGL.
  //----------------------------------------------------------------------------
  inline bool compiled() const
  {
    return m_compiled;
  }

  //----------------------------------------------------------------------------
  //! \return the shader code
  //----------------------------------------------------------------------------
  inline std::string const& code() const
  {
    return m_shader_code;
  }

  //----------------------------------------------------------------------------
  //! \brief Return the shader error message
  //----------------------------------------------------------------------------
  inline bool hasErrored() const
  {
    return !m_error_msg.empty();
  }

  //----------------------------------------------------------------------------
  //! \brief Return the shader error message. member variable is then
  //! cleared.
  //----------------------------------------------------------------------------
  inline std::string error()
  {
    std::string tmp(m_error_msg);
    m_error_msg.clear();
    return tmp;
  }

  //----------------------------------------------------------------------------
  //! \brief (Information) Attached by a prog. Pass 0 for detached
  //----------------------------------------------------------------------------
  inline void attachProg(GLenum prog)
  {
    m_attached = prog;
  }

  inline GLenum attached() const
  {
    return m_attached;
  }

private:

  //----------------------------------------------------------------------------
  //! \brief Remove the file name and store the path to this folder.
  //! \param fullPath
  //----------------------------------------------------------------------------
  static std::string getFilePath(const std::string& fullPath)
  {
    size_t found = fullPath.find_last_of("/\\");
    return fullPath.substr(0, found + 1);
  }

  //----------------------------------------------------------------------------
  //! \brief Increase the buffer size of the size of the full file.
  //! \param infile
  //! \param buffer
  //----------------------------------------------------------------------------
  static bool resize(std::ifstream& infile, std::string& buffer)
  {
    infile.seekg(0, std::ios::end);
    std::streampos pos = infile.tellg();
    if (pos > 0)
      {
        buffer.resize(static_cast<size_t>(pos));
        infile.seekg(0, std::ios::beg);
        return true;
      }

    return false;
  }

  //----------------------------------------------------------------------------
  //! \brief Read the shader code from the given file
  //! \param path the file path to read.
  //! \param fullSourceCode the buffer receiving the content of the file.
  //! \return true if success, else false.
  //----------------------------------------------------------------------------
  static bool load(std::string const& path, std::string& fullSourceCode)
  {
    std::ifstream infile;

    infile.open(path, std::ifstream::in);
    if (infile.fail())
      {
        //TODO m_error_msg += '\n' + msg;
        ERROR("Failed open file '%s'. Reason is '%s'", path.c_str(), strerror(errno));
        return false;
      }

    if (!GLShader::resize(infile, fullSourceCode))
      {
        ERROR("Failed reading file '%s'. Reason is 'cannot get the file size'", path.c_str());
        return false;
      }

    if (!infile.read(&fullSourceCode[0U], static_cast<std::streamsize>(fullSourceCode.size())))
      {
        ERROR("Failed reading file '%s'. Reason is '%s'", path.c_str(), strerror(errno));
        return false;
      }

    return true;
  }

  //----------------------------------------------------------------------------
  //! \brief The shader is created inside the GPU.
  //----------------------------------------------------------------------------
  virtual bool create() override
  {
    DEBUG("Shader::create %s", cname());
    m_handle = glCheck(glCreateShader(m_target));
    return false;
  }

  //----------------------------------------------------------------------------
  //! \brief Dummy method.
  //----------------------------------------------------------------------------
  virtual void activate() override
  {}

  //----------------------------------------------------------------------------
  //! \brief Compile the shader code in the GPU.
  //! \return true if the compilation succeeded. Else return false.
  //! An error message is set and can be read through error().
  //----------------------------------------------------------------------------
  virtual bool setup() override
  {
    DEBUG("Shader::setup %s", cname());
    if (loaded() && !compiled())
      {
        char const *source = m_shader_code.c_str();
        GLint length = static_cast<GLint>(m_shader_code.size());
        glCheck(glShaderSource(m_handle, 1, &source, &length));
        glCheck(glCompileShader(m_handle));
        m_compiled = checkCompilationStatus(m_handle);
        DEBUG("Shader::setup %d", m_compiled);
      }
    else
      {
        std::string msg =
          "Cannot compile the shader %s. Reason is "
          "'already compiled or no shader code attached'";
        m_error_msg += '\n' + msg;
        ERROR("%s", msg.c_str());
      }
    return !m_compiled;
  }

  virtual bool update() override
  {
    return false;
  }

  //----------------------------------------------------------------------------
  //! \brief Dummy method.
  //----------------------------------------------------------------------------
  virtual void deactivate() override
  {}

  //----------------------------------------------------------------------------
  //! \brief The shader is released from the GPU.
  //----------------------------------------------------------------------------
  virtual void release() override
  {
    DEBUG("Shader '%s' release", cname());
    glCheck(glDeleteShader(m_handle));
  }

  //----------------------------------------------------------------------------
  //! \brief Check if a shader script has been loaded.
  //! \return false if no shader script is present.
  //----------------------------------------------------------------------------
  inline bool loaded() const
  {
    return !m_shader_code.empty();
  }

  //----------------------------------------------------------------------------
  //! \param obj the identifer of the loaded shader.
  //! \return true if case of success, else return false.
  //! An error message is set and can be read through error().
  //----------------------------------------------------------------------------
  bool checkCompilationStatus(GLuint obj)
  {
    GLint status;

    glCheck(glGetShaderiv(obj, GL_COMPILE_STATUS, &status));
    if (GL_FALSE == status)
      {
        GLint length;
        glCheck(glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &length));
        std::vector<char> log(static_cast<size_t>(length));
        glCheck(glGetShaderInfoLog(obj, length, &length, &log[0U]));
        std::string msg = &log[0U];
        m_error_msg += '\n' + msg;
        ERROR("%s", msg.c_str());
      }
    else
      {
        m_error_msg.clear();
      }
    return status;
  }

  void throw_if_not_loaded()
  {
    if (unlikely(!loaded()))
      {
        throw OpenGLException("No code attached to the shader");
      }
  }

  void throw_if_already_compiled()
  {
    if (unlikely(!needSetup()))
      {
        throw OpenGLException("Failed Shader already compiled");
      }
  }

  virtual const char* type() const = 0;

private:

  std::string m_shader_code;
  std::string m_error_msg;
  bool m_compiled = false;
  GLenum m_attached = 0;
};

// *****************************************************************************
//!
// *****************************************************************************
class GLVertexShader: public GLShader
{
public:

  GLVertexShader(std::string const& name = "VertexShader")
    : GLShader(name, GL_VERTEX_SHADER)
  {
  }

private:

  virtual const char* type() const override
  {
    return "Vertex Shader script";
  }
};

// *****************************************************************************
//!
// *****************************************************************************
class GLFragmentShader: public GLShader
{
public:

  GLFragmentShader(std::string const& name = "FragmentShader")
    : GLShader(name, GL_FRAGMENT_SHADER)
  {
  }

private:

  virtual const char* type() const override
  {
    return "Fragment Shader script";
  }
};

// *****************************************************************************
//!
// *****************************************************************************
class GLGeometryShader: public GLShader
{
public:

  GLGeometryShader(std::string const& name = "GeometryShader")
    : GLShader(name, GL_GEOMETRY_SHADER)
  {
  }

private:

  virtual const char* type() const override
  {
    return "Geometry Shader script";
  }
};

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_GLSHADER_HPP
