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

#ifndef GLSHADER_HPP
#  define GLSHADER_HPP

#  include "IGLObject.tpp"
#  include <vector>

//------------------------------------------------------------------
//! \param filename the file path to read.
//! \param the buffer receiving the content of the file.
//! \return true if success, else false.
//------------------------------------------------------------------
static bool readAllFile(std::string const& filename, std::string& buffer)
{
  std::ifstream infile;
  bool res = false;

  // Read the shader code from a file
  infile.open(filename, std::ifstream::in);
  if (infile.is_open())
    {
      infile.seekg(0, std::ios::end);
      std::streampos pos = infile.tellg();
      if (pos > 0)
        {
          buffer.resize(static_cast<size_t>(pos));
          infile.seekg(0, std::ios::beg);
          infile.read(&buffer[0U],
                      static_cast<std::streamsize>(buffer.size()));
          res = infile.good();
          if (!res)
            {
              ERROR("Failed reading the whole file '%s'. Reason is '%s'",
                    filename.c_str(), strerror(errno));
            }
        }
      infile.close();
    }
  else
    {
      ERROR("Failed open file '%s'. Reason is '%s'",
            filename.c_str(), strerror(errno));
    }

  return res;
}

// **************************************************************
//! \class GLShader GLShader.hpp this class allows to load a
//! shader (vertex, fragment, geometry) and compile it. This class
//! is managed by GLProgram.
// **************************************************************
class GLShader: public IGLObject<GLuint>
{
public:

  //------------------------------------------------------------------
  //! \brief Empty constructor. Do nothing.
  //------------------------------------------------------------------
  GLShader(const GLenum target) noexcept
    : IGLObject()
  {
    m_target = target;
  }

  //------------------------------------------------------------------
  //! \brief Constructor. Do nothing.
  //------------------------------------------------------------------
  GLShader(std::string const& name, const GLenum target) noexcept
    : IGLObject(name)
  {
    m_target = target;
  }

  //------------------------------------------------------------------
  //! \brief Release GPU memory.
  //------------------------------------------------------------------
  virtual ~GLShader() override
  {
    destroy();
  }

  //------------------------------------------------------------------
  //! \brief Copy the shader code given as string into the class
  //! member variable. Note: the shader code compilation is delayed.
  //------------------------------------------------------------------
  void fromString(std::string const& script)
  {
    throw_if_already_compiled();
    name() = "shader script"; // FIXME: "Vertex Shader" ou "Frag Shader" selon le type
    m_shader_code = script;
  }

  //------------------------------------------------------------------
  //! \brief Read the whole shader code from the given ascii file.
  //! Note: the shader code compilation is delayed. The code is only
  //! store in the member variable.
  //! \param the path of the shader code.
  //! \return true if the whole file has been read, else return false.
  //! An error message is set and can be read through error().
  //------------------------------------------------------------------
  bool fromFile(std::string const& filename)
  {
    throw_if_already_compiled();

    name() = file_name(filename);

    bool loaded = readAllFile(filename, m_shader_code);
    DEBUG("FromFile: Shader: '%s'", m_shader_code.c_str());
    if (false == loaded)
      {
        std::string msg = "Failed loading shader code '"
          + filename + "'";
        ERROR("%s", msg.c_str());
        m_error_msg += '\n' + msg;
      }
    return loaded;
  }

  //------------------------------------------------------------------
  //! \brief Check if the shader has been compiled with success by
  //! OpenGL.
  //------------------------------------------------------------------
  inline bool compiled() const
  {
    return m_compiled;
  }

  //------------------------------------------------------------------
  //! \return the shader code
  //------------------------------------------------------------------
  inline std::string const& code() const
  {
    return m_shader_code;
  }

  //------------------------------------------------------------------
  //! \brief Return the shader error message
  //------------------------------------------------------------------
  inline bool hasErrored() const
  {
    return !m_error_msg.empty();
  }

  //------------------------------------------------------------------
  //! \brief Return the shader error message. member variable is then
  //! cleared.
  //------------------------------------------------------------------
  inline std::string error()
  {
    std::string tmp(m_error_msg);
    m_error_msg.clear();
    return tmp;
  }

  //------------------------------------------------------------------
  //! \brief (Information) Attached by a prog. Pass 0 for detached
  //------------------------------------------------------------------
  inline void attachProg(GLenum prog)
  {
    m_attached = prog;
  }

  inline GLenum attached() const
  {
    return m_attached;
  }

private:

  //------------------------------------------------------------------
  //! \brief The shader is created inside the GPU.
  //------------------------------------------------------------------
  virtual bool create() override
  {
    DEBUG("Shader::create %s", name().c_str());
    m_handle = glCheck(glCreateShader(m_target));
    return false;
  }

  //------------------------------------------------------------------
  //! \brief The shader is released from the GPU.
  //------------------------------------------------------------------
  virtual void release() override
  {
    DEBUG("Shader '%s' release", name().c_str());
    glCheck(glDeleteShader(m_handle));
  }

  //------------------------------------------------------------------
  //! \brief Dummy method.
  //------------------------------------------------------------------
  virtual void activate() override
  {
  }

  //------------------------------------------------------------------
  //! \brief Dummy method.
  //------------------------------------------------------------------
  virtual void deactivate() override
  {
  }

  //------------------------------------------------------------------
  //! \brief Compile the shader code in the GPU.
  //! \return true if the compilation succeeded. Else return false.
  //! An error message is set and can be read through error().
  //------------------------------------------------------------------
  virtual bool setup() override
  {
    DEBUG("Shader::setup %s", name().c_str());
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

  inline bool loaded() const
  {
    return !m_shader_code.empty();
  }

  //------------------------------------------------------------------
  //! \param obj the identifer of the loaded shader.
  //! \return true if case of success, else return false.
  //! An error message is set and can be read through error().
  //------------------------------------------------------------------
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

private:

  std::string m_shader_code;
  std::string m_error_msg;
  bool m_compiled = false;
  GLenum m_attached = 0;
};

// **************************************************************
//!
// **************************************************************
class GLVertexShader: public GLShader
{
public:

  GLVertexShader()
    : GLShader(GL_VERTEX_SHADER)
  {
  }

  GLVertexShader(std::string const& name)
    : GLShader(name, GL_VERTEX_SHADER)
  {
  }
};

// **************************************************************
//!
// **************************************************************
class GLFragmentShader: public GLShader
{
public:

  GLFragmentShader()
    : GLShader(GL_FRAGMENT_SHADER)
  {
  }

  GLFragmentShader(std::string const& name)
    : GLShader(name, GL_FRAGMENT_SHADER)
  {
  }
};

// **************************************************************
//!
// **************************************************************
class GLGeometryShader: public GLShader
{
public:

  GLGeometryShader()
    : GLShader(GL_GEOMETRY_SHADER)
  {
  }

  GLGeometryShader(std::string const& name)
    : GLShader(name, GL_GEOMETRY_SHADER)
  {
  }
};

#endif /* GLSHADER_HPP */
