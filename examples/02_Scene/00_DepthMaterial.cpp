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

#include "00_DepthMaterial.hpp"
#include <iostream>

//------------------------------------------------------------------------------
ShapeDepthMaterial::ShapeDepthMaterial(uint32_t const width, uint32_t const height,
                             const char *title)
    : GLWindow(width, height, title)
{
    std::cout << "Hello DepthMaterial: " << info() << std::endl;
}

//------------------------------------------------------------------------------
ShapeDepthMaterial::~ShapeDepthMaterial()
{
    std::cout << "Bye DepthMaterial" << std::endl;
}

//------------------------------------------------------------------------------
void ShapeDepthMaterial::onWindowResized()
{
    glCheck(glViewport(0, 0, width<int>(), height<int>()));

    // Change the projction matrix of the shape
    Shape<Exported, DepthMaterial>& root = // FIXME
            *(static_cast<Shape<Exported, DepthMaterial>*>(m_scene.root.get()));

    root.projectionMatrix() =  // FIXME: TODO scene.updateMatrix + visitor
            matrix::perspective(maths::toRadian(60.0f),
                                width<float>() / height<float>(),
                                0.1f, 100.0f);
}

//------------------------------------------------------------------------------
bool ShapeDepthMaterial::onSetup()
{
    Shape<Exported, DepthMaterial>::Ptr root = // FIXME
            Shape<Exported, DepthMaterial>::create("Object");

    // Configure the shape: geometry and material
    root->geometry.select("/home/qq/MyGitHub/OpenGLCppWrapper/examples/textures/qq.obj");
    root->material.near() = 1.0f;
    root->material.far() = 10.0f;
    root->material.opacity() = 1.0f;
    std::cout << root->geometry.vertices() << std::endl;

    m_scene.root = std::move(root);
    m_scene.debug();

    return true;
}

//------------------------------------------------------------------------------
//! \brief Renderer our shape
//------------------------------------------------------------------------------
bool ShapeDepthMaterial::onPaint()
{
    glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
    glCheck(glClear(GL_COLOR_BUFFER_BIT));

    Shape<Exported, DepthMaterial>& root =
            *(static_cast<Shape<Exported, DepthMaterial>*>(m_scene.root.get()));

    root.modelMatrix() = Identity44f;
    root.viewMatrix() = matrix::lookAt(Vector3f(5,5,5),
                                    Vector3f(2,2,2),
                                    Vector3f(0,1,0));
    m_scene.update(dt());
    m_scene.draw();

    return true;
}

void ShapeDepthMaterial::onSetupFailed(std::string const& reason)
{
    std::cerr << "Failure during the setup. Reason: " << reason << std::endl;
}

void ShapeDepthMaterial::onPaintFailed(std::string const& reason)
{
    std::cerr << "Failure during rendering. Reason: " << reason << std::endl;
}
