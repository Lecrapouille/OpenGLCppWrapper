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

#include "Scene/Geometry/Plane.hpp"

//------------------------------------------------------------------------------
Plane& Plane::configure(float width,
                        float height,
                        size_t widthSegments,
                        size_t heightSegments)
{
    m_width = width;
    m_height = height;
    m_widthSegments = widthSegments;
    m_heightSegments = heightSegments;

    return *this;
}

//------------------------------------------------------------------------------
bool Plane::doGenerate(GLVertexBuffer<Vector3f>& vertices,
                       GLVertexBuffer<Vector3f>& normals,
                       GLVertexBuffer<Vector2f>& uv,
                       GLIndex32& index)
{
    float const width_half = m_width / 2.0f;
    float const height_half = m_height / 2.0f;
    size_t const gridX = m_widthSegments;
    size_t const gridY = m_heightSegments;
    size_t const gridX1 = gridX + 1_z;
    size_t const gridY1 = gridY + 1_z;
    float const segment_width = m_width / float(gridX);
    float const segment_height = m_height / float(gridY);

    // TODO clear + reserve
    // vertices.reserve();
    // normals.reserve();
    // uv.reserve();
    // index.reserve();

    // Generate vertices, normals and uvs
    for (size_t iy = 0_z; iy < gridY1; ++iy)
    {
        float const y = float(iy) * segment_height - height_half;
        for (size_t ix = 0_z; ix < gridX1; ++ix)
        {
            float const x = float(ix) * segment_width - width_half;
            vertices.append(Vector3f(x, -y, 0.0f));
            normals.append(Vector3f(0.0f, 0.0f, 1.0f));
            float const u = float(ix) / float(gridX);
            float const v = /*1.0f -*/ (float(iy) / float(gridY));
            uv.append(Vector2f(u, v));
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
            index.append(a); index.append(b); index.append(d);
            index.append(b); index.append(c); index.append(d);
        }
    }

    return true;
}
