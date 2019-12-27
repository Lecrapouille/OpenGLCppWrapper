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

#include "07_AlphaTest.hpp"
#include <iostream>

AlphaTest::AlphaTest()
  : m_imgui(*this)
{}

AlphaTest::~AlphaTest()
{}

//------------------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------------------
void AlphaTest::onWindowSizeChanged()
{
  // Note: height is never zero !
  float ratio = width<float>() / height<float>();

  // Make sure the viewport matches the new window dimensionm_shape.
  glCheck(glViewport(0, 0, width<int>(), height<int>()));

  // FIXME
  auto& camera = m_cameraController->camera();
  for (int i = 0; i < 2; ++i)
    {
      m_planes[i]->view() = camera.updateViewMatrix();
      m_planes[i]->projection() = camera.updateProjectionMatrix();
    }
  m_axis->view() = camera.updateViewMatrix();
  m_axis->projection() = camera.updateProjectionMatrix();
  // FIXME
}

// --------------------------------------------------------------
//! \brief Mouse event
// --------------------------------------------------------------
void AlphaTest::onMouseMoved(Mouse const& mouse)
{
  float const dx = static_cast<float>(mouse.displacement.x);
  float const dy = static_cast<float>(mouse.displacement.y);
  m_cameraController->processMouseMovement(dx, dy, dt());
}

// --------------------------------------------------------------
//! \brief Keyboard event
// --------------------------------------------------------------
void AlphaTest::onKeyboardEvent()
{
  if (isKeyDown(GLFW_KEY_W) || isKeyDown(GLFW_KEY_UP))
    m_cameraController->processKeyboard(CameraController::Movement::FORWARD, dt());
  if (isKeyDown(GLFW_KEY_S) || isKeyDown(GLFW_KEY_DOWN))
    m_cameraController->processKeyboard(CameraController::Movement::BACKWARD, dt());
  if (isKeyDown(GLFW_KEY_A) || isKeyDown(GLFW_KEY_LEFT))
    m_cameraController->processKeyboard(CameraController::Movement::LEFT, dt());
  if (isKeyDown(GLFW_KEY_D) || isKeyDown(GLFW_KEY_RIGHT))
    m_cameraController->processKeyboard(CameraController::Movement::RIGHT, dt());

  // FIXME
  auto& camera = m_cameraController->camera();
  for (int i = 0; i < 2; ++i)
    {
      m_planes[i]->view() = camera.updateViewMatrix();
      m_planes[i]->projection() = camera.updateProjectionMatrix();
    }
  m_axis->view() = camera.updateViewMatrix();
  m_axis->projection() = camera.updateProjectionMatrix();
  // FIXME
}

//------------------------------------------------------------------------------
//! \brief Init your scene.
//------------------------------------------------------------------------------
void AlphaTest::createTree(const char* name, const char* texture_path, float const angle)
{
  BasicMaterialConfig config;
  config.useMap = true;
  config.useAlphaTest = true;

  BasicMaterial_SP material;
  material = BasicMaterial::create(config);
  material->color() = Vector3f(1.0f, 1.0f, 1.0f);
  material->diffuse() = Vector3f(1.0f, 1.0f, 1.0f);
  material->alphaTest() = 0.5f;

  Plane_SP plane;
  plane = Plane::create(name, material);
  plane->texture().load(texture_path);
  plane->rotateY(angle);
  m_scene->attach(plane);

  // FIXME
  static int i = 0;
  m_planes[i++] = plane;
}

//------------------------------------------------------------------------------
//! \brief Init your scene.
//------------------------------------------------------------------------------
bool AlphaTest::setup()
{
  glCheck(glEnable(GL_DEPTH_TEST));
  glCheck(glDepthFunc(GL_LESS));
  glCheck(glDisable(GL_BLEND));
  glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  // Show texture on both faces
  glCheck(glDisable(GL_CULL_FACE));

  // Enable IO callbacks
  enableCallbacks(window::Event::MouseMove | window::Event::Keyboard);
  //hideMouseCursor();

  // Create and place a camera
  m_cameraController = CameraController::create(Camera3D::Type::PERSPECTIVE);
  m_cameraController->camera().lookAt(Vector3f(1.0f), Vector3f::ZERO, Vector3f::UNIT_Y);

  // Create the scene
  m_scene = Node3D::create("Root", false);
  createTree("Tree1", "textures/tree-01.png", 0.0f);
  createTree("Tree2", "textures/tree-02.png", float(maths::HALF_PI));
  m_axis = AxesHelper::create("Axis", 10.0f);
  m_scene->attach(m_axis); //TODO m_scene->attach(xesHelper::create("Axis", 10.0f));

  // Success
  return m_imgui.setup(*this);
}

bool AlphaTest::GUI::render()
{
  static float new_alpha = 0.5f;
  static float previous_alpha = 0.5f;

  ImGui::Begin("Hello, world!");
  ImGui::SliderFloat("Alpha Test", &new_alpha, 0.0f, 1.0f);
  ImGui::End();

  if (abs(new_alpha - previous_alpha) < 0.001f)
    return true;
  previous_alpha = new_alpha;

  // Apply new value to all planes
  Node3D::traverse(m_window.m_scene, [&](Node3D_SP node)
    {
      Plane* plane = dynamic_cast<Plane*>(node.get());
      if (plane != nullptr)
        {
          BasicMaterial* material = reinterpret_cast<BasicMaterial*>(&plane->material());
          material->alphaTest() = new_alpha;
        }
    });

  // Success
  return true;
}

//------------------------------------------------------------------------------
//! \brief Paint our scene. Here we are using the delta time to
//! change the color of the window.
//------------------------------------------------------------------------------
bool AlphaTest::draw()
{
  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  // Update internal transformation matrices
  m_scene->update(dt());

  // Draw the scene
  m_scene->render();

  // Then DearImGui
  return m_imgui.draw();
}
