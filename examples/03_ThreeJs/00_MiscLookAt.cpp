//=====================================================================
// OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
// Copyright 2018-2022 Quentin Quadrat <lecrapouille@gmail.com>
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

#include "00_MiscLookAt.hpp"
#include <iostream>

// https://github.com/mrdoob/three.js/blob/master/examples/misc_lookat.html

MiscLookAt::MiscLookAt()
{}

MiscLookAt::~MiscLookAt()
{}

//------------------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------------------
void MiscLookAt::onWindowSizeChanged()
{
  // Note: height is never zero !
  float ratio = width<float>() / height<float>();

  // Make sure the viewport matches the new window dimensionm_shape.
  glCheck(glViewport(0, 0, width<int>(), height<int>()));

  for (auto it: m_scene->children())
    {
      Shape3D* shape = dynamic_cast<Shape3D*>(it.get());
      shape->projection() = m_cameraController->camera().updateProjectionMatrix();
      shape->view() = m_cameraController->camera().updateViewMatrix();
    }
}

// --------------------------------------------------------------
//! \brief Mouse event
// --------------------------------------------------------------
void MiscLookAt::onMouseMoved(Mouse const& mouse)
{
  auto& camera = m_cameraController->camera();
  float const dx = static_cast<float>(mouse.position.x) - camera.position().x;
  float const dy = static_cast<float>(mouse.position.y) - camera.position().y;

  m_mouse_position = Vector3f(dx * 0.05f, dy * 0.05f, 0.0f);
  std::cout << "iiii " << m_mouse_position << std::endl;
}

//------------------------------------------------------------------------------
//! \brief Init your a scene made of a moving cube and cones pointing to it.
//------------------------------------------------------------------------------
bool MiscLookAt::setup()
{
  glCheck(glEnable(GL_DEPTH_TEST));
  glCheck(glDepthFunc(GL_LESS));
  glCheck(glDisable(GL_BLEND));
  glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  // Enable IO callbacks
  enableCallbacks(window::Event::MouseMove);
  hideMouseCursor();

  // Place a camera
  m_cameraController = CameraController::create(Camera3D::Type::PERSPECTIVE);
  m_cameraController->camera().init(10.0f, 0.1f, 15000.0f);
  m_cameraController->camera().lookAt(// Position
                                     Vector3f(0.0f, 0.0f, 3200.0f),
                                     // looking at
                                     Vector3f(0.0f, 0.0f, 0.0f));

  //
  NormalsMaterial_SP material = NormalsMaterial::create();
  m_scene = Node3D::create("Root", false);

  // Create a cube
  m_cube = Cube::create("Cube", material);
  m_scene->attach(m_cube);

  // Create cones
  for (size_t i = 0; i < 1000_z; ++i)
    {
      std::string name = "Cone" + std::to_string(i);
      Cone_SP cone = Cone::create(name, material, 10.0f, 100.0f, 12u); // TODO: eviter de re-creer la geometry 1000 foid
      cone->position(Vector3f(maths::random<float>() * 4000.0f - 2000.0f,
                              maths::random<float>() * 4000.0f - 2000.0f,
                              maths::random<float>() * 4000.0f - 2000.0f));
      cone->scaling(Vector3f(maths::random<float>() * 4.0f + 2.0f,
                             maths::random<float>() * 4.0f + 2.0f,
                             maths::random<float>() * 4.0f + 2.0f));
      // FIXME: to be remove
      cone->texture().load<SOIL>("external/assets/wooden-crate.jpg");

      m_scene->attach(cone);
    }

// FIXME: to be remove
  m_cube->texture().load<SOIL>("external/assets/wooden-crate.jpg");

  return true;
}

//------------------------------------------------------------------------------
//! \brief
//------------------------------------------------------------------------------
bool MiscLookAt::draw()
{
  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  static float time = 0.0f;
  time += dt();

  m_cube->position(Vector3f(sinf(time * 0.7f) * 2000.0f,
                            cosf(time * 0.5f) * 2000.0f,
                            cosf(time * 0.3f) * 2000.0f));

  //for (auto it: m_scene->children())
  //  it->lookAt(m_cube->position());

  auto& camera = m_cameraController->camera();
  camera.translate(m_mouse_position);
  camera.lookAt(m_scene->position());

  m_scene->update(dt());
  m_scene->render();

  return true;
}
