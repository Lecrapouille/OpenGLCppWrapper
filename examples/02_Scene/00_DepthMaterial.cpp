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
    : GLWindow(width, height, title),
      m_shape("Plane")
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
    m_shape.projection() =
            matrix::perspective(maths::toRadian(60.0f),
                                width<float>() / height<float>(),
                                0.1f, 100.0f);
}

//------------------------------------------------------------------------------
bool ShapeDepthMaterial::onSetup()
{
    // Configure the shape: geometry and material
    m_shape.geometry.configure(1.0f, 1.0f, 5.0f, 10u);
    //m_shape.material.configure(); // Pour basic material

    // Compile shaders, generate the geometry
    m_shape.create();

    // If wanted: modify geometry and material values
    //m_shape.geometry.vertices() *= 2.0f;
    m_shape.material.near() = 1.0f;
    m_shape.material.far() = 10.0f;
    m_shape.material.opacity() = 1.0f;

    // You can debug the shape
    std::cout << m_shape.material.near()
              << " " << m_shape.material.far()
              << " " << m_shape.material.opacity()
              << std::endl;
    std::cout << m_shape.geometry.vertices() << std::endl;

    return true;
}

//------------------------------------------------------------------------------
//! \brief Renderer our shape
//------------------------------------------------------------------------------
bool ShapeDepthMaterial::onPaint()
{
    glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
    glCheck(glClear(GL_COLOR_BUFFER_BIT));

    m_shape.model() = Identity44f;
    m_shape.view() = matrix::lookAt(Vector3f(5,5,5),
                                    Vector3f(2,2,2),
                                    Vector3f(0,1,0));
    m_shape.draw();

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
