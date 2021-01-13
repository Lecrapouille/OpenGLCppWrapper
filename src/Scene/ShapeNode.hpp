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

#ifndef OPENGLCPPWRAPPER_SCENEGRAPH_SHAPE_NODE_HPP
#  define OPENGLCPPWRAPPER_SCENEGRAPH_SHAPE_NODE_HPP

#  include "OpenGL/Buffers/iVAO.hpp"
#  include "Scene/Geometry/Geometry.hpp"
#  include "Scene/Material/Material.hpp"
#  include "Scene/SceneTree.hpp"

// *****************************************************************************
//! \brief Base class for predefined 3D shape (cube, cylinder ...). A Shape is
//! a geometry (purely shape construction) associated to a material (albedo ...)
// *****************************************************************************
template<class Geometry, class Material>
class Shape: public SceneObject
{
public:

    //--------------------------------------------------------------------------
    //! \brief Give a name to the shape.
    //--------------------------------------------------------------------------
    Shape(std::string const& name, Geometry& geo, Material& mat)
        : SceneObject(name), material(mat), geometry(geo), m_vao(name)
    {
        if (!generate())
            throw GL::Exception("Shape " + name + " failed during generation");
    }

    //--------------------------------------------------------------------------
    //! \brief Draw the shape.
    //--------------------------------------------------------------------------
    virtual void onDraw(Matrix44f const& model_matrix = Identity44f) override
    {
        modelMatrix() = model_matrix;
        m_vao.draw(Mode::TRIANGLES);
    }

    //--------------------------------------------------------------------------
    //! \brief Return the model matrix of the Model-View-Projection
    //--------------------------------------------------------------------------
    inline Matrix44f& modelMatrix()
    {
        return material.program.matrix44f("modelMatrix");
    }

    //--------------------------------------------------------------------------
    //! \brief Return the view matrix of the Model-View-Projection
    //--------------------------------------------------------------------------
    inline Matrix44f& viewMatrix()
    {
        return material.program.matrix44f("viewMatrix");
    }

    //--------------------------------------------------------------------------
    //! \brief Return the projection matrix of the Model-View-Projection
    //--------------------------------------------------------------------------
    inline Matrix44f& projectionMatrix()
    {
        return material.program.matrix44f("projectionMatrix");
    }

    bool generate()
    {
        if (!material.build())
        {
            std::cerr << "Shape " << name()
                      << ": Failed creating its material"
                      << std::endl;
            return false;
        }

        // FIXME: avoid creating too many VAO => shared VAO
        // FIXME: Not all these attributes are needed: ie depth material only use
        // position so normals ans uv are useless
        if (!geometry.generate(m_vao.vector3f("position"),
                               m_vao.vector3f("normals"),
                               m_vao.vector2f("uv"),
                               m_vao.index()))
        {
            std::cerr << "Shape " << name()
                      << ": Failed creating its geometry"
                      << std::endl;
            return false;
        }

        // Populate VBOs in the VAO (note: should be before geometry.generate()
        if (!material.program.bind(m_vao))
        {
            std::cerr << "Shape " << name()
                      << ": Failed binding its VAO "
                      << std::endl;
            return false;
        }

        return true;
    }

public:

    Material& material;
    Geometry& geometry;

protected:

    //! \brief VAO with index
    GLVAO32 m_vao;
};

#endif // OPENGLCPPWRAPPER_SCENEGRAPH_SHAPE_NODE_HPP
