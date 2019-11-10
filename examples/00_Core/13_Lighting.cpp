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

#include "13_Lighting.hpp"

// lighting
static Vector3f lightPos(1.2f, 1.0f, 2.0f);

// --------------------------------------------------------------
//! \brief Window event
// --------------------------------------------------------------
void GLExample13::onWindowSizeChanged()
{
  // Make sure the viewport matches the new window dimensions.
  glCheck(glViewport(0, 0, width<int>(), height<int>()));

  Matrix44f const& proj =
    m_cameraController.camera().updateProjectionMatrix();
  m_prog_cube.matrix44f("projection") = proj;
  m_prog_lamp.matrix44f("projection") = proj;
}

// --------------------------------------------------------------
//! \brief Mouse event
// --------------------------------------------------------------
void GLExample13::onMouseMoved(Mouse const& mouse)
{
  float const dx = static_cast<float>(mouse.displacement.x);
  float const dy = static_cast<float>(mouse.displacement.y);
  m_cameraController.processMouseMovement(dx, dy, dt());
  //PerspectiveCamera3D& camera = m_cameraController.camera();
  //m_prog_lamp.matrix44f("view") = camera.viewMatrix();
  //m_prog_cube.matrix44f("view") = camera.viewMatrix();
  //m_prog_cube.vector3f("viewPos") = camera.position();
}

// --------------------------------------------------------------
//! \brief Mouse event
// --------------------------------------------------------------
void GLExample13::onMouseScrolled(Mouse const& mouse)
{
  float const z = static_cast<float>(mouse.scroll.y);
  Matrix44f const& proj = m_cameraController.zoom(z);
  m_prog_cube.matrix44f("projection") = proj;
  m_prog_lamp.matrix44f("projection") = proj;
}

//------------------------------------------------------------------
//! \brief Create a cube for the lamp.
//------------------------------------------------------------------
bool GLExample13::createLamp()
{
  // Load vertex and fragment shaders with GLSL code.
  m_vs1.fromFile("shaders/13_Lighting_lamp.vs");
  m_fs1.fromFile("shaders/13_Lighting_lamp.fs");

  // Compile the shader program
  if (!m_prog_lamp.attachShaders(m_vs1, m_fs1).compile())
    {
      std::cerr << "Failed compiling OpenGL program. Reason was '"
                << m_prog_lamp.getError() << "'" << std::endl;
      return false;
    }

  // Create a cube.
  m_prog_lamp.bind(m_lamp);
  m_lamp.vector3f("aPos") =
    {
      #include "../geometry/cube_position.txt"
    };

  float ratio = width<float>() / height<float>();
  m_prog_lamp.matrix44f("projection") =
    matrix::perspective(maths::toRadian(50.0f), ratio, 0.1f, 100.0f);

  Transformable<float, 3U> transformable;
  transformable.position(lightPos);
  transformable.scale(Vector3f(0.05f)); // a smaller cube
  m_prog_lamp.matrix44f("model") = transformable.transform();
  m_prog_lamp.matrix44f("view") = m_cameraController.camera().viewMatrix();

  return true;
}

//------------------------------------------------------------------
//! \brief Create a cube with material.
//------------------------------------------------------------------
bool GLExample13::createCube()
{
  // Load vertex and fragment shaders with GLSL code.
  m_vs2.fromFile("shaders/13_Lighting_material.vs");
  m_fs2.fromFile("shaders/13_Lighting_material.fs");

  // Compile the shader program
  if (!m_prog_cube.attachShaders(m_vs2, m_fs2).compile())
    {
      std::cerr << "failed compiling OpenGL program. Reason was '"
                << m_prog_cube.getError() << "'" << std::endl;
      return false;
    }

  // Create a cube.
  m_prog_cube.bind(m_cube);
  m_cube.vector3f("aPos") =
    {
      Vector3f(-0.5f, -0.5f, -0.5f),
      Vector3f(0.5f, -0.5f, -0.5f),
      Vector3f(0.5f,  0.5f, -0.5f),
      Vector3f(0.5f,  0.5f, -0.5f),
      Vector3f(-0.5f,  0.5f, -0.5f),
      Vector3f(-0.5f, -0.5f, -0.5f),

      Vector3f(-0.5f, -0.5f,  0.5f),
      Vector3f(0.5f, -0.5f,  0.5f),
      Vector3f(0.5f,  0.5f,  0.5f),
      Vector3f(0.5f,  0.5f,  0.5f),
      Vector3f(-0.5f,  0.5f,  0.5f),
      Vector3f(-0.5f, -0.5f,  0.5f),

      Vector3f(-0.5f,  0.5f,  0.5f),
      Vector3f(-0.5f,  0.5f, -0.5f),
      Vector3f(-0.5f, -0.5f, -0.5f),
      Vector3f(-0.5f, -0.5f, -0.5f),
      Vector3f(-0.5f, -0.5f,  0.5f),
      Vector3f(-0.5f,  0.5f,  0.5f),

      Vector3f(0.5f,  0.5f,  0.5f),
      Vector3f(0.5f,  0.5f, -0.5f),
      Vector3f(0.5f, -0.5f, -0.5f),
      Vector3f(0.5f, -0.5f, -0.5f),
      Vector3f(0.5f, -0.5f,  0.5f),
      Vector3f(0.5f,  0.5f,  0.5f),

      Vector3f(-0.5f, -0.5f, -0.5f),
      Vector3f(0.5f, -0.5f, -0.5f),
      Vector3f(0.5f, -0.5f,  0.5f),
      Vector3f(0.5f, -0.5f,  0.5f),
      Vector3f(-0.5f, -0.5f,  0.5f),
      Vector3f(-0.5f, -0.5f, -0.5f),

      Vector3f(-0.5f,  0.5f, -0.5f),
      Vector3f(0.5f,  0.5f, -0.5f),
      Vector3f(0.5f,  0.5f,  0.5f),
      Vector3f(0.5f,  0.5f,  0.5f),
      Vector3f(-0.5f,  0.5f,  0.5f),
      Vector3f(-0.5f,  0.5f, -0.5f)
    };
  m_cube.vector3f("aNormal") =
    {
      #include "../geometry/cube_normals.txt"
    };

  float ratio = width<float>() / height<float>();
  m_prog_cube.matrix44f("projection") =
    matrix::perspective(maths::toRadian(50.0f), ratio, 0.1f, 100.0f);

  Transformable<float, 3U> transformable;
  m_prog_cube.matrix44f("model") = transformable.transform();
  m_prog_cube.matrix44f("view") = m_cameraController.camera().viewMatrix();
  m_prog_cube.vector3f("viewPos") = m_cameraController.camera().position();

  // Material properties
  // Note: specular lighting doesn't have full effect on this object's material
  m_prog_cube.vector3f("material.ambient") = Vector3f(1.0f, 0.5f, 0.31f);
  m_prog_cube.vector3f("material.diffuse") = Vector3f(1.0f, 0.5f, 0.31f);
  m_prog_cube.vector3f("material.specular") = Vector3f(0.5f, 0.5f, 0.5f);
  m_prog_cube.scalarf("material.shininess") = 32.0f;

  return true;
}

//------------------------------------------------------------------
//! \brief Change the light settings.
//------------------------------------------------------------------
void GLExample13::changeLightProperties(float const time)
{
  Vector3f lightColor;
  lightColor.x = std::sin(time * 2.0f);
  lightColor.y = std::sin(time * 0.7f);
  lightColor.z = std::sin(time * 1.3f);

  Vector3f diffuseColor = vector::componentProduct(lightColor, Vector3f(0.5f)); // decrease the influence
  Vector3f ambientColor = vector::componentProduct(diffuseColor, Vector3f(0.2f)); // low influence
  m_prog_cube.vector3f("light.ambient") = ambientColor;
  m_prog_cube.vector3f("light.diffuse") = diffuseColor;
  m_prog_cube.vector3f("light.specular") = Vector3f(1.0f, 1.0f, 1.0f);
  m_prog_cube.vector3f("light.position") = lightPos;
}

//------------------------------------------------------------------
//! \brief Initialize shader and create a quad.
//------------------------------------------------------------------
bool GLExample13::setup()
{
  glCheck(glEnable(GL_DEPTH_TEST));
  glCheck(glDepthFunc(GL_LESS));

  hideMouseCursor();

  // Place a camera controlled by the user
  PerspectiveCamera3D& camera = m_cameraController.camera();
  camera.lookAt(Vector3f(10.0f), Vector3f::ZERO, Vector3f::UNIT_Y);

  if (!createLamp()) return false;
  if (!createCube()) return false;

  return true;
}

//------------------------------------------------------------------
//! \brief Paint our scene. Here we are using the delta time to
//------------------------------------------------------------------
bool GLExample13::draw()
{
  static float time = 0.0f;
  time += dt();

  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  PerspectiveCamera3D& camera = m_cameraController.camera();
  m_prog_lamp.matrix44f("view") = camera.viewMatrix();
  m_prog_cube.matrix44f("view") = camera.viewMatrix();
  m_prog_cube.vector3f("viewPos") = camera.position();

  changeLightProperties(time);

  m_prog_cube.draw(m_cube, Mode::TRIANGLES);
  m_prog_lamp.draw(m_lamp, Mode::TRIANGLES);

  // Key pressed
  if (keyPressed(GLFW_KEY_W) || keyPressed(GLFW_KEY_UP))
    m_cameraController.processKeyboard(CameraController::Movement::FORWARD, dt());
  if (keyPressed(GLFW_KEY_S) || keyPressed(GLFW_KEY_DOWN))
    m_cameraController.processKeyboard(CameraController::Movement::BACKWARD, dt());
  if (keyPressed(GLFW_KEY_A) || keyPressed(GLFW_KEY_LEFT))
    m_cameraController.processKeyboard(CameraController::Movement::LEFT, dt());
  if (keyPressed(GLFW_KEY_D) || keyPressed(GLFW_KEY_RIGHT))
    m_cameraController.processKeyboard(CameraController::Movement::RIGHT, dt());

  return true;
}
