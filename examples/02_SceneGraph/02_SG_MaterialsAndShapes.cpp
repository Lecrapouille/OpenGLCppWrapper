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

#include "02_SG_MaterialsAndShapes.hpp"
#include <iostream>

//------------------------------------------------------------------------------
//! \brief
//------------------------------------------------------------------------------
template<class Material>
class MyShape: public Shape<Model, Material>
{
public:

    MyShape(std::string const& name, std::string const& path)
        : Shape<Model, Material>(name)
    {
        Shape<Model, Material>::geometry.config.path = path;
        initMaterial();
        if (!Shape<Model, Material>::compile())
        {
            throw GL::Exception("Failed create renderable");
        }
    }

    inline std::string const& name() const
    {
        return Shape<Model, Material>::name();
    }

private:

    inline void initMaterial();
};

template<> void MyShape<DepthMaterial>::initMaterial()
{
    Shape<Model, DepthMaterial>::material.near() = 0.1f;
    Shape<Model, DepthMaterial>::material.far() = 5.0f;
}

template<> void MyShape<NormalsMaterial>::initMaterial()
{
    // Nothing to do
}

template<> void MyShape<BasicMaterial>::initMaterial()
{
    Shape<Model, BasicMaterial>::material.color() = Color(1.0f, 0.0f, 0.0f).toVector3f();
}

//------------------------------------------------------------------------------
SGMatAndShape::SGMatAndShape(uint32_t const width, uint32_t const height,
                             const char *title)
    : GLWindow(width, height, title)
{
    std::cout << "Hello Material: " << info() << std::endl;
}

//------------------------------------------------------------------------------
SGMatAndShape::~SGMatAndShape()
{
    std::cout << "Bye Material" << std::endl;
}

//------------------------------------------------------------------------------
void SGMatAndShape::onWindowResized()
{
    glCheck(glViewport(0, 0, width<int>(), height<int>()));

    Matrix44f mat = matrix::perspective(maths::toRadian(60.0f),
                                        width<float>() / height<float>(),
                                        0.1f, 100.0f);

    m_scene.root->traverse([](SceneObject* node, Matrix44f const& mat_)
    {
        auto n = dynamic_cast<BaseShape*>(node);
        if (n != nullptr)
        {
            n->projectionMatrix() = mat_;
        }
    }, mat);
}

//------------------------------------------------------------------------------
bool SGMatAndShape::onSetup()
{
    glCheck(glEnable(GL_DEPTH_TEST));
    glCheck(glDepthFunc(GL_LESS));

    m_scene.root = SceneObject::create<MyShape<BasicMaterial>>("Tree0", "textures/tree.obj");
    MyShape<DepthMaterial>& t1 = m_scene.root->attach<MyShape<DepthMaterial>>("Tree1", "textures/tree.obj");
    MyShape<NormalsMaterial>& t2 = m_scene.root->attach<MyShape<NormalsMaterial>>("Tree2", "textures/tree.obj");
    MyShape<DepthMaterial>& t3 = t1.attach<MyShape<DepthMaterial>>("Tree1.0", "textures/tree.obj");
    MyShape<BasicMaterial>& t4 = t2.attach<MyShape<BasicMaterial>>("Tree1.1", "textures/tree.obj");

    //             Tree0
    //    Tree2             Tree1
    //              Tree1.0       Tree1.1
    //
    t1.transform.position(Vector3f(1.0f, 1.0f, 0.0f));
    t2.transform.position(Vector3f(-1.0f, -1.0f, 0.0f));
    t3.transform.position(Vector3f(0.5f, 0.5f, 0.0f));
    t4.transform.position(Vector3f(-0.5f, -0.5f, 0.0f));

    m_scene.debug();

    return true;
}

//------------------------------------------------------------------------------
//! \brief Renderer our shape
//------------------------------------------------------------------------------
bool SGMatAndShape::onPaint()
{
    glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
    glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    m_scene.update(dt());

    // Simulate camera
    m_scene.root->traverse([](SceneObject* node)
    {
        auto n = dynamic_cast<BaseShape*>(node);
        if (n != nullptr)
        {
            n->viewMatrix() = matrix::lookAt(Vector3f(5,5,5),
                                             Vector3f(0,0,0),
                                             Vector3f(0,1,0));
        }
    });

    m_scene.draw();

    return true;
}

//------------------------------------------------------------------------------
void SGMatAndShape::onSetupFailed(std::string const& reason)
{
    std::cerr << "Failure during the setup. Reason: " << reason << std::endl;
}

//------------------------------------------------------------------------------
void SGMatAndShape::onPaintFailed(std::string const& reason)
{
    std::cerr << "Failure during rendering. Reason: " << reason << std::endl;
}