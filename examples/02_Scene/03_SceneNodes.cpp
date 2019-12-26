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

#include "03_SceneNodes.hpp"
#include <iostream>

SceneNodes::SceneNodes()
{}

SceneNodes::~SceneNodes()
{}

//------------------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------------------
void SceneNodes::onWindowSizeChanged()
{
  // Note: height is never zero !
  float ratio = width<float>() / height<float>();

  // Make sure the viewport matches the new window dimensionm_shape.
  glCheck(glViewport(0, 0, width<int>(), height<int>()));

  // TODO traverse(m_root, [](Node3D_SP node){ node->projection() = proj; });
  auto& camera = m_cameraController->camera();
  m_cube1->view() = camera.updateViewMatrix();
  m_cube1->projection() = camera.updateProjectionMatrix();
  m_cube2->view() = camera.updateViewMatrix();
  m_cube2->projection() = camera.updateProjectionMatrix();
  m_cube3->view() = camera.updateViewMatrix();
  m_cube3->projection() = camera.updateProjectionMatrix();
  m_cube4->view() = camera.updateViewMatrix();
  m_cube4->projection() = camera.updateProjectionMatrix();
}

// --------------------------------------------------------------
//! \brief Keyboard event
// --------------------------------------------------------------
void SceneNodes::onKeyboardEvent()
{
  if (wasKeyPressed(GLFW_KEY_UP) || wasKeyPressed(GLFW_KEY_RIGHT))
    {
      m_switch->next();
      std::cout << "Next child: " << m_switch->selected() << std::endl;
    }
  else if (wasKeyPressed(GLFW_KEY_DOWN) || wasKeyPressed(GLFW_KEY_LEFT))
    {
      m_switch->previous();
      std::cout << "Previous child: " << m_switch->selected() << std::endl;
    }
}

//------------------------------------------------------------------------------
//! \brief Init your scene.
//------------------------------------------------------------------------------
bool SceneNodes::setup()
{
  glCheck(glEnable(GL_DEPTH_TEST));
  glCheck(glDepthFunc(GL_LESS));
  glCheck(glDisable(GL_BLEND));
  glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
  //glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));

  // Create and place a camera
  m_cameraController = CameraController::create(Camera3D::Type::PERSPECTIVE);
  m_cameraController->camera().lookAt(Vector3f(10.0f), Vector3f::ZERO, Vector3f::UNIT_Y);

  BasicMaterialConfig config;

  // Define material 1
  config.useMap = false;
  config.useColor = true;
  m_material1 = BasicMaterial::create(config, "basic1");
  m_material1->color() = Vector3f(1.0f, 0.0f, 0.0f);
  m_material1->opacity() = 0.5f;

  // Define material 2
  config.useMap = true;
  config.useColor = false;
  m_material2 = BasicMaterial::create(config, "basic2");
  m_material2->color() = Vector3f(1.0f, 0.0f, 0.0f);
  // FIXME: m_material2->texture().load("textures/wooden-crate.jpg");

  // Create leaves of the scene graph
  m_cube1 = Cube::create("Cube1", m_material1); m_cube1->texture().load("textures/wooden-crate.jpg"); // FIXME A supprimer + material->load(...)
  m_cube2 = Cube::create("Cube2", m_material2); m_cube2->texture().load("textures/wooden-crate.jpg");
  m_cube3 = Cube::create("Cube3", m_material1); m_cube3->texture().load("textures/wooden-crate.jpg"); // TODO: m_cube3 = Cube::alias(m_cube1);
  m_cube4 = Cube::create("Cube4", m_material2); m_cube4->texture().load("textures/wooden-crate.jpg"); // TODO: m_cube4 = Cube::alias(m_cube2);

  // Create a node switching manually children. In this example use keyboard to
  // select child.
  m_switch = SwitchNode3D::create("Switch");
  m_switch->select(0);

  // Node switching children periodically
  m_blinker = BlinkerNode3D::create("Blinker2s", 2000_z);

  // Create the whole scene graph
  m_root = Node3D::create("Root", false);
  m_switch->attach(m_cube1).attach(m_cube2);
  m_blinker->attach(m_cube3).attach(m_cube4);
  m_root->attach(m_switch).attach(m_blinker);

  // Place shapes inside the scene
  m_cube1->vertices() *= 2.0f;
  m_cube2->vertices() *= 2.0f;
  m_cube3->vertices() *= 2.0f;
  m_cube4->vertices() *= 2.0f;
  m_cube1->move(Vector3f(-2.0f, -2.0f, 0.0f));
  m_cube2->move(Vector3f(-2.0f,  2.0f, 0.0f));
  m_cube3->move(Vector3f( 2.0f, -2.0f, 0.0f));
  m_cube4->move(Vector3f( 2.0f,  2.0f, 0.0f));

  // TODO: render + apply override material

  enableCallbacks(window::Event::Keyboard);

  // Success
  return true;
}

//------------------------------------------------------------------------------
//! \brief Paint our scene. Here we are using the delta time to
//! change the color of the window.
//------------------------------------------------------------------------------
bool SceneNodes::draw()
{
  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  // Update internal transformation matrices
  m_root->update(dt());

  // Draw the scene
  m_root->render();

  // Success
  return true;
}
