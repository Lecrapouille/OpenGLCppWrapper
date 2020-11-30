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

#include "02_TexturedTriangle.hpp"
#include <iostream>

//------------------------------------------------------------------------------
//! \brief
//------------------------------------------------------------------------------
TexturedTriangle::TexturedTriangle(uint32_t const width, uint32_t const height,
                                   const char *title)
    : GLWindow(width, height, title),
      m_prog("Prog"),
      m_triangle("triangle")
{
    std::cout << "Hello TexturedTriangle" << std::endl;
}

//------------------------------------------------------------------------------
//! \brief
//------------------------------------------------------------------------------
TexturedTriangle::~TexturedTriangle()
{
    std::cout << "Bye TexturedTriangle" << std::endl;
}

//------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------
void TexturedTriangle::onWindowResized()
{
    // Make the viewport matches the new window dimensions.
    glCheck(glViewport(0, 0, width<int>(), height<int>()));
}

//------------------------------------------------------------------
//! \brief
//------------------------------------------------------------------
bool TexturedTriangle::loadTextures()
{
    GLTexture2D& texture = m_triangle.texture2D("texID");
    texture.interpolation(GLTexture::Minification::LINEAR,
                          GLTexture::Magnification::LINEAR);
    texture.wrap(GLTexture::Wrap::MIRRORED_REPEAT);
    return texture.load("textures/hazard.png");
}

//------------------------------------------------------------------
//! \brief Load vertex and fragment shaders. Create a VAO and fill
//! its VBOs (vertex and texture position). Load all textures.
//------------------------------------------------------------------
bool TexturedTriangle::onSetup()
{
    // Load vertex and fragment shaders with GLSL code.
    m_vertex_shader.read("01_Core/shaders/02_TexturedTriangle.vs");
    m_fragment_shader.read("01_Core/shaders/02_TexturedTriangle.fs");

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

    // Repeat the texture motif
    m_triangle.vector2f("UV") *= 4.0f;

    // Fill Load texture files
    if (!loadTextures())
    {
        // In case of failure show which textures has failed.
        std::vector<std::string> list;
        size_t count = m_triangle.getUnloadedTextures(list);
        std::cerr << "Failed loading " << count << " textures:" << std::endl;
        for (auto& it: list)
        {
            std::cerr << " " << it;
        }
        std::cerr << std::endl;
        return false;
    }

    // Helper for debugging states of your program
    debug(m_prog);
    debug(m_triangle);

    return true;
}

//------------------------------------------------------------------
//! \brief Paint our scene. Here we are using the delta time to
//------------------------------------------------------------------
bool TexturedTriangle::onPaint()
{
    glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
    glCheck(glClear(GL_COLOR_BUFFER_BIT));

    m_prog.draw(m_triangle, Mode::TRIANGLES, 0, 3);

    return true;
}

//------------------------------------------------------------------------------
//! \brief Callback when onSetup() failed.
//------------------------------------------------------------------------------
void TexturedTriangle::onSetupFailed(std::string const& reason)
{
    std::cerr << "Failure during the setup. Reason: " << reason << std::endl;
}

//------------------------------------------------------------------------------
//! \brief Callback when onPaint() failed.
//------------------------------------------------------------------------------
void TexturedTriangle::onPaintFailed(std::string const& reason)
{
    std::cerr << "Failure during rendering. Reason: " << reason << std::endl;
}
