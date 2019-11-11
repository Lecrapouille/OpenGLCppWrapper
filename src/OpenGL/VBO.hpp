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

#ifndef OPENGLCPPWRAPPER_GLVERTEX_BUFFER_HPP
#  define OPENGLCPPWRAPPER_GLVERTEX_BUFFER_HPP

#  include "OpenGL/GLObject.hpp"
#  include "Common/PendingContainer.hpp"

namespace glwrap
{

//! \brief Used by GLVAO
DECLARE_CLASS(IGLBuffer)

// **************************************************************
//! \brief Intermediate class needed by GLVAO for checking VBOs
//! size.
//!
//! Because GLBuffer is template, GLVAO cannot hold this class
//! in a container. This class is an interface needed by GLVAO.
// **************************************************************
class IGLBuffer
  : public GLObject<GLenum>
{
public:

  explicit IGLBuffer(std::string const& name)
    : GLObject(name)
  {}

  virtual size_t size() const = 0;
};

// **************************************************************
//! \brief Buffer objects are OpenGL objects that store an array of
//! unformatted memory allocated by the OpenGL context (aka: the
//! GPU). These can be used to store vertex data, pixel data retrieved
//! from images or the framebuffer, and a variety of other things.
// **************************************************************
template<typename T>
class GLBuffer
  : public IGLBuffer,
    public PendingContainer<T>
{
public:

  //! \brief Constructor with the object name
  explicit GLBuffer(std::string const& name, GLenum const target, BufferUsage const usage)
    : IGLBuffer(name)
  {
    GLObject::m_target = target;
    m_usage = static_cast<GLenum>(usage);
    PendingContainer<T>::setDebugName(name);
  }

  //! \brief Constructor with the object name and reserved number of
  //! elements.
  explicit GLBuffer(std::string const& name, GLenum const target, size_t const init_size, BufferUsage const usage)
    : IGLBuffer(name),
      PendingContainer<T>(init_size)
  {
    GLObject::m_target = target;
    m_usage = static_cast<GLenum>(usage);
    PendingContainer<T>::setDebugName(name);
  }

  virtual ~GLBuffer() override
  {
    destroy();
  }

  inline GLenum usage() const
  {
    return m_usage;
  }

  virtual size_t size() const override
  {
    return PendingContainer<T>::size();
  }

private:

  virtual bool create() override
  {
    DEBUG("VBO '%s' create", this->cname());
    glCheck(glGenBuffers(1, &m_handle));
    return false;
  }

  virtual void activate() override
  {
    DEBUG("VBO '%s' activate", this->cname());
    glCheck(glBindBuffer(m_target, m_handle));
  }

  virtual bool setup() override
  {
    DEBUG("VBO '%s' setup", this->cname());
    const GLsizeiptr bytes = static_cast<GLsizeiptr>
      (PendingContainer<T>::capacity() * sizeof (T));
    glCheck(glBufferData(m_target, bytes, NULL, m_usage));

    return false;
  }

  virtual inline bool needUpdate() const override
  {
    return PendingContainer<T>::hasPendingData();
  }

  virtual bool update() override
  {
    size_t pos_start, pos_end;
    PendingContainer<T>::getPendingData(pos_start, pos_end);
    PendingContainer<T>::clearPending();

    size_t offset = sizeof (T) * pos_start;
    size_t nbytes = sizeof (T) * (pos_end - pos_start);
    glCheck(glBufferSubData(m_target,
                            static_cast<GLintptr>(offset),
                            static_cast<GLsizeiptr>(nbytes),
                            PendingContainer<T>::to_array()));

    DEBUG("=== FLUSH VBO '%s' to GPU: %zu bytes (%zu -> %zu)",
          cname(), nbytes, pos_start, pos_end);
    return false;
  }

  virtual void deactivate() override
  {
    DEBUG("VBO '%s' deactivate", this->cname());
    glCheck(glBindBuffer(m_target, 0));
  }

  virtual void release() override
  {
    DEBUG("VBO '%s' release", this->cname());
    glCheck(glDeleteBuffers(1, &m_handle));
  }

private:

  GLenum m_usage;
};

// **************************************************************
//! \brief Buffer for vertex attribute data.
// **************************************************************
template<typename T>
class GLVertexBuffer: public GLBuffer<T>
{
public:

  //! \brief Constructor with the object name and reserved number of
  //! elements.
  explicit GLVertexBuffer(std::string const& name, size_t const init_size = 3_z,
                          BufferUsage const usage = BufferUsage::STATIC_DRAW)
    : GLBuffer<T>(name, GL_ARRAY_BUFFER, init_size, usage)
  {
  }

  inline GLVertexBuffer<T>& operator=(std::initializer_list<T> il)
  {
    PendingContainer<T>::operator=(il);
    return *this;
  }

  inline GLVertexBuffer<T>& operator=(GLVertexBuffer<T> const& other)
  {
    PendingContainer<T>::operator=(other);
    return *this;
  }

  inline GLVertexBuffer<T>& operator=(std::vector<T> const& other)
  {
    PendingContainer<T>::operator=(other);
    return *this;
  }
};

// **************************************************************
//! \brief Buffer for index data.
// **************************************************************
template<typename T>
class GLIndexBuffer: public GLBuffer<T>
{
public:

  //! \brief Constructor with the object name
  explicit GLIndexBuffer(std::string const& name,
                         BufferUsage const usage = BufferUsage::STATIC_DRAW)
    : GLBuffer<T>(name, GL_ELEMENT_ARRAY_BUFFER, usage)
  {
  }

  //! \brief Constructor with the object name and reserved number of
  //! elements.
  explicit GLIndexBuffer(std::string const& name, const size_t init_size,
                         BufferUsage const usage = BufferUsage::STATIC_DRAW)
    : GLBuffer<T>(name, GL_ELEMENT_ARRAY_BUFFER, init_size, usage)
  {
  }

  inline GLIndexBuffer<T>& operator=(std::initializer_list<T> il)
  {
    PendingContainer<T>::operator=(il);
    return *this;
  }

  inline GLIndexBuffer<T>& operator=(GLIndexBuffer<T> const& other)
  {
    PendingContainer<T>::operator=(other);
    return *this;
  }

  inline GLIndexBuffer<T>& operator=(std::vector<T> const& other)
  {
    PendingContainer<T>::operator=(other);
    return *this;
  }

  inline GLenum type() const;
};

template<>
inline GLenum GLIndexBuffer<uint32_t>::type() const { return GL_UNSIGNED_INT; }

template<>
inline GLenum GLIndexBuffer<uint16_t>::type() const { return GL_UNSIGNED_SHORT; }

template<>
inline GLenum GLIndexBuffer<uint8_t>::type() const { return GL_UNSIGNED_BYTE; }

//! \brief Human Friendly name (hide template misery)
using GLIndexBuffer32 = GLIndexBuffer<uint32_t>;

//! \brief Human Friendly name (hide template misery)
using GLIndexBuffer16 = GLIndexBuffer<uint16_t>;

//! \brief Human Friendly name (hide template misery)
using GLIndexBuffer8 = GLIndexBuffer<uint8_t>;

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_GLVERTEX_BUFFER_HPP
