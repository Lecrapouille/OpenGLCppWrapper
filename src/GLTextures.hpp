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
#  include <vector>

namespace glwrap
{

//! \brief Internal format storing texture data
using TextureData = PendingContainer<unsigned char>;

//! \brief Texture Pixel Format.
enum class PixelLoadFormat : GLenum
  {
    /* 3 */ LOAD_RGB = SOIL_LOAD_RGB,
    /* 4 */ LOAD_RGBA = SOIL_LOAD_RGBA,
  };

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
  PixelFormat cpuPixelFormat = PixelFormat::RGBA; // FIXME can be in conflict with SOIL_LOAD_xxx
  PixelFormat gpuPixelFormat = PixelFormat::RGBA;
  PixelType pixelType = PixelType::UNSIGNED_BYTE;
  bool generateMipmaps = false;
};

//----------------------------------------------------------------------------
//! \brief Load picture file (jpg, png ...) with SOIL library into or internal
//! format.
//! \param[out] width Buffer width (pixels)
//! \param[out] height Buffer height (pixel)
//----------------------------------------------------------------------------
static bool doload2D(const char *const filename, const PixelLoadFormat format,
                     TextureData& data, uint32_t& width, uint32_t& height)
{
  if (unlikely(nullptr == filename)) return false;
  DEBUG("Loading texture '%s'", filename);
  data.debugDirty("doload");

  // Load the image as a C array.
  int w, h;
  unsigned char* image = SOIL_load_image(filename, &w, &h, 0, static_cast<GLenum>(format));
  if (likely(nullptr != image))
    {
      // Use the max because with framebuffer we can resize texture
      width = std::max(width, static_cast<uint32_t>(w)); // FIXME
      height = std::max(height, static_cast<uint32_t>(h));

      // Convert it as std::vector
      size_t size = w * h * sizeof(unsigned char)
        * ((format == PixelLoadFormat::LOAD_RGBA) ? 4 : 3);
      data.append(image, size); // FIXME: not working with preallocated size
      //data.debugDirty();
      SOIL_free_image_data(image);
      DEBUG("Successfully loaded %ux%u texture '%s'", width, height, filename);
      return true;
    }
  else
    {
      ERROR("Failed loading picture file '%s'", filename);
      width = height = 0;
      data.clear();
      return false;
    }
}

// *****************************************************************************
//! \brief Generic Texture.
//!
//! A texture is an OpenGL Object that contains one or more images
//! that all have the same image format. A texture can be used in two
//! ways: either it can be the source of a texture access from a
//! Shader, or it can be used as a render target (FrameBuffer). A
//! texture can be of dimension 1, 2 or 3.
// *****************************************************************************
class IGLTexture
  : public IGLObject<GLenum>
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
    return m_buffer;
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
  IGLTexture& wrapSTR(TextureWrap const wrapS, TextureWrap const wrapT, TextureWrap const wrapR)
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
    m_width = 0;
  }

protected:

  TextureOptions m_options;
  TextureData    m_buffer;
  uint32_t       m_width = 0;
  //! \brief For Texture2D, Texture3D, TextureCube
  uint32_t       m_height = 0;
  //! \brief For Texture3D, TextureCube
  uint8_t        m_depth = 0u;
};

// *****************************************************************************
//! \brief A 2D Texture.
// *****************************************************************************
class GLTexture2D: public IGLTexture
{
  //! \brief GLTextureCube is made of GLTexture2D. Let it access to
  //! GLTexture2D private states.
  friend class GLTextureCube;

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
  GLTexture2D(std::string const& name)
    : IGLTexture(name, GL_TEXTURE_2D)
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
 GLTexture2D(std::string const& name, const uint32_t width, const uint32_t height)
    : IGLTexture(name, GL_TEXTURE_2D)
  {
    // TODO Forbid texture with 0x0 dimension ?
    m_width = width;
    m_height = height;
  }

  //----------------------------------------------------------------------------
  //! \brief Destructor. Release elements in CPU and GPU memories.
  //----------------------------------------------------------------------------
  virtual ~GLTexture2D()
  {}

  GLTexture2D& operator=(GLTexture2D const& other)
  {
    m_handle = other.m_handle;
    m_target = other.m_target;
    m_width = other.m_width;
    m_height = other.m_height;
    m_options = other.m_options;
    m_buffer = other.m_buffer;

    return *this;
  }

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
    return (0 != m_buffer.size()) // Texture loaded from a file (jpeg ...)
      || ((0 != m_width) && (0 != m_height)); // Or dummy texture for framebuffer
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
    return load(filename.c_str());
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
    m_buffer.clear(); m_width = m_height = 0;
    return doload2D(filename, PixelLoadFormat::LOAD_RGBA, m_buffer, m_width, m_height);
  }

  inline bool save(const char *const filename)
  {
    //TODO 4 because RGBA
    return !!SOIL_save_image(filename, SOIL_SAVE_TYPE_BMP, m_width, m_height, 4, m_buffer.to_array());
  }

  //----------------------------------------------------------------------------
  //! \brief Set to the nth byte of the texture (write access).
  //----------------------------------------------------------------------------
  inline unsigned char& operator[](int nth)
  {
    //TBD ?
    //if (nth > m_width * m_height)
    //  {
    //    reserve(nth);
    //  }
    return m_buffer[nth];
  }

  //----------------------------------------------------------------------------
  //! \brief Get to the nth byte of the texture (read only access).
  //----------------------------------------------------------------------------
  inline const unsigned char& operator[](int nth) const
  {
    return m_buffer[nth];
  }

  //----------------------------------------------------------------------------
  //! \brief Set to the nth byte of the texture (write access).
  //----------------------------------------------------------------------------
  inline unsigned char& at(const size_t u, const size_t v, const size_t off)
  {
    return GLTexture2D::operator[]((u * m_width + v) * 4 + off); //TODO 4 because RGBA
  }

  //----------------------------------------------------------------------------
  //! \brief Get to the nth byte of the texture (read only access.
  //----------------------------------------------------------------------------
  inline const unsigned char& at(const size_t u, const size_t v, const size_t off) const
  {
    return GLTexture2D::operator[]((u * m_width + v) * 4 + off);
  }

  //----------------------------------------------------------------------------
  //! \brief Specify to OpenGL a two-dimensional texture image.
  //----------------------------------------------------------------------------
  inline void specifyTexture2D() const
  {
    // Note: is allowed this case:
    // m_width != 0 and m_height != 0 and buffer == nullptr
    // This will reserve the buffer size.
    glCheck(glTexImage2D(m_target, 0,
                         static_cast<GLint>(m_options.gpuPixelFormat),
                         static_cast<GLsizei>(m_width),
                         static_cast<GLsizei>(m_height),
                         0,
                         static_cast<GLenum>(m_options.cpuPixelFormat),
                         static_cast<GLenum>(m_options.pixelType),
                         m_buffer.to_array()));
  }

  //----------------------------------------------------------------------------
  //! \brief Apply OpenGL texture settings.
  //----------------------------------------------------------------------------
  virtual bool setup() override
  {
    DEBUG("Texture '%s' setup", cname());

    // Note: m_buffer can nullptr
    if (unlikely(!loaded()))
      {
        ERROR("Cannot setup texture '%s'. Reason 'Data not yet loaded'", cname());
        return true;
      }

    applyTextureParam();
    specifyTexture2D();
    return false;
  }

  //----------------------------------------------------------------------------
  //! \brief Upload dirty CPU data to the GPU.
  //----------------------------------------------------------------------------
  virtual bool update() override
  {
    DEBUG("Texture '%s' update", cname());

    size_t start, stop;
    m_buffer.getPendingData(start, stop);

    start = start / 4; //TODO 4 because RGBA
    stop = stop / 4;
    const GLint x = start / m_width;
    const GLint y = start % m_width;
    const GLsizei width = (stop / m_width) - x + 1;
    const GLsizei height = (stop % m_width) - y + 1;

    DEBUG("Texture '%s' update (%zu,%zu) --> ((%d,%d), (%d,%d))",
          cname(), start, stop, x, y, width, height);

    glCheck(glBindTexture(m_target, m_handle));
    glCheck(glTexSubImage2D(m_target, 0, x, y, width, height,
                            static_cast<GLenum>(m_options.cpuPixelFormat),
                            static_cast<GLenum>(m_options.pixelType),
                            m_buffer.to_array()));

    m_buffer.clearPending();
    return false;
  };
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
    m_options.gpuPixelFormat = PixelFormat::DEPTH_COMPONENT;
    m_options.cpuPixelFormat = PixelFormat::DEPTH_COMPONENT;
  }
};

// *****************************************************************************
//! \brief A 1D Texture.
// *****************************************************************************
class GLTexture1D: public IGLTexture
{
public:

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  GLTexture1D(std::string const& name)
    : IGLTexture(name, GL_TEXTURE_1D)
  {}

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
    return 0 != m_buffer.size();
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
                         static_cast<GLint>(m_options.gpuPixelFormat),
                         static_cast<GLsizei>(m_width),
                         0,
                         static_cast<GLenum>(m_options.cpuPixelFormat),
                         static_cast<GLenum>(m_options.pixelType),
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
    m_buffer.getPendingData(pos_start, pos_end);

    // FIXME: pour le moment on envoie toute la texture entiere
    // au lieu de la portion modifiee.
    // TODO pendingData --> x,width
    const GLint x = 0U;
    const GLsizei width = static_cast<GLsizei>(m_width);

    glCheck(glBindTexture(m_target, m_handle));
    glCheck(glTexSubImage1D(m_target, 0, x, width,
                            static_cast<GLenum>(m_options.cpuPixelFormat),
                            static_cast<GLenum>(m_options.pixelType),
                            m_buffer.to_array()));

    m_buffer.clearPending();
    return false;
  }
};

// *****************************************************************************
//! \brief A 3D Texture specialized for rendering skybox.
// *****************************************************************************
class GLTextureCube: public IGLTexture
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
    const int index = static_cast<int>(target) - GL_TEXTURE_CUBE_MAP_POSITIVE_X;
    return m_textures[index]->load(filename);
  }

private:

  //----------------------------------------------------------------------------
  //! \brief Apply OpenGL texture settings.
  //----------------------------------------------------------------------------
  virtual bool setup() override
  {
    // Note: m_buffer can nullptr
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
class GLTexture3D: public IGLTexture
{
public:

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  GLTexture3D(std::string const& name)
    : IGLTexture(name, GL_TEXTURE_3D)
  {}

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

    m_buffer.clear();
    for (size_t i = 0; i < depth; ++i)
      {
        // Load a Texture2D and pack it subsequently into a large 2D texture
        width = height = 0;
        if (unlikely(!doload2D(filenames[i].c_str(), PixelLoadFormat::LOAD_RGBA, m_buffer, width, height)))
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
    m_width = static_cast<uint32_t>(width);
    m_height = static_cast<uint32_t>(height);
    m_depth = static_cast<uint32_t>(depth);
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
    return 0 != m_buffer.size();
  }

private:

  //----------------------------------------------------------------------------
  //! \brief Specify to OpenGL a three-dimensional texture image.
  //----------------------------------------------------------------------------
  inline void specifyTexture3D() const
  {
    glCheck(glTexImage3D(m_target, 0,
                         static_cast<GLint>(m_options.gpuPixelFormat),
                         static_cast<GLsizei>(m_width),
                         static_cast<GLsizei>(m_height),
                         static_cast<GLsizei>(m_depth),
                         0,
                         static_cast<GLenum>(m_options.cpuPixelFormat),
                         static_cast<GLenum>(m_options.pixelType),
                         &m_buffer[0]));
  }

  //----------------------------------------------------------------------------
  //! \brief Apply OpenGL texture settings.
  //----------------------------------------------------------------------------
  virtual bool setup() override
  {
    DEBUG("Texture '%s' setup", cname());

    // Note: m_buffer can nullptr
    if (unlikely(!loaded()))
      {
        ERROR("Cannot setup texture '%s'. Reason 'Data not yet loaded'", cname());
        return true;
      }

    // Data is aligned in byte order
    glCheck(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

    applyTextureParam();
    specifyTexture3D();

    return false;
  }

  //----------------------------------------------------------------------------
  //! \brief Upload dirty CPU data to the GPU.
  //! \todo To be implemeted FIXME
  //----------------------------------------------------------------------------
  virtual bool update() override
  {
    DEBUG("Texture '%s' update", cname());
    return false;
  };
};

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_GLTEXTURES_HPP
