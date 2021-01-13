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

#ifndef GEOMETRY_HPP
#  define GEOMETRY_HPP

#  include "OpenGL/Buffers/VBO.hpp"
#  include "OpenGL/Buffers/EBO.hpp"
#  include "Math/Vector.hpp"

// *****************************************************************************
//! \brief Base class for generating vertex positions, normals, texture
//! coordinates of predefined shapes (cube, tube ...)
// *****************************************************************************
class Geometry
{
public:

    //--------------------------------------------------------------------------
    //! \brief Needed because of virtual methods
    //--------------------------------------------------------------------------
    virtual ~Geometry() = default;

    //--------------------------------------------------------------------------
    //! \brief Entry point method for generating the geometry of the shape.
    //! \return true on success, false on failure.
    //--------------------------------------------------------------------------
    bool generate(GLVertexBuffer<Vector3f>& vertices,
                  GLVertexBuffer<Vector3f>& normals,
                  GLVertexBuffer<Vector2f>& uv,
                  GLIndex32&                index);

    //--------------------------------------------------------------------------
    //! \brief Return the VBO holding vertex positions.
    //--------------------------------------------------------------------------
    GLVertexBuffer<Vector3f>& vertices()
    {
        if (m_vertices == nullptr)
            throw GL::Exception("You forget to call generate(vertice, normals, uv, index)");

        return *m_vertices;
    }

    //--------------------------------------------------------------------------
    //! \brief Return the VBO holding vertex normals.
    //--------------------------------------------------------------------------
    GLVertexBuffer<Vector3f>& normals()
    {
        if (m_vertices == nullptr)
            throw GL::Exception("You forget to call generate(vertice, normals, uv, index)");

        return *m_normals;
    }

    //--------------------------------------------------------------------------
    //! \brief Return the VBO holding texture positions.
    //--------------------------------------------------------------------------
    GLVertexBuffer<Vector2f>& uv()
    {
        if (m_vertices == nullptr)
            throw GL::Exception("You forget to call generate(vertice, normals, uv, index)");

        return *m_uv;
    }

    //--------------------------------------------------------------------------
    //! \brief Return the index for vertex positions.
    //--------------------------------------------------------------------------
    GLIndex32& index()
    {
        if (m_vertices == nullptr)
            throw GL::Exception("You forget to call generate(vertice, normals, uv, index)");

        return *m_index;
    }

private:

    //--------------------------------------------------------------------------
    //! \brief Method generating the shape (to be implemented by derived class)
    //--------------------------------------------------------------------------
    virtual bool doGenerate(GLVertexBuffer<Vector3f>& vertices,
                            GLVertexBuffer<Vector3f>& normals,
                            GLVertexBuffer<Vector2f>& uv,
                            GLIndex32&        index) = 0;

private:

    GLVertexBuffer<Vector3f>* m_vertices = nullptr;
    GLVertexBuffer<Vector3f>* m_normals = nullptr;
    GLVertexBuffer<Vector2f>* m_uv = nullptr;
    GLIndex32*                m_index = nullptr;
};

#endif
