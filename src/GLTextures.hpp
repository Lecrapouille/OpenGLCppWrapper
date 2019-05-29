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

// *****************************************************************************
//! \brief A 2D Texture.
// *****************************************************************************
template <class T>
class IGLTexture2D: public IGLTexture<T>
{
  //! \brief GLTextureCube is made of GLTexture2D. Let it access to
  //! GLTexture2D private states.
  friend class GLTextureCube;
  friend class GLTextureBuffer;

public:

  //----------------------------------------------------------------------------
  //! \brief Constructor. Should be used if you will load texture from
  //! a picture file (jpg, png ...) with the load() method.
  //!
  //! Give a name to the instance. This constructor makes no other
  //! actions.
  //!
  //! \param name the name of this instance used by GLProgram and GLVAO.
  //----------------------------------------------------------------------------
  IGLTexture2D(std::string const& name)
    : IGLTexture<T>(name, GL_TEXTURE_2D)
  {}

  //----------------------------------------------------------------------------
  //! \brief Constructor. Should be used if you will load texture from
  //! a frame buffer.
  //!
  //! Give a name to the instance. Get the texture size. This
  //! constructor makes no other actions.
  //!
  //! \param name the name of this instance used by GLProgram and GLVAO.
  //! \param target the texture type (GL_TEXTURE_1D .. GL_TEXTURE_3D ...)
  //! \param width Buffer width (pixels). Shall be > 0.
  //! \param height Buffer height (pixel). Shall be > 0.
  //----------------------------------------------------------------------------
  IGLTexture2D(std::string const& name, const uint32_t width, const uint32_t height)
    : IGLTexture<T>(name, GL_TEXTURE_2D)
  {
    // TODO Forbid texture with 0x0 dimension ?
    IGLTexture2D::m_width = width;
    IGLTexture2D::m_height = height;
  }

  //----------------------------------------------------------------------------
  //! \brief Destructor. Release elements in CPU and GPU memories.
  //----------------------------------------------------------------------------
  virtual ~IGLTexture2D()
  {}

  //----------------------------------------------------------------------------
  //! \brief Return the texture dimension: 2D.
  //----------------------------------------------------------------------------
  virtual uint8_t dimension() const override
  {
    return 2u;
  }

  //----------------------------------------------------------------------------
  //! \brief Allow to know if data have been transfered into the CPU
  //! memory.
  //!
  //! \return true if texture data have been loaded (a jpeg, png, bmp
  //! file have been loaded)
  //----------------------------------------------------------------------------
  virtual inline bool loaded() const override
  {
    return (0 != IGLTexture2D::m_texture.size()) // Texture loaded from a file (jpeg ...)
      || ((0 != IGLTexture2D::m_width) && (0 != IGLTexture2D::m_height)); // Or dummy texture for framebuffer
  }

  //----------------------------------------------------------------------------
  //! \brief Load a jpeg, png, bmp ... file as texture.
  //!
  //! \param filename the path of the jpeg, png, bmp file.
  //! \param rename if set to true then the name of this instance (set
  //! in the constructor) takes the name of the file. Else, the
  //! instance name is not mdified.
  //!
  //! \return true if texture data have been loaded.
  //----------------------------------------------------------------------------
  inline bool load(std::string const& filename)
  {
    return IGLTexture2D::load(filename.c_str());
  }

  //----------------------------------------------------------------------------
  //! \brief Load a jpeg, png, bmp ... file as texture.
  //!
  //! \param filename the path of the jpeg, png, bmp file.
  //! \param rename if set to true then the name of this instance (set
  //! in the constructor) takes the name of the file. Else, the
  //! instance name is not mdified.
  //!
  //! \return true if texture data have been loaded.
  //----------------------------------------------------------------------------
  inline bool load(const char *const filename)
  {
    DEBUG("Texture2D '%s' load bitmap '%s'", IGLTexture2D::cname(), filename);
    IGLTexture2D::m_texture.clear();
    IGLTexture2D::m_width = IGLTexture2D::m_height = 0;
    return IGLTexture2D::doload2D(filename);
  }

  inline bool save(const char *const filename)
  {
    uint8_t* p = IGLTexture2D::m_texture.to_array();
    if (likely(nullptr != p))
      {
        return !!SOIL_save_image(filename, SOIL_SAVE_TYPE_BMP,
                                 static_cast<int>(IGLTexture2D::m_width),
                                 static_cast<int>(IGLTexture2D::m_height),
                                 IGLTexture2D::m_colorElts, p);
      }
    return false;
  }

  //----------------------------------------------------------------------------
  //! \brief Set to the nth byte of the texture (write access).
  //----------------------------------------------------------------------------
  inline uint8_t& operator[](std::size_t nth)
  {
    //TBD ?
    //if (nth > m_width * m_height)
    //  {
    //    reserve(nth);
    //  }
    return IGLTexture2D::m_texture[nth];
  }

  //----------------------------------------------------------------------------
  //! \brief Get to the nth byte of the texture (read only access).
  //----------------------------------------------------------------------------
  inline const uint8_t& operator[](std::size_t nth) const
  {
    return IGLTexture2D::m_texture[nth];
  }

  //----------------------------------------------------------------------------
  //! \brief Set to the nth byte of the texture (write access).
  //----------------------------------------------------------------------------
  inline uint8_t& at(const size_t u, const size_t v, const size_t off)
  {
    return IGLTexture<T>::operator[]((u * IGLTexture2D::m_width + v) * IGLTexture2D::m_colorElts + off);
  }

  //----------------------------------------------------------------------------
  //! \brief Get to the nth byte of the texture (read only access.
  //----------------------------------------------------------------------------
  inline const uint8_t& at(const size_t u, const size_t v, const size_t off) const
  {
    return IGLTexture<T>::operator[]((u * IGLTexture2D::m_width + v) * IGLTexture2D::m_colorElts + off);
  }

  //----------------------------------------------------------------------------
  //! \brief Specify to OpenGL a two-dimensional texture image.
  //----------------------------------------------------------------------------
  inline void specifyTexture2D() const
  {
    // Note: is allowed this case:
    // m_width != 0 and m_height != 0 and buffer == nullptr
    // This will reserve the buffer size.
    glCheck(glTexImage2D(IGLTexture2D::m_target, 0,
                         static_cast<GLint>(IGLTexture2D::m_gpuPixelFormat),
                         static_cast<GLsizei>(IGLTexture2D::m_width),
                         static_cast<GLsizei>(IGLTexture2D::m_height),
                         0,
                         static_cast<GLenum>(IGLTexture2D::m_cpuPixelFormat),
                         static_cast<GLenum>(IGLTexture2D::m_pixelType),
                         IGLTexture2D::m_texture.to_array()));
  }

  //----------------------------------------------------------------------------
  //! \brief Apply OpenGL texture settings.
  //----------------------------------------------------------------------------
  virtual bool setup() override
  {
    DEBUG("Texture '%s' setup", IGLTexture2D::cname());

    // Note: m_texture can nullptr
    if (unlikely(!loaded()))
      {
        ERROR("Cannot setup texture '%s'. Reason 'Data not yet loaded'",
              IGLTexture2D::cname());
        return true;
      }

    IGLTexture2D::applyTextureParam();
    IGLTexture2D::specifyTexture2D();
    return false;
  }

  //----------------------------------------------------------------------------
  //! \brief Upload dirty CPU data to the GPU.
  //----------------------------------------------------------------------------
  virtual bool update() override
  {
    //TODO
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wsign-conversion"
#  pragma GCC diagnostic ignored "-Wconversion"
    DEBUG("Texture '%s' update", cname());

    size_t start, stop;
    IGLTexture2D::m_texture.getPendingData(start, stop);

    start = start / IGLTexture2D::m_colorElts;
    stop = stop / IGLTexture2D::m_colorElts;
    const GLint x = start / IGLTexture2D::m_width;
    const GLint y = start % IGLTexture2D::m_width;
    const GLsizei width = (stop / IGLTexture2D::m_width) - x + 1;
    const GLsizei height = (stop % IGLTexture2D::m_width) - y + 1;

    DEBUG("Texture '%s' update (%zu,%zu) --> ((%d,%d), (%d,%d))",
          IGLTexture2D::cname(), start, stop, x, y, width, height);

    // FIXME: not working if width and height are not the txture size
    glCheck(glBindTexture(IGLTexture2D::m_target, IGLTexture2D::m_handle));
    glCheck(glTexSubImage2D(IGLTexture2D::m_target, 0, x, y, width, height,
                            static_cast<GLenum>(IGLTexture2D::m_cpuPixelFormat),
                            static_cast<GLenum>(IGLTexture2D::m_pixelType),
                            IGLTexture2D::m_texture.to_array()));

    IGLTexture2D::m_texture.clearPending();
    return false;
#  pragma GCC diagnostic pop
  };
};

// *****************************************************************************
//! \brief Specialization to texture holding float values. This class will be
//! used for storing collection of uniforms values with \ref GLTextureCollection.
// *****************************************************************************
class GLTexture2DFloat: public IGLTexture2D<float>
{
public:

  GLTexture2DFloat(std::string const& name)
    : IGLTexture2D(name)
  {}

  GLTexture2DFloat(std::string const& name, const uint32_t width, const uint32_t height)
    : IGLTexture2D(name, width, height)
  {}
};

// *****************************************************************************
//! \brief Specialization to texture holding bytes. This class is the 'classic'
//! class for holding bitmap files (like jpeg, png, ...).
// *****************************************************************************
class GLTexture2D: public IGLTexture2D<uint8_t>
{
public:

  GLTexture2D(std::string const& name)
    : IGLTexture2D(name)
  {
    internalFormat();
  }

  GLTexture2D(std::string const& name, const uint32_t width, const uint32_t height)
    : IGLTexture2D(name, width, height)
  {
    internalFormat();
  }
};

// *****************************************************************************
//! \brief a 2D Texture storing the depth information of the scene.
// *****************************************************************************
class GLTextureDepth2D: public GLTexture2D
{
public:

  //----------------------------------------------------------------------------
  //! \brief Constructor.
  //!
  //! Give a name to the instance. This constructor makes no other
  //! actions.
  //!
  //! \param name the name of this instance used by GLProgram and GLVAO.
  //----------------------------------------------------------------------------
  GLTextureDepth2D(std::string const& name)
    : GLTexture2D(name)
  {
    // Replace values set by IGLTexture<T>::internalFormat()
    m_gpuPixelFormat = m_cpuPixelFormat = PixelFormat::DEPTH_COMPONENT;
    m_pixelType = PixelType::UNSIGNED_BYTE;
    m_colorElts = 4_z;
  }
};

// *****************************************************************************
//! \brief A 1D Texture.
// *****************************************************************************
class GLTexture1D: public IGLTexture<uint8_t>
{
public:

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  GLTexture1D(std::string const& name)
    : IGLTexture(name, GL_TEXTURE_1D)
  {
    internalFormat();
  }

  //----------------------------------------------------------------------------
  //! \brief Return the texture dimension: 1D
  //----------------------------------------------------------------------------
  virtual uint8_t dimension() const override
  {
    return 1u;
  }

  //----------------------------------------------------------------------------
  //! \brief \brief Allow to know if data have been transfered into
  //! the CPU memory.
  //!
  //! \return true if texture data have been loaded.
  //----------------------------------------------------------------------------
  virtual inline bool loaded() const override
  {
    // Note: return (0u != m_width) && (0u != m_height)
    // is not a good condition because allowed.
    return 0 != m_texture.size();
  }

  //----------------------------------------------------------------------------
  //! \brief sApply OpenGL texture settings.
  //----------------------------------------------------------------------------
  virtual bool setup() override
  {
    if (unlikely(!loaded()))
      {
        ERROR("Cannot setup texture '%s'. Reason 'Data not yet loaded'", cname());
        return true;
      }

    glCheck(glTexImage1D(m_target, 0,
                         static_cast<GLint>(m_gpuPixelFormat),
                         static_cast<GLsizei>(m_width),
                         0,
                         static_cast<GLenum>(m_cpuPixelFormat),
                         static_cast<GLenum>(m_pixelType),
                         nullptr));
    applyTextureParam();
    return false;
  }

  //----------------------------------------------------------------------------
  //! \brief Upload dirty CPU data to the GPU.
  //----------------------------------------------------------------------------
  virtual bool update() override
  {
    DEBUG("Texture '%s' update", cname());
    size_t pos_start;
    size_t pos_end;
    m_texture.getPendingData(pos_start, pos_end);

    // FIXME: pour le moment on envoie toute la texture entiere
    // au lieu de la portion modifiee.
    // TODO pendingData --> x,width
    const GLint x = 0U;
    const GLsizei width = static_cast<GLsizei>(m_width);

    glCheck(glBindTexture(m_target, m_handle));
    glCheck(glTexSubImage1D(m_target, 0, x, width,
                            static_cast<GLenum>(m_cpuPixelFormat),
                            static_cast<GLenum>(m_pixelType),
                            m_texture.to_array()));

    m_texture.clearPending();
    return false;
  }
};

// *****************************************************************************
//! \brief A 3D Texture specialized for rendering skybox.
// *****************************************************************************
class GLTextureCube: public IGLTexture<uint8_t>
{
public:

  //----------------------------------------------------------------------------
  //! \brief Constructor.
  //!
  //! Give a name to the instance. This constructor makes no other
  //! actions.
  //!
  //! \param name the name of this instance used by GLProgram and GLVAO.
  //----------------------------------------------------------------------------
  GLTextureCube(std::string const& name)
    : IGLTexture(name, GL_TEXTURE_CUBE_MAP)
  {
    internalFormat();
  }

  virtual ~GLTextureCube()
  {}

  //----------------------------------------------------------------------------
  //! \brief Return the texture dimension: 3D
  //----------------------------------------------------------------------------
  virtual uint8_t dimension() const override
  {
    return 3u;
  }

  //----------------------------------------------------------------------------
  //! \brief \brief Allow to know if data have been transfered into
  //! the CPU memory.
  //!
  //! \return true if the 6 textures 2D (one by face of the cube) have
  //! been loaded.
  //----------------------------------------------------------------------------
  virtual inline bool loaded() const override
  {
    for (uint8_t i = 0; i < 6u; ++i)
      {
        if (unlikely(false == m_textures[i]->loaded()))
          return false;
      }
    return true;
  }

  //----------------------------------------------------------------------------
  //! \brief Load a texture 2D at the given location on the cube.
  //!
  //! \note to be a valid GLTextureCube all faces of the cube shall
  //! have loaded (so this method has to be called 6 times with the
  //! correct \p target).
  //!
  //! \param target select one of the 6 faces of the cube.
  //! \param filename the path of a jpeg or bmp or png file.
  //!
  //! \return false if the texture failed to be loaded.
  //----------------------------------------------------------------------------
  bool load(CubeMap const target, const char *const filename)
  {
    const size_t index = static_cast<size_t>(target) -
      static_cast<size_t>(GL_TEXTURE_CUBE_MAP_POSITIVE_X);

    DEBUG("TextureCube '%s' %zu load bitmap '%s'", cname(), index, filename);
    return m_textures[index]->load(filename);
  }

private:

  //----------------------------------------------------------------------------
  //! \brief Apply OpenGL texture settings.
  //----------------------------------------------------------------------------
  virtual bool setup() override
  {
    // Note: m_texture can nullptr
    m_depth = computeDepth();
    if (6u != m_depth)
      {
        ERROR("Cannot setup texture '%s'. Reason 'Data not yet loaded'", cname());
        return true;
      }

    for (uint8_t i = 0; i < 6u; ++i)
      {
        m_textures[i]->m_target = targets(i);
        m_textures[i]->options(m_options);
        m_textures[i]->specifyTexture2D();
      }
    applyTextureParam();
    return false;
  }

  //----------------------------------------------------------------------------
  //! \brief Upload dirty CPU data to the GPU.
  //----------------------------------------------------------------------------
  virtual bool update() override
  {
    glCheck(glBindTexture(m_target, m_handle));
    for (uint8_t i = 0; i < 6u; ++i)
      {
        m_textures[i]->update();
      }
    return false;
  }

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  uint8_t computeDepth()
  {
    uint8_t depth = 0u;
    for (uint8_t i = 0; i < 6u; ++i)
      {
        if (likely(m_textures[i]->loaded()))
          ++depth;
      }
    return depth;
  }

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  static GLenum targets(const uint8_t index)
  {
    static constexpr std::array<GLenum, 6> targets =
      {{
          GL_TEXTURE_CUBE_MAP_POSITIVE_X,
          GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
          GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
          GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
          GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
          GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
      }};
    return targets[index];
  }

private:

  //----------------------------------------------------------------------------
  //! \brief Create the 6 textures use for mapping the cube.
  //!
  //! \note: unique_ptr is to avoid calling the deleted GLTexture2D copy
  //! constructor (IGLObject are non copyable).
  //----------------------------------------------------------------------------
  std::array<std::unique_ptr<GLTexture2D>, 6> m_textures
  {{
      std::make_unique<GLTexture2D>("CUBEMAP_POSITIVE_X"),
      std::make_unique<GLTexture2D>("CUBEMAP_NEGATIVE_X"),
      std::make_unique<GLTexture2D>("CUBEMAP_POSITIVE_Y"),
      std::make_unique<GLTexture2D>("CUBEMAP_NEGATIVE_Y"),
      std::make_unique<GLTexture2D>("CUBEMAP_POSITIVE_Z"),
      std::make_unique<GLTexture2D>("CUBEMAP_NEGATIVE_Z"),
  }};
};

// *****************************************************************************
//! \brief A 3D Texture.
// *****************************************************************************
template <class T>
class IGLTexture3D: public IGLTexture<T>
{
public:

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  IGLTexture3D(std::string const& name)
    : IGLTexture<T>(name, GL_TEXTURE_3D)
  {
    IGLTexture<T>::internalFormat();
  }

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  virtual uint8_t dimension() const override
  {
    return 3u;
  }

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  bool load(std::vector<std::string> const& filenames)
  {
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t prevWidth = 0;
    uint32_t prevHeight = 0;
    size_t depth = filenames.size();

    IGLTexture<T>::m_texture.clear();
    for (size_t i = 0; i < depth; ++i)
      {
        DEBUG("Texture3D '%s' load bitmap '%s'", IGLTexture<T>::cname(), filenames[i].c_str());

        // Load a Texture2D and pack it subsequently into a large 2D texture
        width = height = 0;
        if (unlikely(!IGLTexture<T>::doload2D(filenames[i].c_str())))
          {
            return false;
          }

        // Check consistency of Texture2D dimension
        if ((i != 0) && ((prevWidth != width) || (prevHeight != height)))
          {
            ERROR("Failed picture file '%s' has not correct dimension %ux%u",
                  filenames[i].c_str(), prevWidth, prevHeight);
            return false;
          }

        prevWidth = width;
        prevHeight = height;
      }

    // Success
    IGLTexture<T>::m_width = static_cast<uint32_t>(width);
    IGLTexture<T>::m_height = static_cast<uint32_t>(height);
    IGLTexture<T>::m_depth = static_cast<uint8_t>(depth);
    return true;
  }

  //----------------------------------------------------------------------------
  //! \brief Allow to know if data have been transfered into the CPU
  //! memory.
  //----------------------------------------------------------------------------
  virtual inline bool loaded() const override
  {
    // Note: return (0u != m_width) && (0u != m_height)
    // is not a good condition because allowed.
    return 0 != IGLTexture<T>::m_texture.size();
  }

private:

  //----------------------------------------------------------------------------
  //! \brief Specify to OpenGL a three-dimensional texture image.
  //----------------------------------------------------------------------------
  inline void specifyTexture3D() const
  {
    glCheck(glTexImage3D(IGLTexture<T>::m_target, 0,
                         static_cast<GLint>(IGLTexture<T>::m_gpuPixelFormat),
                         static_cast<GLsizei>(IGLTexture<T>::m_width),
                         static_cast<GLsizei>(IGLTexture<T>::m_height),
                         static_cast<GLsizei>(IGLTexture<T>::m_depth),
                         0,
                         static_cast<GLenum>(IGLTexture<T>::m_cpuPixelFormat),
                         static_cast<GLenum>(IGLTexture<T>::m_pixelType),
                         &IGLTexture<T>::m_texture[0]));
  }

  //----------------------------------------------------------------------------
  //! \brief Apply OpenGL texture settings.
  //----------------------------------------------------------------------------
  virtual bool setup() override
  {
    DEBUG("Texture '%s' setup", IGLTexture<T>::cname());

    // Note: m_texture can nullptr
    if (unlikely(!loaded()))
      {
        ERROR("Cannot setup texture '%s'. Reason 'Data not yet loaded'", IGLTexture<T>::cname());
        return true;
      }

    // Data is aligned in byte order
    glCheck(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

    IGLTexture<T>::applyTextureParam();
    specifyTexture3D();

    return false;
  }

  //----------------------------------------------------------------------------
  //! \brief Upload dirty CPU data to the GPU.
  //! \todo To be implemeted FIXME
  //----------------------------------------------------------------------------
  virtual bool update() override
  {
    DEBUG("Texture '%s' update", IGLTexture<T>::cname());
    return false;
  };
};

class GLTexture3D: public IGLTexture3D<uint8_t>
{
public:

  GLTexture3D(std::string const& name)
    : IGLTexture3D(name)
  {
  }
};

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_GLTEXTURES_HPP
