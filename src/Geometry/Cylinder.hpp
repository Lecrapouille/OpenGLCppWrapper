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

#ifndef OPENGLCPPWRAPPER_GEOMETRY_CYLINDER_HPP
#  define OPENGLCPPWRAPPER_GEOMETRY_CYLINDER_HPP

#  include "Geometry/Tube.hpp"

namespace glwrap
{

DECLARE_CLASS(Cylinder)

// *****************************************************************************
//! \brief Class holding meshes of a cylinder. A cylinder is a Tube where top
//! radius is equal to the the base radius.
//! \ingroup Geometry
// *****************************************************************************
class Cylinder: public Tube
{
public:

  //----------------------------------------------------------------------------
  //! \brief Constructor. Z-axis aligned tube centered at origin.
  //! \param radius The radius of the cylinder
  //! \param height The height of the cylinder
  //! \param slices The number of subdivisions around the Z axis.
  //----------------------------------------------------------------------------
  Cylinder(std::string const& name, float const radius, float const height, uint32_t const slices)
    : Tube(name, radius, radius, height, slices)
  {}

  static Cylinder_SP create(std::string const& name,
                            float const radius,
                            float const height,
                            uint32_t const slices)
  {
    return std::make_shared<Cylinder>(name, radius, height, slices);
  }
};

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_GEOMETRY_CYLINDER_HPP
