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
void GLExample00::onWindowSizeChanged()
{
  // Note: height is never zero !
  float ratio = width<float>() / height<float>();

  // Make sure the viewport matches the new window dimensions.
  glCheck(glViewport(0, 0, width<int>(), height<int>()));

  std::cout << "New Window dimension "
    << width<int>() << " x " << height<int>()
    << ". Ratio is " << ratio << std::endl;
}

//------------------------------------------------------------------
//! \brief Add your OpenGL code here for initializing your scene.
//------------------------------------------------------------------
bool GLExample00::setup()
{
  // Enable IO callbacks
  enableCallbacks(Event::MouseMove | Event::MouseScroll |
                  Event::MouseButton | Event::Keyboard);

  // Do not show the mouse cursor
  hideMouseCursor();

  // Success
  return true;
}

//------------------------------------------------------------------
//! \brief Callback for painting our scene.
//!
//! In this example, we are using the delta time between the previous
//! rendering frame for changing dynamically the background color of
//! the window.
//------------------------------------------------------------------
bool GLExample00::draw()
{
  // Cumulate the time
  static float time = 0.0f;
  time += dt();

  // the FPS information is automatically computed by GLWindow
  std::cout << time << "ms. OpenGL is painting at " << fps() << " FPS."
            << std::endl;

  // Change the background color
  const float ct = cosf(time) * 0.5f + 0.5f;
  const float st = sinf(time) * 0.5f + 0.5f;
  glCheck(glClearColor(st, ct, 0.0f, 1.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT));

  // Success
  return true;
}

// -----------------------------------------------------------------------------
//! \brief Keyboard event (Note Escape key is already map to kill the
//! application).

// -----------------------------------------------------------------------------
void GLExample00::onKeyboardEvent()
{
  if (isKeyDown(GLFW_KEY_W))
    std::cout << std::endl << "Key pressed 'W'" << std::endl;

  if (isKeyDown(GLFW_KEY_F1))
    std::cout << std::endl << "Key pressed 'F1'" << std::endl;

  if (isKeyUp(GLFW_KEY_W))
    std::cout << std::endl << "Key released 'W'" << std::endl;

  if (isKeyUp(GLFW_KEY_F1))
    std::cout << std::endl << "Key released 'F1'" << std::endl;
}

// -----------------------------------------------------------------------------
//! \brief Callback on mouse pressed or release event.
//!
//! This example display the mouse status and hide/show the mouse
//! cursor.
// -----------------------------------------------------------------------------
void GLExample00::onMouseButtonPressed(Mouse const& mouse)
{
  std::cout << "Mouse button " << static_cast<int>(mouse.button)
            << " was " << (mouse.pressed ? "pressed" : "released")
            << std::endl;

  static bool state = true;
  state ^= true;
  (false == state) ? hideMouseCursor() : showMouseCursor();
}

// -----------------------------------------------------------------------------
//! \brief Callback on mouse scroll event.
// -----------------------------------------------------------------------------
void GLExample00::onMouseScrolled(Mouse const& mouse)
{
  std::cout << "Mouse scrolled " << mouse.scroll.x << std::endl;
}

// -----------------------------------------------------------------------------
//! \brief Callback on mouse movement event.
// -----------------------------------------------------------------------------
void GLExample00::onMouseMoved(Mouse const& mouse)
{
  std::cout << "Mouse moved to "
    << static_cast<int>(mouse.position.x) << " x "
    << static_cast<int>(mouse.position.y) << std::endl;

  std::cout << "Mouse delta movement "
            << mouse.displacement.x << " -- "
            << mouse.displacement.y << std::endl;
}
