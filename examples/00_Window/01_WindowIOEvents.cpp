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

#include "01_WindowIOEvents.hpp"
#include <iostream>

WindowIOEvents::WindowIOEvents()
{
  std::cout << "Hello WindowIOEvents" << std::endl;
}

WindowIOEvents::~WindowIOEvents()
{
  std::cout << "Bye WindowIOEvents" << std::endl;
}

//------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------
void WindowIOEvents::onWindowSizeChanged()
{
  // Note: height is never zero !
  float ratio = width<float>() / height<float>();

  // Make the viewport matches the new window dimensions.
  glCheck(glViewport(0, 0, width<int>(), height<int>()));

  std::cout << "New Window dimension "
            << width<int>() << " x " << height<int>()
            << ". Ratio is " << ratio << std::endl;
}

// -----------------------------------------------------------------------------
//! \brief Keyboard event (Note Escape key is already map to kill the
//! application).
// -----------------------------------------------------------------------------
void WindowIOEvents::onKeyboardEvent()
{
  if (isKeyDown(GLFW_KEY_F1))
    std::cout << std::endl << "Key pressed F1" << std::endl;
  if (isKeyUp(GLFW_KEY_F1))
    std::cout << std::endl << "Key released F1" << std::endl;
  if (wasKeyReleased(GLFW_KEY_F1))
    std::cout << std::endl << "Key was released F1" << std::endl;
  if (wasKeyPressed(GLFW_KEY_F1))
    std::cout << std::endl << "Key was pressed F1" << std::endl;

  if (isKeyDown(GLFW_KEY_A))
    std::cout << std::endl << "Key pressed A" << std::endl;
  if (isKeyUp(GLFW_KEY_A))
    std::cout << std::endl << "Key released A" << std::endl;
  if (wasKeyReleased(GLFW_KEY_A))
    std::cout << std::endl << "Key was released A" << std::endl;
  if (wasKeyPressed(GLFW_KEY_A))
    std::cout << std::endl << "Key was pressed A" << std::endl;
}

// -----------------------------------------------------------------------------
//! \brief Callback on mouse pressed or release event.
//!
//! This example display the mouse status and hide/show the mouse
//! cursor.
// -----------------------------------------------------------------------------
void WindowIOEvents::onMouseButtonPressed(Mouse const& mouse)
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
void WindowIOEvents::onMouseScrolled(Mouse const& mouse)
{
  std::cout << "Mouse scrolled " << mouse.scroll.x << std::endl;
}

// -----------------------------------------------------------------------------
//! \brief Callback on mouse movement event.
// -----------------------------------------------------------------------------
void WindowIOEvents::onMouseMoved(Mouse const& mouse)
{
  std::cout << "Mouse moved to "
    << static_cast<int>(mouse.position.x) << " x "
    << static_cast<int>(mouse.position.y) << std::endl;

  std::cout << "Mouse delta movement "
            << mouse.displacement.x << " -- "
            << mouse.displacement.y << std::endl;
}

//------------------------------------------------------------------
//! \brief Add your OpenGL code here for initializing your scene.
//------------------------------------------------------------------
bool WindowIOEvents::setup()
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
bool WindowIOEvents::draw()
{
  // Cumulate the time
  static float time = 0.0f;
  time += dt();

  // Change the background color
  float ct = cosf(time) * 0.5f + 0.5f;
  float st = sinf(time) * 0.5f + 0.5f;
  glCheck(glClearColor(st, ct, 0.0f, 1.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT));

  // Success
  return true;
}
