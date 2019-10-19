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

#ifndef EXAMPLE_05_VAOS_VERSUS_VBOS_HPP
#  define EXAMPLE_05_VAOS_VERSUS_VBOS_HPP

#  include <OpenGLCppWrapper/OpenGLCppWrapper.hpp>
#  include <iostream>

using namespace glwrap;

// *****************************************************************
//! \brief
// *****************************************************************
class GLExample05: public IGLWindow
{
public:

  GLExample05()
    : m_cube("VAO_cube"),
      m_floor("VAO_floor"),
      m_prog("Prog")
  {}

  ~GLExample05()
  {}

protected:

  bool cubeSetup();
  bool floorSetup();
  virtual void onWindowSizeChanged() override;
  virtual bool setup() override;
  virtual bool draw() override;

private:

  GLVertexShader     m_vertex_shader;
  GLFragmentShader   m_fragment_shader;
  GLVAO              m_cube;
  GLVAO              m_floor;
  GLProgram          m_prog;
  Transformable<float, 3U> m_transformable;
};

#endif // EXAMPLE_05_VAOS_VERSUS_VBOS_HPP
