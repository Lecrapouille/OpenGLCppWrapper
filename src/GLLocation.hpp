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

#ifndef OPENGLCPPWRAPPER_GLLOCATION_HPP
#define OPENGLCPPWRAPPER_GLLOCATION_HPP

// *****************************************************************************
//! \file GLLocation.hpp manages shader variables (Uniforms, Samplers
//! and Attributes).
// *****************************************************************************

#  include "IGLObject.hpp"
#  include "Matrix.hpp"

namespace glwrap
{

// *****************************************************************************
//! \brief GLLocation makes the interface between a shader variable and your
//! c++ code. It allows to upload CPU data to the GPU.
//!
//! \note this class shall stay private and not be used directly by the
//! developper. Indeed this class is only managed by GLProgram.
// *****************************************************************************
class GLLocation: public IGLObject<GLint>
{
public:

  //----------------------------------------------------------------------------
  //! \brief Constructor. This constructor makes no other actions.
  //!
  //! \param name Give a name to the instance. GLProgram uses names in their hash table.
  //! \param dim set the dimension of variable (1 for scalar else the dimension for vector)
  //! \param gltype set the OpenGL type of data (GL_FLOAT ...)
  //! \param prog set the GLProgram identifier (owner of this instance).
  //----------------------------------------------------------------------------
  GLLocation(const char *name, const GLint dim, const GLenum gltype, const GLuint prog)
    : IGLObject(name),
      m_dim(dim),
      m_program(prog)
  {
    m_target = gltype;
  }

  //----------------------------------------------------------------------------
  //! \brief Alias for IGLObject::getID() but in a more explicit way.
  //----------------------------------------------------------------------------
  inline GLint location() const
  {
    return m_handle;
  }

  //----------------------------------------------------------------------------
  //! \brief Return the dimension of the shader variable.
  //----------------------------------------------------------------------------
  inline GLint dim() const
  {
    return m_dim;
  }

  //----------------------------------------------------------------------------
  //! \brief Return the type of the shader variable.
  //----------------------------------------------------------------------------
  inline GLenum gltype() const
  {
    return m_target;
  }

protected:

  const GLint  m_dim;
  const GLuint m_program;
};

// *****************************************************************************
//! \brief Attribute represents a shader program attribute variable.
//!
//! This class only stores information about the attribute (dimension, type).
//! These info are used by the GLProgam to create VBOs when a VAO is bind to it.
// *****************************************************************************
class GLAttribute: public GLLocation
{
public:

  //----------------------------------------------------------------------------
  //! \brief See GLLocation constructor.
  //----------------------------------------------------------------------------
  GLAttribute(const char *name, const GLint dim, const GLenum gltype, const GLuint prog)
    : GLLocation(name, dim, gltype, prog)
  {
    assert((dim >= 1) && (dim <= 4));
  }

  //----------------------------------------------------------------------------
  //! \brief Destructor. Release elements in CPU and GPU memories.
  //----------------------------------------------------------------------------
  virtual ~GLAttribute() override
  {
    destroy();
  }

private:

  //----------------------------------------------------------------------------
  //! \brief Create a new OpenGL Attribute.
  //----------------------------------------------------------------------------
  virtual bool create() override
  {
    DEBUG("Attrib '%s' create", cname());
    m_handle = glCheck(glGetAttribLocation(m_program, cname()));
    m_index = static_cast<GLuint>(m_handle);
    return false;
  }

  //----------------------------------------------------------------------------
  //! \brief Destroy the OpenGL Attribute. This is a dummy method. No
  //! action is made.
  //----------------------------------------------------------------------------
  virtual void release() override
  {}

  //----------------------------------------------------------------------------
  //! \brief Bind the OpenGL Attribute.
  //----------------------------------------------------------------------------
  virtual void activate() override
  {
    DEBUG("Attrib '%s' activate", cname());
    glCheck(glEnableVertexAttribArray(m_index));
    glCheck(glVertexAttribPointer(m_index,
                                  m_dim,
                                  m_target,
                                  GL_FALSE,
                                  m_stride,
                                  (const GLvoid*) m_offset));
  }

  //----------------------------------------------------------------------------
  //! \brief Unbind the OpenGL Attribute.
  //----------------------------------------------------------------------------
  virtual void deactivate() override
  {
    DEBUG("Attrib '%s' deactivate", cname());
    glCheck(glDisableVertexAttribArray(m_index));
  }

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

private:

  //! \brief Hack ! This is an alias for m_handle but of different
  //! type.
  GLuint m_index = 0;
  //! \brief Specifies the byte offset between consecutive generic
  //! vertex attributes.
  size_t m_stride = 0;
  //! \brief Specifies a offset of the first component of the first
  //! generic vertex attribute in the array in the data store.
  size_t m_offset = 0;
};

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
  IGLUniform(const char *name, const GLint dim, const GLenum gltype, const GLuint prog)
    : GLLocation(name, dim, gltype, prog)
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
  //! \brief Destroy the OpenGL Uniform. This is a dummy method. No
  //! action is made.
  //----------------------------------------------------------------------------
  virtual void release() override
  {}

  //----------------------------------------------------------------------------
  //! \brief Bind the OpenGL Uniform. This is a dummy method. No
  //! action is made.
  //----------------------------------------------------------------------------
  virtual void activate() override
  {}

  //----------------------------------------------------------------------------
  //! \brief Unbind the OpenGL Uniform. This is a dummy method. No
  //! action is made.
  //----------------------------------------------------------------------------
  virtual void deactivate() override
  {
    DEBUG("Uniform '%s' deactivate", cname());
  }

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
};

// *****************************************************************************
//! \brief GLUniform represents a program uniform variable.
// *****************************************************************************
template<class T>
class GLUniform: public IGLUniform
{
public:

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  GLUniform(const char *name, const GLint dim, const GLenum gltype, const GLuint prog)
    : IGLUniform(name, dim, gltype, prog)
  {}

  //----------------------------------------------------------------------------
  //! \brief Change the CPU data. It will be automatically transfered to the GPU.
  //----------------------------------------------------------------------------
  template<class U>
  GLUniform<T>& operator=(const U& val)
  {
    GLUniform<T>::data() = T(val);
    return *this;
  }

  //----------------------------------------------------------------------------
  //! \brief Return the CPU data.
  //TODO a deplacer dans GLUniform car on distingue GLSampler::texture
  //----------------------------------------------------------------------------
  inline T const& data() const
  {
    return m_data;
  }

  //----------------------------------------------------------------------------
  //! \brief Return the CPU data.
  //----------------------------------------------------------------------------
  inline T& data()
  {
    // FIXME always modified ?
    forceUpdate();

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
  glCheck(glUniform1i(m_handle, value));
}

template<>
inline void GLUniform<float>::setValue(const float& value) const
{
  glCheck(glUniform1f(m_handle, value));
}

template<>
inline void GLUniform<Vector2f>::setValue(const Vector2f& value) const
{
  glCheck(glUniform2f(m_handle, value.x, value.y));
}

template<>
inline void GLUniform<Vector3f>::setValue(const Vector3f& value) const
{
  glCheck(glUniform3f(m_handle, value.x, value.y, value.z));
}

template<>
inline void GLUniform<Vector4f>::setValue(const Vector4f& value) const
{
  glCheck(glUniform4f(m_handle, value.x, value.y, value.z, value.w));
}

template<>
inline void GLUniform<Vector2i>::setValue(const Vector2i& value) const
{
  glCheck(glUniform2i(m_handle, value.x, value.y));
}

template<>
inline void GLUniform<Vector3i>::setValue(const Vector3i& value) const
{
  glCheck(glUniform3i(m_handle, value.x, value.y, value.z));
}

template<>
inline void GLUniform<Vector4i>::setValue(const Vector4i& value) const
{
  glCheck(glUniform4i(m_handle, value.x, value.y, value.z, value.w));
}

template<>
inline void GLUniform<Matrix22f>::setValue(const Matrix22f& value) const
{
  glCheck(glUniformMatrix2fv(m_handle, 1, GL_FALSE, &value[0][0]));
}

template<>
inline void GLUniform<Matrix33f>::setValue(const Matrix33f& value) const
{
  glCheck(glUniformMatrix3fv(m_handle, 1, GL_FALSE, &value[0][0]));
}

template<>
inline void GLUniform<Matrix44f>::setValue(const Matrix44f& value) const
{
  glCheck(glUniformMatrix4fv(m_handle, 1, GL_FALSE, &value[0][0]));
}

// *****************************************************************************
//! \brief A GLSampler is an OpenGL uniform for texture.
// *****************************************************************************
class GLSampler: public IGLUniform
{
public:

  //----------------------------------------------------------------------------
  //! \brief See GLLocation constructor.
  //! \param name
  //! \param gltype
  //! \param texture_id count texture.
  //! \param prog
  //----------------------------------------------------------------------------
  GLSampler(const char *name, const GLenum gltype, const GLenum texture_id,
            const GLuint prog)
    : IGLUniform(name, 0, gltype, prog),
      m_texture_id(texture_id)
  {
    forceUpdate();
  }

  //----------------------------------------------------------------------------
  //! \brief Return the texture identifier.
  //----------------------------------------------------------------------------
  inline GLenum textureID() const
  {
    return m_texture_id;
  }

private:

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  virtual void activate() override
  {
    DEBUG("Sampler '%s' activate GL_TEXTURE0 + %u", cname(), m_texture_id);
    glCheck(glActiveTexture(GL_TEXTURE0 + m_texture_id));
  }

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  virtual bool update() override
  {
    DEBUG("Sampler '%s' update", cname());
    glCheck(glUniform1i(m_handle, static_cast<GLint>(m_texture_id)));
    return false;
  }

protected:

  const GLenum m_texture_id;
};

// *****************************************************************************
//! \brief Sampler for 1D texture.
// *****************************************************************************
class GLSampler1D: public GLSampler
{
public:

  //----------------------------------------------------------------------------
  //! \brief See GLLocation constructor.
  //----------------------------------------------------------------------------
  GLSampler1D(const char *name, const GLenum texture_id, const GLuint prog)
    : GLSampler(name, GL_SAMPLER_1D, texture_id, prog)
  {}
};

// *****************************************************************************
//! \brief Sampler for 2D texture.
// *****************************************************************************
class GLSampler2D: public GLSampler
{
public:

  //----------------------------------------------------------------------------
  //! \brief See GLLocation constructor.
  //----------------------------------------------------------------------------
  GLSampler2D(const char *name, const GLenum texture_id, const GLuint prog)
    : GLSampler(name, GL_SAMPLER_2D, texture_id, prog)
  {}
};

// *****************************************************************************
//! \brief Sampler for 3D texture.
// *****************************************************************************
class GLSampler3D: public GLSampler
{
public:

  //----------------------------------------------------------------------------
  //! \brief See GLLocation constructor.
  //----------------------------------------------------------------------------
  GLSampler3D(const char *name, const GLenum texture_id, const GLuint prog)
    : GLSampler(name, GL_SAMPLER_3D, texture_id, prog)
  {}
};

// *****************************************************************************
//! \brief Sampler for Cubic texture.
// *****************************************************************************
class GLSamplerCube: public GLSampler
{
public:

  //----------------------------------------------------------------------------
  //! \brief See GLLocation constructor.
  //----------------------------------------------------------------------------
  GLSamplerCube(const char *name, const GLenum texture_id, const GLuint prog)
    : GLSampler(name, GL_SAMPLER_CUBE, texture_id, prog)
  {}
};

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_GLLOCATION_HPP
