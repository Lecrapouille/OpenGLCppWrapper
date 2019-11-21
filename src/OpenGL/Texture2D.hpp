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

#ifndef OPENGLCPPWRAPPER_GLTEXTURE2D_HPP
#  define OPENGLCPPWRAPPER_GLTEXTURE2D_HPP

#  include "OpenGL/Textures.hpp"

// *****************************************************************************
//! \file GLTexture2D.hpp file implements:
//!   - GLTexture2D:
//!   - GLTextureDepth2D:
// *****************************************************************************

namespace glwrap
{

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

  // Load the image as a C array.
  int w, h;
  unsigned char* image = SOIL_load_image(filename, &w, &h, 0, static_cast<int>(format));
  if (likely(nullptr != image))
    {
      // Use the max because with framebuffer we can resize texture
      width = std::max(width, static_cast<uint32_t>(w)); // FIXME
      height = std::max(height, static_cast<uint32_t>(h));

      // Convert it as std::vector
      size_t size = static_cast<size_t>(w * h) * sizeof(unsigned char)
        * ((format == PixelLoadFormat::LOAD_RGBA) ? 4 : 3);
      data.append(image, size); // FIXME: not working with preallocated size
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

//! \brief Used by GLTexture3D and GLTextureCube
DECLARE_CLASS(GLTexture2D)

// *****************************************************************************
//! \brief A 2D Texture.
// *****************************************************************************
class GLTexture2D: public IGLTexture
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
  GLTexture2D(std::string const& name)
    : IGLTexture(2u, name, GL_TEXTURE_2D)
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
    : IGLTexture(2u, name, GL_TEXTURE_2D)
  {
    // Note: Allow texture with no size for FrameBuffers
    m_width = width;
    m_height = height;
  }

  //----------------------------------------------------------------------------
  //! \brief Destructor. Release elements in CPU and GPU memories.
  //----------------------------------------------------------------------------
  virtual ~GLTexture2D()
  {}

  //----------------------------------------------------------------------------
  //! \brief Allow to know if data have been transfered into the CPU
  //! memory.
  //!
  //! \return true if texture data have been loaded (a jpeg, png, bmp
  //! file have been loaded)
  //----------------------------------------------------------------------------
  virtual inline bool loaded() const override
  {
    return
      // Texture loaded from a file (jpeg ...)
      IGLTexture::loaded() ||
      // Dummy textures accepted by FrameBuffer
      ((0 != m_width) && (0 != m_height));
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
    DEBUG("Texture2D '%s' load bitmap '%s'", cname(), filename);
    m_buffer.clear(); m_width = m_height = 0;
    return doload2D(filename, PixelLoadFormat::LOAD_RGBA, m_buffer, m_width, m_height);
  }

  inline bool save(const char *const filename)
  {
    unsigned char* p = m_buffer.to_array();
    if (likely(nullptr != p))
      {
        //TODO 4 because RGBA
        return !!SOIL_save_image(filename, SOIL_SAVE_TYPE_BMP,
                                 static_cast<int>(m_width),
                                 static_cast<int>(m_height),
                                 4, p);
      }
    return false;
  }

  //----------------------------------------------------------------------------
  //! \brief Set to the nth byte of the texture (write access).
  //----------------------------------------------------------------------------
  inline unsigned char& set(std::size_t const nth)
  {
    //TBD ?
    //if (nth > m_width * m_height)
    //  {
    //    reserve(nth);
    //  }
    return m_buffer.set(nth);
  }

  //----------------------------------------------------------------------------
  //! \brief Get to the nth byte of the texture (read only access).
  //----------------------------------------------------------------------------
  inline const unsigned char& get(std::size_t const nth) const
  {
    return m_buffer.get(nth);
  }

  //----------------------------------------------------------------------------
  //! \brief Set to the nth byte of the texture (write access).
  //----------------------------------------------------------------------------
  inline unsigned char& set(const size_t u, const size_t v, const size_t off)
  {
    return GLTexture2D::set((u * m_width + v) * 4 + off); //TODO 4 because RGBA
  }

  //----------------------------------------------------------------------------
  //! \brief Get to the nth byte of the texture (read only access.
  //----------------------------------------------------------------------------
  inline const unsigned char& get(const size_t u, const size_t v, const size_t off) const
  {
    return GLTexture2D::get((u * m_width + v) * 4 + off);
  }

private:

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
    //TODO
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wsign-conversion"
#  pragma GCC diagnostic ignored "-Wconversion"
    DEBUG("Texture '%s' update", cname());

    size_t start, stop;
    m_buffer.getPendingData(start, stop);

    start = start / 4; //TODO 4 because RGBA
    stop = stop / 4;
    const GLint x = start / m_width;
    const GLint y = start % m_width;
    const GLsizei width = (stop / m_width) - x;
    const GLsizei height = (stop % m_width) - y;

    DEBUG("Texture '%s' update (%zu,%zu) --> ((%d,%d), (%d,%d))",
          cname(), start, stop, x, y, width, height);

    // FIXME: not working if width and height are not the txture size
    glCheck(glBindTexture(m_target, m_handle));
    glCheck(glTexSubImage2D(m_target, 0, x, y, width, height,
                            static_cast<GLenum>(m_options.cpuPixelFormat),
                            static_cast<GLenum>(m_options.pixelType),
                            m_buffer.to_array()));

    m_buffer.clearPending();
    return false;
#  pragma GCC diagnostic pop
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

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_GLTEXTURE2D_HPP
