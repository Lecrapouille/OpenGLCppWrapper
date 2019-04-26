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

#ifndef EXAMPLE_06_HPP
#  define EXAMPLE_06_HPP

#  include <OpenGLCppWrapper/OpenGLCppWrapper.hpp>
#  include "Maths.hpp"
#  include <iostream>

using namespace glwrap;

class GLExample06: public IGLWindow
{
public:

  GLExample06()
    : m_vao("VAO"),
      m_prog("prog")
  {}

  ~GLExample06()
  {
    std::cout << "Bye" << std::endl;
  }

private:

  virtual void onWindowSizeChanged(const float width, const float height) override;
  virtual bool setup() override;
  virtual bool draw() override;
  void buildTerrain(const int dim);
  void drawTerrain(const int dim);

private:

  GLVertexShader      vs;
  GLFragmentShader    fs;
  GLVAO               m_vao;
  GLProgram           m_prog;
  std::vector<float>  m_terrain; // Store heights of the terrain
  int                 m_nbVertices = 0;
};

#endif // EXAMPLE_06_HPP
