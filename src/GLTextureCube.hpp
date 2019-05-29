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

#ifndef OPENGLCPPWRAPPER_GLTEXTURE_CUBE_HPP
#  define OPENGLCPPWRAPPER_GLTEXTURE_CUBE_HPP

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

#endif // OPENGLCPPWRAPPER_GLTEXTURE_CUBE_HPP
