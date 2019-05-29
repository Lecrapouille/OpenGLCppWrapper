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

#ifndef OPENGLCPPWRAPPER_GLTEXTURE_1D_HPP
#  define OPENGLCPPWRAPPER_GLTEXTURE_1D_HPP

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

#endif // OPENGLCPPWRAPPER_GLTEXTURE_1D_HPP
