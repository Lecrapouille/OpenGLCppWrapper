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

#ifndef EXAMPLE_00_BASIC_WINDOW_HPP
#  define EXAMPLE_00_BASIC_WINDOW_HPP

#  include <OpenGLCppWrapper/OpenGLCppWrapper.hpp>

using namespace glwrap;
using namespace glwrap::window;

//------------------------------------------------------------------------------
//! \brief This example shows how to create a window instanciating an OpenGL
//! context. No OpenGL stuffs are rendered here for the moment. The basic OpenGL
//! routine called here is just to clear the window. No input/ouput events (IO)
//! are observed for the moment.
//------------------------------------------------------------------------------
class BasicWindow: public IGLWindow
{
public:

  BasicWindow();
  ~BasicWindow();

private:

  virtual bool setup() override;
  virtual bool draw() override;
  virtual void onSetupFailed() override;
  virtual void onDrawFailed() override;
};

#endif // EXAMPLE_00_BASIC_WINDOW_HPP
