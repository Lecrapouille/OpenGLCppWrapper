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

#ifndef OPENGLCPPWRAPPER_GEOMETRY_CUBE_HPP
#  define OPENGLCPPWRAPPER_GEOMETRY_CUBE_HPP

#  include "Geometry/Geometry.hpp"

namespace glwrap
{

// *****************************************************************************
//! \brief Create an unity-size cube centered at 0
// *****************************************************************************
class Cube: public Shape3D
{
public:

  Cube(std::string const& name)
    : Shape3D(name)
  {
    DEBUG("%s", "<<<---------- Fill Cube ------------------------------------------------------");

    vertices() = {
       #include "Geometry/CubeVertices.incl"
    };

    normals() = {
       #include "Geometry/CubeNormals.incl"
    };

    uv() = {
       #include "Geometry/CubeUV.incl"
    };

    color() = Vector4f(1.0f, 0.0f, 0.0f, 1.0f);

    DEBUG("%s", "------------- Fill Cube --------------------------------------------------->>>");
  }
};

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_GEOMETRY_CUBE_HPP
