//=====================================================================
// OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of OpenGLCppWrapper.
//
// OpenGLCppWrapper is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with OpenGLCppWrapper.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef GLIMGUI_HPP
#  define GLIMGUI_HPP

#  include "GLWindow.hpp"
#  include "imgui/imgui.h"
#  include "imgui/imgui_impl_glfw.h"
#  include "imgui/imgui_impl_opengl3.h"

// **************************************************************
//! \brief Class wrapper for the dear imgui library: an imediate
//! mode (im) graphical user interface (GUI) for OpenGL.
//! https://github.com/ocornut/imgui
// **************************************************************
class IGLImGUI
{
public:

  //------------------------------------------------------------------
  //! \brief Empty constructor. Do nothing.
  //------------------------------------------------------------------
  IGLImGUI() {}

  //------------------------------------------------------------------
  //! \brief Release Dear imgui allocated resources.
  //------------------------------------------------------------------
  ~IGLImGUI()
  {
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }

  //------------------------------------------------------------------
  //! \brief Start Dear imgui context. Use glfw routines.
  //------------------------------------------------------------------
  bool setup(IGLWindow &window)
  {
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window.obj(), true);
    ImGui_ImplOpenGL3_Init(NULL);
    ImGui::StyleColorsDark();

    return true;
  }

  //------------------------------------------------------------------
  //! \brief Prepare the Dear imgui to draw the HMI. This will call the
  //! render() pure virtual method.
  //------------------------------------------------------------------
  bool draw()
  {
    bool res;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    res = render();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    return res;
  }

protected:

  //------------------------------------------------------------------
  //! \brief Method for drawing the HMI. This has to be implemented by
  //! the derived class.
  //------------------------------------------------------------------
  virtual bool render() = 0;
};

#endif
