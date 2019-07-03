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

#include "15_Text.hpp"

//------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------
void GLExample15::onWindowSizeChanged(float const width, float const height)
{
  // Note: height is never zero !
  float ratio = width / height;

  // Make sure the viewport matches the new window dimensions.
  glCheck(glViewport(0, 0, static_cast<int>(width), static_cast<int>(height)));

  std::cout << "New Window dimension "
    << static_cast<int>(width) << " x " << static_cast<int>(height)
    << ". Ratio is " << ratio << std::endl;
}

//------------------------------------------------------------------
//! \brief Init your scene.
//------------------------------------------------------------------
bool GLExample15::setup()
{
  glCheck(glEnable(GL_PROGRAM_POINT_SIZE));
  glCheck(glEnable(GL_DEPTH_TEST));
  glCheck(glDepthFunc(GL_LESS));
  glCheck(glDisable(GL_BLEND));
  glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  if (!m_fonts[0].load("textures/firasans-medium.ttf"))
    return false;

  if (!m_fonts[1].load("textures/NotoNaskh-Regular.ttf"))
    return false;

  //FreeTypeFontAtlas f("foo");
  //if (!f.load(m_fonts[0].face(), 48u))
  //  ERROR("ici");
  //f.begin();
  //ERROR("eee");

  GLString("textures/firasans-medium.ttf");

ERROR("eee");

  // Success
  return true;
}

//------------------------------------------------------------------
//! \brief Paint our scene. Here we are using the delta time to
//! change the color of the window.
//------------------------------------------------------------------
bool GLExample15::draw()
{
  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  // Success
  return true;
}
