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

#include "10_SkyBoxShapeCamera.hpp"
#include <iostream>

SkyBoxShapeCamera::SkyBoxShapeCamera()
  : m_shape("Shape"),
    m_skybox("SkyBox"),
    m_progShape("progShape"),
    m_progSkyBox("progSkyBox"),
    m_cameraController(Camera3D::Type::PERSPECTIVE)
{}

SkyBoxShapeCamera::~SkyBoxShapeCamera()
{}

//------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------
void SkyBoxShapeCamera::onWindowSizeChanged()
{
  // Make sure the viewport matches the new window dimensions.
  glCheck(glViewport(0, 0, width<int>(), height<int>()));

  Matrix44f const& proj =
    m_cameraController.camera().updateProjectionMatrix();
  m_progShape.matrix44f("projection") = proj;
  m_progSkyBox.matrix44f("projection") = proj;
}

// --------------------------------------------------------------
//! \brief Mouse event
// --------------------------------------------------------------
void SkyBoxShapeCamera::onMouseMoved(Mouse const& mouse)
{
  float const dx = static_cast<float>(mouse.displacement.x);
  float const dy = static_cast<float>(mouse.displacement.y);
  m_cameraController.processMouseMovement(dx, dy, dt());
}

// --------------------------------------------------------------
//! \brief Mouse event
// --------------------------------------------------------------
void SkyBoxShapeCamera::onMouseScrolled(Mouse const& mouse)
{
  float const z = static_cast<float>(mouse.scroll.y);
  Matrix44f const& proj = m_cameraController.zoom(z);
  m_progShape.matrix44f("projection") = proj;
  m_progSkyBox.matrix44f("projection") = proj;
}

// --------------------------------------------------------------
//! \brief Keyboard event
// --------------------------------------------------------------
void SkyBoxShapeCamera::onKeyboardEvent()
{
  if (isKeyDown(GLFW_KEY_W) || isKeyDown(GLFW_KEY_UP))
    m_cameraController.processKeyboard(CameraController::Movement::FORWARD, dt());
  if (isKeyDown(GLFW_KEY_S) || isKeyDown(GLFW_KEY_DOWN))
    m_cameraController.processKeyboard(CameraController::Movement::BACKWARD, dt());
  if (isKeyDown(GLFW_KEY_A) || isKeyDown(GLFW_KEY_LEFT))
    m_cameraController.processKeyboard(CameraController::Movement::LEFT, dt());
  if (isKeyDown(GLFW_KEY_D) || isKeyDown(GLFW_KEY_RIGHT))
    m_cameraController.processKeyboard(CameraController::Movement::RIGHT, dt());
}

//------------------------------------------------------------------
//! \brief Create a skybox.
//------------------------------------------------------------------
bool SkyBoxShapeCamera::createSkyBox()
{
  // Load from ASCII file the vertex sahder (vs) as well the fragment shader
  vs1.fromFile("01_Core/shaders/09_SkyBoxTextureCube.vs");
  fs1.fromFile("01_Core/shaders/09_SkyBoxTextureCube.fs");

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
//! \brief Create a 3D shape (Cone, Pyramid, Cylinder, Tube). See
//! these class like factories for instanciating VAO.
//------------------------------------------------------------------
bool SkyBoxShapeCamera::createShape()
{
  // Load from ASCII file the vertex sahder (vs) as well the fragment shader
  vs2.fromFile("01_Core/shaders/06_MultipleObjects.vs");
  fs2.fromFile("01_Core/shaders/06_MultipleObjects.fs");

  // Compile shader as OpenGL program. This one will instanciate all OpenGL objects for you.
  if (!m_progShape.attachShaders(vs2, fs2).compile())
    {
      std::cerr << "Failed compiling OpenGL program. Reason was '"
                << m_progShape.getError() << "'" << std::endl;
      return false;
    }

  m_progShape.scalarf("scale") = 1.0f;
  m_progShape.vector4f("color") = Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
  m_progShape.bind(m_shape);

  // Fill VBOs of the VAO: init triangle vertex positions.
  m_shape.vector3f("position") =
    {
      #include "geometry/cube_position.txt"
    };
  m_shape.vector3f("position") *= 0.5f;

  // Fill VBOs of the VAO: init triangle texture positions.
  m_shape.vector2f("UV") =
    {
      #include "geometry/cube_texture.txt"
    };

  GLTexture2D& texture = m_shape.texture2D("texID");
  texture.interpolation(TextureMinFilter::LINEAR, TextureMagFilter::LINEAR);
  texture.wrap(TextureWrap::MIRRORED_REPEAT);
  return texture.load("textures/wooden-crate.jpg");
}

//------------------------------------------------------------------
//! \brief Init your scene.
//------------------------------------------------------------------
bool SkyBoxShapeCamera::setup()
{
  // Enable some OpenGL states
  glCheck(glEnable(GL_DEPTH_TEST));
  glCheck(glDepthFunc(GL_LESS));
  glCheck(glDisable(GL_BLEND));
  glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  // Enable IO callbacks
  enableCallbacks(window::Event::MouseMove | window::Event::MouseScroll | window::Event::Keyboard);
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
void SkyBoxShapeCamera::drawShape()
{
  static float time = 0.0f;
  time += dt();
  float ct = cosf(time);

  // Apply a rotation to the box around the Y-axis
  m_transformable.reset(); // restore to identity matrix
  m_transformable.rotateY(4.0f * ct);
  m_progShape.scalarf("scale") = ct + 0.5f;
  m_progShape.matrix44f("model") = m_transformable.transform();
  m_progShape.matrix44f("view") = m_cameraController.camera().viewMatrix();

  // Set depth function back to default
  glCheck(glDepthFunc(GL_LESS));
  m_progShape.draw(m_shape, Mode::TRIANGLES, 0, 36);
}

// --------------------------------------------------------------
//! \brief Draw skybox. Should be draw as last.
// --------------------------------------------------------------
void SkyBoxShapeCamera::drawSkyBox()
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
bool SkyBoxShapeCamera::draw()
{
  // Clear OpenGL color and depth buffers.
  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  // Draw scene as normal.
  drawShape();
  // Draw skybox as last. Set depth function back to default
  drawSkyBox();

  return true;
}
