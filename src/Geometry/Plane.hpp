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

#ifndef OPENGLCPPWRAPPER_GEOMETRY_PLANE_HPP
#  define OPENGLCPPWRAPPER_GEOMETRY_PLANE_HPP

#  include "Scene/Shape.hpp"

namespace glwrap
{

DECLARE_CLASS(Plane);

// *****************************************************************************
//! \brief Create
// *****************************************************************************
class Plane: public Shape3D
{
public:

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  Plane(std::string const& name, Material_SP material,
        float const width = 1.0f, float const height = 1.0f,
        size_t const widthSegments = 1_z, size_t const heightSegments = 1_z)
    : Shape3D(name, Mode::TRIANGLES, material)
  {
    float const width_half = width / 2.0f;
    float const height_half = height / 2.0f;
    size_t const gridX = widthSegments;
    size_t const gridY = heightSegments;
    size_t const gridX1 = gridX + 1_z;
    size_t const gridY1 = gridY + 1_z;
    float const segment_width = width / float(gridX);
    float const segment_height = height / float(gridY);

    // TODO reserve
    // vertices().reserve();
    // normals().reserve();
    // uv().reserve();
    // index().reserve();

    // Generate vertices, normals and uvs
    for (size_t iy = 0_z; iy < gridY1; ++iy)
      {
        float const y = float(iy) * segment_height - height_half;
        for (size_t ix = 0_z; ix < gridX1; ++ix)
          {
            float const x = float(ix) * segment_width - width_half;
            vertices().append(Vector3f(x, -y, 0.0f));
            normals().append(Vector3f(0.0f, 0.0f, 1.0f));
            float const u = float(ix) / float(gridX);
            float const v = /*1.0f -*/ (float(iy) / float(gridY));
            uv().append(Vector2f(u, v));
          }
      }

    // Indices
    for (size_t iy = 0_z; iy < gridY; ++iy)
      {
        for (size_t ix = 0_z; ix < gridX; ++ix)
          {
            uint32_t const a = uint32_t(ix + gridX1 * iy);
            uint32_t const b = uint32_t(ix + gridX1 * (iy + 1_z));
            uint32_t const c = uint32_t((ix + 1_z) + gridX1 * (iy + 1_z));
            uint32_t const d = uint32_t((ix + 1_z) + gridX1 * iy);

            // Faces
            index().append(a); index().append(b); index().append(d);
            index().append(b); index().append(c); index().append(d);
          }
      }
  }

  static Plane_SP create(std::string const& name, Material_SP material,
                         float const width = 1.0f, float const height = 1.0f,
                         size_t const widthSegments = 1_z, size_t const heightSegments = 1_z)
  {
    return std::make_shared<Plane>(name, material, width, height,
                                   widthSegments, heightSegments);
  }
};

} // namespace glwrap


#endif // OPENGLCPPWRAPPER_GEOMETRY_PLANE_HPP
