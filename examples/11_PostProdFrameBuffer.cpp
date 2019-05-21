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

#include "11_PostProdFrameBuffer.hpp"
#include "Maths.hpp"

//------------------------------------------------------------------
//! \file This examples show a simple cube draw in a framebuffer.
//------------------------------------------------------------------

//------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------
void GLExample11::onWindowSizeChanged(const float width, const float height)
{
  // Note: height is never zero !
  float ratio = width / height;

  // Make sure the viewport matches the new window dimensions.
  glCheck(glViewport(0, 0, static_cast<int>(width), static_cast<int>(height)));

  //m_fbo.resize(width, height);
  m_prog_scene.matrix44f("projection") =
    matrix::perspective(maths::radians(50.0f), ratio, 0.1f, 10.0f);
}

//------------------------------------------------------------------
//! \brief Init your scene.
//------------------------------------------------------------------
bool GLExample11::setup()
{
  // Enable some OpenGL states
  glCheck(glEnable(GL_DEPTH_TEST));
  glCheck(glDisable(GL_BLEND));
  glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
  //glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));

  // First program
  m_vs_scene.fromFile("shaders/11_RenderFrameBuffer_cube.vs");
  m_fs_scene.fromFile("shaders/11_RenderFrameBuffer_cube.fs");
  if (!m_prog_scene.attachShaders(m_vs_scene, m_fs_scene).compile())
    {
      std::cerr << "failed compiling OpenGL program. Reason was '"
                << m_prog_scene.getError() << "'" << std::endl;
      return false;
    }

  m_prog_scene.bind(m_cube);
  m_cube.vector3f("aPos") =
    {
       #include "geometry/cube_position.txt"
    };

  m_cube.vector2f("aTexCoords") =
    {
       #include "geometry/cube_texture.txt"
    };

  m_prog_scene.bind(m_floor);
  m_floor.vector3f("aPos") =
    {
      #include "geometry/floor_position.txt"
    };
  m_floor.vector2f("aTexCoords") =
    {
      #include "geometry/floor_texture.txt"
    };

  // Apply textures
  if (!m_cube.texture2D("texture1").load("textures/wooden-crate.jpg")) return false;
  if (!m_floor.texture2D("texture1").load("textures/path.png")) return false;

  // Init Model-View-Project matrices (shader uniforms)
  float ratio = static_cast<float>(width()) / (static_cast<float>(height()) + 0.1f);
  m_prog_scene.matrix44f("projection") =
    matrix::perspective(maths::radians(50.0f), ratio, 0.1f, 10.0f);
  m_prog_scene.matrix44f("model") = Matrix44f(matrix::Identity);
  m_prog_scene.matrix44f("view") =
    matrix::lookAt(Vector3f(3,3,3), Vector3f(0,0,0), Vector3f(0,1,0));

  // Second program
  m_vs_screen.fromFile("shaders/11_RenderFrameBuffer_screen.vs");
  m_fs_screen.fromFile("shaders/11_RenderFrameBuffer_screen.fs");
  if (!m_prog_screen.attachShaders(m_vs_screen, m_fs_screen).compile())
    {
      std::cerr << "failed compiling OpenGL program. Reason was '"
                << m_prog_screen.getError() << "'" << std::endl;
      return false;
    }

  m_prog_screen.bind(m_screen);
  m_screen.vector2f("aPos") =
    {
      Vector2f(-1.0f, 1.0f), Vector2f(-1.0f, -1.0f), Vector2f(1.0f, -1.0f),
      Vector2f(-1.0f, 1.0f), Vector2f( 1.0f, -1.0f), Vector2f(1.0f,  1.0f)
    };
  m_screen.vector2f("aTexCoords") =
    {
      Vector2f(0.0f, 1.0f), Vector2f(0.0f, 0.0f), Vector2f(1.0f, 0.0f),
      Vector2f(0.0f, 1.0f), Vector2f(1.0f, 0.0f), Vector2f(1.0f, 1.0f)
    };

  // Framebuffer
  m_fbo.resize(width(), height());
 // m_screen.texture2D("screenTexture") = m_fbo.createColorTexture();
  m_fbo.createColorTexture(m_screen.texture2D("screenTexture"));
  m_fbo.createDepthBuffer();

  return true;
}

//------------------------------------------------------------------
//! \brief Paint our scene.
//------------------------------------------------------------------
bool GLExample11::draw()
{
  // First pass: draw to the framebuffer texture
  m_fbo.render(0, 0, width(), height(), [this]() {
      glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
      glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
      glEnable(GL_DEPTH_TEST);
      m_prog_scene.draw(m_floor, Mode::TRIANGLES, 0, 6);
      m_prog_scene.draw(m_cube, Mode::TRIANGLES, 0, 36);
    });

  // Second pass: draw to the screen
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);
  m_prog_screen.scalarf("time") = dt();
  m_prog_screen.draw(m_screen, Mode::TRIANGLES, 0, 6);

  return true;
}
