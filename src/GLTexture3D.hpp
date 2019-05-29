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

#ifndef OPENGLCPPWRAPPER_GLTEXTURE_3D_HPP
#  define OPENGLCPPWRAPPER_GLTEXTURE_3D_HPP

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

#endif // OPENGLCPPWRAPPER_GLTEXTURE_3D_HPP
