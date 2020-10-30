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

#ifndef OPENGLCPPWRAPPER_GLTEXTURE3D_HPP
#  define OPENGLCPPWRAPPER_GLTEXTURE3D_HPP

#  include "OpenGL/Texture2D.hpp"
#  include <array>

// *****************************************************************************
//! \file GLTexture3D.hpp file implements:
//!   - GLTexture3D: which is a set of 2D Textures.
//!   - GLTextureCube: A 3D Texture specialized for rendering skybox.
// *****************************************************************************

namespace glwrap
{

// *****************************************************************************
//! \brief A 3D Texture specialized for rendering skybox.
// *****************************************************************************
class GLTextureCube: public GLTexture
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
    : GLTexture(3u, name, GL_TEXTURE_CUBE_MAP)
  {}

  virtual ~GLTextureCube()
  {}

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
  //! constructor (GLObject are non copyable).
  //----------------------------------------------------------------------------
  std::array<GLTexture2D_UP, 6> m_textures
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
class GLTexture3D: public GLTexture
{
public:

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  GLTexture3D(std::string const& name)
    : GLTexture(3u, name, GL_TEXTURE_3D)
  {}

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
    SOIL soil(m_cpuPixelFormat);

    m_buffer.clear();
    for (size_t i = 0; i < depth; ++i)
      {
        DEBUG("Texture3D '%s' load bitmap '%s'", cname(), filenames[i].c_str());

        // Load a Texture2D and pack it subsequently into a large 2D texture
        width = height = 0;
        if (unlikely(!soil.load(filenames[i].c_str(), m_buffer, width, height)))
          return false;

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
    m_depth = static_cast<uint8_t>(depth);
    return true;
  }

private:

  //----------------------------------------------------------------------------
  //! \brief Specify to OpenGL a three-dimensional texture image.
  //----------------------------------------------------------------------------
  inline void specifyTexture3D() const
  {
    glCheck(glTexImage3D(m_target, 0,
                         static_cast<GLint>(m_gpuPixelFormat),
                         static_cast<GLsizei>(m_width),
                         static_cast<GLsizei>(m_height),
                         static_cast<GLsizei>(m_depth),
                         0,
                         static_cast<GLenum>(m_cpuPixelFormat),
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

#endif // OPENGLCPPWRAPPER_GLTEXTURE3D_HPP
