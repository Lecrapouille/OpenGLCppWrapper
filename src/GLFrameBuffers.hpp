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

#ifndef OPENGLCPPWRAPPER_GLFRAMEBUFFER_HPP
#define OPENGLCPPWRAPPER_GLFRAMEBUFFER_HPP

#  include "GLTextures.hpp"
#  include <vector>

namespace glwrap
{

// *****************************************************************************
//! \brief Base class for render buffer object.
// *****************************************************************************
class GLRenderBuffer
  : public IGLObject<GLenum>
{
public:

  //----------------------------------------------------------------------------
  //! \brief Constructor. This constructor does no actions.
  //!
  //! \param name Name of the buffer needed for hash table.
  //! \param width Buffer width (pixels)
  //! \param height Buffer height (pixel)
  //! \param format Buffer format
  //----------------------------------------------------------------------------
  GLRenderBuffer(std::string const& name,
                 const uint32_t width,
                 const uint32_t height,
                 const GLenum attachment,
                 const GLenum format)
    : IGLObject(name)
  {
    m_width = width;
    m_height = height;
    m_attachment = attachment;
    m_format = format;
    m_target = GL_RENDERBUFFER;
  }

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  virtual void attach() = 0;

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  virtual void draw() const
  {}

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  virtual ~GLRenderBuffer()
  {
    destroy();
  }

  //----------------------------------------------------------------------------
  //! \brief Return the Buffer width (pixels).
  //----------------------------------------------------------------------------
  inline uint32_t width() const
  {
    return m_width;
  }

  //----------------------------------------------------------------------------
  //! \brief Return the Buffer height (pixels).
  //----------------------------------------------------------------------------
  inline uint32_t height() const
  {
    return m_height;
  }

  //----------------------------------------------------------------------------
  //! \brief Resize the buffer.
  //!
  //! This operation is deferred (applied at next cycle).
  //!
  //! \fixme Width/height should be checked against maximum size
  //! \param width New buffer width (pixels)
  //! \param height New buffer height (pixel)
  //----------------------------------------------------------------------------
  void resize(const uint32_t width, const uint32_t height)
  {
    if ((m_width != width) || (m_height != height))
      {
        m_width = width;
        m_height = height;
        redoSetup();
      }
  }

private:

  //----------------------------------------------------------------------------
  //! \brief Create buffer on GPU.
  //!
  //! \return false to avoid calling again this method.
  //----------------------------------------------------------------------------
  virtual bool create() override
  {
    glCheck(glGenRenderbuffers(1, &m_handle));
    return false;
  }

  //----------------------------------------------------------------------------
  //! \brief Set the buffer as active buffer.
  //----------------------------------------------------------------------------
  virtual void activate() override
  {
    glCheck(glBindRenderbuffer(m_target, m_handle));
  }

  //----------------------------------------------------------------------------
  //! \brief Apply new size.
  //!
  //! \return false to avoid calling again this method.
  //----------------------------------------------------------------------------
  virtual bool setup() override
  {
    glCheck(glRenderbufferStorage(m_target, m_format, m_width, m_height));
    return false;
  }

  //----------------------------------------------------------------------------
  //! \brief No action is made (dummy function).
  //!
  //! \return false to avoid calling again this method.
  //----------------------------------------------------------------------------
  virtual bool update() override
  {
    return false;
  }

  //----------------------------------------------------------------------------
  //! \brief Unbind the buffer.
  //----------------------------------------------------------------------------
  virtual void deactivate() override
  {
    glCheck(glBindRenderbuffer(m_target, 0));
  }

  //----------------------------------------------------------------------------
  //! \brief Release GPU memory.
  //----------------------------------------------------------------------------
  virtual void release() override
  {
    glCheck(glDeleteRenderbuffers(1, &m_handle));
  }

protected:

  uint32_t m_width;
  uint32_t m_height;
  GLenum   m_attachment;
  GLenum   m_format;
};

// *****************************************************************************
//! \brief GLRenderBuffer holding a texture.
// *****************************************************************************
class GLTextureBuffer
  : public GLRenderBuffer
{
public:

  GLTextureBuffer(std::string const& name,
                  const uint32_t width,
                  const uint32_t height,
                  const GLenum attachment,
                  const PixelFormat format = PixelFormat::RGBA)
    : GLRenderBuffer(name, width, height, attachment, static_cast<GLenum>(format)),
      m_texture(name, width, height)
  {}

  inline GLTexture2D& texture()
  {
    return m_texture;
  }

  virtual void attach() override
  {
    glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, m_attachment,
                                   m_texture.target(), m_texture.gpuID(), 0));
  }

private:

  virtual bool create() override
  {
    m_texture.begin();
    return false;
  }

  virtual void activate() override
  {
    m_texture.begin();
  }

  virtual bool setup() override
  {
    return false;
  }

  virtual bool update() override
  {
    return false;
  }

  virtual void deactivate() override
  {
    m_texture.end();
  }

  virtual void release() override
  {}

private:

  GLTexture2D m_texture;
};

// *****************************************************************************
//! \brief Color buffer object.
// *****************************************************************************
class GLColorBuffer
  : public GLRenderBuffer
{
public:

  GLColorBuffer(std::string const& name,
                const uint32_t width,
                const uint32_t height,
                const GLenum attachment,
                const PixelFormat format = PixelFormat::RGBA)
    : GLRenderBuffer(name, width, height, attachment, static_cast<GLenum>(format))
  {}

  virtual void attach() override
  {
    glCheck(glFramebufferRenderbuffer(GL_FRAMEBUFFER, m_attachment, m_target, m_handle));
  }

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  virtual void draw() const override
  {
    glCheck(glDrawBuffers(1, &m_attachment));
  }
};

// *****************************************************************************
//! \brief Depth buffer object.
// *****************************************************************************
class GLDepthBuffer
  : public GLRenderBuffer
{
public:

  GLDepthBuffer(std::string const& name,
                const uint32_t width,
                const uint32_t height,
                const PixelFormat format = PixelFormat::DEPTH_COMPONENT)
    : GLRenderBuffer(name, width, height, GL_DEPTH_ATTACHMENT, static_cast<GLenum>(format))
  {}

  virtual void attach() override
  {
    glCheck(glFramebufferRenderbuffer(GL_FRAMEBUFFER, m_attachment, m_target, m_handle));
  }
};

// *****************************************************************************
//! \brief Stencil buffer object.
// *****************************************************************************
class GLStencilBuffer
  : public GLRenderBuffer
{
public:

  GLStencilBuffer(std::string const& name,
                  const uint32_t width,
                  const uint32_t height,
                  const PixelFormat format = PixelFormat::STENCIL_INDEX)
    : GLRenderBuffer(name, width, height, GL_STENCIL_ATTACHMENT, static_cast<GLenum>(format))
  {}

  virtual void attach() override
  {
    glCheck(glFramebufferRenderbuffer(GL_FRAMEBUFFER, m_attachment, m_target, m_handle));
  }
};

// *****************************************************************************
//! \class GLFrameBuffer GLFrameBuffers.hpp
//!
//! \brief A framebuffer is a collection of buffers that can be used
//! as the destination for rendering.
//!
//! A framebuffer has at least one buffer (color, depth or stencil buffer).
//! It has one or several color buffers, zero or one depth buffer and zero
//! or one stencil buffer.
// *****************************************************************************
class GLFrameBuffer
  : public IGLObject<GLenum>
{
public:

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  GLFrameBuffer(std::string const& name)
    : IGLObject(name)
  {
    m_target = GL_FRAMEBUFFER;
  }

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  GLFrameBuffer(std::string const& name,
                const uint32_t width, const uint32_t height,
                const uint8_t nb_colors = 1u, // FIXME: use enum to detect error at compile-time
                const bool with_depth = true, const bool with_stencil = false)
    : IGLObject(name)
  {
    m_target = GL_FRAMEBUFFER;
    m_width = width;
    m_height = height;

    if (likely(nb_colors <= 16))
      {
        uint8_t i = nb_colors;
        while (i--)
          {
            createColorBuffer();
          }

        if (with_depth)
          {
            createDepthBuffer();
          }

        if (with_stencil)
          {
            createStencilBuffer();
          }
      }
    else
      {
        throw OpenGLException("FrameBuffer cannot hold more than 16 color buffers");
      }
  }

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  virtual ~GLFrameBuffer()
  {
    destroy();
  }

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  template<typename Functor>
  void render(Functor functor)
  {
    begin();
    functor();
    end();
  }

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  template<typename Functor>
  void render(const uint32_t x, const uint32_t y,
              const uint32_t width, const uint32_t height,
              Functor functor)
  {
    begin();
    glCheck(glViewport(static_cast<GLint>(x), static_cast<GLint>(y),
                       static_cast<GLsizei>(width), static_cast<GLsizei>(height)));
    functor();
    end();
  }

  //----------------------------------------------------------------------------
  //! \brief Resize all buffer sizes.
  //! \param width New buffer width (pixels)
  //! \param height New buffer height (pixel)
  //----------------------------------------------------------------------------
  GLFrameBuffer& resize(const uint32_t width, const uint32_t height)
  {
    m_width = width;
    m_height = height;

    for (auto& it: m_color_buffers) {
      it->resize(width, height);
    }

    if (nullptr != m_depth_buffer) {
      m_depth_buffer->resize(width, height);
    }

    if (nullptr != m_stencil_buffer) {
      m_stencil_buffer->resize(width, height);
    }

    return *this;
  }

  //----------------------------------------------------------------------------
  //! \brief Return the Buffer width (pixels).
  //----------------------------------------------------------------------------
  inline uint32_t width() const
  {
    return m_width;
  }

  //----------------------------------------------------------------------------
  //! \brief Return the Buffer height (pixels).
  //----------------------------------------------------------------------------
  inline uint32_t height() const
  {
    return m_height;
  }

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  GLTexture2D& createColorTexture()
  {
    throw_if_reached_max_buffers();
    const GLenum id = static_cast<GLenum>(m_color_buffers.size());
    const GLenum attachment = GL_COLOR_ATTACHMENT0 + id;
    const std::string name("ColorTexture" + std::to_string(id));

    GLTextureBuffer* buf = new GLTextureBuffer(name, m_width, m_height, attachment);
    m_color_buffers.push_back(buf);
    m_pending_attachments.push_back(buf);
    forceSetup();
    return buf->texture();
  }

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  GLColorBuffer& createColorBuffer()
  {
    throw_if_reached_max_buffers();
    const GLenum id = static_cast<GLenum>(m_color_buffers.size());
    const GLenum attachment = GL_COLOR_ATTACHMENT0 + id;
    const std::string name("ColorBuffer" + std::to_string(id));

    GLColorBuffer* buf = new GLColorBuffer(name, m_width, m_height, attachment);
    m_color_buffers.push_back(buf); // TODO: max 16 elements
    m_pending_attachments.push_back(buf);
    forceSetup();
    return *buf;
  }

  //----------------------------------------------------------------------------
  //! \brief
  //! \fixme use smart pointers
  //----------------------------------------------------------------------------
  GLDepthBuffer& getDepthBuffer()
  {
    if (unlikely(nullptr == m_depth_buffer))
      {
        m_depth_buffer = new GLDepthBuffer("DepthBuffer", m_width, m_height);
        m_pending_attachments.push_back(m_depth_buffer);
        forceSetup();
      }
    return *m_depth_buffer;
  }

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  GLStencilBuffer& getStencilBuffer()
  {
    if (unlikely(nullptr == m_stencil_buffer))
      {
        m_stencil_buffer = new GLStencilBuffer("StencilBuffer", m_width, m_height);
        m_pending_attachments.push_back(m_stencil_buffer);
        forceSetup();
      }
    return *m_stencil_buffer;
  }

  //----------------------------------------------------------------------------
  //! \brief Alias method for getDepthBuffer()
  //----------------------------------------------------------------------------
  inline GLDepthBuffer& createDepthBuffer()
  {
    return getDepthBuffer();
  }

  //----------------------------------------------------------------------------
  //! \brief Alias method for getStencilBuffer()
  //----------------------------------------------------------------------------
  inline GLStencilBuffer& createStencilBuffer()
  {
    return getStencilBuffer();
  }

private:

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  virtual bool create() override
  {
    glCheck(glGenFramebuffers(1, &m_handle));
    return false;
  }

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  virtual void activate() override
  {
    glCheck(glBindFramebuffer(m_target, m_handle));
  }

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  virtual bool setup() override
  {
    if (likely(checkNumberOfBuffers()))
      {
        for (auto& it: m_pending_attachments)
          {
            glCheck(glClearColor(1.0f, 0.0f, 0.4f, 0.0f));
            DEBUG("Framebuffer '%s' is attaching '%s'", cname(), it->cname());
            it->begin();
            it->attach();
            it->draw();
            it->end();
          }
        m_pending_attachments.clear();
        forceUpdate();
        return false;
      }
    else
      {
        ERROR("Framebuffer '%s' needs at least one image attached to it", cname());
      }
    return true;
  }

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  virtual bool update() override
  {
    GLenum res = glCheck(glCheckFramebufferStatus(m_target));
    switch (res)
      {
      case GL_FRAMEBUFFER_COMPLETE:
        /* success */
        break;
      case GL_FRAMEBUFFER_UNDEFINED:
        throw OpenGLException("Framebuffer '" + name() + "' is undefined");
      case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        throw OpenGLException("FrameBuffer '" + name() + "' has incomplete attachments");
      case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        throw OpenGLException("Framebuffer '" + name() + "' does not have at least one image attached to it");
      case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        throw OpenGLException("FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
      case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        throw OpenGLException("FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
      case GL_FRAMEBUFFER_UNSUPPORTED:
        throw OpenGLException("Framebuffer '" + name() + "' has a combination of internal formats used by attachments is not supported");
      case 0:
      default:
        throw OpenGLException("Framebuffer '" + name() + "' has its target not equal to GL_FRAMEBUFFER");
      }
    return false;
  }

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  virtual void deactivate() override
  {
    glCheck(glBindFramebuffer(m_target, 0));
  }

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  virtual void release() override
  {
    glCheck(glDeleteFramebuffers(1, &m_handle));

    m_pending_attachments.clear();
    m_color_buffers.clear();
    m_depth_buffer = nullptr;
    m_stencil_buffer = nullptr;
    m_width = 0;
    m_height = 0;
  }

  //----------------------------------------------------------------------------
  //! \brief Check if the framebuffer has at least one render buffer.
  //! \throw OpenGLException if the framebuffer has not at least one render buffer.
  //----------------------------------------------------------------------------
  bool checkNumberOfBuffers() const
  {
    const size_t count = m_color_buffers.size() +
      ((nullptr == m_depth_buffer) ? 0_z : 1_z) +
      ((nullptr == m_stencil_buffer) ? 0_z : 1_z);

    return (count >= 1_z);
  }

  //----------------------------------------------------------------------------
  //! \brief Check if the max number of render buffers is not reached.
  //!
  //! Frame buffers can hold up to 16 render buffers.
  //!
  //! \throw OpenGLException is 16 render buffers is reached.
  //----------------------------------------------------------------------------
  void throw_if_reached_max_buffers()
  {
    if (m_color_buffers.size() >= 16_z) {
      throw OpenGLException("Reached the maximum number of render buffers");
    }
  }

private:

  //! \brief GLColorBuffer or GLTexture2D
  std::vector<GLRenderBuffer*> m_color_buffers;  // at least one buffer
  GLDepthBuffer*               m_depth_buffer = nullptr;   // 0 or 1 buffer
  GLStencilBuffer*             m_stencil_buffer = nullptr; // 0 or 1 buffer
  std::vector<GLRenderBuffer*> m_pending_attachments;
  uint32_t                     m_width = 0;
  uint32_t                     m_height = 0;
};

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_GLFRAMEBUFFER_HPP
