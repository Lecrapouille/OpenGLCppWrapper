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

#ifndef GLVERTEX_BUFFER_HPP_
#  define GLVERTEX_BUFFER_HPP_

#  include "IGLObject.tpp"
#  include "PendingContainer.tpp"

// **************************************************************
//! \brief Buffer objects are OpenGL objects that store an array of
//! unformatted memory allocated by the OpenGL context (aka: the
//! GPU). These can be used to store vertex data, pixel data retrieved
//! from images or the framebuffer, and a variety of other things.
// **************************************************************
template<typename T>
class GLBuffer
  : public IGLObject<GLenum>,
    public PendingContainer<T>
{
public:

  //! \brief Constructor with the object name
  GLBuffer(std::string const& name, const GLenum target,
           BufferUsage const usage = BufferUsage::DYNAMIC_DRAW)
    : IGLObject(name)
  {
    IGLObject::m_target = target;
    m_usage = static_cast<GLenum>(usage);
  }

  //! \brief Constructor with the object name
  GLBuffer(const char *name, const GLenum target,
           BufferUsage const usage = BufferUsage::DYNAMIC_DRAW)
    : IGLObject(name)
  {
    IGLObject::m_target = target;
    m_usage = static_cast<GLenum>(usage);
  }

  virtual ~GLBuffer() override
  {
    destroy();
  }

  inline GLenum usage() const
  {
    return m_usage;
  }

private:

  virtual bool create() override
  {
    DEBUG("VBO '%s' create", name().c_str());
    glCheck(glGenBuffers(1, &m_handle));
    return false;
  }

  virtual void release() override
  {
    DEBUG("VBO '%s' release", name().c_str());
    glCheck(glDeleteBuffers(1, &m_handle));
  }

  virtual void activate() override
  {
    DEBUG("VBO '%s' activate", name().c_str());
    glCheck(glBindBuffer(m_target, m_handle));
  }

  virtual void deactivate() override
  {
    DEBUG("VBO '%s' deactivate", name().c_str());
    glCheck(glBindBuffer(m_target, 0));
  }

  virtual bool setup() override
  {
    DEBUG("VBO '%s' setup", name().c_str());
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
    DEBUG("VBO '%s' update %u -> %u",
         name().c_str(), pos_start, pos_end);

    size_t offset = sizeof (T) * pos_start;
    size_t nbytes = sizeof (T) * (pos_end - pos_start + 1_z);
    glCheck(glBufferSubData(m_target,
                            static_cast<GLintptr>(offset),
                            static_cast<GLsizeiptr>(nbytes),
                            PendingContainer<T>::to_array()));
    return false;
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

  //! \brief Constructor with the object name
  GLVertexBuffer(std::string const& name,
                 BufferUsage const usage = BufferUsage::DYNAMIC_DRAW)
    : GLBuffer<T>(name, GL_ARRAY_BUFFER, usage)
  {
  }

  //! \brief Constructor with the object name
  GLVertexBuffer(const char *name,
                 BufferUsage const usage = BufferUsage::DYNAMIC_DRAW)
    : GLBuffer<T>(name, GL_ARRAY_BUFFER, usage)
  {
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
  GLIndexBuffer(std::string const& name,
                BufferUsage const usage = BufferUsage::DYNAMIC_DRAW)
    : GLBuffer<T>(name, GL_ELEMENT_ARRAY_BUFFER, usage)
  {
  }

  //! \brief Constructor with the object name
  GLIndexBuffer(const char *name,
                BufferUsage const usage = BufferUsage::DYNAMIC_DRAW)
    : GLBuffer<T>(name, GL_ELEMENT_ARRAY_BUFFER, usage)
  {
  }

  inline GLenum type() const;
};

template<>
inline GLenum GLIndexBuffer<uint32_t>::type() const { return GL_UNSIGNED_INT; }

template<>
inline GLenum GLIndexBuffer<uint16_t>::type() const { return GL_UNSIGNED_SHORT; }

template<>
inline GLenum GLIndexBuffer<uint8_t>::type() const { return GL_UNSIGNED_BYTE; }

#endif /* GLVERTEX_BUFFER_HPP_ */
