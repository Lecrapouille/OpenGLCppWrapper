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

#ifndef OPENGLCPPWRAPPER_GEOMETRY_PYRAMID_HPP
#  define OPENGLCPPWRAPPER_GEOMETRY_PYRAMID_HPP

#  include "Geometry/Cone.hpp"

namespace glwrap
{

DECLARE_CLASS(Pyramid);

// *****************************************************************************
//! \brief Class holding meshes of a pyramid. A pyramid is a cone with 4 slices.
//! \ingroup Geometry
// *****************************************************************************
class Pyramid: public Cone
{
public:

  //----------------------------------------------------------------------------
  //! \brief Constructor. Z-axis aligned tube centered at origin.
  //! \param radius The radius of the pyramid.
  //! \param height The height of the pyramid.
  //----------------------------------------------------------------------------
  Pyramid(std::string const& name, Material_SP material, float const radius, float const height)
    : Cone(name, material, radius, height, 4)
  {}

  static Pyramid_SP create(std::string const& name, Material_SP material,
                           float const radius,
                           float const height)
  {
    return std::make_shared<Pyramid>(name, material, radius, height);
  }
};

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_GEOMETRY_PYRAMID_HPP
