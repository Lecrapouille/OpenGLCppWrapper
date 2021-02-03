//=====================================================================
// OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
// Copyright 2018-2021 Quentin Quadrat <lecrapouille@gmail.com>
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
    Shape<Model, BasicMaterial>::material.color() = Color(0.5f, 0.0f, 0.0f).toVector3f();
    Shape<Model, BasicMaterial>::material.diffuse() = Color(0.4f, 0.0f, 0.0f).toVector3f();
    Shape<Model, BasicMaterial>::material.opacity() = 1.0f;
}

//------------------------------------------------------------------------------
SGMatAndShape::SGMatAndShape(uint32_t const width, uint32_t const height,
                             const char *title)
    : GLWindow(width, height, title),
      m_camera("camera"),
      m_imgui(*this)
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
    m_camera.projection(width<float>(), height<float>());
}

//------------------------------------------------------------------------------
bool SGMatAndShape::onSetup()
{
    glCheck(glEnable(GL_DEPTH_TEST));
    glCheck(glDepthFunc(GL_LESS));
    glCheck(glEnable(GL_BLEND));
    glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    //glCheck(glDisable(GL_CULL_FACE));

    m_scene.root = AxesHelper::create<AxesHelper>("Axis", 10.0f);
    m_scene.root->attach<MyShape<BasicMaterial>>("Tree0", "textures/tree.obj");
    MyShape<DepthMaterial>& t1 = m_scene.root->attach<MyShape<DepthMaterial>>("Tree1", "textures/tree.obj");
    MyShape<NormalsMaterial>& t2 = m_scene.root->attach<MyShape<NormalsMaterial>>("Tree2", "textures/tree.obj");
    MyShape<DepthMaterial>& t3 = t1.attach<MyShape<DepthMaterial>>("Tree1.0", "textures/tree.obj");
    MyShape<BasicMaterial>& t4 = t1.attach<MyShape<BasicMaterial>>("Tree1.1", "textures/tree.obj");

    //      Y
    //     |
    //     |
    //     |________ X
    //    /
    //   /          Tree0
    //  /  Tree2             Tree1
    // Z             Tree1.0       Tree1.1
    //
    t1.transform.position(Vector3f(2.0f, 0.0f, 0.0f));
    t2.transform.position(Vector3f(0.0f, 0.0f, 2.0f));
    t3.transform.position(Vector3f(0.0f, 0.0f, 2.0f));
    t4.transform.position(Vector3f(2.0f, 0.0f, 0.0f));

    //m_scene.debug();
    //return false;
    return m_imgui.setup(*this);
}

//------------------------------------------------------------------
//! \brief Paint some DearImGui widgets.
//------------------------------------------------------------------
bool SGMatAndShape::GUI::render()
{
    static float new_near = 3.0f;
    static float previous_near = 0.0f;
    static float new_far = 7.0f;
    static float previous_far = 0.0f;

    ImGui::Begin("Hello, world!");
    ImGui::Text("Depth Material:");
    ImGui::SliderFloat("near", &new_near, 0.01f, 15.0f);
    ImGui::SliderFloat("far ", &new_far, 0.01f, 15.0f);
    ImGui::End();

    // Apply new depth near value to the shape
    if (abs(new_near - previous_near) > 0.001f)
    {
        previous_near = new_near;
        m_window.m_scene.root->traverse([](SceneObject* node, float const near_)
        {
            auto n = dynamic_cast<MyShape<DepthMaterial>*>(node);
            if (n != nullptr)
            {
                n->material.near() = near_;
            }
        }, new_near);
    }

    // Apply new depth far value to the shape
    if (abs(new_far - previous_far) > 0.001f)
    {
        previous_far = new_far;
        m_window.m_scene.root->traverse([](SceneObject* node, float const far_)
        {
            auto n = dynamic_cast<MyShape<DepthMaterial>*>(node);
            if (n != nullptr)
            {
                n->material.far() = far_;
            }
        }, new_far);
    }

    return true;
}

//------------------------------------------------------------------------------
//! \brief Renderer our shape
//------------------------------------------------------------------------------
bool SGMatAndShape::onPaint()
{
    glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
    glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    // Update parent-child transform matrix for nodes
    m_scene.update(dt());

    // Camera movement
    m_camera.transform.lookAt(Vector3f(5,5,5),
                              Vector3f(0,0,0),
                              Vector3f(0,1,0));

    // Perspective camera
    m_camera.setMode(Camera::Type::PERSPECTIVE);
    m_camera.setViewPort(0.0f, 0.0f, 0.5f, 1.0f);
    m_scene.draw(m_camera);

    // Orthographic camera
    m_camera.setMode(Camera::Type::ORTHOGRAPHIC);
    m_camera.setViewPort(0.5f, 0.0f, 1.0f, 1.0f);
    m_scene.draw(m_camera);

    // DearImGui
    return m_imgui.draw();
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
