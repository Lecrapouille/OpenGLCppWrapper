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
class Renderable
{
public:

    //--------------------------------------------------------------------------
    //! \brief Give a name to the shape.
    //--------------------------------------------------------------------------
    Renderable(std::string const& name)// Geometry& geo, Material& mat)
        : vao(name), material(vao)
    {
        //if (!generate())
        //    throw GL::Exception("Shape " + name + " failed during generation");
    }

    //--------------------------------------------------------------------------
    //! \brief Draw the shape.
    //--------------------------------------------------------------------------
    bool draw(Mode const mode = Mode::TRIANGLES, Matrix44f const& model_matrix = Identity44f)
    {
        modelMatrix() = model_matrix;
        return vao.draw(mode);
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
    {std::cout << "Renderable " << name() << " generate:" << std::endl;
        // Compile shader program
        if (!material.build())
        {
            std::cerr << "Shape " << name()
                      << ": Failed creating its material"
                      << std::endl;

            std::vector<std::string> names;
            size_t count = material.program.getFailedShaders(names);
            std::cout << "  Has " << count << " failed shaders: " << std::endl;
            for (auto& it: names)
            {
                std::cout << "    '" << it << "'" << std::endl;
            }

            return false;
        }

        // Display the list of Uniforms hold by the program
        {
            std::cout << "Debug GLProgram material " << name() << ":" << std::endl;
            std::vector<std::string> names;
            size_t count = material.program.getUniformNames(names);
            std::cout << "  Has " << count << " uniforms: " << std::endl;
            for (auto& it: names)
            {
                std::cout << "    '" << it << "'" << std::endl;
            }
        }

        // FIXME: avoid creating too many VAO => shared VAO
        // FIXME: Not all these attributes are needed: ie depth material only use
        // position so normals ans uv are useless
        if (!geometry.generate(vao.vector3f("position"),
                               vao.vector3f("normals"),
                               vao.vector2f("uv"),
                               vao.index()))
        {
            std::cerr << "Shape " << name()
                      << ": Failed creating its geometry"
                      << std::endl;
            return false;
        }

        // Populate VBOs in the VAO (note: should be before geometry.generate()
        if (!material.program.bind(vao))
        {
            std::cerr << "Shape " << name()
                      << ": Failed binding its VAO "
                      << std::endl;
            return false;
        }

        return true;
    }

    //--------------------------------------------------------------------------
    //! \brief Return the object name. Should be unique.
    //--------------------------------------------------------------------------
    std::string const& name() const
    {
        return vao.name();
    }

public:

    GLVAO32  vao;
    Material material;
    Geometry geometry;
};

template<class Geometry, class Material>
class Shape : public SceneObject
{
public:

    Shape(std::string const& name, Renderable<Model, BasicMaterial> &r) // TODO shared_ptr
        : SceneObject(name), renderable(r)
    {}

    //--------------------------------------------------------------------------
    //! \brief Draw the shape.
    //--------------------------------------------------------------------------
    virtual void onDraw(Matrix44f const& model_matrix = Identity44f) override
    {
        renderable.draw(Mode::TRIANGLES, model_matrix);
    }

public:

    Renderable<Geometry,Material>& renderable;
};

#endif // OPENGLCPPWRAPPER_SCENEGRAPH_SHAPE_NODE_HPP
