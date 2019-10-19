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

#ifndef OPENGLCPPWRAPPER_GLSAMPLERS_HPP
#  define OPENGLCPPWRAPPER_GLSAMPLERS_HPP

// *****************************************************************************
//! \file Sampler.hpp file implements:
//!   - GLSampler:
//!   - GLSampler1D:
//!   - GLSampler2D:
//!   - GLSampler3D:
//!   - GLSamplerCube:
// *****************************************************************************

#  include "OpenGL/LocationUniform.hpp"
#  include "Math/Matrix.hpp"

namespace glwrap
{

//! \brief Used by GLProgram
DECLARE_CLASS(GLSampler)

// *****************************************************************************
//! \brief A GLSampler is an OpenGL uniform for texture.
// *****************************************************************************
class GLSampler: public IGLUniform
{
public:

  //----------------------------------------------------------------------------
  //! \brief See GLLocation constructor.
  //! \param name
  //! \param gltype
  //! \param texture_id count texture.
  //! \param prog
  //----------------------------------------------------------------------------
  GLSampler(const char *name, const GLint gltype, const GLenum texture_id,
            const GLuint prog)
    : IGLUniform(name, 0, gltype, prog),
      m_texture_id(texture_id)
  {
    forceUpdate();
  }

  //----------------------------------------------------------------------------
  //! \brief Return the texture identifier.
  //----------------------------------------------------------------------------
  inline GLenum textureID() const
  {
    return m_texture_id;
  }

private:

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  virtual void activate() override
  {
    DEBUG("Sampler '%s' activate GL_TEXTURE0 + %u", cname(), m_texture_id);
    glCheck(glActiveTexture(GL_TEXTURE0 + m_texture_id));
  }

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  virtual bool update() override
  {
    DEBUG("Sampler '%s' update", cname());
    glCheck(glUniform1i(m_handle, static_cast<GLint>(m_texture_id)));
    return false;
  }

protected:

  const GLenum m_texture_id;
};

// *****************************************************************************
//! \brief Sampler for 1D texture.
// *****************************************************************************
class GLSampler1D: public GLSampler
{
public:

  //----------------------------------------------------------------------------
  //! \brief See GLLocation constructor.
  //----------------------------------------------------------------------------
  GLSampler1D(const char *name, const GLenum texture_id, const GLuint prog)
    : GLSampler(name, GL_SAMPLER_1D, texture_id, prog)
  {}
};

// *****************************************************************************
//! \brief Sampler for 2D texture.
// *****************************************************************************
class GLSampler2D: public GLSampler
{
public:

  //----------------------------------------------------------------------------
  //! \brief See GLLocation constructor.
  //----------------------------------------------------------------------------
  GLSampler2D(const char *name, const GLenum texture_id, const GLuint prog)
    : GLSampler(name, GL_SAMPLER_2D, texture_id, prog)
  {}
};

// *****************************************************************************
//! \brief Sampler for 3D texture.
// *****************************************************************************
class GLSampler3D: public GLSampler
{
public:

  //----------------------------------------------------------------------------
  //! \brief See GLLocation constructor.
  //----------------------------------------------------------------------------
  GLSampler3D(const char *name, const GLenum texture_id, const GLuint prog)
    : GLSampler(name, GL_SAMPLER_3D, texture_id, prog)
  {}
};

// *****************************************************************************
//! \brief Sampler for Cubic texture.
// *****************************************************************************
class GLSamplerCube: public GLSampler
{
public:

  //----------------------------------------------------------------------------
  //! \brief See GLLocation constructor.
  //----------------------------------------------------------------------------
  GLSamplerCube(const char *name, const GLenum texture_id, const GLuint prog)
    : GLSampler(name, GL_SAMPLER_CUBE, texture_id, prog)
  {}
};

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_GLSAMPLERS_HPP
