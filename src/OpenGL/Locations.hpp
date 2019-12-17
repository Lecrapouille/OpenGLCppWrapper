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

#ifndef OPENGLCPPWRAPPER_GLLOCATIONS_HPP
#define OPENGLCPPWRAPPER_GLLOCATIONS_HPP

// *****************************************************************************
//! \file GLLocation.hpp manages shader variables (Uniforms, Samplers
//! and Attributes).
// *****************************************************************************

#  include "OpenGL/GLObject.hpp"

namespace glwrap
{

// Used by GLProgram
DECLARE_CLASS(GLLocation);

// *****************************************************************************
//! \class GLLocation Locations.hpp
//! \ingroup OpenGL
//!
//! \brief Serve as interface between a shader variable and your C++ code. It
//! allows to upload CPU data to the GPU.
//!
//! \note this class shall stay private and not be used directly by the
//! developper. Indeed this class is only managed by GLProgram.
// *****************************************************************************
class GLLocation: public GLObject<GLint>
{
  //! \brief GLProgram directly modifies GLLocation states.
  friend class GLProgram;

public:

  //----------------------------------------------------------------------------
  //! \brief Constructor. This constructor makes no other actions.
  //!
  //! \param[in] name Give a name to the instance. GLProgram uses these names in their
  //! internal hash table.
  //! \param[in] dim set the dimension of variable (1 for scalar else the
  //! dimension for vector)
  //! \param[in] gltype set the OpenGL type of data (GL_FLOAT ...)
  //! \param[in] prog set the GLProgram identifier (which is the owner of this
  //! instance).
  //----------------------------------------------------------------------------
  GLLocation(const char *name, const GLint dim, const GLenum gltype, const GLuint prog)
    : GLObject(name),
      m_dim(dim),
      m_program(prog)
  {
    m_target = gltype;
  }

  //----------------------------------------------------------------------------
  //! \brief Alias for GLObject::handle() but in a more explicit way.
  //----------------------------------------------------------------------------
  inline GLint location() const
  {
    return handle();
  }

  //----------------------------------------------------------------------------
  //! \brief Return the dimension of the shader variable.
  //----------------------------------------------------------------------------
  inline GLint dim() const
  {
    return m_dim;
  }

  //----------------------------------------------------------------------------
  //! \brief Return the type of the shader variable.
  //----------------------------------------------------------------------------
  inline GLenum gltype() const
  {
    return m_target;
  }

private:

  //----------------------------------------------------------------------------
  //! \brief Bind to a new program
  //----------------------------------------------------------------------------
  inline void setProgram(GLuint const prog)
  {
    m_program = prog;
  }

protected:

  //! \brief Dimension of the attribute variable (scalar, vector, matrix).
  const GLint  m_dim;
  //! \brief The handle of the GLProgram owning this instance.
  /*const*/ GLuint m_program; // FIXME
};

} // namespace glwrap

//#  include "OpenGL/LocationAttribute.hpp"
//#  include "OpenGL/LocationUniform.hpp"
//#  include "OpenGL/LocationSamplers.hpp"

#endif // OPENGLCPPWRAPPER_GLLOCATIONS_HPP
