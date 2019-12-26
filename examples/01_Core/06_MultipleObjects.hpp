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

#ifndef EXAMPLE_06_MULTIPLE_OBJECTS_HPP
#  define EXAMPLE_06_MULTIPLE_OBJECTS_HPP

#  include <OpenGLCppWrapper/OpenGLCppWrapper.hpp>

using namespace glwrap;

// *****************************************************************************
//! \brief This example show differences between drawing multiple VAOs versus
//! drawing multiple VBOs. This example also shows diiferents way to draw an
//! object.
// *****************************************************************************
class MultipleObjects: public IGLWindow
{
public:

  MultipleObjects();
  ~MultipleObjects();

private:

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

#endif // EXAMPLE_06_MULTIPLE_OBJECTS_HPP
