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

#ifndef OPENGLCPPWRAPPER_VERTEX_SHADER_HPP
#  define OPENGLCPPWRAPPER_VERTEX_SHADER_HPP

#  include "OpenGL/Shaders.hpp"

// *****************************************************************************
//! \file ShaderVertex.hpp file implements:
//!   - GLVertexShader:
// *****************************************************************************

namespace glwrap
{

// *****************************************************************************
//!
// *****************************************************************************
class GLVertexShader: public GLShader
{
public:

  GLVertexShader(std::string const& name = "VertexShader")
    : GLShader(name, GL_VERTEX_SHADER)
  {
  }

private:

  virtual const char* type() const override
  {
    return "Vertex Shader script";
  }
};

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_VERTEX_SHADER_HPP
