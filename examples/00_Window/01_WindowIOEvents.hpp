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

#ifndef EXAMPLE_01_EMPTY_WINDOW_WITH_IO_EVENTS_HPP
#  define EXAMPLE_01_EMPTY_WINDOW_WITH_IO_EVENTS_HPP

#  include <OpenGLCppWrapper/OpenGLCppWrapper.hpp>

using namespace glwrap;
using namespace glwrap::window;

//------------------------------------------------------------------------------
//! \brief This example shows how to create a window for OpenGL reacting to
//! input/ouput (IO) events: keyboard pressed, mouse moved, button
//! pressed/released, scrolled, window size changed. No OpenGL stuffs are
//! rendered here for the moment. The basic OpenGL routine called here is just
//! to clear the window with color varying in the time.
//------------------------------------------------------------------------------
class WindowIOEvents: public IGLWindow
{
public:

  WindowIOEvents();
  ~WindowIOEvents();

private:

  virtual void onWindowSizeChanged() override;
  virtual void onMouseButtonPressed(Mouse const& mouse) override;
  virtual void onMouseMoved(Mouse const& mouse) override;
  virtual void onMouseScrolled(Mouse const& mouse) override;
  virtual void onKeyboardEvent() override;
  virtual bool setup() override;
  virtual bool draw() override;
};

#endif // EXAMPLE_01_EMPTY_WINDOW_WITH_IO_EVENTS_HPP
