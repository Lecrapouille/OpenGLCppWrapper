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

#include "00_EmptyWindowWithIOEvents.hpp"

//------------------------------------------------------------------
//! \file This example shows how to create an empty window reacting
//! to IO events (keyboard pressed, mouse moved, window changed). In
//! the draw() method, we only call OpenGL clear the window with
//! colors depending on the time.
//------------------------------------------------------------------

//------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------
void GLExample00::onWindowSizeChanged(const float width, const float height)
{
  // Note: height is never zero !
  float ratio = width / height;

  // Make sure the viewport matches the new window dimensions.
  glCheck(glViewport(0, 0, static_cast<int>(width), static_cast<int>(height)));

  std::cout << "New Window dimension "
    << static_cast<int>(width) << " x " << static_cast<int>(height)
    << ". Ratio is " << ratio << std::endl;
}

//------------------------------------------------------------------
//! \brief Init your scene.
//------------------------------------------------------------------
bool GLExample00::setup()
{
  std::cout << "Add your OpenGL code here for initializing your scene." << std::endl;

  // Do not show the mouse cursor
  hideMouseCursor();

  // Success
  return true;
}

//------------------------------------------------------------------
//! \brief Paint our scene. Here we are using the delta time to
//! change the color of the window.
//------------------------------------------------------------------
bool GLExample00::draw()
{
  static float time = 0.0f;
  time += dt();

  std::cout << time << "ms. OpenGL is painting at " << fps() << " FPS."
            << std::endl;

  const float ct = cosf(time) * 0.5f + 0.5f;
  const float st = sinf(time) * 0.5f + 0.5f;
  glCheck(glClearColor(st, ct, 0.0f, 1.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT));

  // Key pressed event (Note Escape key is already map to kill the
  // application).
  if (keyPressed(GLFW_KEY_W)) { std::cout << std::endl << "Key pressed 'W'" << std::endl; }
  if (keyPressed(GLFW_KEY_F1)) { std::cout << std::endl << "Key pressed 'F1'" << std::endl; }

  // Success
  return true;
}
// --------------------------------------------------------------
//! \brief Mouse event
// --------------------------------------------------------------
void GLExample00::onMouseMoved(const double xpos, const double ypos)
{
  std::cout << "Mouse moved to "
    << static_cast<int>(xpos) << " x " << static_cast<int>(ypos)
    << std::endl;
}
