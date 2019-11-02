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

#ifndef OPENGLCPPWRAPPER_GEOMETRY_HPP
#  define OPENGLCPPWRAPPER_GEOMETRY_HPP

#  include <vector>
#  include <limits>
#  include "Common/PendingContainer.hpp"
#  include "Math/Vector.hpp"

namespace glwrap
{

static const float PI2 = static_cast<float>(2.0 * maths::PI);

// *****************************************************************************
//! \brief Class holding list of vertice positions, normals, texture coordinate
//! and vertice index.
//! \defgroup Geometry Geometry
// *****************************************************************************
class Shape
{
public:

  Shape()
  {}

  ~Shape()
  {}

  //----------------------------------------------------------------------------
  //! \brief Return the list of vertice positions.
  //----------------------------------------------------------------------------
  inline PendingContainer<Vector3f>& vertices()
  {
    // FIXME: use leazy method ? return positon();
    // position() { if (!done) { create position list; done; } return { list } }
    // this avoid creating other list like normals ...
    return m_positions;
  }

  //----------------------------------------------------------------------------
  //! \brief Return the list of vertice index.
  //----------------------------------------------------------------------------
  inline PendingContainer<uint32_t>& indices()
  {
    return m_indices;
  }

  //----------------------------------------------------------------------------
  //! \brief Return the list of texture coordinate.
  //----------------------------------------------------------------------------
  inline PendingContainer<Vector2f>& textures()
  {
    return m_textures;
  }

  //----------------------------------------------------------------------------
  //! \brief Return the list of vertices normals.
  //----------------------------------------------------------------------------
  inline PendingContainer<Vector3f>& normals()
  {
    return m_normals;
  }

protected:

  PendingContainer<Vector3f>   m_positions;
  PendingContainer<Vector3f>   m_normals;
  PendingContainer<Vector2f>   m_textures;
  PendingContainer<uint32_t>   m_indices; // TODO uint8_t ?
};

// *****************************************************************************
//! \brief Create a Z-axis aligned circle shape (with 3d coordinates).
//! \ingroup Geometry
// *****************************************************************************
class Circle: public Shape
{
public:

  //----------------------------------------------------------------------------
  //! \brief Constructor. Z-axis aligned tube centered at origin.
  //! \param radius The radius of the circle.
  //! \param slices The number of subdivisions around the Z axis.
  //----------------------------------------------------------------------------
  Circle(float const radius, uint32_t const slices/*, bool const inversed*/)
  {
    std::vector<float> angle;
    maths::linspace(0.0f, PI2, slices + 1u, angle, true);

    // Reserve memory
    m_positions.reserve(slices);
    m_normals.reserve(slices);
    m_textures.reserve(slices);

    // Constants
    const float hypothenus = std::sqrt(2.0f * radius * radius);
    const float hh = radius / hypothenus;

    // Center
    m_positions.append(Vector3f(0.0f, 0.0f, 0.0f));
    m_normals.append(Vector3f(0.0f, 0.0f, 1.0f));
    m_textures.append(Vector2f(0.5f, 0.5f));

    // Arc
    for (uint32_t i = 0; i <= slices; ++i)
      {
        float c = std::cos(angle[i]);
        float s = std::sin(angle[i]);

        m_positions.append(Vector3f(radius * c, radius * s, 0.0f));
        m_normals.append(Vector3f(hh * c, hh * s, -hh));
        m_textures.append(Vector2f((1.0f + c) / 2.0f, (1.0f + s) / 2.0f));
      }

    const uint32_t c0 = 0u;
    const uint32_t i0 = 1u;

    // Indices for the top cap
    m_indices.reserve(slices);
    for (uint32_t i = 0u; i < slices; ++i)
      {
        m_indices.append(c0); m_indices.append(i0 + i); m_indices.append(i0 + i + 1u);
      }
  }
};

// *****************************************************************************
//! \brief Create a tube shape. Tube is a generic shape that can create Cylinder
//! Cone and Pyramid.
//! \ingroup Geometry
// *****************************************************************************
class Tube: public Shape
{
public:

  //----------------------------------------------------------------------------
  //! \brief Constructor. Z-axis aligned tube centered at origin.
  //! \param top_radius The radius at the top of the tube.
  //! \param base_radius The radius at the base of the tube.
  //! \param height The height of the tube.
  //! \param slices The number of subdivisions around the Z axis.
  //----------------------------------------------------------------------------
  Tube(float const top_radius, float const base_radius, float const height, uint32_t const slices)
  {
    std::vector<float> angle;
    maths::linspace(0.0f, PI2, slices + 1u, angle, true);

    std::vector<float> texture;
    maths::linspace(0.0f, 1.0f, slices, texture, true);

    // Reserve memory
    m_positions.reserve(2u * slices);
    m_normals.reserve(2u * slices);
    m_textures.reserve(2u * slices);

    // Constants
    const float h2 = height / 2.0f;
    const float r = top_radius - base_radius;
    const float hypothenus = std::sqrt(r * r + height * height);
    const float hh = height / hypothenus;
    const float rh = -r / hypothenus;

    // Top of the tube
    for (uint32_t i = 0; i < slices; ++i)
      {
        float c = std::cos(angle[i]);
        float s = std::sin(angle[i]);

        m_positions.append(Vector3f(top_radius * c, top_radius * s, h2));
        m_normals.append(Vector3f(hh * c, hh * s, rh));
        m_textures.append(Vector2f(texture[i], 0.0f));
      }

    // Bottom of the tube
    for (uint32_t i = 0; i < slices; ++i)
      {
        float c = std::cos(angle[i]);
        float s = std::sin(angle[i]);

        m_positions.append(Vector3f(base_radius * c, base_radius * s, -h2));
        m_normals.append(Vector3f(hh * c, hh * s, rh));
        m_textures.append(Vector2f(texture[i], 1.0f));
      }

    const uint32_t i0 = 0u;
    const uint32_t i1 = i0 + slices;

    // Indices for the tube
    m_indices.reserve(3u * slices);
    for (uint32_t i = 0u; i < slices; ++i)
      {
        m_indices.append(i0 + i); m_indices.append(i0 + i + 1u); m_indices.append(i1 + i);
        m_indices.append(i1 + i); m_indices.append(i1 + i + 1u); m_indices.append(i0 + i + 1u);
      }
  }
};

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
  Cylinder(float const radius, float const height, uint32_t const slices)
    : Tube(radius, radius, height, slices)
  {}
};

// *****************************************************************************
//! \brief Class holding meshes of a cone. A cone is a Tube with no top radius.
//! \ingroup Geometry
// *****************************************************************************
class Cone: public Tube
{
public:

  //----------------------------------------------------------------------------
  //! \brief Constructor. Z-axis aligned tube centered at origin.
  //! \param radius The radius of the cone.
  //! \param height The height of the cone.
  //----------------------------------------------------------------------------
  Cone(float const radius, float const height, uint32_t const slices)
    : Tube(0.0f, radius, height, slices)
  {}
};

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
  Pyramid(float const radius, float const height)
    : Cone(radius, height, 4)
  {}
};

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_GEOMETRY_HPP
