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

// *****************************************************************************
//! \file GLTextures.hpp
// *****************************************************************************

#  include "IGLObject.hpp"
#  include "PendingData.hpp"
#  include "SOIL/SOIL.h"
#  include <array>
#  include <vector>

// *****************************************************************************
//! \brief Default options for textures when setup.
// *****************************************************************************
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
  : public IGLObject<GLenum>,
    protected PendingData
{
protected:

  //----------------------------------------------------------------------------
  //! \brief Deleter for std::unique_ptr.
  //!
  //! Allow to automaticaly delete the CPU memory of the texture when
  //! no longer used. For loading textures we use a C library and in C
  //! deleting resources shall be made manually (that we do not want).
  //----------------------------------------------------------------------------
  struct SOILDeleter
  {
    void operator()(unsigned char* buf)
    {
      DEBUG("%s", "Texture deleter");
      if (buf != nullptr)
        SOIL_free_image_data(buf);
    }
  };

  //! \brief Smart Pointer holding the CPU memory for storing the
  //! texture.
  using TextBufPtr = std::unique_ptr<unsigned char, SOILDeleter>;

public:

  //----------------------------------------------------------------------------
  //! \brief Constructor.
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
  //! \brief \brief Allow to know if data have been transfered into
  //! the CPU memory.
  //!
  //! \return true if texture data have been loaded.
  //----------------------------------------------------------------------------
  virtual inline bool loaded() const = 0;

  //----------------------------------------------------------------------------
  //! \brief Change minifier and magnifier options.
  //----------------------------------------------------------------------------
  void interpolation(TextureMinFilter const min_filter,
                     TextureMagFilter const mag_filter)
  {
    m_options.minFilter = min_filter;
    m_options.magFilter = mag_filter;
    redoSetup();
  }

  //----------------------------------------------------------------------------
  //! \brief Change wrapping options.
  //----------------------------------------------------------------------------
  void wrapping(TextureWrap const wrap)
  {
    m_options.wrapS = wrap;
    m_options.wrapT = wrap;
    redoSetup();
  }

  //----------------------------------------------------------------------------
  //! \brief Replace current texture settings by a new one.
  //----------------------------------------------------------------------------
  void options(TextureOptions const& options)
  {
    m_options = options;
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
  inline virtual uint32_t height() const
  {
    return 0u;
  }

  //----------------------------------------------------------------------------
  //! \brief Return the texture depth (in number of textures 2D).
  //!
  //! Only available for texture 3D.
  //----------------------------------------------------------------------------
  inline virtual uint32_t depth() const
  {
    return 0u;
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
    glCheck(glTexParameteri(m_target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
  }

private:

  //----------------------------------------------------------------------------
  //! \brief Check if CPU data of the texture have been modified.
  //! If this is the case "dirty" data will be transfered to the GPU.
  //----------------------------------------------------------------------------
  virtual inline bool needUpdate() const override
  {
    return PendingData::hasPendingData();
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
  //! \brief Destroy the OpenGL Texture.
  //----------------------------------------------------------------------------
  virtual void release() override
  {
    glCheck(glDeleteTextures(1U, &m_handle));
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

protected:

  TextureOptions m_options;
  uint32_t m_width = 0;
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
  //! \brief Constructor.
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
  //! \brief Destructor. Release elements in CPU and GPU memories.
  //----------------------------------------------------------------------------
  virtual ~GLTexture2D()
  {}

  //----------------------------------------------------------------------------
  //! \brief Return the texture dimension: 2D.
  //----------------------------------------------------------------------------
  virtual uint8_t dimension() const override
  {
    return 2u;
  }

  //----------------------------------------------------------------------------
  //! \brief Return the texture height (in pixel).
  //----------------------------------------------------------------------------
  inline virtual uint32_t height() const override
  {
    return m_height;
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
    return (0u != m_width) && (0u != m_height) && (nullptr != m_buffer.get());
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
  inline bool load(std::string const& filename, const bool rename = false)
  {
    return load(filename.c_str(), rename);
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
  bool load(const char *const filename, const bool rename = false)
  {
    int width, height;

    DEBUG("Loading texture '%s'", filename);

    // FIXME: SOIL_LOAD_RGBA: should adapt from m_options.cpuPixelFormat
    unsigned char* image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGBA);
    if (unlikely(nullptr == image))
      {
        ERROR("Failed loading picture file '%s'", filename);
        return false;
      }

    // Success
    m_buffer = std::move(TextBufPtr(image));
    m_width = static_cast<uint32_t>(width);
    m_height = static_cast<uint32_t>(height);
    DEBUG("texture dimension %ux%u", m_width, m_height);

    // Set the whole texture data to "dirty" forcing its upload to the
    // GPU. Unit is pixel (not byte).
    PendingData::tagAsPending(0_z, m_width * m_height);

    // Rename the instance ?
    if (rename || name().empty())
      {
        name() = filename;
        DEBUG("Renaming texture '%s'", filename);
      }

    DEBUG("Successfuly load picture file '%s'", filename);
    return true;
  }

  //----------------------------------------------------------------------------
  //! \brief Set to the nth byte of the texture.
  //----------------------------------------------------------------------------
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

  //----------------------------------------------------------------------------
  //! \brief Get to the nth byte of the texture.
  //----------------------------------------------------------------------------
  inline const unsigned char& operator[](size_t nth) const
  {
    return m_buffer.get()[nth];
  }

private:

  //----------------------------------------------------------------------------
  //! \brief Specify to OpenGL a two-dimensional texture image.
  //----------------------------------------------------------------------------
  inline void specifyTexture2D() const
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

  //----------------------------------------------------------------------------
  //! \brief Apply OpenGL texture settings.
  //----------------------------------------------------------------------------
  virtual bool setup() override
  {
    DEBUG("Texture '%s' setup", name().c_str());

    // Note: m_buffer can nullptr
    if (unlikely(!loaded()))
      {
        ERROR("Cannot setup texture '%s'. Reason 'Data not yet loaded'", name().c_str());
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
    return (0 != m_width) && (nullptr != m_buffer.get());
  }

  //----------------------------------------------------------------------------
  //! \brief sApply OpenGL texture settings.
  //----------------------------------------------------------------------------
  virtual bool setup() override
  {
    if (unlikely(!loaded()))
      {
        ERROR("Cannot setup texture '%s'. Reason 'Data not yet loaded'", name().c_str());
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
  //! \brief Return the texture height (in pixel).
  //----------------------------------------------------------------------------
  inline virtual uint32_t height() const override
  {
    return m_height;
  }

  //----------------------------------------------------------------------------
  //! \brief Return the texture depth (in number of textures 2D).
  //----------------------------------------------------------------------------
  inline virtual uint32_t depth() const override
  {
    return m_depth;
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
    return m_textures[index]->load(filename, false);
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
        ERROR("Cannot setup texture '%s'. Reason 'Data not yet loaded'", name().c_str());
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

protected:

  uint32_t m_height = 0u;
  uint8_t m_depth = 0u;

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
  inline uint32_t height() const
  {
    return m_height;
  }

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  inline uint32_t depth() const
  {
    return m_depth;
  }

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  bool load(std::vector<std::string> const& filenames)
  {
    int width = 0;
    int height = 0;
    int prevWidth = 0;
    int prevHeight = 0;
    size_t depth = filenames.size();

    m_data3d.clear();
    for (size_t i = 0; i < depth; ++i)
      {
        // Load a Texture2D
        unsigned char* image = SOIL_load_image(filenames[i].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
        if (likely(nullptr == image))
          {
            ERROR("Failed loading picture file '%s'", filenames[i].c_str());
            m_data3d.clear();
            return false;
          }

        // Check consistency of Texture2D dimension
        if ((i != 0) && ((prevWidth != width) || (prevHeight != height)))
          {
            SOIL_free_image_data(image);
            ERROR("Failed picture file '%s' has not correct dimension %ux%u",
                  filenames[i].c_str(), prevWidth, prevHeight);
            m_data3d.clear();
            return false;
          }

        // Pack Texture2D subsequently into a large 3D buffer
        m_data3d.insert(m_data3d.end(), image, image + m_width * m_height * 4 * sizeof(unsigned char));
        SOIL_free_image_data(image);
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
    return (0u != m_width) && (0u != m_height) &&
      (0u != m_depth) && (0_z != m_data3d.size());
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
                         &m_data3d[0]));
  }

  //----------------------------------------------------------------------------
  //! \brief Apply OpenGL texture settings.
  //----------------------------------------------------------------------------
  virtual bool setup() override
  {
    DEBUG("Texture '%s' setup", name().c_str());

    // Note: m_buffer can nullptr
    if (unlikely(!loaded()))
      {
        ERROR("Cannot setup texture '%s'. Reason 'Data not yet loaded'", name().c_str());
        return true;
      }

    // Data is aligned in byte order
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Texture colors should replace the original color values
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); //GL_MODULATE

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
    DEBUG("Texture '%s' update", name().c_str());
    return false;
  };

protected:

  uint32_t m_height = 0u;
  uint32_t m_depth = 0u;
  std::vector<unsigned char> m_data3d;
};

#endif /* GLTEXTURES_HPP */
