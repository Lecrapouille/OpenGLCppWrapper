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

#include "07_SkyBoxTextureCube.hpp"
#include "Maths.hpp"

//------------------------------------------------------------------
//! \file this example paints a cube inside a skybox. A skybox is
//! cubic texture simuling a infinite landscape
//!
//! \note This example takes its inspiration from
//! https://learnopengl.com/Advanced-OpenGL/Cubemaps
//------------------------------------------------------------------

//------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------
void GLExample07::onWindowSizeChanged(const float width, const float height)
{
  // Note: height is never zero !
  float ratio = width / height;

  // Make sure the viewport matches the new window dimensions.
  glCheck(glViewport(0, 0, static_cast<int>(width), static_cast<int>(height)));

  m_progCube.matrix44f("projection") =
    matrix::perspective(maths::radians(50.0f), ratio, 0.1f, 10.0f);
  m_progSkyBox.matrix44f("projection") = m_progCube.matrix44f("projection");
}

//------------------------------------------------------------------
//! \brief Create a skybox.
//------------------------------------------------------------------
bool GLExample07::createSkyBox()
{
  // Load from ASCII file the vertex sahder (vs) as well the fragment shader
  vs1.fromFile("shaders/07_SkyBoxTextureCube_skybox.vs");
  fs1.fromFile("shaders/07_SkyBoxTextureCube_skybox.fs");

  // Compile shader as OpenGL program. This one will instanciate all OpenGL objects for you.
  if (!m_progSkyBox.attachShaders(vs1, fs1).compile())
    {
      std::cerr << "failed compiling OpenGL program. Reason was '"
                << m_progSkyBox.getError() << "'" << std::endl;
      return false;
    }

  // Init uniforms.
  float ratio = static_cast<float>(width()) / (static_cast<float>(height()) + 0.1f);
  m_progSkyBox.matrix44f("projection") =
    matrix::perspective(maths::radians(50.0f), ratio, 0.1f, 10.0f);

  // Binding empty VAO to OpenGL program will make it be populated
  // with all VBOs needed.
  m_progSkyBox.bind(m_skybox);

  // Now we have to fill VBOs with data: here vertices. Because in
  // vertex shader a_position is vect3 we have to cast to Vector3f.
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

//------------------------------------------------------------------
//! \brief Create a cube.
//------------------------------------------------------------------
bool GLExample07::createCube()
{
  // Load from ASCII file the vertex sahder (vs) as well the fragment shader
  vs2.fromFile("shaders/07_SkyBoxTextureCube_cubemap.vs");
  fs2.fromFile("shaders/07_SkyBoxTextureCube_cubemap.fs");

  // Compile shader as OpenGL program. This one will instanciate all OpenGL objects for you.
  if (!m_progCube.attachShaders(vs2, fs2).compile())
    {
      std::cerr << "failed compiling OpenGL program. Reason was '"
                << m_progCube.getError() << "'" << std::endl;
      return false;
    }

  // Init uniforms.
  float ratio = static_cast<float>(width()) / (static_cast<float>(height()) + 0.1f);
  m_progCube.matrix44f("projection") =
    matrix::perspective(maths::radians(50.0f), ratio, 0.1f, 10.0f);

  // Binding empty VAO to OpenGL program will make it be populated
  // with all VBOs needed.
  m_progCube.bind(m_cube);

  // Now we have to fill VBOs with data: here vertices. Because in
  // vertex shader a_position is vect3 we have to cast to Vector3f.
  m_cube.vector3f("aPos") =
    {
       #include "geometry/cube_position.txt"
    };
  m_cube.vector3f("aPos") /= 2.0f;

  // Now we have to fill VBOs with data: here texture coordinates.
  // Because in vertex shader a_texcoord is vect2 we have to cast
  // to Vector2f.
  m_cube.vector2f("aTexCoords") =
    {
       #include "geometry/cube_texture.txt"
    };

  // Add a texture to the cube
  if (!m_cube.texture2D("texture1").load("textures/path.png")) return false;

  return true;
}

//------------------------------------------------------------------
//! \brief Init your scene.
//------------------------------------------------------------------
bool GLExample07::setup()
{
  // Enable some OpenGL states
  glCheck(glEnable(GL_DEPTH_TEST));
  glCheck(glDepthFunc(GL_LESS));
  glCheck(glDisable(GL_BLEND));
  glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  hideMouseCursor();

  return createCube() && createSkyBox();
}

// --------------------------------------------------------------
//! \brief Draw the cube.
// --------------------------------------------------------------
void GLExample07::drawCube()
{
  m_progCube.matrix44f("model") = Matrix44f(matrix::Identity);
  m_progCube.matrix44f("view") = m_camera.GetViewMatrix();

  glCheck(glDepthFunc(GL_LESS));
  m_progCube.draw(m_cube, Mode::TRIANGLES, m_indices);
}

// --------------------------------------------------------------
//! \brief Draw skybox. Shall be draw as last.
// --------------------------------------------------------------
void GLExample07::drawSkyBox()
{
  // Remove translation from the view matrix
  Matrix44f view = m_camera.GetViewMatrix();
  m_progSkyBox.matrix44f("view") = Matrix44f(Matrix33f(view));

  // Note: change depth function so depth test passes when values are
  // equal to depth buffer's content
  glCheck(glDepthFunc(GL_LEQUAL));
  m_progSkyBox.draw(m_skybox, Mode::TRIANGLES, 0, 36);
}

//------------------------------------------------------------------
//! \brief Paint our scene.
//------------------------------------------------------------------
bool GLExample07::draw()
{
  // Clear OpenGL color and depth buffers.
  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  drawCube();
  drawSkyBox();

  // Delta time
  const float deltaTime = dt();

  // Key pressed
  if (keyPressed(GLFW_KEY_W))
    m_camera.ProcessKeyboard(FORWARD, deltaTime);
  if (keyPressed(GLFW_KEY_S))
    m_camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (keyPressed(GLFW_KEY_A))
    m_camera.ProcessKeyboard(LEFT, deltaTime);
  if (keyPressed(GLFW_KEY_D))
    m_camera.ProcessKeyboard(RIGHT, deltaTime);

  return true;
}

// --------------------------------------------------------------
//! \brief Mouse event
// --------------------------------------------------------------
void GLExample07::onMouseMoved(const double xpos, const double ypos)
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
