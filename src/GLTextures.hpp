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

#ifndef GLTEXTURES_HPP
#  define GLTEXTURES_HPP

#  include "IGLObject.tpp"
#  include "PendingData.hpp"
#  include "SOIL/SOIL.h"
#  include <array>

// **************************************************************
//
// **************************************************************
struct TextureOptions
{
  TextureMinFilter minFilter = TextureMinFilter::LINEAR;
  TextureMagFilter magFilter = TextureMagFilter::LINEAR;
  TextureWrap wrapS = TextureWrap::REPEAT;
  TextureWrap wrapT = TextureWrap::REPEAT;
  PixelFormat cpuPixelFormat = PixelFormat::RGBA;
  PixelFormat gpuPixelFormat = PixelFormat::RGBA;
  PixelType pixelType = PixelType::UNSIGNED_BYTE;
  bool generateMipmaps = false;
};

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

  virtual inline bool loaded() const = 0;

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

  virtual uint8_t dimension() const
  {
    return 0u;
  }

  inline uint32_t width() const
  {
    return m_width;
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

protected:

  TextureOptions m_options;
  uint32_t m_width = 0;
};

// **************************************************************
//!
// **************************************************************
class GLTexture2D: public IGLTexture
{
  friend class GLTexture3D;

  struct SOILDeleter
  {
    void operator()(unsigned char* buf)
    {
      DEBUG("Texture deleter");
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

  virtual uint8_t dimension() const override
  {
    return 2u;
  }

  inline uint32_t height() const
  {
    return m_height;
  }

  virtual inline bool loaded() const override
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

    DEBUG("Loading texture '%s'", filename);

    // FIXME: SOIL_LOAD_RGBA should adapt from moptions.cpuPixelFormat
    TextBufPtr buf(SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGBA));
    m_buffer = std::move(buf);

    // Success
    if (likely(nullptr != m_buffer.get()))
      {
        m_width = static_cast<uint32_t>(width);
        m_height = static_cast<uint32_t>(height);
        DEBUG("texture dimension %ux%u", m_width, m_height);
        if (rename || name().empty())
          {
            name() = filename;
            DEBUG("Renaming texture '%s'", filename);
          }
        PendingData::tagAsPending(0_z, m_width * m_height);
        DEBUG("Successfuly load picture file '%s'", filename);
        res = true;
      }
    else
      {
        ERROR("Failed loading picture file '%s'", filename);
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

private:

  inline void doGLTexImage2D() const
  {
    glCheck(glTexImage2D(m_target, 0,
                         static_cast<GLint>(m_options.gpuPixelFormat),
                         static_cast<GLsizei>(m_width),
                         static_cast<GLsizei>(m_height),
                         0,
                         static_cast<GLenum>(m_options.cpuPixelFormat),
                         static_cast<GLenum>(m_options.pixelType),
                         m_buffer.get()));
  }

  virtual bool setup() override
  {
    DEBUG("Texture '%s' setup", name().c_str());

    // Note: m_buffer can nullptr
    if (unlikely((0u == m_width) || (0u == m_height)))
      {
        ERROR("Cannot setup texture '%s' with width or height set to 0", name().c_str());
        return true;
      }

    applyTextureParam();
    glBindTexture(m_target, m_handle);
    doGLTexImage2D();
    return false;
  }

  virtual bool update() override
  {
    DEBUG("Texture '%s' update", name().c_str());
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

private:

  uint32_t   m_height = 0;
  TextBufPtr m_buffer;
};

// **************************************************************
//!
// **************************************************************
class GLTextureDepth2D: public GLTexture2D
{
  GLTextureDepth2D()
    : GLTexture2D()
  {
    m_options.gpuPixelFormat = PixelFormat::DEPTH_COMPONENT;
    m_options.cpuPixelFormat = PixelFormat::DEPTH_COMPONENT;
  }

  GLTextureDepth2D(std::string const& name)
    : GLTexture2D(name)
  {
    m_options.gpuPixelFormat = PixelFormat::DEPTH_COMPONENT;
    m_options.cpuPixelFormat = PixelFormat::DEPTH_COMPONENT;
  }
};

// **************************************************************
//!
// **************************************************************
class GLTexture1D: public IGLTexture
{
  struct SOILDeleter
  {
    void operator()(unsigned char* buf)
    {
      DEBUG("Texture deleter");
      if (buf != nullptr)
        SOIL_free_image_data(buf);
    }
  };

  using TextBufPtr = std::unique_ptr<unsigned char, SOILDeleter>;

public:

  GLTexture1D()
    : IGLTexture(GL_TEXTURE_1D)
  {
  }

  GLTexture1D(std::string const& name)
    : IGLTexture(name, GL_TEXTURE_1D)
  {
  }

  virtual uint8_t dimension() const override
  {
    return 1u;
  }

  virtual inline bool loaded() const override
  {
    return nullptr != m_buffer.get();
  }

  virtual bool setup() override
  {
    if (unlikely(0 == m_width))
      {
        ERROR("Cannot setup texture '%s' with width set to 0", name().c_str());
        return true;
      }

    applyTextureParam();
    glBindTexture(m_target, m_handle);
    glCheck(glTexImage1D(m_target, 0,
                         static_cast<GLint>(m_options.gpuPixelFormat),
                         static_cast<GLsizei>(m_width),
                         0,
                         static_cast<GLenum>(m_options.cpuPixelFormat),
                         static_cast<GLenum>(m_options.pixelType),
                         nullptr));
    return false;
  }

  virtual bool update() override
  {
    DEBUG("Texture '%s' update", name().c_str());
    size_t pos_start;
    size_t pos_end;
    PendingData::getPendingData(pos_start, pos_end);

    // FIXME: pour le moment on envoie toute la texture entiere
    // au lieu de la portion modifiee.
    // TODO pendingData --> x,width
    const GLint x = 0U;
    const GLsizei width = static_cast<GLsizei>(m_width);

    glCheck(glBindTexture(m_target, m_handle));
    glCheck(glTexSubImage1D(m_target, 0, x, width,
                            static_cast<GLenum>(m_options.cpuPixelFormat),
                            static_cast<GLenum>(m_options.pixelType),
                            m_buffer.get()));

    PendingData::clearPending();
    return false;
  }

private:

  TextBufPtr m_buffer;
};

// **************************************************************
//!
// **************************************************************
class GLTexture3D: public IGLTexture
{
public:

  GLTexture3D()
    : IGLTexture(GL_TEXTURE_CUBE_MAP)
  {
  }

  GLTexture3D(std::string const& name)
    : IGLTexture(name, GL_TEXTURE_CUBE_MAP)
  {
  }

  virtual uint8_t dimension() const override
  {
    return 3u;
  }

  inline uint32_t depth() const
  {
    return m_depth;
  }

  virtual inline bool loaded() const override
  {
    for (uint8_t i = 0; i < 6u; ++i)
      {
        if (false == m_textures[i].loaded())
          return false;
      }
    return true;
  }

  bool load(CubeMap const target, const char *const filename)
  {
    return m_textures[static_cast<int>(target)].load(filename, false);
  }

private:

  virtual bool setup() override
  {
    // Note: m_buffer can nullptr
    m_depth = computeDepth();
    if (6u != m_depth)
      {
        ERROR("Cannot setup texture '%s' with width or height set to 0"
              "or depth != 6", name().c_str());
        return true;
      }

    applyTextureParam();
    glBindTexture(m_target, m_handle);
    for (uint8_t i = 0; i < 6u; ++i)
      {
        m_textures[i].m_handle = m_targets[i];
        m_textures[i].options(m_options);
        m_textures[i].doGLTexImage2D();
      }
    return true;
  }

  virtual bool update() override
  {
    glCheck(glBindTexture(m_target, m_handle));
    for (uint8_t i = 0; i < 6u; ++i)
      {
        m_textures[i].update();
      }
    return false;
  }

  uint8_t computeDepth()
  {
    uint8_t depth = 0u;
    for (uint8_t i = 0; i < 6u; ++i)
      {
        if (m_textures[i].loaded())
          ++depth;
      }
    return depth;
  }

private:

  // TODO: static
  // https://stackoverflow.com/questions/11709859/how-to-have-static-data-members-in-a-header-only-library
  const std::array<GLenum,6> m_targets {{
    GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
  }};

  uint8_t m_depth = 0u;
  std::array<GLTexture2D, 6> m_textures;
};



#endif /* GLTEXTURES_HPP */
