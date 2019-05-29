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

#ifndef OPENGLCPPWRAPPER_GLTEXTURE_2D_HPP
#  define OPENGLCPPWRAPPER_GLTEXTURE_2D_HPP

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

#endif // OPENGLCPPWRAPPER_GLTEXTURE_2D_HPP
