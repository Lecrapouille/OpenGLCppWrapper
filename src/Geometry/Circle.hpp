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

#ifndef OPENGLCPPWRAPPER_GEOMETRY_CIRCLE_HPP
#  define OPENGLCPPWRAPPER_GEOMETRY_CIRCLE_HPP

#  include "Geometry/Geometry.hpp"

namespace glwrap
{

DECLARE_CLASS(Circle)

// *****************************************************************************
//! \brief Create a Z-axis aligned circle shape (with 3d coordinates).
//! \ingroup Geometry
// *****************************************************************************
class Circle: public Shape3D
{
public:

  //----------------------------------------------------------------------------
  //! \brief Constructor. Z-axis aligned tube centered at origin.
  //! \param radius The radius of the circle.
  //! \param slices The number of subdivisions around the Z axis.
  //----------------------------------------------------------------------------
  Circle(std::string const& name, Material_SP material, float const radius, uint32_t const slices/*, bool const inversed*/)
    : Shape3D(name, material)
  {
    std::vector<float> angle;
    maths::linspace(0.0f, float(maths::TWO_PI), slices + 1u, angle, true);

    // Reserve memory
    vertices().reserve(slices + 2u);
    normals().reserve(slices + 2u);
    uv().reserve(slices + 2u);
    index().reserve(3 * slices);

    // Constants
    const float hypothenus = std::sqrt(2.0f * radius * radius);
    const float hh = radius / hypothenus;

    // Vertex at the center of the circle
    vertices().append(Vector3f(0.0f, 0.0f, 0.0f));
    normals().append(Vector3f(0.0f, 0.0f, 1.0f));
    uv().append(Vector2f(0.5f, 0.5f));

    // Vertices constituing the arc
    for (uint32_t i = 0; i <= slices; ++i)
      {
        float c = std::cos(angle[i]);
        float s = std::sin(angle[i]);

        vertices().append(Vector3f(radius * c, radius * s, 0.0f));
        normals().append(Vector3f(hh * c, hh * s, -hh));
        uv().append(Vector2f((1.0f + c) / 2.0f, (1.0f + s) / 2.0f));
      }

    const uint32_t c0 = 0u;
    const uint32_t i0 = 1u;

    // Indices for the top cap
    for (uint32_t i = 0u; i <= slices; ++i)
      {
        index().append(c0);
        index().append(i0 + i);
        index().append(i0 + i + 1u);
      }
  }

  static Circle_SP create(std::string const& name, Material_SP material, float const radius, uint32_t const slices/*, bool const inversed*/)
  {
    return std::make_shared<Circle>(name, material, radius, slices);
  }
};

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_GEOMETRY_CIRCLE_HPP
