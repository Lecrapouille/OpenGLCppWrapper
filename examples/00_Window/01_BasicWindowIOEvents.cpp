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

#include "01_BasicWindowIOEvents.hpp"
#include <iostream>

//------------------------------------------------------------------------------
//! \brief
//------------------------------------------------------------------------------
BasicWindowIOEvents::BasicWindowIOEvents(uint32_t const width, uint32_t const height,
                                         const char *title)
    : GLWindow(width, height, title)
{
    std::cout << "Hello BasicWindowIOEvents" << std::endl;
}

//------------------------------------------------------------------------------
//! \brief
//------------------------------------------------------------------------------
BasicWindowIOEvents::~BasicWindowIOEvents()
{
    std::cout << std::endl; // flush the \r char
    std::cout << "Bye BasicWindowIOEvents" << std::endl;
}

//------------------------------------------------------------------------------
//! \brief Callback when the window changed its size. Use template methods
//! width<T>(), height<T>() to get windows inforamtion. T is the destination type.
//------------------------------------------------------------------------------
void BasicWindowIOEvents::onWindowResized()
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
void BasicWindowIOEvents::onKeyboardEvent()
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
void BasicWindowIOEvents::onMouseButtonPressed()
{
    std::cout << "Mouse button " << static_cast<int>(mouse().button)
              << " was " << (mouse().pressed ? "pressed" : "released")
              << std::endl;

    static bool state = true;
    state ^= true;
    (false == state) ? hideCursor() : showCursor();
}

// -----------------------------------------------------------------------------
//! \brief Callback on mouse scroll event.
// -----------------------------------------------------------------------------
void BasicWindowIOEvents::onMouseScrolled()
{
    std::cout << "Mouse scrolled: " << mouse().scroll.x << " x "
              << mouse().scroll.y << std::endl;
}

// -----------------------------------------------------------------------------
//! \brief Callback on mouse movement event.
// -----------------------------------------------------------------------------
void BasicWindowIOEvents::onMouseMoved()
{
    std::cout << "Mouse moved to: "
              << static_cast<int>(mouse().position.x) << " x "
              << static_cast<int>(mouse().position.y) << std::endl;

    std::cout << "Mouse delta movement: "
              << mouse().displacement.x << " x "
              << mouse().displacement.y << std::endl;
}

//------------------------------------------------------------------
//! \brief Display the GPU memory usage.
//------------------------------------------------------------------
void BasicWindowIOEvents::onGPUMemoryChanged(size_t size)
{
    std::cout << "Estimated GPU memory usage: "
              << size << " bytes"
              << std::endl;
}

//------------------------------------------------------------------
//! \brief Add your OpenGL code here for initializing your scene.
//------------------------------------------------------------------
bool BasicWindowIOEvents::onSetup()
{
    // Enable IO callbacks. Without this method: no events are triggered.
    makeReactOn(Event::MouseMove | Event::MouseScroll |
                Event::MouseButton | Event::Keyboard);

    // Do not show the mouse cursor
    hideCursor();

    // Success
    return true;
}

//------------------------------------------------------------------------------
//! \brief Callback when onSetup() failed.
//------------------------------------------------------------------------------
void BasicWindowIOEvents::onSetupFailed(std::string const& reason)
{
    // To reach this code make setup() return false;
    std::cerr << "Failure during the setup. Reason: " << reason << std::endl;
}

//------------------------------------------------------------------
//! \brief Callback for painting our scene.
//!
//! In this example, we are using the delta time between the previous
//! rendering frame for changing dynamically the background color of
//! the window.
//------------------------------------------------------------------
bool BasicWindowIOEvents::onPaint()
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

//------------------------------------------------------------------------------
//! \brief Callback when onPaint() failed.
//------------------------------------------------------------------------------
void BasicWindowIOEvents::onPaintFailed(std::string const& reason)
{
    std::cerr << "Failure during rendering. Reason: " << reason << std::endl;
}
