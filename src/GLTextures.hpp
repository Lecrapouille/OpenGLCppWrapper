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

#ifndef GLTEXTURES_HPP_
#  define GLTEXTURES_HPP_

#  include "IGLObject.tpp"
#  include "PendingData.hpp"
#  include "SOIL/SOIL.h"

// **************************************************************
//
// **************************************************************
class IGLTexture
  : public IGLObject<GLenum>,
    protected PendingData
{
public:

  IGLTexture(const GLenum target)
    : IGLObject()
  {
    m_target = target;
  }

  IGLTexture(std::string const& name, const GLenum target)
    : IGLObject(name)
  {
    m_target = target;
  }

  virtual ~IGLTexture()
  {
    destroy();
  }

  void interpolation(TextureMinFilter const min_filter,
                     TextureMagFilter const mag_filter)
  {
    m_options.minFilter = min_filter;
    m_options.magFilter = mag_filter;
    redoSetup();
  }

  void wrapping(TextureWrap const wrap)
  {
    m_options.wrapS = wrap;
    m_options.wrapT = wrap;
    redoSetup();
  }

  void options(TextureOptions const& options)
  {
    m_options = options;
  }

protected:

  void applyTextureParam()
  {
    glCheck(glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER,
                            static_cast<GLint>(m_options.minFilter)));
    glCheck(glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER,
                            static_cast<GLint>(m_options.magFilter)));
    glCheck(glTexParameteri(m_target, GL_TEXTURE_WRAP_S,
                            static_cast<GLint>(m_options.wrapS)));
    glCheck(glTexParameteri(m_target, GL_TEXTURE_WRAP_T,
                            static_cast<GLint>(m_options.wrapT)));
    glCheck(glTexParameteri(m_target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
  }

private:

  virtual inline bool needUpdate() const override
  {
    return PendingData::hasPendingData();
  }

  virtual bool create() override
  {
    glCheck(glGenTextures(1U, &m_handle));
    return false;
  }

  virtual void release() override
  {
    glCheck(glDeleteTextures(1U, &m_handle));
  }

  virtual void activate() override
  {
    glCheck(glBindTexture(m_target, m_handle));
  }

  virtual void deactivate() override
  {
    glCheck(glBindTexture(m_target, 0U));
  }

  virtual bool setup() override
  {
    applyTextureParam();
    return false;
  }

protected:

  TextureOptions m_options;
};

// **************************************************************
//!
// **************************************************************
class GLTexture2D: public IGLTexture
{
  struct SOILDeleter
  {
    void operator()(unsigned char* buf)
    {
      std::cout << "Texture deleter" << std::endl;
      if (buf != nullptr)
        SOIL_free_image_data(buf);
    }
  };

  using TextBufPtr = std::unique_ptr<unsigned char, SOILDeleter>;

public:

  GLTexture2D()
    : IGLTexture(GL_TEXTURE_2D)
  {
  }

  GLTexture2D(std::string const& name)
    : IGLTexture(name, GL_TEXTURE_2D)
  {
  }

  virtual ~GLTexture2D()
  {
  }

  inline bool loaded() const
  {
    return nullptr != m_buffer.get();
  }

  inline bool load(std::string const& filename, const bool rename = false)
  {
    return load(filename.c_str(), rename);
  }

  bool load(const char *const filename, const bool rename = true)
  {
    int width, height;
    bool res;

    LOGI("Loading texture '%s'", filename);

    // FIXME: SOIL_LOAD_RGBA should adapt from moptions.cpuPixelFormat
    TextBufPtr buf(SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGBA));
    m_buffer = std::move(buf);

    // Success
    if (likely(nullptr != m_buffer.get()))
      {
        m_width = static_cast<uint32_t>(width);
        m_height = static_cast<uint32_t>(height);
        if (rename || name().empty())
          {
            name() = filename;
            LOGI("Renaming texture '%s'", filename);
          }
        PendingData::tagAsPending(0_z, m_width * m_height);
        LOGI("Successfuly load picture file '%s'", filename);
        res = true;
      }
    else
      {
        LOGES("Failed loading picture file '%s'", filename);
        res = false;
      }

    return res;
  }

  inline unsigned char& operator[](size_t nth)
  {
    //TBD ?
    //if (nth > m_width * m_height)
    //  {
    //    reserve(nth);
    //  }
    PendingData::tagAsPending(nth);
    return m_buffer.get()[nth];
  }

  inline const unsigned char& operator[](size_t nth) const
  {
    return m_buffer.get()[nth];
  }

  inline uint32_t width() const
  {
    return m_width;
  }

  inline uint32_t height() const
  {
    return m_height;
  }

private:

  virtual bool setup() override
  {
    LOGD("Texture '%s' setup", name().c_str());

    // Note: m_buffer can nullptr
    if (unlikely((0 == m_width) || (0 == m_height)))
      {
        LOGE("Cannot setup texture with width or hieght set to 0");
        return true;
      }

    applyTextureParam();
    glCheck(glTexImage2D(m_target, 0,
                         static_cast<GLint>(m_options.gpuPixelFormat),
                         static_cast<GLsizei>(m_width),
                         static_cast<GLsizei>(m_height),
                         0,
                         static_cast<GLenum>(m_options.cpuPixelFormat),
                         static_cast<GLenum>(m_options.pixelType),
                         m_buffer.get()));
    return false;
  }

  virtual bool update() override
  {
    LOGD("Texture '%s' update", name().c_str());
    size_t pos_start;
    size_t pos_end;
    PendingData::getPendingData(pos_start, pos_end);

    // FIXME: pour le moment on envoie toute la texture entiere
    // au lieu du rectangle modifie.
    // TODO pendingData --> x,y,width,height
    const GLint x = 0U;
    const GLint y = 0U;
    const GLsizei width = static_cast<GLsizei>(m_width);
    const GLsizei height = static_cast<GLsizei>(m_height);

    glCheck(glBindTexture(m_target, m_handle));
    glCheck(glTexSubImage2D(m_target, 0, x, y, width, height,
                            static_cast<GLenum>(m_options.cpuPixelFormat),
                            static_cast<GLenum>(m_options.pixelType),
                            m_buffer.get()));

    PendingData::clearPending();
    return false;
  };

  uint32_t m_width = 0;
  uint32_t m_height = 0;
  TextBufPtr m_buffer;
};

// **************************************************************
//!
// **************************************************************
/*class GLTextureDepth2D: public GLTexture2D
{
  GLTextureDepth2D()
    : GLTexture2D()
  {
    m_options.gpuPixelFormat = GL_DEPTH_COMPONENT; // FIXME incompatible avec load() ??
    m_options.cpuPixelFormat = GL_DEPTH_COMPONENT;
  }

  GLTextureDepth2D(std::string const& name)
    : GLTexture2D(name)
  {
    m_options.gpuPixelFormat = GL_DEPTH_COMPONENT;
    m_options.cpuPixelFormat = GL_DEPTH_COMPONENT;
  }
  };*/

#endif /* GLTEXTURES_HPP_ */
