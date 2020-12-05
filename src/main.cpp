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

#include "main.hpp"
#include <iostream>

Example::Example(uint32_t const width, uint32_t const height, const char *title)
    : GLWindow(width, height, title),
      m_shape("Plane")
{
    std::cout << "Hello Example" << std::endl;
}

Example::~Example()
{
    std::cout << "Bye Example" << std::endl;
}

//------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------
void Example::onWindowResized()
{
    // Make the viewport matches the new window dimensions.
    glCheck(glViewport(0, 0, width<int>(), height<int>()));
}

//------------------------------------------------------------------
//! \brief Load vertex and fragment shaders. Create a VAO and fill
//! its VBOs (vertex and texture position). Load all textures.
//------------------------------------------------------------------
bool Example::onSetup()
{

    m_shape.geometry.configure(1.0f, 1.0f, 10u, 10u);
    //m_shape.material.configure(); // Pour basic material
    m_shape.compile();
    m_shape.geometry.uv() *= 2.0f;
    m_shape.material.near() = 1.0f;

    return true;
}

//------------------------------------------------------------------
//! \brief Paint our scene. Here we are using the delta time to
//------------------------------------------------------------------
bool Example::onPaint()
{
    glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
    glCheck(glClear(GL_COLOR_BUFFER_BIT));

    m_shape.draw();

    return true;
}

void Example::onSetupFailed(std::string const& reason)
{
    std::cerr << "Failure during the setup. Reason: " << reason << std::endl;
}

void Example::onPaintFailed(std::string const& reason)
{
    std::cerr << "Failure during rendering. Reason: " << reason << std::endl;
}

int main()
{
    bool res;

    try
    {
        GLApplication app;
        app.create<Example>(800u, 600u, "example1");
        res = app.start() ? EXIT_SUCCESS : EXIT_FAILURE;
    }
    catch (const GL::Exception& e)
    {
        std::cerr << e.message() << std::endl;
        res = EXIT_FAILURE;
    }

    return res;
}
