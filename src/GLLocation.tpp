// -*- c++ -*- Coloration Syntaxique pour Emacs
//=====================================================================
// OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of OpenGLCppWrapper.
//
// OpenGLCppWrapper is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with OpenGLCppWrapper.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================
//
// This file is a derivated work of https://github.com/glumpy/glumpy
//
// Copyright (c) 2009-2016 Nicolas P. Rougier. All rights reserved.
// Distributed under the (new) BSD License.
//=====================================================================

#ifndef GLLOCATION_HPP
#define GLLOCATION_HPP

#  include "IGLObject.tpp"
#  include "Matrix.tpp"

// **************************************************************
//! \brief Class linking
// **************************************************************
class GLLocation: public IGLObject<GLint>
{
public:

  //! \brief
  GLLocation(const char *name, GLint dim, GLenum gltype, GLuint prog)
    : IGLObject(name)
  {
    m_dim = dim;
    m_gltype = gltype;
    m_program = prog;
  }

  //! \brief Alias for getID() but in a more explicit way
  inline GLint location() const
  {
    return m_handle;
  }

  inline GLint dim() const
  {
    return m_dim;
  }

protected:

  GLint  m_dim;
  GLenum  m_gltype;
  GLuint  m_program;
};

// **************************************************************
//!
// **************************************************************
class GLAttribute: public GLLocation
{
public:

  GLAttribute(const char *name, GLint dim, GLenum gltype, GLuint prog)
    : GLLocation(name, dim, gltype, prog)
  {
    assert((dim >= 1) && (dim <= 4));
    m_stride = 0;
    m_offset = 0;
  }

  virtual ~GLAttribute() override
  {
    destroy();
  }

private:

  virtual bool create() override
  {
    DEBUG("Attrib '%s' create", name().c_str());
    m_handle = glCheck(glGetAttribLocation(m_program, name().c_str()));
    m_index = static_cast<GLuint>(m_handle);
    return false;
  }

  virtual void release() override
  {
  }

  virtual void activate() override
  {
    DEBUG("Attrib '%s' activate", name().c_str());
    glCheck(glEnableVertexAttribArray(m_index));
    glCheck(glVertexAttribPointer(m_index,
                                  m_dim,
                                  m_gltype,
                                  GL_FALSE,
                                  m_stride,
                                  (const GLvoid*) m_offset));
  }

  virtual void deactivate() override
  {
    DEBUG("Attrib '%s' deactivate", name().c_str());
    if (likely(isValid()))
      {
        glCheck(glDisableVertexAttribArray(m_index));
      }
  }

  virtual bool setup() override
  {
    return false;
  }

  virtual bool update() override
  {
    return false;
  }

private:

  GLuint m_index;
  size_t m_stride;
  size_t m_offset;
};

// **************************************************************
//!
// **************************************************************
template<class T>
class IGLUniform: public GLLocation
{
public:

  // Note T and gltype shall match. Not checks are made
  IGLUniform(const char *name, GLint dim, GLenum gltype, GLuint prog)
    : GLLocation(name, dim, gltype, prog)
  {
  }

  virtual ~IGLUniform() override
  {
    destroy();
  }

  //TODO a deplacer dans GLUniform car on distingue GLSampler::texture
  inline T const& data() const
  {
    return m_data;
  }

  inline T& data()
  {
    // FIXME always modified ?
    forceUpdate();

    return m_data;
  }

private:

  virtual bool create() override
  {
    DEBUG("Uniform '%s' create", name().c_str());
    m_handle = glCheck(glGetUniformLocation(m_program, name().c_str()));
    return false;
  }

  virtual void release() override
  {
  }

  virtual void activate() override
  {
  }

  virtual void deactivate() override
  {
    DEBUG("Uniform '%s' deactivate", name().c_str());
  }

  virtual bool setup() override
  {
    return false;
  }

  virtual bool update() override
  {
    return false;
  }

protected:

  T m_data;
};

// **************************************************************
//!
// **************************************************************
template<class T>
class GLUniform: public IGLUniform<T>
{
public:

  GLUniform(const char *name, GLint dim, GLenum gltype, GLuint prog)
    : IGLUniform<T>(name, dim, gltype, prog)
  {
  }

  // Manipule donnee depuis le CPU
  template<class U>
  GLUniform<T>& operator=(const U& val)
  {
    IGLUniform<T>::data() = T(val);
    return *this;
  }

private:

  virtual bool update() override
  {
    DEBUG("Uniform '%s' update", IGLUniform<T>::name().c_str());
    setValue(IGLUniform<T>::m_data);
    return false;
  }

  inline void setValue(const T& value) const;
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

// **************************************************************
//! \brief A GLSampler is an Opengl uniform for texture
// **************************************************************
template<class T>
class GLSampler: public IGLUniform<T>
{
public:

  GLSampler(const char *name, GLenum gltype, uint32_t texture_count, GLuint prog)
    : IGLUniform<T>(name, 0, gltype, prog)
  {
    m_texture_count = texture_count;
  }

  inline uint32_t textureID() const
  {
    return m_texture_count;
  }

  inline T const& texture() const
  {
    return IGLUniform<T>::data();
  }

  inline T& texture()
  {
    return IGLUniform<T>::data();
  }

private:

  virtual void activate() override
  {
    DEBUG("Sampler '%s' activate", IGLUniform<T>::name().c_str());
    glCheck(glActiveTexture(GL_TEXTURE0 + m_texture_count));
    IGLUniform<T>::m_data.begin();
  }

  virtual bool update() override
  {
    DEBUG("Sampler '%s' update", IGLUniform<T>::name().c_str());
    glCheck(glUniform1i(IGLUniform<T>::m_handle,
                        static_cast<GLint>(m_texture_count)));
    return false;
  }

private:

  uint32_t m_texture_count = 0;
};

// **************************************************************
//! TODO
// **************************************************************
#if 0
class GLSampler1D: public GLSampler<GLTexture1D>
{
public:

  GLSampler1D(const char *name, uint32_t texture_count, GLuint prog)
    : GLSampler<GLTexture1D>(name, GL_SAMPLER_1D, texture_count, prog)
  {
  }
};
#endif

// **************************************************************
//!
// **************************************************************
class GLSampler2D: public GLSampler<GLTexture2D>
{
public:

  GLSampler2D(const char *name, uint32_t texture_count, GLuint prog)
    : GLSampler<GLTexture2D>(name, GL_SAMPLER_2D, texture_count, prog)
  {
  }
};

// **************************************************************
//!
// **************************************************************
#if 0
class GLSampler3D: public GLSampler<GLTexture3D>
{
public:

  GLSampler3D(const char *name, uint32_t texture_count, GLuint prog)
    : GLSampler<GLTexture3D>(name, GL_SAMPLER_CUBE, texture_count, prog)
  {
  }
};
#endif

#endif
