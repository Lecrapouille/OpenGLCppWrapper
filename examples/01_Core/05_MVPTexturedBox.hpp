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

#ifndef EXAMPLE_05_MVP_TEXTURED_BOX_HPP
#  define EXAMPLE_05_MVP_TEXTURED_BOX_HPP

#  include <OpenGLCppWrapper/OpenGLCppWrapper.hpp>

using namespace glwrap;

// *****************************************************************************
//! \brief This example shows how to create a textured box and apply the model
//! view projection transformation matrices.
//! The model matrix allow to place the object in the scene. Here we use it for
//! spining the box.
//! The view matrix is the scene paint from the point of view of a camera.
//! The projection matrix is how the scene is projected to the camera.
// *****************************************************************************
class MVPTexturedBox: public IGLWindow
{
public:

  MVPTexturedBox();
  ~MVPTexturedBox();

private:

  void debug();
  bool loadTextures();
  virtual void onWindowSizeChanged() override;
  virtual bool setup() override;
  virtual bool draw() override;

private:

  GLVertexShader     m_vertex_shader;
  GLFragmentShader   m_fragment_shader;
  GLProgram          m_prog;
  GLVAO              m_mesh;
  // \brief Allow to specify and combine several transformation (translation,
  // scaling, rotation) and get the transformation matrix 4x4 to apply to the
  // shader.
  Transformable<float, 3U> m_transformable;
};

#endif // EXAMPLE_05_MVP_TEXTURED_BOX_HPP
