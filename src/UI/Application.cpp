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

#include "Application.hpp"

std::list<std::unique_ptr<GLWindow>> GLApplication::m_windows;
GLWindow* GLApplication::m_window = nullptr;

//------------------------------------------------------------------------------
//! \brief Callback triggered when GLFW failed.
//------------------------------------------------------------------------------
__attribute__((__noreturn__))
static void on_GLFW_error(int /*errorCode*/, const char* msg)
{
    throw GL::Exception(msg);
}

//------------------------------------------------------------------------------
GLApplication::GLApplication()
{
    // Initialise GLFW
    glfwSetErrorCallback(on_GLFW_error);

    if (!glfwInit())
        throw GL::Exception("Failed to initialize GLFW");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Mac OS X
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
}

//------------------------------------------------------------------------------
GLApplication::~GLApplication()
{
    m_windows.clear();
    glfwTerminate();
}

//------------------------------------------------------------------------------
bool GLApplication::start()
{
    for (auto& window : m_windows)
    {
        if (!window->setup())
            return false;
    }

    while (true)
    {
        for (auto& window : m_windows)
        {
            // TODO if one of the window is full screen, do not update other
            // windows ?
            if (!window->update())
                return false;
            if (window->shouldHalt())
                return true;
        }
    }

    return true;
}
