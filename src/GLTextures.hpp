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

#ifndef OPENGLCPPWRAPPER_GLTEXTURES_HPP
#  define OPENGLCPPWRAPPER_GLTEXTURES_HPP

// *****************************************************************************
//! \file GLTextures.hpp
// *****************************************************************************

#  include "IGLObject.hpp"
#  include "PendingContainer.hpp"
#  include "SOIL/SOIL.h"
#  include <array>

namespace glwrap
{

// *****************************************************************************
//! \brief Default options for textures when setup.
// *****************************************************************************
struct TextureOptions
{
  TextureMinFilter minFilter = TextureMinFilter::LINEAR;
  TextureMagFilter magFilter = TextureMagFilter::LINEAR;
  TextureWrapS wrapS = TextureWrapS::REPEAT;
  TextureWrapT wrapT = TextureWrapT::REPEAT;
  TextureWrapR wrapR = TextureWrapR::REPEAT;
  GLfloat borderColor[4] = { 1.0f, 0.0f, 0.0f, 0.0f };
};

// *****************************************************************************
//! \brief Generic Texture.
//!
//! A texture is an OpenGL Object that contains one or more images
//! that all have the same image format. A texture can be used in two
//! ways: either it can be the source of a texture access from a
//! Shader, or it can be used as a render target (FrameBuffer). A
//! texture can be of dimension 1, 2 or 3.
// *****************************************************************************
template <class T>
class IGLTexture
  : public IGLObject<GLenum>
{
public:

  //! \brief Internal format storing texture data
  using TextureData = PendingContainer<T>;

  //----------------------------------------------------------------------------
  //! \brief Constructor. Should be used if you will load texture from
  //! a picture file (jpg, png ...) with the load() method.
  //!
  //! Give a name to the instance. This constructor makes no other
  //! actions.
  //!
  //! \param name the name of this instance used by GLProgram and GLVAO.
  //! \param target the texture type (GL_TEXTURE_1D .. GL_TEXTURE_3D ...)
  //----------------------------------------------------------------------------
  IGLTexture(std::string const& name, const GLenum target)
    : IGLObject(name)
  {
    m_target = target;
  }

  //----------------------------------------------------------------------------
  //! \brief Destructor. Release elements in CPU and GPU memories.
  //----------------------------------------------------------------------------
  virtual ~IGLTexture()
  {
    destroy();
  }

  //----------------------------------------------------------------------------
  //! \brief Return the container holding texture data.
  //----------------------------------------------------------------------------
  inline TextureData& data()
  {
    return m_texture;
  }

  //----------------------------------------------------------------------------
  //! \brief Return the container holding texture data.
  //----------------------------------------------------------------------------
  inline const TextureData& data() const
  {
    return m_texture;
  }

  //----------------------------------------------------------------------------
  //! \brief \brief Allow to know if data have been transfered into
  //! the CPU memory.
  //!
  //! \return true if texture data have been loaded.
  //----------------------------------------------------------------------------
  virtual inline bool loaded() const = 0;

  //----------------------------------------------------------------------------
  //! \brief Change minifier and magnifier options.
  //! \return the reference of this instence.
  //----------------------------------------------------------------------------
  IGLTexture& interpolation(TextureMinFilter const min_filter,
                            TextureMagFilter const mag_filter)
  {
    m_options.minFilter = min_filter;
    m_options.magFilter = mag_filter;
    redoSetup();
    return *this;
  }

  //----------------------------------------------------------------------------
  //! \brief Change wrapping options for S, T and R.
  //! \return the reference of this instence.
  //----------------------------------------------------------------------------
  IGLTexture& wrap(TextureWrap const wrap)
  {
    return wrapSTR(TextureWrapS(wrap), TextureWrapT(wrap), TextureWrapR(wrap));
  }

  //----------------------------------------------------------------------------
  //! \brief Change wrapping options for S, T and R.
  //! \return the reference of this instence.
  //----------------------------------------------------------------------------
  IGLTexture& wrapSTR(TextureWrapS const wrapS, TextureWrapT const wrapT,
                      TextureWrapR const wrapR)
  {
    m_options.wrapS = wrapS;
    m_options.wrapT = wrapT;
    m_options.wrapR = wrapR;
    redoSetup();
    return *this;
  }

  //----------------------------------------------------------------------------
  //! \brief Replace current texture settings by a new one.
  //! \return the reference of this instence.
  //----------------------------------------------------------------------------
  IGLTexture& options(TextureOptions const& options)
  {
    m_options = options;
    return *this;
  }

  IGLTexture& borderColor(GLfloat const borderColor[4])
  {
    uint8_t i = 4u;
    while (i--) {
      m_options.borderColor[i] = borderColor[i];
    }
    return *this;
  }

  IGLTexture& borderColor(GLfloat const r, GLfloat const g,
                          GLfloat const b, GLfloat const a)
  {
    m_options.borderColor[0] = r;
    m_options.borderColor[1] = g;
    m_options.borderColor[2] = b;
    m_options.borderColor[3] = a;
    return *this;
  }

  //----------------------------------------------------------------------------
  //! \brief Return the texture dimension (1D, 2D, 3D).
  //! FIXME can we use template ?
  //----------------------------------------------------------------------------
  virtual uint8_t dimension() const
  {
    return 0u;
  }

  //----------------------------------------------------------------------------
  //! \brief Return the texture width (in pixel).
  //!
  //! Available for all kind of texture.
  //----------------------------------------------------------------------------
  inline uint32_t width() const
  {
    return m_width;
  }

  //----------------------------------------------------------------------------
  //! \brief Return the texture height (in pixel).
  //!
  //! Only available for texture 2D and 3D.
  //----------------------------------------------------------------------------
  inline uint32_t height() const
  {
    return m_height;
  }

  //----------------------------------------------------------------------------
  //! \brief Return the texture depth (in number of textures 2D).
  //!
  //! Only available for texture 3D.
  //----------------------------------------------------------------------------
  inline uint32_t depth() const
  {
    return m_depth;
  }

  inline PixelFormat cpuPixelFormat() const
  {
    return m_cpuPixelFormat;
  }

  inline PixelFormat gpuPixelFormat() const
  {
    return m_gpuPixelFormat;
  }

  inline PixelType pixelType() const
  {
    return m_pixelType;
  }

protected:

  //----------------------------------------------------------------------------
  //! \brief Load picture file (jpg, png ...) with SOIL library into or internal
  //! format.
  //! \param[in] filename the path of the bitmap file (jpeg, png ...)
  //! \param[in] format
  //! \param[inout] texture the container holding bytes (or T) of the texture.
  //! \param[out] width Buffer width (pixels)
  //! \param[out] height Buffer height (pixel)
  //----------------------------------------------------------------------------
  bool doload2D(const char *const filename)
  {
    if (unlikely(nullptr == filename)) return false;
    DEBUG("Loading texture '%s'", filename);

    // Load the image as a C array.
    int w, h;
    uint8_t* image = SOIL_load_image(filename, &w, &h, 0, SOIL_LOAD_RGBA);
    if (likely(nullptr != image))
      {
        // Use the max because with framebuffer we can resize texture
        m_width = std::max(m_width, static_cast<uint32_t>(w)); // FIXME
        m_height = std::max(m_height, static_cast<uint32_t>(h));

        // Convert it as std::vector. Warning reinterpret_cast is not the
        // best idea ever !
        size_t size = static_cast<size_t>(w * h) * m_colorElts / sizeof(T);
        m_texture.append(reinterpret_cast<T*>(image), size); // FIXME: not working if already preallocated
        SOIL_free_image_data(image);
        DEBUG("Successfully loaded %ux%u texture '%s'", width, height, filename);
        return true;
      }
    else
      {
        ERROR("Failed loading picture file '%s'", filename);
        return false;
      }
  }

  //----------------------------------------------------------------------------
  //! \brief Configure pixel format for CPU and GPU depending on the value of
  //! the template T.
  //----------------------------------------------------------------------------
  inline void internalFormat();

  //----------------------------------------------------------------------------
  //! \brief Apply all settings to the texture.
  //----------------------------------------------------------------------------
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
    glCheck(glTexParameteri(m_target, GL_TEXTURE_WRAP_R,
                            static_cast<GLint>(m_options.wrapR)));
    glCheck(glTexParameterfv(m_target, GL_TEXTURE_BORDER_COLOR,
                             m_options.borderColor));
  }

private:

  //----------------------------------------------------------------------------
  //! \brief Check if CPU data of the texture have been modified.
  //! If this is the case "dirty" data will be transfered to the GPU.
  //----------------------------------------------------------------------------
  virtual inline bool needUpdate() const override
  {
    return m_texture.hasPendingData();
  }

  //----------------------------------------------------------------------------
  //! \brief Create a new OpenGL texture.
  //----------------------------------------------------------------------------
  virtual bool create() override
  {
    glCheck(glGenTextures(1U, &m_handle));
    return false;
  }

  //----------------------------------------------------------------------------
  //! \brief Bind the Texture to OpenGL.
  //----------------------------------------------------------------------------
  virtual void activate() override
  {
    glCheck(glBindTexture(m_target, m_handle));
  }

  //----------------------------------------------------------------------------
  //! \brief Unbind the Texture to OpenGL.
  //----------------------------------------------------------------------------
  virtual void deactivate() override
  {
    glCheck(glBindTexture(m_target, 0U));
  }

  //----------------------------------------------------------------------------
  //! \brief Destroy the OpenGL Texture.
  //----------------------------------------------------------------------------
  virtual void release() override
  {
    glCheck(glDeleteTextures(1U, &m_handle));
    m_texture.clear();
    m_width = m_height = m_depth = 0;
  }

protected:

  //! \brief Settings to apply on the texture
  TextureOptions m_options;
  PixelFormat    m_cpuPixelFormat;
  PixelFormat    m_gpuPixelFormat;
  PixelType      m_pixelType;
  size_t         m_colorElts;

  //! \brief Container holding bytes of the texture
  TextureData    m_texture;
  //! \brief For Texture1D, Texture2D, Texture3D, TextureCube
  uint32_t       m_width = 0;
  //! \brief For Texture2D, Texture3D, TextureCube
  uint32_t       m_height = 0;
  //! \brief For Texture3D, TextureCube
  uint8_t        m_depth = 0u;
};

template <>
inline void IGLTexture<float>::internalFormat()
{
  m_cpuPixelFormat = m_gpuPixelFormat = PixelFormat::RGBAF;
  m_pixelType = PixelType::FLOAT;
  m_colorElts = 4_z;
}

template <>
inline void IGLTexture<uint8_t>::internalFormat()
{
  m_cpuPixelFormat = m_gpuPixelFormat = PixelFormat::RGBA;
  m_pixelType = PixelType::UNSIGNED_BYTE;
  m_colorElts = 4_z;
}

template <>
inline void IGLTexture<int8_t>::internalFormat()
{
  m_cpuPixelFormat = m_gpuPixelFormat = PixelFormat::RGBA;
  m_pixelType = PixelType::BYTE;
  m_colorElts = 4_z;
}

template <>
inline void IGLTexture<uint16_t>::internalFormat()
{
  m_cpuPixelFormat = m_gpuPixelFormat = PixelFormat::RGBA;
  m_pixelType = PixelType::UNSIGNED_SHORT;
}

template <>
inline void IGLTexture<int16_t>::internalFormat()
{
  m_cpuPixelFormat = m_gpuPixelFormat = PixelFormat::RGBA;
  m_pixelType = PixelType::SHORT;
}

template <>
inline void IGLTexture<uint32_t>::internalFormat()
{
  m_cpuPixelFormat = m_gpuPixelFormat = PixelFormat::RGBA;
  m_pixelType = PixelType::UNSIGNED_INT;
  m_colorElts = 4_z;
}

template <>
inline void IGLTexture<int32_t>::internalFormat()
{
  m_cpuPixelFormat = m_gpuPixelFormat = PixelFormat::RGBA;
  m_pixelType = PixelType::INT;
  m_colorElts = 4_z;
}

#  include "GLTexture1D.hpp"
#  include "GLTexture2D.hpp"
#  include "GLTexture3D.hpp"
#  include "GLTextureCube.hpp"

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_GLTEXTURES_HPP
