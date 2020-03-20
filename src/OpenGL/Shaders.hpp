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

#ifndef OPENGLCPPWRAPPER_GLSHADERS_HPP
#  define OPENGLCPPWRAPPER_GLSHADERS_HPP

// *****************************************************************************
//! \file Shaders.hpp holds the GLSL shader code, offers some macros
//! code for processing the code (like code inclusion) and allow to
//! compile code source.
//!
//! \file Shaders.hpp file implements:
//!   - GLShader:
//!   - GLFragmentShader:
//!   - GLVertexShader:
//!   - GLGeometryShader:
// *****************************************************************************

#  include "OpenGL/GLObject.hpp"
#  include <vector>
#  include <fstream>
#  include <iostream>
#  include <sstream>

namespace glwrap
{

// *****************************************************************************
//! \class GLShader Shaders.hpp
//!
//! \brief holds the GLSL shader code, offers some macros
//! code for processing the code (like code inclusion) and allow to
//! compile code source.
// *****************************************************************************
class GLShader: public GLObject<GLenum>
{
  friend std::ostream& operator<<(std::ostream& os, GLShader const& shader);
  template<typename T> friend GLShader& operator>>(T const& s, GLShader& shader);

public:

  //----------------------------------------------------------------------------
  //! \brief Constructor. Do nothing.
  //----------------------------------------------------------------------------
  GLShader(std::string const& name, const GLenum target)
    : GLObject(name)
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
    m_path.clear();
    m_shader_code = script;
    DEBUG("From script '%s' %s: '%s'", cname(), type(), m_shader_code.c_str());
    return true;
  }

  //----------------------------------------------------------------------------
  //! \brief Append code to the shader.
  //----------------------------------------------------------------------------
  template<typename T>
  GLShader& operator<<(T const& code)
  {
    throw_if_already_compiled();
    m_shader_code += code;
    return *this;
  }

  //----------------------------------------------------------------------------
  //! \brief For accepting std::endl to the sahder
  //----------------------------------------------------------------------------
  GLShader& operator<<(std::ostream& (*os)(std::ostream&))
  {
    throw_if_already_compiled();
    std::stringstream ss;
    ss << os;
    m_shader_code += ss.str();
    return *this;
  }

  //----------------------------------------------------------------------------
  //! \brief Read the whole shader code from the given ascii file.
  //! Note: the shader code compilation is delayed. The code is only
  //! store in the member variable.
  //! \param[in] path the path of the shader code.
  //! \return true if the whole file has been read, else return false.
  //! An error message is set and can be read through getError().
  //----------------------------------------------------------------------------
  bool fromFile(std::string const& path)
  {
    throw_if_already_compiled();

    std::string msg;
    std::ifstream infile;

    m_path = path;
    infile.open(path, std::ifstream::in);
    if (infile.fail())
      {
        msg = "Failed opening file '" + path + "'. Reason was '" + strerror(errno) + "'";
        goto l_error;
      }

    if (!GLShader::resize(infile, m_shader_code))
      {
        msg = "Failed reading file '" + path + "'. Reason was 'cannot get the file size'";
        goto l_error;
      }

    if (!infile.read(&m_shader_code[0U], static_cast<std::streamsize>(m_shader_code.size())))
      {
        msg = "Failed reading file '" + path + "'. Reason was '" + strerror(errno) + "'";
        goto l_error;
      }

    DEBUG("From file '%s' %s: '%s'", cname(), type(), m_shader_code.c_str());
    return true;

l_error:
    concatError(msg);
    ERROR("%s", msg.c_str());
    return false;
  }

  //----------------------------------------------------------------------------
  //! \brief Check if the shader has been compiled with success by
  //! OpenGL.
  //----------------------------------------------------------------------------
  inline bool isCompiled() const
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
  inline std::string getError()
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

  //----------------------------------------------------------------------------
  //! \brief Check if a shader script has been loaded.
  //! \return false if no shader script is present.
  //----------------------------------------------------------------------------
  inline bool loaded() const
  {
    return !m_shader_code.empty();
  }

  inline std::string const& path() const
  {
    return m_path;
  }

private:

  //----------------------------------------------------------------------------
  //! \brief Remove the file name and store the path to this folder.
  //! \param[in] fullPath
  //----------------------------------------------------------------------------
  static std::string getFilePath(const std::string& fullPath)
  {
    size_t found = fullPath.find_last_of("/\\");
    return fullPath.substr(0, found + 1);
  }

  //----------------------------------------------------------------------------
  //! \brief Increase the buffer size of the size of the full file.
  //! \param[in,out] infile
  //! \param[in,out] buffer
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
  //! \brief The shader is created inside the GPU.
  //----------------------------------------------------------------------------
  virtual bool create() override
  {
    DEBUG("  Shader '%s' create", cname());
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
  //! An error message is set and can be read through getError().
  //----------------------------------------------------------------------------
  virtual bool setup() override
  {
    DEBUG("  Shader '%s' setup", cname());
    if (likely((loaded() && !isCompiled())))
      {
        char const *source = m_shader_code.c_str();
        GLint length = static_cast<GLint>(m_shader_code.size());
        glCheck(glShaderSource(m_handle, 1, &source, &length));
        glCheck(glCompileShader(m_handle));
        m_compiled = checkCompilationStatus(m_handle);
      }
    else
      {
        std::string msg =
          "   Could not compile the shader named '" + name() + "'. Reason was ";
        msg += (loaded()) ? "'already compiled'" : "'compilation errored'";
        concatError(msg);
        ERROR("%s", msg.c_str());
      }
    DEBUG("  Shader '%s' compiled? %d", cname(), m_compiled);
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
  //! \brief Check if a shader script has been correctly compiled.
  //!
  //! An error message is set in m_error_msg and which can be read through
  //! getError().
  //!
  //! \param[in] obj the identifer of the loaded shader.
  //! \return true if the shader has been compiled with success else return false.
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
        glCheck(glGetShaderInfoLog(obj, length - 1, &length, &log[0U]));
        concatError(&log[0U]);
        std::string path;
        if (!m_path.empty())
            path = " (" + m_path + ")";
        ERROR("Failed compiling '%s'%s. Reason was '%s'", cname(), path.c_str(),
              m_error_msg.c_str());
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
        throw OpenGLException("Shader already compiled");
      }
  }

  //----------------------------------------------------------------------------
  //! \brief Concat the last error to the list of errors
  //----------------------------------------------------------------------------
  void concatError(std::string const& msg)
  {
    if (!m_error_msg.empty())
      {
        m_error_msg += '\n';
      }
    m_error_msg += msg;
  }

  virtual const char* type() const = 0;

private:

  //! \brief Hold the code source of the shader
  std::string m_shader_code;
  //! \brief Hold error messages
  std::string m_error_msg;
  //! \brief Hold the file path (when shader is loaded from a file)
  std::string m_path;
  //! \brief Compiled with success?
  bool m_compiled = false;
  //! \brief GLProgram bound to this shader
  GLenum m_attached = 0;
};

//----------------------------------------------------------------------------
//! \brief Display on stream the shader code.
//----------------------------------------------------------------------------
inline std::ostream& operator<<(std::ostream& os, GLShader const& shader)
{
  return os << shader.m_shader_code;
}

//----------------------------------------------------------------------------
//! \brief Prepend the shader code to the shader.
//----------------------------------------------------------------------------
template<typename T>
inline GLShader& operator>>(T const& code, GLShader& shader)
{
  shader.m_shader_code = code + shader.m_shader_code;
  return shader;
}

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

#endif // OPENGLCPPWRAPPER_GLSHADERS_HPP
