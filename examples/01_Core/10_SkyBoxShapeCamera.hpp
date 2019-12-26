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

#ifndef EXAMPLE_10_SKYBOX_SHAPE_CAMERA_HPP
#  define EXAMPLE_10_SKYBOX_SHAPE_CAMERA_HPP

#  include <OpenGLCppWrapper/OpenGLCppWrapper.hpp>

using namespace glwrap;
using namespace glwrap::window;

//------------------------------------------------------------------
//! \file this example paints a shape inside a skybox. A skybox is
//! cubic texture simuling a infinite landscape
//!
//! \note This example takes its inspiration from
//! https://learnopengl.com/Advanced-OpenGL/Cubemaps
//------------------------------------------------------------------
class SkyBoxShapeCamera: public IGLWindow
{
public:

  SkyBoxShapeCamera();
  ~SkyBoxShapeCamera();

private:

  bool createShape();
  void drawShape();
  bool createSkyBox();
  void drawSkyBox();
  virtual void onMouseScrolled(Mouse const& mouse) override;
  virtual void onMouseMoved(Mouse const& mouse) override;
  virtual void onKeyboardEvent() override;
  virtual void onWindowSizeChanged() override;
  virtual bool setup() override;
  virtual bool draw() override;

private:

  GLVertexShader     vs1, vs2;
  GLFragmentShader   fs1, fs2;
  GLVAO              m_shape, m_skybox;
  GLProgram          m_progShape, m_progSkyBox;
  CameraController   m_cameraController;
  Transformable<float, 3U> m_transformable;
};

#endif // EXAMPLE_10_SKYBOX_SHAPE_CAMERA_HPP
