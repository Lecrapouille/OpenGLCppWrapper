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

#ifndef SHAPE_HPP
#  define SHAPE_HPP

#  include "SceneGraph/Geometry/Geometry.hpp"
#  include "SceneGraph/Material/Material.hpp"

// *****************************************************************************
//! \brief Base class for predefined 3D shape (cube, cylinder ...). A Shape is
//! a geometry (purely shape construction) associated to a material (albedo ...)
// *****************************************************************************
template<class Geometry, class Material>
class Shape
{
public:

    //--------------------------------------------------------------------------
    //! \brief Give a name to the shape.
    //--------------------------------------------------------------------------
    Shape(std::string const& name)
        : m_name(name), m_vao(name)
    {}

    //--------------------------------------------------------------------------
    //! \brief Compile the material shaders and populate the geomatry.
    //! \note some geometry have to be configurate before their generation ie
    //! see their configure() method.
    //--------------------------------------------------------------------------
    bool create()
    {
        // FIXME since prog has MVP matrix and vertice position should noy be
        // external ?
        if (!material.create())
            return false;

        // FIXME: Not all these attributes are needed: ie depth material only use
        // position so normals ans uv are useless
        // FIXME because of that generate() has to be placed before binding VAO but
        // should be after.
        if (!geometry.generate(m_vao.vector3f("position"),
                               m_vao.vector3f("normals"),
                               m_vao.vector2f("uv"),
                               m_vao.index()))
        {
            return false;
        }

        // Populate VBOs in the VAO (note: should be before geometry.generate()
        material.program.bind(m_vao);
        return true;
    }

    //--------------------------------------------------------------------------
    //! \brief Return the projection matrix of the Model-View-Projection
    //--------------------------------------------------------------------------
    void draw()
    {
        material.program.draw(m_vao, Mode::TRIANGLES);
    }

    //--------------------------------------------------------------------------
    //! \brief Return the model matrix of the Model-View-Projection
    //--------------------------------------------------------------------------
    Matrix44f& model()
    {
        return material.program.matrix44f("modelMatrix");
    }

   //--------------------------------------------------------------------------
    //! \brief Return the view matrix of the Model-View-Projection
    //--------------------------------------------------------------------------
     Matrix44f& view()
    {
        return material.program.matrix44f("viewMatrix");
    }

    //--------------------------------------------------------------------------
    //! \brief Return the projection matrix of the Model-View-Projection
    //--------------------------------------------------------------------------
    Matrix44f& projection()
    {
        return material.program.matrix44f("projectionMatrix");
    }

    //--------------------------------------------------------------------------
    //! \brief Return the shape name.
    //--------------------------------------------------------------------------
    inline std::string const& name() const
    {
        return m_name;
    }

public:

    Material material;
    Geometry geometry;

protected:

    std::string m_name;

    //! \brief VAO with index
    GLVAO32 m_vao;
};

#endif
