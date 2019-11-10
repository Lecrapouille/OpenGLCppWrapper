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

#ifndef EXAMPLE_01_EMPTY_WINDOW_WITH_DEAR_IMGUI_HPP
#  define EXAMPLE_01_EMPTY_WINDOW_WITH_DEAR_IMGUI_HPP

#  include "00_EmptyWindowWithIOEvents.hpp"

using namespace glwrap;

// *****************************************************************
//! \brief
// *****************************************************************
class GLExample01: public GLExample00
{
private:

  //! \brief
  //! \note this class can be place outside \c GLExample01
  class GLImGUI: public IGLImGUI
  {
  public:

    GLImGUI() {}

  protected:

    virtual bool render() override;
  };

public:

  GLExample01()
  {}

  ~GLExample01()
  {}

protected:

  virtual void onWindowSizeChanged() override;
  virtual bool setup() override;
  virtual bool draw() override;

private:

  GLImGUI     m_imgui;
};

#endif // EXAMPLE_01_EMPTY_WINDOW_WITH_DEAR_IMGUI_HPP
