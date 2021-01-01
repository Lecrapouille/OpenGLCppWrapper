//=====================================================================
// OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
// Copyright 2018-2020 Quentin Quadrat <lecrapouille@gmail.com>
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

#include "Scene/Geometry/Geometry.hpp"

//--------------------------------------------------------------------------
bool Geometry::generate(GLVertexBuffer<Vector3f>& vertices,
                        GLVertexBuffer<Vector3f>& normals,
                        GLVertexBuffer<Vector2f>& uv,
                        GLIndex32&                index)
{
    m_vertices = &vertices;
    m_normals = &normals;
    m_uv = &uv;
    m_index = &index;
    return doGenerate(vertices, normals, uv, index);
}
