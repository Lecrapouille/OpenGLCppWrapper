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

#ifndef OPENGLCPPWRAPPER_GLFRAMEBUFFERS_HPP
#define OPENGLCPPWRAPPER_GLFRAMEBUFFERS_HPP

#  include "IGLObject.hpp"

namespace glwrap
{

// **************************************************************
//
// **************************************************************
class GLRenderBuffer
  : public IGLObject<GLenum>
{
public:

  GLRenderBuffer(const uint32_t width,
                 const uint32_t height,
                 const GLenum format)
    : IGLObject()
  {
    m_width = width;
    m_height = height;
    m_format = format;
    m_target = GL_RENDERBUFFER;
    m_need_resize = true;
  }

  inline uint32_t width() const
  {
    return m_width;
  }

  inline uint32_t height() const
  {
    return m_height;
  }

  void resize(const uint32_t width,
              const uint32_t height)
  {
    if ((m_width != width) || (m_height != height))
      {
        m_width = width;
        m_height = height;
        m_need_resize = true;
      }
  }

private:

  virtual bool create() override
  {
    m_handle = glGenRenderbuffers(1);
    return false;
  }

  virtual void release() override
  {
    glDeleteRenderbuffer(m_handle);
  }

  virtual bool setup() override
  {
    return false;
  }

  virtual void activate() override
  {
    glBindRenderbuffer(m_target, m_handle);
    if (m_need_resize)
      {
        glRenderbufferStorage(m_target, m_format,
                              m_width, m_height);
        m_need_resize = false;
      }
  }

  virtual bool update() override
  {
    return false;
  }

  virtual void deactivate() override
  {
    glBindRenderbuffer(m_target, 0);
  }

private:

  uint32_t m_width;
  uint32_t m_height;
  GLenum   m_format;
  bool     m_need_resize;
};

// **************************************************************
//
// **************************************************************
class GLColorBuffer
  : public GLRenderBuffer
{
public:

  GLColorBuffer(const uint32_t width,
                const uint32_t height,
                const GLenum format = GL_RGBA)
    : GLRenderBuffer(width, height, format)
  {
  }
};

// **************************************************************
//
// **************************************************************
class GLDepthBuffer
  : public GLRenderBuffer
{
public:

  GLDepthBuffer(const uint32_t width,
                const uint32_t height,
                const GLenum format = GL_DEPTH_COMPONENT)
    : GLRenderBuffer(width, height, format)
  {
  }
};

// **************************************************************
//
// **************************************************************
class GLStencilBuffer
  : public GLRenderBuffer
{
public:

  GLStencilBuffer(const uint32_t width,
                  const uint32_t height,
                  const GLenum format = GL_STENCIL_INDEX8)
    : GLRenderBuffer(width, height, format)
  {
  }
};

// **************************************************************
//
// **************************************************************
class GLFrameBuffer
  : public IGLObject<GLenum>
{
public:

  GLFrameBuffer()
    : IGLObject()
  {
  }

  void addColorTexture(GLTexture2D const& texture)
  {
  }

  void addDepthTexture(GLTexture2D const& texture)
  {
  }

  void addStencilTexture(GLTexture2D const& texture)
  {
  }

  void addColorBuffer(GLColorBuffer const& color_buf)
  {
    throw_if_bad_dim(color_buf);
    m_color_buffers.push_back(color_buf);
    target = GL_COLOR_ATTACHMENT0 + m_color_buffers.size() - 1;
    m_pending_attachments.push_back(std::make_pair(target, &(m_color_buffers.back())));
    m_need_attach = true;
  }

  void addDepthBuffer(GLDepthBuffer const& depth_buf)
  {
    throw_if_bad_dim(depth_buf);
    m_depth_buffer = depth_buf;
    target = GL_DEPTH_ATTACHMENT;
    m_pending_attachments.push_back(std::make_pair(target, &m_depth_buffer));
    m_need_attach = true;
  }

  void addStencilBuffer(GLStencilBuffer const& stencil_buf)
  {
    throw_if_bad_dim(stencil_buf);
    m_stencil_buffer = stencil_buf;
    target = GL_STENCIL_ATTACHMENT;
    m_pending_attachments.push_back(std::make_pair(target, &m_stencil_buffer));
    m_need_attach = true;
  }

  inline uint32_t width() const
  {
    return m_width;
  }

  inline uint32_t height() const
  {
    return m_height;
  }

  void resize(const uint32_t width, const uint32_t height)
  {
    m_width = width;
    m_height = height;
    for (auto& it: m_color_buffers)
      {
        it.resize(width, height);
      }
    if (m_has_depth)
      {
        m_depth_buffer.resize(width, height);
      }
    if (m_has_stencil)
      {
        m_stencil_buffer.resize(width, height);
      }
    //TODO for (auto& it: m_color_textures)
    //  {
    //    it.resize(width, height);
    //  }
    //TODO for (auto& it: m_depth_textures)
    //  {
    //    it.resize(width, height);
    //  }
  }

private:

  virtual bool create() override
  {
    glGenFramebuffers(1, &m_handle);
    return false;
  }

  virtual void release() override
  {
    glDeleteFramebuffer(m_handle);
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
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  virtual void activate() override
  {
    glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
    if (m_need_attach)
      {
        do_attach();
        m_need_attach = false;
      }
  }

  void do_attach()
  {
    for (auto &it: m_pending_data)
      {
        auto pair = m_pending_data.pop_back();
        if ()
          {
            it.begin();
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment,
                                      GL_RENDERBUFFER, it.gpuID());
            it.end();
          }
        else if ()
          {
            it.begin();
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachment,
                                   it.target(), it.gpuID(), 0);
            it.end();
          }
        else
          {
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment,
                                      GL_RENDERBUFFER, 0);
          }
      }
    throw_if_frame_buffer_failed();
  }

  void throw_if_bad_dim(GLRenderBuffer const& buf)
  {
    if ((m_width != 0) && (m_width != color_buf.width))
      {
        throw exception("Buffer width does not match");
      }
    if ((m_height != 0) && (m_height != color_buf.height))
      {
        throw exception("Buffer height does not match");
      }
  }

  void throw_if_frame_buffer_failed()
  {
    GLenum res = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch (res)
      {
      case GL_FRAMEBUFFER_COMPLETE:
        /* success */
        break;
      case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        throw exception("FrameBuffer attachments are incomplete");
        break;
      case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        throw exception("No valid attachments in the FrameBuffer");
        break;
      case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
        throw exception("attachments do not have the same width and height");
        break;
      case GL_FRAMEBUFFER_INCOMPLETE_FORMATS:
        throw exception(")Internal format of attachment is not renderable");
        break;
      case GL_FRAMEBUFFER_UNSUPPORTED:
        throw exception("Combination of internal formats used by attachments is not supported");
        break;
      case 0:
      default:
        throw exception("Target not equal to GL_FRAMEBUFFER");
        break;
      }
  }

private:

  std::vector<GLColorBuffer>   m_color_buffers;
  std::vector<GLRenderBuffer*> m_pending_data;
  GLDepthBuffer                m_depth_buffer;
  GLStencilBuffer              m_stencil_buffer;
  uint32_t                     m_width;
  uint32_t                     m_height;
  bool                         m_need_attach;
};

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_GLFRAMEBUFFERS_HPP
