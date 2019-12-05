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
// General Public License for more detailm_shape.
//
// You should have received a copy of the GNU General Public License
// along with OpenGLCppWrapper.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#include "16_MaterialDepth.hpp"

//------------------------------------------------------------------
//! \file
//------------------------------------------------------------------

//------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------
void GLExample16::onWindowSizeChanged()
{
  // Note: height is never zero !
  float ratio = width<float>() / height<float>();

  // Make sure the viewport matches the new window dimensionm_shape.
  glCheck(glViewport(0, 0, width<int>(), height<int>()));

  m_shape->projection() =
    m_cameraController.camera().updateProjectionMatrix();
}

// --------------------------------------------------------------
//! \brief Mouse event
// --------------------------------------------------------------
void GLExample16::onMouseMoved(Mouse const& mouse)
{
  float const dx = static_cast<float>(mouse.displacement.x);
  float const dy = static_cast<float>(mouse.displacement.y);
  m_cameraController.processMouseMovement(dx, dy, dt());
}

//------------------------------------------------------------------
//! \brief Init your scene.
//------------------------------------------------------------------
bool GLExample16::setup()
{
  glCheck(glEnable(GL_DEPTH_TEST));
  glCheck(glDepthFunc(GL_LESS));
  glCheck(glDisable(GL_BLEND));
  glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
  //glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));

  hideMouseCursor();

  // Place a camera controlled by the user
  PerspectiveCamera3D& camera = m_cameraController.camera();
  camera.lookAt(Vector3f(0.3f, 0.3f, 3.0f), Vector3f(0.0f, 0.0f, -1.0f));

  m_material->near() = 0.1f;
  m_material->far() = 2.0f;
ERROR("MMM %f", m_material->far());

  // Create shapes
  m_tube1 = Tube::create("Tube1", m_material, 1.0f, m_base_radius, 1.0f, 128u);
  m_tube2 = Tube::create("Tube2", m_material, 1.0f, -m_base_radius, 1.0f, 128u);
  m_cone1 = Cone::create("Cone1", m_material, m_base_radius, 1.0f, 128u);
  m_cone2 = Cone::create("Cone2", m_material, -m_base_radius, 1.0f, 128u);
  m_pyra1 = Pyramid::create("Pyramid1", m_material, m_base_radius, 1.0f);
  m_pyra2 = Pyramid::create("Pyramid2", m_material, -m_base_radius, 1.0f);

  pimpShape(m_tube1);
  pimpShape(m_tube2);
  pimpShape(m_cone1);
  pimpShape(m_cone2);
  pimpShape(m_pyra1);
  pimpShape(m_pyra2);

  // Default shape
  m_shape = m_tube1;

  // Success
  return true;
}

//------------------------------------------------------------------
//! \brief
//------------------------------------------------------------------
bool GLExample16::pimpShape(Shape3D_SP shape)
{
  if (!shape->texture().load("../textures/wooden-crate.jpg"))
    {
      std::cerr << "Failed loading texture" << std::endl;
      return false;
    }

  shape->texture().wrap(TextureWrap::CLAMP_TO_EDGE)
    .interpolation(TextureMinFilter::LINEAR, TextureMagFilter::LINEAR);

  return true;
}

//------------------------------------------------------------------
//! \brief Paint our scene. Here we are using the delta time to
//! change the color of the window.
//------------------------------------------------------------------
bool GLExample16::draw()
{
  static float time = 0.0f;
  time += dt();
  const float ct = cosf(time) * 0.5f + 0.5f;
  const float st = sinf(time) * 0.5f + 0.5f;

  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  if (keyPressed(GLFW_KEY_UP))
    m_cameraController.processKeyboard(CameraController::Movement::FORWARD, dt());
  if (keyPressed(GLFW_KEY_DOWN))
    m_cameraController.processKeyboard(CameraController::Movement::BACKWARD, dt());
  if (keyPressed(GLFW_KEY_LEFT))
    m_cameraController.processKeyboard(CameraController::Movement::LEFT, dt());
  if (keyPressed(GLFW_KEY_RIGHT))
    m_cameraController.processKeyboard(CameraController::Movement::RIGHT, dt());

  if (keyPressed(GLFW_KEY_M))
    m_shape = m_tube1;
  if (keyPressed(GLFW_KEY_L))
    m_shape = m_tube2;
  if (keyPressed(GLFW_KEY_K))
    m_shape = m_cone1;
  if (keyPressed(GLFW_KEY_J))
    m_shape = m_cone2;
  if (keyPressed(GLFW_KEY_H))
    m_shape = m_pyra1;
  if (keyPressed(GLFW_KEY_G))
    m_shape = m_pyra2;

  if (keyPressed(GLFW_KEY_A))
    m_slices = std::min(128u, m_slices + 1u);
  if (keyPressed(GLFW_KEY_Z))
    m_slices = std::max(4u, m_slices - 1u);
  if (keyPressed(GLFW_KEY_Q))
    m_base_radius = std::min(4.0f, m_base_radius + 0.1f);
  if (keyPressed(GLFW_KEY_S))
    m_base_radius = std::max(1.0f, m_base_radius - 0.1f);

  // FIXME no this shall be main_prog->view() =
  m_shape->view() = m_cameraController.camera().viewMatrix();

  //m_shape->reset();
  //m_shape->draw();
  //m_shape->rotateY(4.0f * ct * dt()).translate(Vector3f(2.0f, 0.0f, 0.0f));
  m_shape->draw();

  //std::cout << m_shape->name() << std::endl;

  // Success
  return true;
}
