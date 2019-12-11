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

#ifndef EXAMPLE_17_MATERIAL_NORMAL_HPP
#  define EXAMPLE_17_MATERIAL_NORMAL_HPP

#  include <OpenGLCppWrapper/OpenGLCppWrapper.hpp>
#  include <iostream>
#  include <map>

using namespace glwrap;
using namespace glwrap::window;

class GLExample17: public IGLWindow
{
public:

  GLExample17()
    : m_material(MaterialNormal::create()),
      m_cameraController(Camera3D::Type::PERSPECTIVE)
  {}

  ~GLExample17()
  {}

protected:

  bool pimpShape(Shape3D_SP shape);
  virtual void onWindowSizeChanged() override;
  virtual void onMouseMoved(Mouse const& mouse) override;
  virtual void onKeyboardEvent() override;
  virtual bool setup() override;
  virtual bool draw() override;

private:

  MaterialNormal_SP m_material;
  Tube_SP          m_tube1;
  Tube_SP          m_tube2;
  Cone_SP          m_cone1;
  Cone_SP          m_cone2;
  Pyramid_SP       m_pyra1;
  Pyramid_SP       m_pyra2;
  Shape3D_SP       m_shape;
  CameraController m_cameraController;
  uint32_t         m_slices = 8u;
  float            m_base_radius = 1.0f;
};

#endif // EXAMPLE_17_MATERIAL_NORMAL_HPP
