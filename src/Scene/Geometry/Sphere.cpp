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

#include "Scene/Geometry/Sphere.hpp"

//------------------------------------------------------------------------------
static void repeat(std::vector<float> const& input, size_t const N,
                   std::vector<float>& output)
{
    output.clear();
    output.reserve(N * input.size());
    for (auto const it: input)
    {
        for (uint32_t i = 0u; i < N; ++i)
        {
            output.push_back(it);
        }
    }
}

//------------------------------------------------------------------------------
static void tile(std::vector<float> const& input, size_t const N,
                 std::vector<float>& output)
{
    output.clear();
    output.reserve(N * input.size());
    for (uint32_t i = 0u; i < N; ++i)
    {
        for (auto const it: input)
        {
            output.push_back(it);
        }
    }
}

//------------------------------------------------------------------------------
Sphere& Sphere::configure(float const radius, uint32_t const slices, uint32_t const stacks)
{
    m_radius = radius;
    m_slices = slices;
    m_stacks = stacks;
    return *this;
}

//------------------------------------------------------------------------------
bool Sphere::doGenerate(GLVertexBuffer<Vector3f>& vertices,
                        GLVertexBuffer<Vector3f>& normals,
                        GLVertexBuffer<Vector2f>& uv,
                        GLIndex32& index)
{
    std::vector<float> tmp, theta1, theta2;
    const uint32_t n = (m_slices + 1u) * (m_stacks + 1u);

    // Vertices
    maths::linspace(0.0f, maths::PI<float>, m_stacks, tmp, true);
    repeat(tmp, m_slices, theta1);
    maths::linspace(0.0f, maths::TWO_PI<float>, m_slices, tmp, true);
    tile(tmp, m_stacks, theta2);

    vertices.resize(n);
    for (size_t i = 0u; i < vertices.size(); ++i)
    {
        vertices[i] = Vector3f(
            std::sin(theta1[i]) * std::sin(theta2[i]) * m_radius,
            std::sin(theta1[i]) * std::cos(theta2[i]) * m_radius,
            std::cos(theta1[i]) * m_radius);
    }

    // Normals
    normals = vertices;

    // Texture coord
    maths::linspace(0.0f, 1.0f, m_slices, tmp, true);
    repeat(tmp, m_stacks, theta1);
    maths::linspace(1.0f, 0.0f, m_stacks, tmp, true);
    tile(tmp, m_slices, theta2);

    for (size_t i = 0u; i < tmp.size(); ++i)
    {
        uv[i] = Vector2f(theta1[i], theta2[i]);
    }

    // Indices
    for (uint32_t i = 0u; i < m_stacks - 1u; ++i)
    {
        for (uint32_t j = 0u; j < m_slices - 1u; ++j)
        {
            index.append(i * m_slices + j);
            index.append(i * m_slices + j + 1u);
            index.append(i * m_slices + j + m_slices + 1u);
            index.append(i * m_slices + j + m_slices);
            index.append(i * m_slices + j + m_slices + 1u);
            index.append(i * m_slices + j);
        }
    }

    return true;
}
