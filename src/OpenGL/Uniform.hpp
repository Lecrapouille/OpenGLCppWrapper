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

#ifndef OPENGLCPPWRAPPER_GLUNIFORM_HPP
#  define OPENGLCPPWRAPPER_GLUNIFORM_HPP

#  include "OpenGL/Locations.hpp"

// *****************************************************************************
//! \file Uniform.hpp file implements:
//!   - GLUniform:
// *****************************************************************************

#  include "Math/Matrix.hpp"

namespace glwrap
{

// *****************************************************************************
//! \brief IGLUniform represents a program uniform variable.
//!
//! See them like constants values in shaders of type T.
// *****************************************************************************
class IGLUniform: public GLLocation
{
public:

  //----------------------------------------------------------------------------
  //! \brief See GLLocation constructor.
  // \note T and gltype shall match. Not checks are made !
  //----------------------------------------------------------------------------
  IGLUniform(const char *name, const GLint dim, const GLint gltype, const GLuint prog)
    : GLLocation(name, dim, static_cast<GLenum>(gltype), prog)
  {}

  //----------------------------------------------------------------------------
  //! \brief Destructor. Release elements from CPU and GPU.
  //----------------------------------------------------------------------------
  virtual ~IGLUniform() override
  {
    destroy();
  }

private:

  //----------------------------------------------------------------------------
  //! \brief Create a new OpenGL Uniform.
  //----------------------------------------------------------------------------
  virtual bool create() override
  {
    DEBUG("Uniform '%s' create", cname());
    m_handle = glCheck(glGetUniformLocation(m_program, cname()));
    return false;
  }

  //----------------------------------------------------------------------------
  //! \brief Bind the OpenGL Uniform. This is a dummy method. No
  //! action is made.
  //----------------------------------------------------------------------------
  virtual void activate() override
  {}

  //----------------------------------------------------------------------------
  //! \brief Setup the behavior of the instance. This is a dummy
  //! method. No action is made.
  //----------------------------------------------------------------------------
  virtual bool setup() override
  {
    return false;
  }

  //----------------------------------------------------------------------------
  //! \brief This is a dummy method. No action is made.
  //----------------------------------------------------------------------------
  virtual bool update() override
  {
    return false;
  }

  //----------------------------------------------------------------------------
  //! \brief Unbind the OpenGL Uniform. This is a dummy method. No
  //! action is made.
  //----------------------------------------------------------------------------
  virtual void deactivate() override
  {
    DEBUG("Uniform '%s' deactivate", cname());
  }

  //----------------------------------------------------------------------------
  //! \brief Destroy the OpenGL Uniform. This is a dummy method. No
  //! action is made.
  //----------------------------------------------------------------------------
  virtual void release() override
  {}
};

// *****************************************************************************
//! \brief GLUniform represents a program uniform variable.
//! \tparam T float or int or VectorXf or VectorXi or MatrixXXf with X = [2 .. 4]
//! or GLSamplerXD with X = [1 .. 3] or GLSamplerCube.
// *****************************************************************************
template<class T>
class GLUniform: public IGLUniform
{
public:

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  GLUniform(const char *name, const GLint dim, const GLint gltype, const GLuint prog)
    : IGLUniform(name, dim, gltype, prog)
  {}

  //----------------------------------------------------------------------------
  //! \brief Change the CPU data. It will be automatically transfered to the GPU.
  //----------------------------------------------------------------------------
  template<class U>
  GLUniform<T>& operator=(const U& val)
  {
    GLUniform<T>::m_data = T(val);
    DEBUG("Uniform '%s' operator=", cname());
    redoUpdate();
    return *this;
  }

  //----------------------------------------------------------------------------
  //! \brief Getter. Return the reference of the CPU data in read only mode.
  //----------------------------------------------------------------------------
  inline operator const T&() const
  {
    DEBUG("Uniform '%s' get data", cname());
    return m_data;
  }

  //----------------------------------------------------------------------------
  //! \brief Setter. Return the reference of CPU data in write mode. New value
  //! will be transfered to GPU memory.
  //! \fixme Could be nice to avoid transfering to the GPU if we set the same val
  //----------------------------------------------------------------------------
  inline operator T&()
  {
    DEBUG("Uniform '%s' set data", cname());
    redoUpdate();
    return m_data;
  }

private:

  //----------------------------------------------------------------------------
  //! \brief Transfer the CPU data to the GPU data.
  //----------------------------------------------------------------------------
  virtual bool update() override
  {
    DEBUG("Uniform '%s' update", cname());
    setValue(GLUniform<T>::m_data);
    return false;
  }

  //----------------------------------------------------------------------------
  //! \brief Transfer the CPU data to the GPU data.
  //----------------------------------------------------------------------------
  inline void setValue(const T& value) const;

protected:

  T m_data {};
};

template<>
inline void GLUniform<int>::setValue(const int& value) const
{
  DEBUG("Uniform '%s' set %i", cname(), value);
  glCheck(glUniform1i(m_handle, value));
}

template<>
inline void GLUniform<float>::setValue(const float& value) const
{
  DEBUG("Uniform '%s' set %f", cname(), value);
  glCheck(glUniform1f(m_handle, value));
}

template<>
inline void GLUniform<Vector2f>::setValue(const Vector2f& value) const
{
  DEBUG("Uniform '%s' set [%f %f]", cname(), value.x, value.y);
  glCheck(glUniform2f(m_handle, value.x, value.y));
}

template<>
inline void GLUniform<Vector3f>::setValue(const Vector3f& value) const
{
  DEBUG("Uniform '%s' set [%f %f %f]", cname(), value.x, value.y, value.z);
  glCheck(glUniform3f(m_handle, value.x, value.y, value.z));
}

template<>
inline void GLUniform<Vector4f>::setValue(const Vector4f& value) const
{
  DEBUG("Uniform '%s' set [%f %f %f %f]", cname(), value.x, value.y, value.z, value.w);
  glCheck(glUniform4f(m_handle, value.x, value.y, value.z, value.w));
}

template<>
inline void GLUniform<Vector2i>::setValue(const Vector2i& value) const
{
  DEBUG("Uniform '%s' set [%i %i]", cname(), value.x, value.y);
  glCheck(glUniform2i(m_handle, value.x, value.y));
}

template<>
inline void GLUniform<Vector3i>::setValue(const Vector3i& value) const
{
  DEBUG("Uniform '%s' set [%i %i %i]", cname(), value.x, value.y, value.z);
  glCheck(glUniform3i(m_handle, value.x, value.y, value.z));
}

template<>
inline void GLUniform<Vector4i>::setValue(const Vector4i& value) const
{
  DEBUG("Uniform '%s' set [%i %i %i %i]", cname(), value.x, value.y, value.z, value.w);
  glCheck(glUniform4i(m_handle, value.x, value.y, value.z, value.w));
}

template<>
inline void GLUniform<Matrix22f>::setValue(const Matrix22f& m) const
{
  DEBUG("Uniform '%s' set matrix", cname());
  glCheck(glUniformMatrix2fv(m_handle, 1, GL_FALSE, m));
}

template<>
inline void GLUniform<Matrix33f>::setValue(const Matrix33f& m) const
{
  DEBUG("Uniform '%s' set matrix", cname());
  glCheck(glUniformMatrix3fv(m_handle, 1, GL_FALSE, m));
}

template<>
inline void GLUniform<Matrix44f>::setValue(const Matrix44f& m) const
{
  DEBUG("Uniform '%s' set matrix", cname());
  glCheck(glUniformMatrix4fv(m_handle, 1, GL_FALSE, m));
}

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_GLUNIFORM_HPP
