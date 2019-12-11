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

#ifndef EXAMPLE_07_SKYBOX_TEXTURECUBE_HPP
#  define EXAMPLE_07_SKYBOX_TEXTURECUBE_HPP

#  include <OpenGLCppWrapper/OpenGLCppWrapper.hpp>
#  include <iostream>

using namespace glwrap;
using namespace glwrap::window;

class GLExample07: public IGLWindow
{
public:

  GLExample07()
    : m_shape("Shape"),
      m_skybox("SkyBox"),
      m_progSkyBox("progSkyBox"),
      m_cameraController(Camera3D::Type::PERSPECTIVE)
  {}

  ~GLExample07()
  {}

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

  GLVertexShader     vs1;
  GLFragmentShader   fs1;
  Shape3D            m_shape;
  GLVAO              m_skybox;
  GLProgram          m_progSkyBox;
  CameraController   m_cameraController;
};

#endif // EXAMPLE_07_SKYBOX_TEXTURECUBE_HPP
