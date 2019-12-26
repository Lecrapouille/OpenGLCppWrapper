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

#include "02_WindowImGui.hpp"
#include <iostream>

WindowImGui::WindowImGui()
{
  std::cout << "Hello WindowImGui" << std::endl;
}

WindowImGui::~WindowImGui()
{
  std::cout << "Bye WindowImGui" << std::endl;
}

//------------------------------------------------------------------
//! \brief Init the context of the DearImGui library
//------------------------------------------------------------------
bool WindowImGui::setup()
{
  return m_imgui.setup(*this);
}

//------------------------------------------------------------------
//! \brief Paint some DearImGui widgets.
//------------------------------------------------------------------
bool WindowImGui::GUI::render()
{
  ImGui::Begin("Hello, world!");
  static float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
  ImGui::ColorEdit3("color", color);
  glCheck(glClearColor(color[0], color[1], color[2], color[3]));
  ImGui::End();

  // Success
  return true;
}

//------------------------------------------------------------------
//! \brief Paint our scene. Here we are using the delta time to
//------------------------------------------------------------------
bool WindowImGui::draw()
{
  // First draw your OpenGL scene
  glCheck(glClear(GL_COLOR_BUFFER_BIT));

  // Then DearImGui
  return m_imgui.draw();
}
