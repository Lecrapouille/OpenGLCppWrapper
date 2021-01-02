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

#include "01_SG_MaterialsAndShapes.hpp"
#include <iostream>

//------------------------------------------------------------------------------
SGMatAndShape::SGMatAndShape(uint32_t const width, uint32_t const height,
                             const char *title)
    : GLWindow(width, height, title)
{
    std::cout << "Hello DepthMaterial: " << info() << std::endl;
}

//------------------------------------------------------------------------------
SGMatAndShape::~SGMatAndShape()
{
    std::cout << "Bye DepthMaterial" << std::endl;
}

//------------------------------------------------------------------------------
void SGMatAndShape::onWindowResized()
{
    glCheck(glViewport(0, 0, width<int>(), height<int>()));

    // Change the projction matrix of the shape
    MyModel* root = static_cast<MyModel*>(m_scene.root.get());

    root->projectionMatrix() =  // FIXME: TODO scene.updateMatrix + visitor
            matrix::perspective(maths::toRadian(60.0f),
                                width<float>() / height<float>(),
                                0.1f, 100.0f);
}

//------------------------------------------------------------------------------
bool SGMatAndShape::onSetup()
{
    m_scene.root = SceneObject::create<MyModel>("Tree0");
    MyModel& t1 = m_scene.root->attach<MyModel>("Tree1");
    MyModel& t2 = m_scene.root->attach<MyModel>("Tree2");
    t1.attach<MyModel>("Tree1.0");
    t2.attach<MyModel>("Tree2.0");
    m_scene.debug();

    return true;
}

//------------------------------------------------------------------------------
//! \brief Renderer our shape
//------------------------------------------------------------------------------
bool SGMatAndShape::onPaint()
{
    glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
    glCheck(glClear(GL_COLOR_BUFFER_BIT));

    MyModel& root = *(static_cast<MyModel*>(m_scene.root.get())); // FIXME proper solution ?

    root.modelMatrix() = Identity44f;
    root.viewMatrix() = matrix::lookAt(Vector3f(5,5,5),
                                       Vector3f(2,2,2),
                                       Vector3f(0,1,0));
    m_scene.update(dt());
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
