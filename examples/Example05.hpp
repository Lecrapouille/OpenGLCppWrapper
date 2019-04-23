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

#ifndef EXAMPLE_05_HPP
#  define EXAMPLE_05_HPP

#  include <OpenGLCppWrapper/OpenGLCppWrapper.hpp>
#  include "Camera.hpp"
#  include "Maths.hpp"
#  include <iostream>

using namespace glwrap;

class GLExample05: public IGLWindow
{
public:

  GLExample05()
    : m_cube("Cube"),
      m_skybox("SkyBox"),
      m_progCube("progCube"),
      m_progSkyBox("progSkyBox"),
      m_camera(Vector3f(0.0f, 0.0f, 3.0f))
  {
  }

  ~GLExample05()
  {
    std::cout << "Bye" << std::endl;
  }

  virtual void onMouseMoved(const double xpos, const double ypos) override;

private:

  virtual void onWindowSizeChanged(const float width, const float height) override;
  virtual bool setup() override;
  virtual bool draw() override;

private:

  GLVertexShader     vs1, vs2;
  GLFragmentShader   fs1, fs2;
  GLVAO              m_cube, m_skybox;
  GLProgram          m_progCube, m_progSkyBox;
  Camera             m_camera;
};

#endif // EXAMPLE_05_HPP
