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

#include "03_MultiTexturedTriangle.hpp"
#include <iostream>

//------------------------------------------------------------------------------
MultiTexturedTriangle::MultiTexturedTriangle(uint32_t const width, uint32_t const height,
                                             const char *title)
    : GLWindow(width, height, title),
      m_prog("Prog"),
      m_triangle("triangle")
{
    std::cout << "Hello MultiTexturedTriangle: " << info() << std::endl;
}

//------------------------------------------------------------------------------
MultiTexturedTriangle::~MultiTexturedTriangle()
{
    std::cout << "Bye MultiTexturedTriangle" << std::endl;
}

//------------------------------------------------------------------------------
void MultiTexturedTriangle::onWindowResized()
{
    glCheck(glViewport(0, 0, width<int>(), height<int>()));
}

//------------------------------------------------------------------------------
bool MultiTexturedTriangle::loadTextures()
{
    if (!m_triangle.texture2D("blendMap").load("textures/blendMap.png"))
        return false;
    if (!m_triangle.texture2D("backgroundTexture").load("textures/grassy2.png"))
        return false;
    if (!m_triangle.texture2D("rTexture").load("textures/mud.png"))
        return false;
    if (!m_triangle.texture2D("gTexture").load("textures/grassFlowers.png"))
        return false;
    if (!m_triangle.texture2D("bTexture").load("textures/path.png"))
        return false;

    return true;
}

//------------------------------------------------------------------------------
bool MultiTexturedTriangle::onSetup()
{
    // Load vertex and fragment shaders with GLSL code.
    m_vertex_shader.read("01_Core/shaders/03_MultiTexturedTriangle.vs");
    m_fragment_shader.read("01_Core/shaders/03_MultiTexturedTriangle.fs");

    // Compile the shader program
    if (!m_prog.compile(m_vertex_shader, m_fragment_shader))
    {
        std::cerr << "Failed compiling OpenGL program. Reason was '"
                  << m_prog.strerror() << "'" << std::endl;
        return false;
    }

    // Create VBOs of the VAO.
    m_prog.bind(m_triangle);

    // Fill VBOs of the VAO: init triangle vertex positions.
    m_triangle.vector3f("position") =
    {
        Vector3f(-1.0f, -1.0f, 0.0f),
        Vector3f(1.0f, -1.0f, 0.0f),
        Vector3f(0.0f, 1.0f, 0.0f)
    };

    // Fill VBOs of the VAO: init triangle texture positions.
    m_triangle.vector2f("UV") =
    {
        Vector2f(0.0f, 0.0f),
        Vector2f(1.0f, 0.0f),
        Vector2f(0.5f, 1.0f)
    };

    // Fill Load texture files
    if (!loadTextures())
    {
        showUnloadedTextures(m_triangle);
        return false;
    }

    // Helper for debugging states of your program
    debug(m_prog);
    debug(m_triangle);

    return true;
}

//------------------------------------------------------------------------------
bool MultiTexturedTriangle::onPaint()
{
    glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
    glCheck(glClear(GL_COLOR_BUFFER_BIT));

    m_prog.draw(m_triangle, Mode::TRIANGLES, 0, 3);

    return true;
}

//------------------------------------------------------------------------------
void MultiTexturedTriangle::onSetupFailed(std::string const& reason)
{
    std::cerr << "Failure during the setup. Reason: " << reason << std::endl;
}

//------------------------------------------------------------------------------
void MultiTexturedTriangle::onPaintFailed(std::string const& reason)
{
    std::cerr << "Failure during rendering. Reason: " << reason << std::endl;
}
