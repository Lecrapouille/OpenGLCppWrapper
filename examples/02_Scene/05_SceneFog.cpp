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

#include "05_SceneFog.hpp"
#include <iostream>

SceneFog::SceneFog()
{}

SceneFog::~SceneFog()
{}

//------------------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------------------
void SceneFog::onWindowSizeChanged()
{
  // Note: height is never zero !
  float ratio = width<float>() / height<float>();

  // Make sure the viewport matches the new window dimensionm_shape.
  glCheck(glViewport(0, 0, width<int>(), height<int>()));

  auto& camera = m_cameraController->camera();
  m_cube->view() = camera.updateViewMatrix();
  m_cube->projection() = camera.updateProjectionMatrix();
}

// --------------------------------------------------------------
//! \brief Keyboard event
// --------------------------------------------------------------
void SceneFog::onKeyboardEvent()
{
  if (isKeyDown(GLFW_KEY_W) || isKeyDown(GLFW_KEY_UP))
    m_cameraController->processKeyboard(CameraController::Movement::FORWARD, dt());
  if (isKeyDown(GLFW_KEY_S) || isKeyDown(GLFW_KEY_DOWN))
    m_cameraController->processKeyboard(CameraController::Movement::BACKWARD, dt());
  if (isKeyDown(GLFW_KEY_A) || isKeyDown(GLFW_KEY_LEFT))
    m_cameraController->processKeyboard(CameraController::Movement::LEFT, dt());
  if (isKeyDown(GLFW_KEY_D) || isKeyDown(GLFW_KEY_RIGHT))
    m_cameraController->processKeyboard(CameraController::Movement::RIGHT, dt());

  auto& camera = m_cameraController->camera();
  m_cube->view() = camera.updateViewMatrix();
}

//------------------------------------------------------------------------------
//! \brief Init your scene.
//------------------------------------------------------------------------------
bool SceneFog::setup()
{
  glCheck(glEnable(GL_DEPTH_TEST));
  glCheck(glDepthFunc(GL_LESS));
  glCheck(glDisable(GL_BLEND));
  glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
  //glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));

  // Create and place a camera
  m_cameraController = CameraController::create(Camera3D::Type::PERSPECTIVE);
  m_cameraController->camera().lookAt(Vector3f(10.0f), Vector3f::ZERO, Vector3f::UNIT_Y);
  m_cameraController->setMoveVelocity(10.0f);

  // Define material with fog
  BasicMaterialConfig config;
  config.useMap = true;
  config.useColor = false;
  config.useFog = true;

  m_material = BasicMaterial::create(config, "basic2");
  m_material->color() = Vector3f(1.0f, 1.0f, 1.0f);
  m_material->diffuse() = Vector3f(1.0f, 1.0f, 1.0f);

  // Fog paramaters
  m_material->fogNear() = 1.0f;
  m_material->fogFar() = 20.0f;
  m_material->fogColor() = Vector3f(0.5f, 0.5f, 0.5f);

  // Create leaves of the scene graph
  m_cube = Cube::create("Cube1", m_material); m_cube->texture().load("textures/wooden-crate.jpg"); // FIXME A supprimer + material->load(...)

  enableCallbacks(window::Event::Keyboard);

  // Success
  return true;
}

//------------------------------------------------------------------------------
//! \brief Paint our scene. Here we are using the delta time to
//! change the color of the window.
//------------------------------------------------------------------------------
bool SceneFog::draw()
{
  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  // Update internal transformation matrices
  m_cube->update(dt());

  // Draw the scene
  m_cube->render();

  // Success
  return true;
}
