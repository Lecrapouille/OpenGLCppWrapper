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

#ifndef OPENGLCPPWRAPPER_GEOMETRY_TUBE_HPP
#  define OPENGLCPPWRAPPER_GEOMETRY_TUBE_HPP

#  include "Material/Geometry.hpp"

// *****************************************************************************
//! \brief Create a tube shape. Tube is a generic shape that can create Cylinder
//! Cone and Pyramid.
//! \ingroup Geometry
// *****************************************************************************
class Tube: public IGeometry
{
public:

    Tube& configure(float const top_radius,
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

    //--------------------------------------------------------------------------
    //! \brief Constructor. Z-axis aligned tube centered at origin.
    //! \param[in] m_top_radius The radius at the top of the tube.
    //! \param[in] m_base_radius The radius at the base of the tube.
    //! \param[in] m_height The m_height of the tube.
    //! \param[in] m_slices The number of subdivisions around the Z axis.
    //--------------------------------------------------------------------------
    virtual bool doGenerate(GLVertexBuffer<Vector3f>& vertices,
                            GLVertexBuffer<Vector3f>& normals,
                            GLVertexBuffer<Vector2f>& uv,
                            GLIndex32&        index) override
    {
        // Create caps ?
        const bool base_caps = m_base_radius > 0.0f;
        const bool top_caps = m_top_radius > 0.0f;


        std::vector<float> angle;
        maths::linspace(0.0f, float(maths::TWO_PI), m_slices + 1u, angle, true);

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

private:

    float m_top_radius = 1.0f;
    float m_base_radius = 1.0f;
    float m_height = 1.0f;
    uint32_t m_slices = 8.0f;
};

#endif // OPENGLCPPWRAPPER_GEOMETRY_TUBE_HPP
