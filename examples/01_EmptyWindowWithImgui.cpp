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

#include "01_EmptyWindowWithImgui.hpp"

//------------------------------------------------------------------
//! \file This example extends the previous example and shows how to
//! add imgui widgets.
//------------------------------------------------------------------

//------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------
void GLExample01::onWindowSizeChanged(const float width, const float height)
{
  //setWindowSize(width, height);
  GLExample00::onWindowSizeChanged(width, height);
}

//------------------------------------------------------------------
//! \brief Init the context of the DearIMgui library
//------------------------------------------------------------------
bool GLExample01::setup()
{
  return m_imgui.setup(*this);
}

//------------------------------------------------------------------
//! \brief Paint some DearIMgui widgets.
//------------------------------------------------------------------
bool GLExample01::GLImGUI::render()
{
  ImGui::Begin("Hello, world!");
  ImGui::End();

  // Success
  return true;
}

//------------------------------------------------------------------
//! \brief Paint our scene. Here we are using the delta time to
//------------------------------------------------------------------
bool GLExample01::draw()
{
  // Paint first your OpenGL scene
  if (false == GLExample00::draw())
    return false;

  // Then DearIMgui
  return m_imgui.draw();
}

// --------------------------------------------------------------
//! \brief Mouse event
// --------------------------------------------------------------
void GLExample01::onMouseMoved(const double xpos, const double ypos)
{
  GLExample00::onMouseMoved(xpos, ypos);
}
