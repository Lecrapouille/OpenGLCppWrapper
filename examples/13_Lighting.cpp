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
#include "Maths.hpp"

// lighting
static Vector3f lightPos(1.2f, 1.0f, 2.0f);

void GLExample13::onWindowSizeChanged(const float width, const float height)
{
  // Note: height is never zero !
  float ratio = width / height;

  // Make sure the viewport matches the new window dimensions.
  glCheck(glViewport(0, 0, static_cast<int>(width), static_cast<int>(height)));

  m_prog_lamp.matrix44f("projection") =
    matrix::perspective(maths::toRadian(50.0f), ratio, 0.1f, 100.0f);
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
      std::cerr << "failed compiling OpenGL program. Reason was '"
                << m_prog_lamp.getError() << "'" << std::endl;
      return false;
    }

  // Create a cube.
  m_prog_lamp.bind(m_lamp);
  m_lamp.vector3f("aPos") =
    {
      #include "geometry/cube_position.txt"
    };

  float ratio = static_cast<float>(width()) / static_cast<float>(height());
  m_prog_lamp.matrix44f("projection") =
    matrix::perspective(maths::toRadian(50.0f), ratio, 0.1f, 100.0f);

  Transformable<float, 3U> transformable;
  transformable.position(lightPos);
  transformable.scale(Vector3f(0.05f)); // a smaller cube
  m_prog_lamp.matrix44f("model") = transformable.transform();

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
      #include "geometry/cube_normals.txt"
    };

  float ratio = static_cast<float>(width()) / static_cast<float>(height());
  m_prog_cube.matrix44f("projection") =
    matrix::perspective(maths::toRadian(50.0f), ratio, 0.1f, 100.0f);

  Transformable<float, 3U> transformable;
  m_prog_cube.matrix44f("model") = transformable.transform();

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

  Vector3f diffuseColor = lightColor   * Vector3f(0.5f); // decrease the influence
  Vector3f ambientColor = diffuseColor * Vector3f(0.2f); // low influence
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

  m_prog_lamp.matrix44f("view") = m_camera.GetViewMatrix();
  m_prog_cube.matrix44f("view") = m_camera.GetViewMatrix();
  m_prog_cube.vector3f("viewPos") = m_camera.Position;
  changeLightProperties(time);

  m_prog_cube.draw(m_cube, Mode::TRIANGLES);
  m_prog_lamp.draw(m_lamp, Mode::TRIANGLES);

  // std::cout << "P: " << m_camera.Position << std::endl;
  // std::cout << "Y: " << m_camera.Yaw << std::endl;
  // std::cout << "p: " << m_camera.Pitch << std::endl;

  // Key pressed
  if (keyPressed(GLFW_KEY_W))
    m_camera.ProcessKeyboard(FORWARD, dt());
  if (keyPressed(GLFW_KEY_S))
    m_camera.ProcessKeyboard(BACKWARD, dt());
  if (keyPressed(GLFW_KEY_A))
    m_camera.ProcessKeyboard(LEFT, dt());
  if (keyPressed(GLFW_KEY_D))
    m_camera.ProcessKeyboard(RIGHT, dt());

  return true;
}

// --------------------------------------------------------------
//! \brief Mouse event
// --------------------------------------------------------------
void GLExample13::onMouseMoved(const double xpos, const double ypos)
{
  static double lastX = 0.0f;
  static double lastY = 0.0f;
  static bool firstMouse = true;

  if (firstMouse)
    {
      lastX = xpos;
      lastY = ypos;
      firstMouse = false;
    }

  double xoffset = xpos - lastX;
  double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

  lastX = xpos;
  lastY = ypos;
  m_camera.ProcessMouseMovement(static_cast<float>(xoffset),
                                static_cast<float>(yoffset));
}
