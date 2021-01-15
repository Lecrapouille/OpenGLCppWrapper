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

template<class Geometry, class Material>
class Shape: public SceneObject
{
public:

    Shape(std::string const& name, Mode const mode = Mode::TRIANGLES)
        : SceneObject(name),
          m_vao("VAO_" + name),
          material(m_vao), // will use the material default config
          m_drawMode(mode)
    {}

    // FIXME Does not compile but does not need
    // bool compile(Geometry::Config& geo_conf, Material::Config& mat_conf)
    // {
    //    material.config = geo_conf;
    //    geometry.config = mat_conf;
    //    return compile();
    // }

    bool compile()
    {
        if (!material.compile())
        {
            std::cerr << "Shape " << name()
                      << ": Failed creating its material"
                      << std::endl;

            return false;
        }

        // FIXME: should be called after material.program.bind(m_vao)
        // but because not all materials use attributes "normals" or "UV"
        // this will fail
        if (!geometry.generate(m_vao.vector3f("position"),
                               m_vao.vector3f("normals"),
                               m_vao.vector2f("UV"),
                               m_vao.index()))
        {
            std::cerr << "Shape " << name()
                      << ": Failed creating its geometry"
                      << std::endl;
            return false;
        }

        if (!material.program.bind(m_vao))
        {
            std::cerr << "Shape " << name()
                      << ": Failed binding its VAO "
                      << std::endl;
            return false;
        }

        return true;
    }

    virtual bool onDraw(Matrix44f const& model_matrix = Identity44f) override
    {
        modelMatrix() = model_matrix;
        return m_vao.draw(m_drawMode);
    }

    GLIndex32& index()
    {
        return m_vao.index();
    }

    GLVertexBuffer<Vector3f>& vertices()
    {
        return m_vao.vector3f("position");
    }

    GLVertexBuffer<Vector3f>& normals()
    {
        return m_vao.vector3f("normals");
    }

    GLVertexBuffer<Vector2f>& uv()
    {
        return m_vao.vector2f("UV");
    }

    Matrix44f& modelMatrix()
    {
        return material.program.matrix44f("modelMatrix");
    }

    Matrix44f& viewMatrix()
    {
        return material.program.matrix44f("viewMatrix");
    }

    Matrix44f& projectionMatrix()
    {
        return material.program.matrix44f("projectionMatrix");
    }

protected:

    GLVAO32 m_vao;

public:

    Material material;
    Geometry geometry;

private:

    Mode const m_drawMode;
};

#endif // OPENGLCPPWRAPPER_SCENEGRAPH_SHAPE_NODE_HPP
