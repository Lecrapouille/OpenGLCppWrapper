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

#include "00_BasicMaterial.hpp"
#include <iostream>

ShapeBasicMaterial::ShapeBasicMaterial()
  : m_cameraController(Camera3D::Type::PERSPECTIVE)
{}

ShapeBasicMaterial::~ShapeBasicMaterial()
{}

//------------------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------------------
void ShapeBasicMaterial::onWindowSizeChanged()
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
void ShapeBasicMaterial::onMouseMoved(Mouse const& mouse)
{
  float const dx = static_cast<float>(mouse.displacement.x);
  float const dy = static_cast<float>(mouse.displacement.y);
  m_cameraController.processMouseMovement(dx, dy, dt());
}

// --------------------------------------------------------------
//! \brief Keyboard event
// --------------------------------------------------------------
void ShapeBasicMaterial::onKeyboardEvent()
{
  if (isKeyDown(GLFW_KEY_UP))
    m_cameraController.processKeyboard(CameraController::Movement::FORWARD, dt());
  if (isKeyDown(GLFW_KEY_DOWN))
    m_cameraController.processKeyboard(CameraController::Movement::BACKWARD, dt());
  if (isKeyDown(GLFW_KEY_LEFT))
    m_cameraController.processKeyboard(CameraController::Movement::LEFT, dt());
  if (isKeyDown(GLFW_KEY_RIGHT))
    m_cameraController.processKeyboard(CameraController::Movement::RIGHT, dt());

  // Change the shape
  if (isKeyDown(GLFW_KEY_M))
    m_shape = m_tube1;
  if (isKeyDown(GLFW_KEY_L))
    m_shape = m_tube2;
  if (isKeyDown(GLFW_KEY_K))
    m_shape = m_cone1;
  if (isKeyDown(GLFW_KEY_J))
    m_shape = m_cone2;
  if (isKeyDown(GLFW_KEY_H))
    m_shape = m_pyra1;
  if (isKeyDown(GLFW_KEY_G))
    m_shape = m_pyra2;

  // Increase/decrease the number of vertices
  if (isKeyDown(GLFW_KEY_A))
    m_slices = std::min(128u, m_slices + 1u);
  if (isKeyDown(GLFW_KEY_Z))
    m_slices = std::max(4u, m_slices - 1u);
  if (isKeyDown(GLFW_KEY_Q))
    m_base_radius = std::min(4.0f, m_base_radius + 0.1f);
  if (isKeyDown(GLFW_KEY_S))
    m_base_radius = std::max(1.0f, m_base_radius - 0.1f);
}

//------------------------------------------------------------------------------
//! \brief Init your scene.
//------------------------------------------------------------------------------
bool ShapeBasicMaterial::setup()
{
  glCheck(glEnable(GL_DEPTH_TEST));
  glCheck(glDepthFunc(GL_LESS));
  glCheck(glDisable(GL_BLEND));
  glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
  //glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));

  // Enable IO callbacks
  enableCallbacks(window::Event::MouseMove | window::Event::Keyboard);
  hideMouseCursor();

  // Place a camera controlled by the user
  PerspectiveCamera3D& camera = m_cameraController.camera();
  camera.lookAt(// Position
                Vector3f(2.437f, 0.26761f, 1.49998f),
                // looking at
                Vector3f(-0.826219f, -0.0917643f, -0.555825f));

  // Create a material
  BasicMaterialConfig config;
  config.useMap = true;
  config.useColor = true;
  m_material = BasicMaterial::create(config);

  // Create different shapes
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

//------------------------------------------------------------------------------
//! \brief
//------------------------------------------------------------------------------
bool ShapeBasicMaterial::pimpShape(Shape3D_SP shape)
{
  if (!shape->texture().load("textures/path.png"))
    {
      std::cerr << "Failed loading texture" << std::endl;
      return false;
    }

  shape->texture().wrap(TextureWrap::CLAMP_TO_EDGE)
    .interpolation(TextureMinFilter::LINEAR, TextureMagFilter::LINEAR);

  return true;
}

//------------------------------------------------------------------------------
//! \brief Paint our scene. Here we are using the delta time to
//! change the color of the window.
//------------------------------------------------------------------------------
bool ShapeBasicMaterial::draw()
{
  static float time = 0.0f;
  time += dt();
  const float ct = cosf(time) * 0.5f + 0.5f;
  const float st = sinf(time) * 0.5f + 0.5f;

  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  m_material->opacity() = 1.0f;
  m_material->diffuse() = Vector3f(st, ct, 0.0f);
  m_material->color() = m_material->diffuse();

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
