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

#include "05_VAOvsVBO.hpp"
#include "Maths.hpp"

//------------------------------------------------------------------
//! \file This example show differences between drawing multiple VAOs
//! versus drawing multiple VBOs. This example also shows how to move
//! models using Model-View-Project matrices (change of basis).
//------------------------------------------------------------------

//------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------
void GLExample05::onWindowSizeChanged(const float width, const float height)
{
  // Note: height is never zero !
  float ratio = width / height;

  // Make sure the viewport matches the new window dimensions.
  glCheck(glViewport(0, 0, static_cast<int>(width), static_cast<int>(height)));

  m_prog.matrix44f("projection") =
    matrix::perspective(maths::radians(60.0f), ratio, 0.1f, 10.0f);
}

//------------------------------------------------------------------
//! \brief Create a cube (= first VAO).
//------------------------------------------------------------------
bool GLExample05::cubeSetup()
{
  m_prog.bind(m_cube);

  // Fill VBOs of the VAO: init vertex positions.
  m_cube.vector3f("position") =
    {
      #include "geometry/cube_position.txt"
    };

  // Fill VBOs of the VAO: init texture positions.
  m_cube.vector2f("UV") =
    {
      #include "geometry/cube_texture.txt"
    };

  GLTexture2D& texture = m_cube.texture2D("texID");
  texture.interpolation(TextureMinFilter::LINEAR, TextureMagFilter::LINEAR);
  texture.wrap(TextureWrap::CLAMP_TO_EDGE);
  return texture.load("textures/hazard.png");
}

//------------------------------------------------------------------
//! \brief Create a floor (= second VAO).
//------------------------------------------------------------------
bool GLExample05::floorSetup()
{
  // Create VBOs of the VAO.
  m_prog.bind(m_floor);

  // Fill VBOs of the VAO: init vertex positions.
  m_floor.vector3f("position") =
    {
      #include "geometry/floor_position.txt"
    };

  // Fill VBOs of the VAO: init texture positions.
  m_floor.vector2f("UV") =
    {
      #include "geometry/floor_texture.txt"
    };

  // Repeat the texture pattern
  m_floor.vector2f("UV") *= 4.0f;

  GLTexture2D& texture = m_floor.texture2D("texID");
  texture.interpolation(TextureMinFilter::LINEAR, TextureMagFilter::LINEAR);
  texture.wrap(TextureWrap::MIRRORED_REPEAT);
  return texture.load("textures/wooden-crate.jpg");
}

//------------------------------------------------------------------
//! \brief Load vertex and fragment shaders. Create two VAOs and fill
//! their VBOs and textures. Note that examples of EBO (aka vertice
//! index) are shown in examples 06 and 07.
//------------------------------------------------------------------
bool GLExample05::setup()
{
  // Enable some OpenGL states
  glCheck(glEnable(GL_DEPTH_TEST));
  glCheck(glDepthFunc(GL_LESS));
  glCheck(glDisable(GL_BLEND));
  glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  // Load vertex and fragment shaders with GLSL code.
  m_vertex_shader.fromFile("shaders/04_VAOvsVBO.vs");
  m_fragment_shader.fromFile("shaders/04_VAOvsVBO.fs");

  // Compile the shader program
  if (!m_prog.attachShaders(m_vertex_shader, m_fragment_shader).compile())
    {
      std::cerr << "failed compiling OpenGL program. Reason was '"
                << m_prog.getError() << "'" << std::endl;
      return false;
    }

  // Init shader uniforms
  m_prog.scalarf("scale") = 1.0f;

  // Init Model-View-Project matrices (shader uniforms)
  float ratio = static_cast<float>(width()) / (static_cast<float>(height()) + 0.1f);
  m_prog.matrix44f("projection") =
    matrix::perspective(maths::radians(60.0f), ratio, 0.1f, 10.0f);
  m_prog.matrix44f("model") = m_movable.transform();
  m_prog.matrix44f("view") =
    matrix::lookAt(Vector3f(3,3,3), Vector3f(0,0,0), Vector3f(0,1,0));

  // Create 2 VAOs from the same shader program: one cube and one plane.
  return floorSetup() && cubeSetup();
}

//------------------------------------------------------------------
//! \brief Paint our scene. We are using the delta time to spin
//! one cube. We show different ways to draw VAO.
//------------------------------------------------------------------
bool GLExample05::draw()
{
  static float time = 0.0f;
  time += dt();
  float ct = cosf(time);

  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  // Draw the first cube with a "pinkished" coloration.
  // Make this cube spining around itself.
  m_prog.vector4f("color") = Vector4f(0.8f, 0.2f, 0.8f, 0.8f);
  m_movable.rotate(4.0f * ct, Vector3f(0, 1, 0));    // Apply a rotation around Y-axis
  m_movable.position(Vector3f(-1.0f, 0.0f, -1.0f));  // Apply a translation
  m_prog.matrix44f("model") = m_movable.transform(); // Rotate and translate the cube
  m_prog.draw(m_cube, Mode::TRIANGLES, 0, 36);       // Style 01: pass all parameters

  // Draw a second cube (same model = same VAO) with a "darkished"
  // coloration. Make this cube static.
  m_prog.vector4f("color") = Vector4f(0.2f, 0.2f, 0.2f, 0.2f);
  m_movable.reset();
  m_movable.position(Vector3f(3.0f, 0.0f, 0.0f)); // Apply a translation
  m_prog.matrix44f("model") = m_movable.transform();
  m_prog.draw(m_cube, Mode::TRIANGLES /*, 0, 36*/); // Style 02: do not pass first and count vertices

  // Draw the floor (second model = second VAO).
  m_prog.vector4f("color") = Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
  m_prog.bind(m_floor);
  m_movable.reset();
  m_movable.position(Vector3f(0.0f, 0.0f, 0.0f)); // Apply a translation
  m_prog.matrix44f("model") = m_movable.transform();
  m_prog.draw(Mode::TRIANGLES); // Style 03: do not pass implict bound VAO and no vertices count

  return true;
}
