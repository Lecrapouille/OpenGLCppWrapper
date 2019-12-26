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

#ifndef EXAMPLE_02_EMPTY_WINDOW_WITH_DEAR_IMGUI_HPP
#  define EXAMPLE_02_EMPTY_WINDOW_WITH_DEAR_IMGUI_HPP

#  include <OpenGLCppWrapper/OpenGLCppWrapper.hpp>

using namespace glwrap;
using namespace glwrap::window;

// *****************************************************************************
//! \brief This example shows how to add and use Dear ImGui widgets.
//! For more information see: https://github.com/ocornut/imgui
// *****************************************************************************
class WindowImGui: public IGLWindow
{
public:

  WindowImGui();
  ~WindowImGui();

private:

  virtual bool setup() override;
  virtual bool draw() override;

private:

  //----------------------------------------------------------------------------
  //! \brief Sub class managing Dear ImGui context and allowing to draw widgets.
  //! \note this class can be place outside \c WindowImGui
  //----------------------------------------------------------------------------
  class GUI: public DearImGui
  {
  public:

    GUI() {}

  protected:

    virtual bool render() override;
  };

  GUI   m_imgui;
};

#endif // EXAMPLE_02_EMPTY_WINDOW_WITH_DEAR_IMGUI_HPP
