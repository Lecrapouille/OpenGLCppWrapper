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

#ifndef EXAMPLE_04_HPP
#  define EXAMPLE_04_HPP

#  include "OpenGL.hpp"
#  include "Movable.tpp"
#  include "Maths.hpp"
#  include <iostream>

class GLExample04: public IGLWindow
{
public:

  GLExample04()
    : m_vao("VAO"),
      m_prog("prog")
  {
  }

  ~GLExample04()
  {
    std::cout << "Bye" << std::endl;
  }

private:

  virtual void onWindowSizeChanged(const float width, const float height) override;
  virtual bool setup() override;
  virtual bool draw() override;

private:

  GLVertexShader     vs;
  GLFragmentShader   fs;
  GLVAO              m_vao;
  GLProgram          m_prog;
};

#endif // EXAMPLE_04_HPP
