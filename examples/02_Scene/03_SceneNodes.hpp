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

#ifndef EXAMPLE_03_SCENENODES_HPP
#  define EXAMPLE_03_SCENENODES_HPP

#  include <OpenGLCppWrapper/OpenGLCppWrapper.hpp>

using namespace glwrap;

// *****************************************************************************
//! \brief
// *****************************************************************************
class SceneNodes
  : public IGLWindow
{
public:

  SceneNodes();
  ~SceneNodes();

private:

  virtual void onWindowSizeChanged() override;
  virtual void onKeyboardEvent() override;
  virtual bool setup() override;
  virtual bool draw() override;

private:

  Node3D_SP           m_root;
  Cube_SP             m_cube1;
  Cube_SP             m_cube2;
  Cube_SP             m_cube3;
  Cube_SP             m_cube4;
  SwitchNode3D_SP     m_switch;
  BlinkerNode3D_SP    m_blinker;
  BasicMaterial_SP    m_material1;
  BasicMaterial_SP    m_material2;
  CameraController_SP m_cameraController;
};

#endif // EXAMPLE_03_SCENENODES_HPP
