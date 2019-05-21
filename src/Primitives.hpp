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

class Shape
{
public:

  Shape()
  {}

  ~Shape()
  {}

  inline std::vector<Vector3f> const& vertices() const
  {
    return m_positions;
  }

  inline std::vector<uint32_t> const& indices() const
  {
    return m_indices;
  }

  inline std::vector<Vector2f> const& textures() const
  {
    return m_textures;
  }

  inline std::vector<Vector3f> const& normals() const
  {
    return m_normals;
  }

protected:

  std::vector<Vector3f>   m_positions;
  std::vector<Vector3f>   m_normals;
  std::vector<Vector2f>   m_textures;
  std::vector<uint32_t>   m_indices; // todo uint8_t
};

class Tube: public Shape
{
public:

  Tube(float const top_radius, float const base_radius, float const height, uint32_t slices)
  {
    // None, Top, Bottom, Top+Bottom
    static const uint32_t caps[4] = { 0u, 1u, 1u, 2u };
    const float pi = 3.1415f;

    std::vector<float> angle;
    linspace(0.0f, 2.0f * pi, slices + 1u, angle, true);

    std::vector<float> texture;
    linspace(0.0f, 1.0f, slices + 1u, texture, true);

    // Reserve memory
    const uint32_t n = (slices + 1u) * (2u + caps[0]); // todo
    m_positions.reserve(n);
    m_normals.reserve(n);
    m_textures.reserve(n);

    //
    const float r = top_radius - base_radius;
    const float hypothenus = std::sqrt(r * r + height * height);
    const float hh = height / hypothenus;
    const float rh = -r / hypothenus;

    // Top of the tube
    for (uint32_t i = 0; i <= slices; ++i)
      {
        float c = std::cos(angle[i]);
        float s = std::sin(angle[i]);

        m_positions.push_back(Vector3f(top_radius * c, top_radius * s, height / 2.0f));
        m_normals.push_back(Vector3f(hh * c, hh * s, rh));
        m_textures.push_back(Vector2f(texture[i], 0.0f));
      }

    // Bottom of the tube
    for (uint32_t i = 0; i <= slices; ++i)
      {
        float c = std::cos(angle[i]);
        float s = std::sin(angle[i]);

        m_positions.push_back(Vector3f(base_radius * c, base_radius * s, -height / 2.0f));
        m_normals.push_back(Vector3f(hh * c, hh * s, rh));
        m_textures.push_back(Vector2f(texture[i], 1.0f));
      }

    const uint32_t i0 = 0;
    const uint32_t i1 = i0 + slices + 1;

    m_indices.reserve(4u * slices);
    for (uint32_t i = 0; i <= slices; ++i)
      {
        m_indices.push_back(i0 + i); m_indices.push_back(i0 + i + 1); m_indices.push_back(i1 + i);
        m_indices.push_back(i1 + i); m_indices.push_back(i1 + i + 1); m_indices.push_back(i0 + i + 1);
      }
  }
};

class Cylinder: public Tube
{
public:

  Cylinder(float const radius, float const height, uint32_t slices)
    : Tube(radius, radius, height, slices)
  {}
};

class Cone: public Tube
{
public:

  Cone(float const radius, float const height, uint32_t slices)
    : Tube(0.0f, radius, height, slices)
  {
    m_positions.push_back(Vector3f(0.0f, 0.0f, height / 2.0f));
  }
};

class Pyramid: public Cone
{
public:

  Pyramid(float const radius, float const height)
    : Cone(radius, height, 4)
  {}
};

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_PRIMITIVES_HPP
