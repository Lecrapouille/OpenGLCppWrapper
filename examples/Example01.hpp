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

#ifndef EXAMPLE_01_HPP
#  define EXAMPLE_01_HPP

#  include "OpenGL.hpp"
#  include "Movable.tpp"
#  include "Maths.hpp"
#  include <iostream>

class GLExample01: public IGLWindow
{
public:

  GLExample01()
    : m_vao_quad("VAO_quad"),
      m_vao_floor("VAO_floor"),
      m_indices("indices"),
      m_prog("prog")
  {
  }

  ~GLExample01()
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
  GLVAO              m_vao_quad;
  GLVAO              m_vao_floor;
  GLIndexBuffer<uint8_t> m_indices;
  GLProgram          m_prog;
  Movable<float, 3U> m_movable1;
  Movable<float, 3U> m_movable2;
  Movable<float, 3U> m_movable3;
};

#endif // EXAMPLE_01_HPP
