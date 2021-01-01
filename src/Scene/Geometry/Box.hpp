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

#ifndef BOX_HPP
#  define BOX_HPP

#  include "SceneGraph/Geometry/Geometry.hpp"

// *****************************************************************************
//! \brief Generate the geometry of a box.
// *****************************************************************************
class Box: public Geometry
{
public:

    //--------------------------------------------------------------------------
    //! \brief Configure the shape.
    //! Call it before calling generate() else default parameters will be used.
    //--------------------------------------------------------------------------
    Box& configure(float width,
                   float height,
                   float depth,
                   size_t segmentsWidth,
                   size_t segmentsHeight,
                   size_t segmentsDepth)
    {
        m_width = width;
        m_height = height;
        m_depth = depth;
        m_segmentsWidth = segmentsWidth;
        m_segmentsHeight = segmentsHeight;
        m_segmentsDepth = segmentsDepth;

        return *this;
    }

private:

    //--------------------------------------------------------------------------
    //! \brief Generate the box.
    //--------------------------------------------------------------------------
    virtual bool doGenerate(GLVertexBuffer<Vector3f>& vertices,
                            GLVertexBuffer<Vector3f>& normals,
                            GLVertexBuffer<Vector2f>& uv,
                            GLIndex32&        index) override
    {
        vertices =
        {
            Vector3f(-0.5f,  0.5f,  0.5f),  // Front Top Left
            Vector3f( 0.5f,  0.5f,  0.5f),  // Front Top Right
            Vector3f( 0.5f, -0.5f,  0.5f),  // Front Bottom Right
            Vector3f(-0.5f, -0.5f,  0.5f),  // Front Bottom Left
            Vector3f(-0.5f,  0.5f, -0.5f),  // Back Top Left
            Vector3f( 0.5f,  0.5f, -0.5f),  // Back Top Right
            Vector3f( 0.5f, -0.5f, -0.5f),  // Back Bottom Right
            Vector3f(-0.5f, -0.5f, -0.5f),  // Back Bottom Left
        };

        normals =
        {
        };

        uv =
        {
        };

        index =
        {
            0u,3u,2u,  // Front
            2u,1u,0u,
            1u,5u,6u,  // Right
            6u,2u,1u,
            5u,4u,7u,  // Left
            7u,6u,5u,
            4u,7u,3u,  // Back
            3u,0u,4u,
            4u,5u,1u,  // Top
            1u,0u,4u,
            3u,2u,6u,  // Bottom
            6u,7u,3u,
        };

        return true;
    }

private:

    float m_width = 1.0f;
    float m_height = 1.0f;
    float m_depth = 1.0f;
    size_t m_segmentsWidth = 1_z;
    size_t m_segmentsHeight = 1_z;
    size_t m_segmentsDepth = 1_z;
};

#endif
