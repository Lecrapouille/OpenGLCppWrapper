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

#ifndef OPENGLCPPWRAPPER_GEOMETRY_AXIS_HPP
#  define OPENGLCPPWRAPPER_GEOMETRY_AXIS_HPP

#  include "Scene/Polyline.hpp"

namespace glwrap
{

DECLARE_CLASS(AxesHelper);

// *****************************************************************************
//! \brief Create a Z-axis aligned circle shape (with 3d coordinates).
//! \ingroup Geometry
// *****************************************************************************
class AxesHelper: public Polyline3D
{
public:

  //----------------------------------------------------------------------------
  //! \brief Constructor.
  //----------------------------------------------------------------------------
  AxesHelper(std::string const& name, float const size = 1.0f, Material_SP material = LineBasicMaterial::create())
    : Polyline3D(name, Mode::LINES, material)
  {
    vertices() =
    {
       Vector3f(0.0f, 0.0f, 0.0f),  Vector3f(size, 0.0f, 0.0f),
       Vector3f(0.0f, 0.0f, 0.0f),  Vector3f(0.0f, size, 0.0f),
       Vector3f(0.0f, 0.0f, 0.0f),  Vector3f(0.0f, 0.0f, size)
    };

    colors() =
    {
       Vector4f(1.0f, 0.6f, 0.0f, 1.0f),  Vector4f(1.0f, 0.0f, 0.0f, 1.0f),
       Vector4f(0.6f, 1.0f, 0.0f, 1.0f),  Vector4f(0.0f, 1.0f, 0.0f, 1.0f),
       Vector4f(0.0f, 0.6f, 1.0f, 1.0f),  Vector4f(0.0f, 0.0f, 1.0f, 1.0f)
    };
  }

  static AxesHelper_SP create(std::string const& name, float const size = 1.0f, Material_SP material = LineBasicMaterial::create())
  {
    return std::make_shared<AxesHelper>(name, size, material);
  }
};

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_GEOMETRY_AXIS_HPP
