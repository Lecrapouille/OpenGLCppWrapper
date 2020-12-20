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
#  include "SceneGraph/SceneGraph.hpp"

// *****************************************************************************
//! \brief Base class for predefined 3D shape (cube, cylinder ...). A Shape is
//! a geometry (purely shape construction) associated to a material (albedo ...)
// *****************************************************************************
template<class Geometry, class Material>
class Shape: public SceneObject
{
public:

    using Ptr = std::unique_ptr<Shape>;

    //--------------------------------------------------------------------------
    //! \brief Give a name to the shape.
    //--------------------------------------------------------------------------
    Shape(std::string const& name)
        : SceneObject(name), m_vao(name)
    {}

    // FIXME SceneObject::create should suffice
    /*using Ptr = std::unique_ptr<Shape<Geometry,Material>>;
    static Ptr create(std::string const& name)
    {
        Ptr obj = std::make_unique<Shape<Geometry,Material>>(name);
        if (obj->onCreate())
            throw GL::Exception("Failed creating the shape");
        return std::move(obj);
        }*/
    // FIXME

    //--------------------------------------------------------------------------
    //! \brief Compile the material shaders and populate the geomatry.
    //! \note some geometry have to be configurate before their generation ie
    //! see their configure() method.
    //--------------------------------------------------------------------------
    bool onCreate()
    {
        std::cout << "Shape::onCreate()" << std::endl;
        // FIXME since prog has MVP matrix and vertice position should not the
        // GLProgram be external ?
        if (!material.create())
        {
            std::cerr << "Failed creating material" << std::endl;
            return false;
        }

        // FIXME: Not all these attributes are needed: ie depth material only use
        // position so normals ans uv are useless
        // FIXME because of that generate() has to be placed before binding VAO but
        // should be after.
        if (!geometry.generate(m_vao.vector3f("position"),
                               m_vao.vector3f("normals"),
                               m_vao.vector2f("uv"),
                               m_vao.index()))
        {
            std::cerr << "Failed creating geometry" << std::endl;
            return false;
        }

        // Populate VBOs in the VAO (note: should be before geometry.generate()
        material.program.bind(m_vao);
        return true;
    }

    //--------------------------------------------------------------------------
    //! \brief Draw the shape.
    //--------------------------------------------------------------------------
    virtual void onDraw(Matrix44f const& modelMatrix = Identity44f) override
    {
        material.program.matrix44f("modelMatrix") = modelMatrix;
        material.program.draw(m_vao, Mode::TRIANGLES);
    }

    //--------------------------------------------------------------------------
    //! \brief Return the model matrix of the Model-View-Projection
    //--------------------------------------------------------------------------
    Matrix44f& modelMatrix()
    {
        return material.program.matrix44f("modelMatrix");
    }

    //--------------------------------------------------------------------------
    //! \brief Return the view matrix of the Model-View-Projection
    //--------------------------------------------------------------------------
    Matrix44f& viewMatrix()
    {
        return material.program.matrix44f("viewMatrix");
    }

    //--------------------------------------------------------------------------
    //! \brief Return the projection matrix of the Model-View-Projection
    //--------------------------------------------------------------------------
    Matrix44f& projectionMatrix()
    {
        return material.program.matrix44f("projectionMatrix");
    }

public:

    Material material;
    Geometry geometry;

protected:

    //! \brief VAO with index
    GLVAO32 m_vao;
};

#endif
