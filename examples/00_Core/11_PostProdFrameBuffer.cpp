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

//------------------------------------------------------------------
//! \file This examples show a simple cube draw in a framebuffer.
//------------------------------------------------------------------

//------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------
void GLExample11::onWindowSizeChanged()
{
  // Note: height is never zero !
  float ratio = width<float>() / height<float>();

  // Make sure the viewport matches the new window dimensions.
  glCheck(glViewport(0, 0, width<int>(), height<int>()));

  //m_fbo.resize(width, height);
  m_prog_scene.matrix44f("projection") =
    matrix::perspective(maths::toRadian(50.0f), ratio, 0.1f, 10.0f);
}

//------------------------------------------------------------------
//! \brief Shader for drawing the scene
//------------------------------------------------------------------
bool GLExample11::firstProgram()
{
  // Compile the shader for the scene
  m_vs_scene.fromFile("shaders/11_RenderFrameBuffer_cube.vs");
  m_fs_scene.fromFile("shaders/11_RenderFrameBuffer_cube.fs");
  if (!m_prog_scene.attachShaders(m_vs_scene, m_fs_scene).compile())
    {
      std::cerr << "failed compiling OpenGL program. Reason was '"
                << m_prog_scene.getError() << "'" << std::endl;
      return false;
    }

  // Create the cube
  m_prog_scene.bind(m_cube);
  m_cube.vector3f("aPos") =
    {
       #include "../geometry/cube_position.txt"
    };

  m_cube.vector2f("aTexCoords") =
    {
       #include "../geometry/cube_texture.txt"
    };

  // Create the floor
  m_prog_scene.bind(m_floor);
  m_floor.vector3f("aPos") =
    {
      #include "../geometry/floor_position.txt"
    };
  m_floor.vector2f("aTexCoords") =
    {
      #include "../geometry/floor_texture.txt"
    };

  // Apply textures
  if (!m_cube.texture2D("texture1").load("../textures/wooden-crate.jpg")) return false;
  if (!m_floor.texture2D("texture1").load("../textures/path.png")) return false;

  // Init Model-View-Project matrices (shader uniforms)
  float ratio = width<float>() / height<float>();
  m_prog_scene.matrix44f("projection") =
    matrix::perspective(maths::toRadian(50.0f), ratio, 0.1f, 10.0f);
  m_prog_scene.matrix44f("model") = Matrix44f(matrix::Identity);
  m_prog_scene.matrix44f("view") =
    matrix::lookAt(Vector3f(3,3,3), Vector3f(0,0,0), Vector3f(0,1,0));

  return true;
}

//------------------------------------------------------------------
//! \brief Shader for drawing the screen
//------------------------------------------------------------------
bool GLExample11::secondProgram()
{
  // Compile the shader for the screen
  m_vs_screen.fromFile("shaders/11_RenderFrameBuffer_screen.vs");
  m_fs_screen.fromFile("shaders/11_RenderFrameBuffer_screen.fs");
  if (!m_prog_screen.attachShaders(m_vs_screen, m_fs_screen).compile())
    {
      std::cerr << "failed compiling OpenGL program. Reason was '"
                << m_prog_screen.getError() << "'" << std::endl;
      return false;
    }

  // Create a quad
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

  return true;
}

//------------------------------------------------------------------
//! \brief Init your scene.
//------------------------------------------------------------------
bool GLExample11::setup()
{
  glCheck(glEnable(GL_DEPTH_TEST));
  glCheck(glDisable(GL_BLEND));
  glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  if (!firstProgram()) return false;
  if (!secondProgram()) return false;

  // Framebuffer
  m_fbo.resize(width<uint32_t>(), height<uint32_t>());
  m_fbo.createColorTexture(m_screen.texture2D("screenTexture"));
  m_fbo.createDepthBuffer();

  return true;
}

//------------------------------------------------------------------
//! \brief Paint our scene.
//------------------------------------------------------------------
bool GLExample11::draw()
{
  static float time = 0.0f;
  time += dt();

  // First pass: draw to the framebuffer texture
  m_fbo.render(0u, 0u, width<uint32_t>(), height<uint32_t>(), [this]() {
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
  m_prog_screen.scalarf("time") = time;
  m_prog_screen.draw(m_screen, Mode::TRIANGLES, 0, 6);

  return true;
}
