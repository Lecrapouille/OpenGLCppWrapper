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

#ifndef EXAMPLE_13_LIGHTING_HPP
#define EXAMPLE_13_LIGHTING_HPP

#  include <OpenGLCppWrapper/OpenGLCppWrapper.hpp>
#  include <iostream>

using namespace glwrap;
using namespace glwrap::window;

// *****************************************************************
//! \brief
// *****************************************************************
class GLExample13: public IGLWindow
{
public:

  GLExample13()
    : m_cube("VAO_cube"),
      m_lamp("VAO_lamp"),
      m_prog_cube("Prog_cube"),
      m_prog_lamp("Prog_lamp"),
      m_cameraController(Camera3D::Type::PERSPECTIVE)
  {}

  ~GLExample13()
  {}

protected:

  bool createCube();
  bool createLamp();
  void changeLightProperties(float const time);
  virtual void onMouseScrolled(Mouse const& mouse) override;
  virtual void onMouseMoved(Mouse const& mouse) override;
  virtual void onWindowSizeChanged() override;
  virtual bool setup() override;
  virtual bool draw() override;

private:

  GLVertexShader     m_vs1, m_vs2;
  GLFragmentShader   m_fs1, m_fs2;
  GLVAO              m_cube;
  GLVAO              m_lamp;
  GLProgram          m_prog_cube;
  GLProgram          m_prog_lamp;
  CameraController   m_cameraController;
};

#endif // EXAMPLE_13_LIGHTING_HPP
