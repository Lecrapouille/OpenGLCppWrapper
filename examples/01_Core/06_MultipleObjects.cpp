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

#include "06_MultipleObjects.hpp"
#include <iostream>

MultipleObjects::MultipleObjects()
  : m_cube1("VAO1_cube"),
    m_cube2("VAO2_cube"),
    m_floor("VAO_floor"),
    m_prog("Prog")
{
  std::cout << "Hello MultipleObjects" << std::endl;
}

MultipleObjects::~MultipleObjects()
{
  std::cout << "Bye MultipleObjects" << std::endl;
}

//------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------
void MultipleObjects::onWindowSizeChanged()
{
  // Note: height is never zero !
  float ratio = width<float>() / height<float>();

  // Make sure the viewport matches the new window dimensions.
  glCheck(glViewport(0, 0, width<int>(), height<int>()));

  m_prog.matrix44f("projection") =
    matrix::perspective(maths::toRadian(60.0f), ratio, 0.1f, 10.0f);
}

//------------------------------------------------------------------
//! \brief Create a cube.
//------------------------------------------------------------------
bool MultipleObjects::cubeSetup(GLVAO& cube, const char* texture_path)
{
  m_prog.bind(cube);

  // Fill VBOs of the VAO: init vertex positions.
  cube.vector3f("position") =
    {
      #include "geometry/cube_position.txt"
    };

  // Fill VBOs of the VAO: init texture positions.
  cube.vector2f("UV") =
    {
      #include "geometry/cube_texture.txt"
    };

  // Apply a texture
  GLTexture2D& texture = cube.texture2D("texID");
  static int i = 0; // Randomize texture settings
  if (++i % 2 == 0)
    {
      texture.interpolation(TextureMinFilter::LINEAR,
                            TextureMagFilter::LINEAR);
      texture.wrap(TextureWrap::CLAMP_TO_EDGE);
    }
  else
    {
      texture.interpolation(TextureMinFilter::NEAREST,
                            TextureMagFilter::NEAREST);
      texture.wrap(TextureWrap::MIRRORED_REPEAT);
    }
  return texture.load(texture_path);
}

//------------------------------------------------------------------
//! \brief Create a floor (= second VAO).
//------------------------------------------------------------------
bool MultipleObjects::floorSetup()
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
//! index) are shown in next examples.
//------------------------------------------------------------------
bool MultipleObjects::setup()
{
  // Enable some OpenGL states
  glCheck(glEnable(GL_DEPTH_TEST));
  glCheck(glDepthFunc(GL_LESS));
  glCheck(glDisable(GL_BLEND));
  glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  // Load vertex and fragment shaders with GLSL code.
  m_vertex_shader.fromFile("01_Core/shaders/06_MultipleObjects.vs");
  m_fragment_shader.fromFile("01_Core/shaders/06_MultipleObjects.fs");

  // Compile the shader program
  if (!m_prog.attachShaders(m_vertex_shader, m_fragment_shader).compile())
    {
      std::cerr << "Failed compiling OpenGL program. Reason was '"
                << m_prog.getError() << "'" << std::endl;
      return false;
    }

  // Init shader uniforms
  m_prog.scalarf("scale") = 1.0f;

  // Init Model-View matrices (shader uniforms). Note that projection
  // matrix is init inside onWindowSizeChanged() which is called just
  // after this method.
  m_prog.matrix44f("model") = m_transformable.transform();
  m_prog.matrix44f("view") =
    matrix::lookAt(Vector3f(3,3,3), Vector3f(0,0,0), Vector3f(0,1,0));

  // Create 2 VAOs from the same shader program: one cube and one plane.
  return floorSetup() &&
    cubeSetup(m_cube1, "textures/hazard.png") &&
    cubeSetup(m_cube2, "textures/path.png");
}

//------------------------------------------------------------------
//! \brief Paint our scene. We are using the delta time to spin
//! one cube. We show different ways to draw VAO.
//------------------------------------------------------------------
bool MultipleObjects::draw()
{
  static float time = 0.0f;
  time += dt();
  float ct = cosf(time);

  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  // Draw the first cube from the 1st VAO. Apply a "pinkished" coloration and
  // make this cube spining around itself.
  m_prog.vector4f("color") = Vector4f(0.8f, 0.2f, 0.8f, 0.8f);
  m_transformable.rotateY(4.0f * ct);                      // Apply a rotation around Y-axis
  m_transformable.position(Vector3f(-1.0f, 0.0f, -1.0f));  // Apply a translation
  m_prog.matrix44f("model") = m_transformable.transform(); // Rotate and translate the cube
  m_prog.draw(m_cube1, Mode::TRIANGLES, 0, 36);            // Style 01: pass all parameters

  // Draw a second cube from the 1st VAO (same model = same VAO). Apply a
  // "darkished" coloration and let this cube static (no motion).
  m_prog.vector4f("color") = Vector4f(0.2f, 0.2f, 0.2f, 0.2f);
  m_transformable.reset();
  m_transformable.position(Vector3f(3.0f, 0.0f, 0.0f)); // Apply a translation
  m_prog.matrix44f("model") = m_transformable.transform();
  m_prog.draw(m_cube1, Mode::TRIANGLES /*, 0, 36*/); // Style 02: do not pass first and count vertices

  // Draw a thrid cube from the 2nd VAO. Apply a "darkished" coloration and let
  // this cube static (no motion).
  m_prog.vector4f("color") = Vector4f(0.4f, 0.4f, 0.5f, 0.2f);
  m_transformable.reset();
  m_transformable.position(Vector3f(-1.0f, 0.0f, 2.0f)); // Apply a translation
  m_prog.matrix44f("model") = m_transformable.transform();
  m_prog.draw(m_cube2, Mode::TRIANGLES /*, 0, 36*/); // Style 02: do not pass first and count vertices

  // Draw the floor (second model = second VAO).
  m_prog.vector4f("color") = Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
  m_prog.bind(m_floor);
  m_transformable.reset();
  m_transformable.position(Vector3f(0.0f, 0.0f, 0.0f)); // Apply a translation
  m_prog.matrix44f("model") = m_transformable.transform();
  m_prog.draw(Mode::TRIANGLES); // Style 03: do not pass implict bound VAO and no vertices count

  return true;
}
