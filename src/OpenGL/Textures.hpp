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
//! \file GLTextures.hpp file implements:
//!   - GLTexture1D:
//!   - GLTexture2D:
//!   - GLTextureDepth2D:
//!   - GLTexture3D: which is a set of 2D Textures.
//!   - GLTextureCube: A 3D Texture specialized for rendering skybox.
// *****************************************************************************

#  include "OpenGL/GLObject.hpp"
#  include "Common/PendingContainer.hpp"

namespace glwrap
{

//! \brief Internal format storing texture data
using TextureData = PendingContainer<unsigned char>;

// *****************************************************************************
//! \brief Default options for textures when setup.
// *****************************************************************************
struct TextureOptions
{
  TextureMinFilter minFilter = TextureMinFilter::LINEAR;
  TextureMagFilter magFilter = TextureMagFilter::LINEAR;
  TextureWrap wrapS = TextureWrap::REPEAT;
  TextureWrap wrapT = TextureWrap::REPEAT;
  TextureWrap wrapR = TextureWrap::REPEAT;
  PixelType pixelType = PixelType::UNSIGNED_BYTE;
  bool generateMipmaps = false;
};

//! \brief Used by GLVAO
DECLARE_CLASS(GLTexture);

// *****************************************************************************
//! \brief Generic Texture.
//!
//! A texture is an OpenGL Object that contains one or more images
//! that all have the same image format. A texture can be used in two
//! ways: either it can be the source of a texture access from a
//! Shader, or it can be used as a render target (FrameBuffer). A
//! texture can be of dimension 1, 2 or 3.
// *****************************************************************************
class GLTexture
  : public GLObject<GLenum>
{
public:

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
  GLTexture(const uint8_t dimension, std::string const& name, const GLenum target)
    : GLObject(name),
      m_buffer(name),
      m_dimension(dimension)
  {
    m_target = target;
  }

  //----------------------------------------------------------------------------
  //! \brief Destructor. Release elements in CPU and GPU memories.
  //----------------------------------------------------------------------------
  virtual ~GLTexture()
  {
    destroy();
  }

  //----------------------------------------------------------------------------
  //! \brief Return the container holding texture data.
  //----------------------------------------------------------------------------
  inline TextureData& data()
  {
    return m_buffer;
  }

  //----------------------------------------------------------------------------
  //! \brief Return the container holding texture data.
  //----------------------------------------------------------------------------
  inline const TextureData& data() const
  {
    return m_buffer;
  }

  //----------------------------------------------------------------------------
  //! \brief \brief Allow to know if data have been transfered into
  //! the CPU memory.
  //!
  //! \return true if texture data have been loaded.
  //----------------------------------------------------------------------------
  virtual inline bool loaded() const
  {
    // Do not check m_width and m_height because FrameBuffer allows to have 0
    // sizes.
    return 0 != m_buffer.size();
  }

  //----------------------------------------------------------------------------
  //! \brief Change minifier and magnifier options.
  //! \return the reference of this instence.
  //----------------------------------------------------------------------------
  GLTexture& interpolation(TextureMinFilter const min_filter,
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
  GLTexture& wrap(TextureWrap const wrap)
  {
    m_options.wrapS = wrap;
    m_options.wrapT = wrap;
    m_options.wrapR = wrap;
    redoSetup();
    return *this;
  }

  //----------------------------------------------------------------------------
  //! \brief Change wrapping options for S, T and R.
  //! \return the reference of this instence.
  //----------------------------------------------------------------------------
  GLTexture& wrapSTR(TextureWrap const wrapS, TextureWrap const wrapT, TextureWrap const wrapR)
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
  GLTexture& options(TextureOptions const& options)
  {
    m_options = options;
    return *this;
  }

  //----------------------------------------------------------------------------
  //! \brief Return the texture dimension (1D, 2D, 3D).
  //----------------------------------------------------------------------------
  inline uint8_t dimension() const
  {
    return m_dimension;
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

protected:

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

    //TODO
    //GLfloat borderColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f};
    //glCheck(glTexParameterfv(m_target, GL_TEXTURE_BORDER_COLOR, borderColor));
  }

private:

  //----------------------------------------------------------------------------
  //! \brief Check if CPU data of the texture have been modified.
  //! If this is the case "dirty" data will be transfered to the GPU.
  //----------------------------------------------------------------------------
  virtual inline bool needUpdate() const override
  {
    return m_buffer.hasPendingData();
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
    m_buffer.clear();
    m_width = m_height = m_depth = 0;
  }

protected:

  //! \brief Options to pass to OpenGL
  TextureOptions m_options;
  //! \brief Hold the texture (CPU side)
  TextureData    m_buffer;
  //! \brief For Texture1D, Texture2D, Texture3D, TextureCube
  uint32_t       m_width = 0;
  //! \brief For Texture2D, Texture3D, TextureCube
  uint32_t       m_height = 0;
  //! \brief For Texture3D, TextureCube
  uint8_t        m_depth = 0u;
  //! \brief Desired format of texture once loaded from the picture file (CPU
  //! side).  \note Beware all format are not supported by loaders: ie SOIL only
  //! manages RGB, RGBA, luminance greyscale and luminance with alpha.
  PixelFormat    m_cpuPixelFormat = PixelFormat::RGBA;
  //! \brief Desired format of texture once loaded into the GPU.
  PixelFormat    m_gpuPixelFormat = PixelFormat::RGBA;

private:

  //! \brief Texture1D, Texture2D, Texture3D, TextureCube
  const uint8_t  m_dimension;
};

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_GLTEXTURES_HPP
