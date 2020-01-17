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
#  include "OpenGL/SOIL.hpp"

// *****************************************************************************
//! \file GLTexture2D.hpp file implements:
//!   - GLTexture2D:
//!   - GLDepthTexture2D:
// *****************************************************************************

namespace glwrap
{

  template<class T>
  static T getMaxTextureSize()
  {
    GLint maxTextureSize;
    glCheck(glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize));
    return static_cast<T>(maxTextureSize);
  }

//! \brief Used by GLTexture3D and GLTextureCube
DECLARE_CLASS(GLTexture2D);

// *****************************************************************************
//! \brief A 2D Texture.
// *****************************************************************************
class GLTexture2D: public GLTexture
{
  //! \brief GLTextureCube is made of GLTexture2D. Let it access to
  //! GLTexture2D private states.
  friend class GLTextureCube;
  friend class GLTextureBuffer;

public:

  //----------------------------------------------------------------------------
  //! \brief Constructor.
  //!
  //! Give a name to the instance. This constructor makes no other actions.
  //!
  //! \param name the sampler name used inside sader code. This name will be
  //! used by GLProgram and GLVAO. Please do not give the name or path of the
  //! picture file (jpeg, tga ...) but the sampler name! Path will be given
  //! thought the method load().
  //----------------------------------------------------------------------------
  GLTexture2D(std::string const& name)
    : GLTexture(2u, name, GL_TEXTURE_2D, CPUPixelFormat::RGBA, GPUPixelFormat::RGBA)
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
    : GLTexture(2u, name, GL_TEXTURE_2D, CPUPixelFormat::RGBA, GPUPixelFormat::RGBA)
  {
    // Note: Allow texture with no size for FrameBuffers
    m_width = width;
    m_height = height;
  }

protected:

  //----------------------------------------------------------------------------
  //! \brief Constructor.
  //!
  //! Give a name to the instance. This constructor makes no other actions.
  //!
  //! \param name the sampler name used inside sader code. This name will be
  //! used by GLProgram and GLVAO. Please do not give the name or path of the
  //! picture file (jpeg, tga ...) but the sampler name! Path will be given
  //! thought the method load().
  //----------------------------------------------------------------------------
  GLTexture2D(std::string const& name,
              CPUPixelFormat const cpuPixelFormat,
              GPUPixelFormat const gpuPixelFormat)
    : GLTexture(2u, name, GL_TEXTURE_2D, cpuPixelFormat, gpuPixelFormat)
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
  GLTexture2D(std::string const& name, const uint32_t width, const uint32_t height,
              CPUPixelFormat const cpuPixelFormat,
              GPUPixelFormat const gpuPixelFormat)
    : GLTexture(2u, name, GL_TEXTURE_2D, cpuPixelFormat, gpuPixelFormat)
  {
    // Note: Allow texture with no size for FrameBuffers
    m_width = width;
    m_height = height;
    // TODO #warning "ICI m_cpuPixelType et m_cpuPixelCount ne seont pas renseignes"
  }

public:

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
      GLTexture::loaded() ||
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
  inline bool load(std::string const& filename) // TODO hold the filename. name == filename
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
  virtual bool load(const char *const filename)
  {
    DEBUG("Texture2D '%s' load from bitmap '%s'", cname(), filename);
    m_buffer.clear(); m_width = m_height = 0;
    SOIL soil(m_cpuPixelFormat);
    bool ret = soil.load(filename, m_buffer, m_width, m_height);
    soil.getPixelInfo(m_cpuPixelType, m_cpuPixelCount);
    return ret;
  }

  //----------------------------------------------------------------------------
  //! \brief Save the texture into a picture file depending on the file extension
  //! on filename.
  //! \note Beware loaders may not manage all picture format. For example
  //! SOIL only manages BMP, TGA and DDS.
  //!
  //! \return true if texture data have been writen in the file, else false.
  //----------------------------------------------------------------------------
  virtual bool save(const char *const filename)
  {
    DEBUG("Texture2D '%s' save to bitmap '%s'", cname(), filename);
    SOIL soil(m_cpuPixelFormat);
    return soil.save(filename, m_buffer, m_width, m_height);
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
    return GLTexture2D::set((u * m_width + v) * m_cpuPixelCount + off);
  }

  //----------------------------------------------------------------------------
  //! \brief Get to the nth byte of the texture (read only access.
  //----------------------------------------------------------------------------
  inline const unsigned char& get(const size_t u, const size_t v, const size_t off) const
  {
    return GLTexture2D::get((u * m_width + v) * m_cpuPixelCount + off);
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
                         static_cast<GLint>(m_gpuPixelFormat),
                         static_cast<GLsizei>(m_width),
                         static_cast<GLsizei>(m_height),
                         0,
                         static_cast<GLenum>(m_cpuPixelFormat),
                         m_cpuPixelType,
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
        ERROR("Cannot setup texture '%s' because no texture has not yet been loaded. "
              "Please call load(filepath) before using the texture.", cname());
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

    DEBUG("=== FLUSH Texture2D '%s' to GPU: (%zu,%zu) --> ((%d,%d), (%d,%d))",
          cname(), start, stop, x, y, width, height);

    // FIXME: not working if width and height are not the txture size
    glCheck(glBindTexture(m_target, m_handle));
    glCheck(glTexSubImage2D(m_target, 0, x, y, width, height,
                            static_cast<GLenum>(m_cpuPixelFormat),
                            static_cast<GLenum>(m_cpuPixelType),
                            m_buffer.to_array()));

    m_buffer.clearPending();
    return false;
#  pragma GCC diagnostic pop
  };
};

// *****************************************************************************
//! \brief a 2D Texture storing the depth information of the scene.
// *****************************************************************************
class GLDepthTexture2D: public GLTexture2D
{
public:

  //----------------------------------------------------------------------------
  //! \brief Constructor.
  //!
  //! Give a name to the instance. This constructor makes no other actions.
  //!
  //! \param name the sampler name used inside sader code. This name will be
  //! used by GLProgram and GLVAO. Please do not give the name or path of the
  //! picture file (jpeg, tga ...) but the sampler name! Path will be given
  //! thought the method load().
  //----------------------------------------------------------------------------
  GLDepthTexture2D(std::string const& name)
    : GLTexture2D(name, CPUPixelFormat::DEPTH_COMPONENT, GPUPixelFormat::DEPTH_COMPONENT32F)
  {}

  GLDepthTexture2D(std::string const& name, const uint32_t width, const uint32_t height)
    : GLTexture2D(name, width, height, CPUPixelFormat::DEPTH_COMPONENT, GPUPixelFormat::DEPTH_COMPONENT32F)
  {}
};

// *****************************************************************************
//! \brief a 2D Texture storing float values.
// *****************************************************************************
class GLFloatTexture2D: public GLTexture2D
{
public:

  //----------------------------------------------------------------------------
  //! \brief Constructor.
  //!
  //! Give a name to the instance. This constructor makes no other actions.
  //!
  //! \param name the sampler name used inside sader code. This name will be
  //! used by GLProgram and GLVAO. Please do not give the name or path of the
  //! picture file (jpeg, tga ...) but the sampler name! Path will be given
  //! thought the method load().
  //----------------------------------------------------------------------------
  GLFloatTexture2D(std::string const& name)
    : GLTexture2D(name, CPUPixelFormat::RGBA, GPUPixelFormat::RGBA32F)
  {}

  GLFloatTexture2D(std::string const& name, const uint32_t width, const uint32_t height)
    : GLTexture2D(name, width, height, CPUPixelFormat::RGBA, GPUPixelFormat::RGBA32F)
  {}
};

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_GLTEXTURE2D_HPP
