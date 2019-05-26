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

#ifndef OPENGLCPPWRAPPER_PRIMITIVES_HPP
#  define OPENGLCPPWRAPPER_PRIMITIVES_HPP

#  include <vector>
#  include <limits>
#  include "Vector.hpp"

namespace glwrap
{

// *****************************************************************************
//! \brief Return evenly spaced numbers over a specified interval.
//! \param[in] start The starting scalar value of the sequence.
//! \param[in] end The end value of the sequence, unless endpoint is set to false.
//!    In that case, the sequence consists of all but the last of num + 1
//!    evenly spaced samples, so that stop is excluded.
//! \param[in] N Number of samples to generate. Should be > 1.
//! \param[out] The vector of equally spaced samples in the closed interval [start, stop]
//!   or the half-open interval [start, stop) (depending on whether endpoint is true or false).
//! \return Size of spacing between samples. Return NaN if this value cannot be
//! computed.
//! \note: This code has been inpsired by the Numpy.linspace function.
// *****************************************************************************
template <typename T>
static T linspace(T const start, T const end, size_t const N, std::vector<T>& result, const bool endpoint)
{
  const T not_a_number = std::numeric_limits<T>::quiet_NaN();
  result.clear();

  if (0 == N) return not_a_number;
  if (1 == N)
    {
      result.push_back(start);
      return not_a_number;
    }

  result.reserve(N);
  T delta = (end - start) / static_cast<T>(N-1);
  for (size_t i = 0; i < N-1; ++i)
    {
      result.push_back(start + delta * T(i));
    }

  if (endpoint)
    {
      result.push_back(end);
    }
  return delta;
}

// *****************************************************************************
//! \brief Class holding list of vertice positions, normals, texture coordinate
//! and vertice index.
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
//! \brief Create a tube shape
// *****************************************************************************
class Circle: public Shape
{
public:

  Circle(float const radius, uint32_t const slices/*, bool const inversed*/)
  {
    const float pi = 3.1415f;

    std::vector<float> angle;
    linspace(0.0f, 2.0f * pi, slices + 1u, angle, true);

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
//! \brief Create a tube shape
// *****************************************************************************
class Tube: public Shape
{
public:

  //----------------------------------------------------------------------------
  //! \brief Constructor. Z-axis aligned tube centered at origin.
  //! \param top_radius The radius at the top of the cylinder
  //! \param base_radius The radius at the base of the cylinder
  //! \param height The height of the cylinder
  //! \param slices The number of subdivisions around the Z axis.
  //----------------------------------------------------------------------------
  Tube(float const top_radius, float const base_radius, float const height, uint32_t const slices)
  {
    // None, Top, Bottom, Top+Bottom
    const float pi = 3.1415f;

    std::vector<float> angle;
    linspace(0.0f, 2.0f * pi, slices, angle, true);

    std::vector<float> texture;
    linspace(0.0f, 1.0f, slices, texture, true);

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

//----------------------------------------------------------------------------
//! \brief
//----------------------------------------------------------------------------
class Cylinder: public Tube
{
public:

  Cylinder(float const radius, float const height, uint32_t const slices)
    : Tube(radius, radius, height, slices)
  {}
};

//----------------------------------------------------------------------------
//! \brief
//----------------------------------------------------------------------------
class Cone: public Tube
{
public:

  Cone(float const radius, float const height, uint32_t const slices)
    : Tube(0.0f, radius, height, slices)
  {}
};

//----------------------------------------------------------------------------
//! \brief
//----------------------------------------------------------------------------
class Pyramid: public Cone
{
public:

  Pyramid(float const radius, float const height)
    : Cone(radius, height, 4)
  {}
};

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_PRIMITIVES_HPP
