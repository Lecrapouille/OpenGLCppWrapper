//=====================================================================
// OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
// Copyright 2018-2021 Quentin Quadrat <lecrapouille@gmail.com>
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

BasicWindow::BasicWindow()
{
  std::cout << "Hello BasicWindow" << std::endl;
}

BasicWindow::~BasicWindow()
{
  std::cout << "Bye BasicWindow" << std::endl;
}

//------------------------------------------------------------------------------
//! \brief Add your C++/OpenGL code here for initializing your scene.
//------------------------------------------------------------------------------
bool BasicWindow::setup()
{
  // Two cases:
  // - setup ends with success => return true. In this case the function draw()
  //   will be called periodically.
  // - setup reaches an error => return false. In case, the draw() function will
  //   not be called and instead the function onSetupFailed() is called and the
  //   program immediatley exit (and release CPU and GPU memory before).
  return true;
}

//------------------------------------------------------------------------------
//! \brief Callback for painting our scene. Note that swap buffer are called
//! automaticaly.
//------------------------------------------------------------------------------
bool BasicWindow::draw()
{
  // Note: wrap each OpenGL function with the glCheck() to check everything is
  // alright.
  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT));

  // The delta time (in seconds) from the previous draw() and frames per seconds
  // (FPS) are automatically computed.
  std::cout << "DT: " << dt() << " seconds. FPS=" << fps() << "        \r" << std::flush;

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
//! \brief Callback when setup() failed.
//------------------------------------------------------------------------------
void BasicWindow::onSetupFailed()
{
  // To reach this code make setup() return false;
  std::cerr << "onSetupFailed() called" << std::endl;
}

//------------------------------------------------------------------------------
//! \brief Callback when draw() failed.
//------------------------------------------------------------------------------
void BasicWindow::onDrawFailed()
{
  // To reach this code make draw() return false;
  std::cerr << "onDrawFailed() called" << std::endl;
}
