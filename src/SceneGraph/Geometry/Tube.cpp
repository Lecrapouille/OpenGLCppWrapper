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

#include "SceneGraph/Geometry/Tube.hpp"

//------------------------------------------------------------------------------
Tube& Tube::configure(float const top_radius,
                      float const base_radius,
                      float const height,
                      uint32_t const slices)
{
    m_top_radius = top_radius;
    m_base_radius = base_radius;
    m_height = height;
    m_slices = slices;

    return *this;
}

//------------------------------------------------------------------------------
bool Tube::doGenerate(GLVertexBuffer<Vector3f>& vertices,
                      GLVertexBuffer<Vector3f>& normals,
                      GLVertexBuffer<Vector2f>& uv,
                      GLIndex32& index)
{
    // Create caps ?
    const bool base_caps = m_base_radius > 0.0f;
    const bool top_caps = m_top_radius > 0.0f;

    std::vector<float> angle;
    maths::linspace(0.0f, maths::TWO_PI<float>, m_slices + 1u, angle, true);

    std::vector<float> texture;
    maths::linspace(0.0f, 1.0f, m_slices + 1u, texture, true);

    // Reserve memory
    //  (m_slices + 1u) vertices for by circle
    //  +1 by radius if cap it
    //  x2 because of top and base radius
    uint32_t nb_points = 2u * (m_slices + 1u);
    vertices.resize(nb_points);
    normals.resize(nb_points);
    uv.resize(nb_points);
    nb_points = 6u * (m_slices + 1u)
                + (base_caps ? 3u * m_slices : 0u)
                + (top_caps ? 3u * m_slices : 0u);
    index.reserve(nb_points);

    // Constants
    const float abs_top_radius = maths::abs(m_top_radius);
    const float abs_base_radius = maths::abs(m_base_radius);
    const float h2 = m_height / 2.0f;
    const float r = abs_top_radius - abs_base_radius;
    const float hypothenus = std::sqrt(r * r + m_height * m_height);
    const float hh = m_height / hypothenus;
    const float rh = -r / hypothenus;

    // First index for top and base radius
    uint32_t const i0 = 0u;
    uint32_t const i1 = i0 + m_slices + 1u;

    for (uint32_t i = 0; i <= m_slices; ++i)
    {
        float c = std::cos(angle[i]);
        float s = std::sin(angle[i]);

        // Top of the tube
        vertices[i0 + i] = Vector3f(abs_top_radius * c, abs_top_radius * s, h2);
        normals[i0 + i] = Vector3f(hh * c, hh * s, rh);
        uv[i0 + i] = Vector2f(texture[i], 0.0f);

        // Bottom of the tube
        vertices[i1 + i] = Vector3f(abs_base_radius * c, abs_base_radius * s, -h2);
        normals[i1 + i] = Vector3f(hh * c, hh * s, rh);
        uv[i1 + i] = Vector2f(texture[i], 1.0f);

        // Indices for the tube
        index.append(i0 + i); index.append(i0 + i + 1u); index.append(i1 + i);
        index.append(i1 + i); index.append(i1 + i + 1u); index.append(i0 + i + 1u);
    }

    if (top_caps)
    {
        // c0: Vertex at the center of the top cap
        uint32_t const c0 = uint32_t(vertices.size());
        vertices.append(Vector3f(0.0f, 0.0f, h2));
        normals.append(Vector3f(0.0f, 0.0f, 1.0f));
        uv.append(Vector2f(0.5f, 0.5f));

        for (uint32_t i = 0; i < m_slices; ++i)
        {
            index.append(c0);
            index.append(i0 + i);
            index.append(i0 + i + 1u);
        }
    }

    if (base_caps)
    {
        // c1: Vertex at the center of the bottom cap
        uint32_t const c1 = uint32_t(vertices.size());
        vertices.append(Vector3f(0.0f, 0.0f, -h2));
        normals.append(Vector3f(0.0f, 0.0f, -1.0f));
        uv.append(Vector2f(0.5f, 0.5f));

        for (uint32_t i = 0; i < m_slices; ++i)
        {
            index.append(c1);
            index.append(i1 + i);
            index.append(i1 + i + 1u);
        }
    }

    return true;
}
