//=====================================================================
// OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of OpenGLCppWrapper.
//
// OpenGLCppWrapper is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with OpenGLCppWrapper.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef EXAMPLE02_HPP
#  define EXAMPLE02_HPP

#  include "GLImGUI.hpp"
#  include "OpenGL.hpp"
#  include "SceneGraph.tpp"

//------------------------------------------------------------------
//! \brief Define a 3D SceneGraph node (matrix 4x4 of float + VAO)
//------------------------------------------------------------------
typedef SceneNode<GLVAO, float, 3U> SceneNode_t;

//------------------------------------------------------------------
//! \brief This class allows to display a GUI for debuging the scene
//! graph.
//------------------------------------------------------------------
class GLImGUI: public IGLImGUI
{
public:

  GLImGUI()
    : m_root(nullptr)
  {
  }

  inline void setNode(SceneNode_t &root)
  {
    m_root = &root;
  }

protected:

  virtual bool render() override;
  void drawNode(SceneNode_t &node);

private:

  SceneNode_t *m_root;
};

//------------------------------------------------------------------
//! \brief a CubicRobot is an robot made of cubes. A CubicRobot is a
//! node of the scene graph. A robot is made of a head, body, legs
//! and arms which are also a node of the scene graph.
//------------------------------------------------------------------
class CubicRobot: public SceneNode_t
{
public:

  CubicRobot(const char *name, GLVAO& cube);
  ~CubicRobot()
  {
    LOGD("---------------- destroy CubicRobot -----------------");
  }
  virtual void update(float const dt) override;

private:

  SceneNode_t *m_body;
  SceneNode_t *m_head;
  SceneNode_t *m_leftArm;
  SceneNode_t *m_rightArm;
  SceneNode_t *m_leftLeg;
  SceneNode_t *m_rightLeg;
  float degreesRotated = 0.0f;
};

//------------------------------------------------------------------
//! \brief Display a scere graphe made of 3 moving robots. Each robots
//! are nodes of the scere graph. Each element of robots is also a
//! part of the scene graph.
//------------------------------------------------------------------
class GLExample02: public IGLWindow
{
public:

  GLExample02();
  ~GLExample02();

private:

  virtual void onWindowSizeChanged(const float width, const float height) override;
  virtual bool setup() override;
  virtual bool draw() override;
  void drawNode(SceneNode_t &node);
  void CreateCube();

private:

  GLVertexShader     vs;
  GLFragmentShader   fs;
  GLVAO              m_cube; // FIXME interverting with m_prog produce a crash
  GLProgram          m_prog; // FIXME
  CubicRobot        *m_robot1;
  CubicRobot        *m_robot2;
  CubicRobot        *m_robot3;
  SceneNode_t       *m_root;
  GLImGUI            m_gui;
};

#endif // EXAMPLE_02_HPP
