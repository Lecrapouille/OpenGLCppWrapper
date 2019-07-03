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

#ifndef EXAMPLE_15_TEXT_HPP
#  define EXAMPLE_15_TEXT_HPP

#  include <OpenGLCppWrapper/OpenGLCppWrapper.hpp>
#  include <iostream>

using namespace glwrap;

class GLExample15: public IGLWindow
{
public:

  GLExample15()
  // : m_string("textures/firasans-medium.ttf")
  {}

  ~GLExample15()
  {}

protected:

  virtual void onWindowSizeChanged(const float width, const float height) override;
  virtual bool setup() override;
  virtual bool draw() override;

private:

  FTFont   m_fonts[2];
  //GLString m_string;
};

#endif // EXAMPLE_15_TEXT_HPP
