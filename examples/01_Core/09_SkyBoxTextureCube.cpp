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

#include "09_SkyBoxTextureCube.hpp"
#include <iostream>

//------------------------------------------------------------------------------
SkyBox::SkyBox(uint32_t const width, uint32_t const height,
               const char *title)
    : GLWindow(width, height, title),
      m_skybox("SkyBox"),
      m_prog("progSkyBox")
{
    std::cout << "Hello SkyBox: " << info() << std::endl;
}

//------------------------------------------------------------------------------
SkyBox::~SkyBox()
{
    std::cout << "Hello SkyBox: " << info() << std::endl;
}

//------------------------------------------------------------------------------
void SkyBox::onWindowSizeChanged()
{
    glCheck(glViewport(0, 0, width<int>(), height<int>()));

    m_prog.matrix44f("projection") =
            matrix::perspective(maths::toRadian(60.0f),
                                width<float>() / height<float>(),
                                0.1f,
                                10.0f);
}

// -----------------------------------------------------------------------------
//! \brief Keyboard event
// -----------------------------------------------------------------------------
void SkyBox::onKeyboardEvent()
{
    if (isKeyDown(GLFW_KEY_W) || isKeyDown(GLFW_KEY_UP))
        m_lookat += Vector3f(0.0f, 0.0f, 0.1f);
    if (isKeyDown(GLFW_KEY_S) || isKeyDown(GLFW_KEY_DOWN))
        m_lookat -= Vector3f(0.0f, 0.0f, 0.1f);
    if (isKeyDown(GLFW_KEY_A) || isKeyDown(GLFW_KEY_LEFT))
        m_lookat += Vector3f(0.1f, 0.0f, 0.0f);
    if (isKeyDown(GLFW_KEY_D) || isKeyDown(GLFW_KEY_RIGHT))
        m_lookat -= Vector3f(0.1f, 0.0f, 0.0f);
}

//------------------------------------------------------------------------------
//! \brief Create a skybox.
//------------------------------------------------------------------------------
bool SkyBox::createSkyBox()
{
    vs1.read("01_Core/shaders/09_SkyBoxTextureCube.vs");
    fs1.read("01_Core/shaders/09_SkyBoxTextureCube.fs");

    if (!m_prog.attachShaders(vs1, fs1).compile())
    {
        std::cerr << "Failed compiling OpenGL program. Reason was '"
                  << m_prog.strerror() << "'" << std::endl;
        return false;
    }

    m_prog.bind(m_skybox);

    m_skybox.vector3f("aPos") =
    {
#include "geometry/cube_position.txt"
    };

    // Add 6 textures to the sky box
    if (!m_skybox.textureCube("skybox").load(CubeMap::POSITIVE_X, "textures/right.jpg")) return false;
    if (!m_skybox.textureCube("skybox").load(CubeMap::NEGATIVE_X, "textures/left.jpg")) return false;
    if (!m_skybox.textureCube("skybox").load(CubeMap::POSITIVE_Y, "textures/top.jpg")) return false;
    if (!m_skybox.textureCube("skybox").load(CubeMap::NEGATIVE_Y, "textures/bottom.jpg")) return false;
    if (!m_skybox.textureCube("skybox").load(CubeMap::POSITIVE_Z, "textures/front.jpg")) return false;
    if (!m_skybox.textureCube("skybox").load(CubeMap::NEGATIVE_Z, "textures/back.jpg")) return false;

    return true;
}

//------------------------------------------------------------------------------
//! \brief Init your scene.
//------------------------------------------------------------------------------
bool SkyBox::onSetup()
{
    enableCallbacks(window::Event::Keyboard);

    // Enable some OpenGL states
    glCheck(glEnable(GL_DEPTH_TEST));
    glCheck(glDepthFunc(GL_LESS));
    glCheck(glDisable(GL_BLEND));
    glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    // Place a camera
    m_lookat = Vector3f(0,0,0);
    m_prog.matrix44f("model") = m_transformable.transform();
    m_prog.matrix44f("view") =
            matrix::lookAt(Vector3f(3,3,3), m_lookat, Vector3f(0,1,0));

    return createSkyBox();
}

// -----------------------------------------------------------------------------
//! \brief Draw skybox. Should be draw as last.
// -----------------------------------------------------------------------------
void SkyBox::drawSkyBox()
{
    // Remove translation from the view matrix
    Matrix44f view =
            matrix::lookAt(Vector3f(3,3,3), m_lookat, Vector3f(0,1,0));
    m_prog.matrix44f("view") = Matrix44f(Matrix33f(view));

    // Change depth function so depth test passes when values are equal
    // to depth buffer's content
    glCheck(glDepthFunc(GL_LEQUAL));
    m_prog.draw(m_skybox, Mode::TRIANGLES, 0, 36);
}

//------------------------------------------------------------------------------
//! \brief Paint our scene.
//------------------------------------------------------------------------------
bool SkyBox::onPaint()
{
    glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
    glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    drawSkyBox();

    return true;
}

//------------------------------------------------------------------------------
void SkyBox::onSetupFailed(std::string const& reason)
{
    std::cerr << "Failure during the onSetup. Reason: " << reason << std::endl;
}

//------------------------------------------------------------------------------
void SkyBox::onPaintFailed(std::string const& reason)
{
    std::cerr << "Failure during rendering. Reason: " << reason << std::endl;
}
