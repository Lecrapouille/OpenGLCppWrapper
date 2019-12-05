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

//------------------------------------------------------------------
//! \file this example paints a shape inside a skybox. A skybox is
//! cubic texture simuling a infinite landscape
//!
//! \note This example takes its inspiration from
//! https://learnopengl.com/Advanced-OpenGL/Cubemaps
//------------------------------------------------------------------

//------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------
void GLExample07::onWindowSizeChanged()
{
  // Make sure the viewport matches the new window dimensions.
  glCheck(glViewport(0, 0, width<int>(), height<int>()));

  Matrix44f const& proj =
    m_cameraController.camera().updateProjectionMatrix();
  m_shape.projection() = proj;
  m_progSkyBox.matrix44f("projection") = proj;
}

// --------------------------------------------------------------
//! \brief Mouse event
// --------------------------------------------------------------
void GLExample07::onMouseMoved(Mouse const& mouse)
{
  float const dx = static_cast<float>(mouse.displacement.x);
  float const dy = static_cast<float>(mouse.displacement.y);
  m_cameraController.processMouseMovement(dx, dy, dt());
}

// --------------------------------------------------------------
//! \brief Mouse event
// --------------------------------------------------------------
void GLExample07::onMouseScrolled(Mouse const& mouse)
{
  float const z = static_cast<float>(mouse.scroll.y);
  Matrix44f const& proj = m_cameraController.zoom(z);
  m_shape.projection() = proj;
  m_progSkyBox.matrix44f("projection") = proj;
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
      std::cerr << "Failed compiling OpenGL program. Reason was '"
                << m_progSkyBox.getError() << "'" << std::endl;
      return false;
    }

  // Binding empty VAO to OpenGL program will make it be populated
  // with all VBOs needed.
  m_progSkyBox.bind(m_skybox);

  // Now we have to fill VBOs with data: here vertices. Because in
  // vertex shader a_position is vect3 we have to cast to Vector3f.
  m_skybox.vector3f("aPos") =
    {
       #include "../geometry/cube_position.txt"
    };

  // Add 6 textures to the sky box
  if (!m_skybox.textureCube("skybox").load(CubeMap::POSITIVE_X, "../textures/right.jpg")) return false;
  if (!m_skybox.textureCube("skybox").load(CubeMap::NEGATIVE_X, "../textures/left.jpg")) return false;
  if (!m_skybox.textureCube("skybox").load(CubeMap::POSITIVE_Y, "../textures/top.jpg")) return false;
  if (!m_skybox.textureCube("skybox").load(CubeMap::NEGATIVE_Y, "../textures/bottom.jpg")) return false;
  if (!m_skybox.textureCube("skybox").load(CubeMap::POSITIVE_Z, "../textures/front.jpg")) return false;
  if (!m_skybox.textureCube("skybox").load(CubeMap::NEGATIVE_Z, "../textures/back.jpg")) return false;

  return true;
}

//------------------------------------------------------------------
//! \brief Create a 3D shape (Cone, Pyramid, Cylinder, Tube). See
//! these class like factories for instanciating VAO.
//------------------------------------------------------------------
bool GLExample07::createShape()
{
  // Add a texture to the cube
  if (!m_shape.texture().load("../textures/path.png"))
    return false;

  uint32_t const slices = 32u;
  float const radius = 1.0f;
  float const height = 1.0f;

  // FIXME Pure Geometry classes
  // Create a cylinder with two caps (bottom & top). Merge them into a
  // single VAO.
  MaterialBasic_SP material = MaterialBasic::create();
  Cylinder cylinder("", material, radius, height, slices);
  Circle circle1("", material, radius, slices); circle1.vertices() += Vector3f(0.0f, 0.0f, 0.5f);
  Circle circle2("", material, radius, slices); circle2.vertices() -= Vector3f(0.0f, 0.0f, 0.5f);

  m_shape.vertices()
    .append(circle1.vertices())
    .append(circle2.vertices())
    .append(cylinder.vertices());

  m_shape.uv()
    .append(circle1.uv())
    .append(circle2.uv())
    .append(cylinder.uv());

  m_shape.uv() *= 2.0f; // Repeat the texture motif

  m_shape.index()
    .appendIndex(circle1.index())   // Starting index: 0
    .appendIndex(circle2.index())   // Starting index: 34
    .appendIndex(cylinder.index()); // Starting index: 68

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

  // Place a camera controlled by the user
  PerspectiveCamera3D& camera = m_cameraController.camera();
  camera.lookAt(Vector3f(-5.0f, 1.0f, 4.0f),
                Vector3f(-4.5f, 1.0f, 3.5f));

  return createShape() && createSkyBox();
}

// --------------------------------------------------------------
//! \brief Draw the shape.
// --------------------------------------------------------------
void GLExample07::drawShape()
{
  m_shape.model() = Matrix44f(matrix::Identity);
  m_shape.view() = m_cameraController.camera().viewMatrix();

  // Set depth function back to default
  glCheck(glDepthFunc(GL_LESS));
  m_shape.draw();
}

// --------------------------------------------------------------
//! \brief Draw skybox. Should be draw as last.
// --------------------------------------------------------------
void GLExample07::drawSkyBox()
{
  // Remove translation from the view matrix
  Matrix44f view = m_cameraController.camera().viewMatrix();
  m_progSkyBox.matrix44f("view") = Matrix44f(Matrix33f(view));

  // Change depth function so depth test passes when values are equal
  // to depth buffer's content
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

  // Draw scene as normal.
  drawShape();
  // Draw skybox as last. Set depth function back to default
  drawSkyBox();

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
