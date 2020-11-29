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

#include "00_BasicWindow.hpp"
#include <iostream>

//------------------------------------------------------------------------------
//! \brief
//------------------------------------------------------------------------------
BasicWindow::BasicWindow(uint32_t const width, uint32_t const height,
                         const char *title)
    : GLWindow(width, height, title)
{
    std::cout << "Hello BasicWindow" << std::endl;
}

//------------------------------------------------------------------------------
//! \brief
//------------------------------------------------------------------------------
BasicWindow::~BasicWindow()
{
    std::cout << std::endl; // flush the \r char
    std::cout << "Bye BasicWindow" << std::endl;
}

//------------------------------------------------------------------------------
//! \brief Add your C++/OpenGL code here for initializing your scene.
//------------------------------------------------------------------------------
bool BasicWindow::onSetup()
{
    // Add here you code for initializing your game.
    std::cout << "Setup BasicWindow" << std::endl;

    // Two cases:
    // - setup ends with success => return true. In this case the function draw()
    //   will be called periodically.
    // - setup reaches an error => return false. In case, the draw() function will
    //   not be called and instead the function onSetupFailed() is called and the
    //   program immediatley exit (and release CPU and GPU memory before).
    return true;
}

//------------------------------------------------------------------------------
//! \brief Callback when onSetup() failed.
//------------------------------------------------------------------------------
void BasicWindow::onSetupFailed(std::string const& reason)
{
    // To reach this code make onSetup() return false;
    std::cerr << "Failure during the setup. Reason: " << reason << std::endl;
}

//------------------------------------------------------------------------------
//! \brief Callback for painting our scene. Note that swap buffer are called
//! automaticaly.
//------------------------------------------------------------------------------
bool BasicWindow::onPaint()
{
    // Note: this is not mandatory but you can wrap each OpenGL function with
    // the glCheck() to verify if everything is alright.
    glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
    glCheck(glClear(GL_COLOR_BUFFER_BIT));

    // The delta time (in seconds) from the previous draw() and frames per
    // seconds (FPS) are automatically computed.
    std::cout << "Delta time: " << uint32_t(dt() * 1000.0f) << " ms."
              << "   FPS: " << fps()
              << "        \r"
              << std::flush;

    // Two cases:
    // - draw() ends with success => return true. In this case the function draw()
    //   will be keep be called periodically.
    // - draw() reaches an error => return false. In case, the next draw()
    //   function will not be called and instead the function onDrawFailed() is
    //   called and the program immediatley exit (and release CPU and GPU memory
    //   before).
    return true;
}

//------------------------------------------------------------------------------
//! \brief Callback when onPaint() failed.
//------------------------------------------------------------------------------
void BasicWindow::onPaintFailed(std::string const& reason)
{
    // To reach this code make onPaint() return false;
    std::cerr << "Failure during rendering. Reason: " << reason << std::endl;
}
