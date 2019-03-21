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

#ifndef EXAMPLE02_HPP
#  define EXAMPLE02_HPP

#  include "GLImGUI.hpp"
#  include "OpenGL.hpp"
#  include "SceneGraph.hpp"
#  include "Maths.hpp"
#  include <iostream>

//------------------------------------------------------------------
//! \brief Define a 3D SceneGraph node (matrix 4x4 of float + VAO)
//------------------------------------------------------------------
using SceneGraph    = SceneGraph_t<std::string, GLVAO, float, 3u>;
using SceneNode     = SceneGraph::Node;
using SceneNodePtr  = std::shared_ptr<SceneNode>;
using VAOPtr = std::shared_ptr<GLVAO>;

// *****************************************************************
//! \brief This class allows to display a GUI for debuging a scene
//! graph attached to it.
// *****************************************************************
class GLImGUI: public IGLImGUI
{
public:

  GLImGUI()
  {}

  //------------------------------------------------------------------
  //! \brief Attach a scene graph for its monotoring
  //------------------------------------------------------------------
  inline void observeGraph(SceneGraph& graph)
  {
    m_graph = &graph;
  }

protected:

  //------------------------------------------------------------------
  //! \brief override IGLImGUI method: debug the scenegraph
  //------------------------------------------------------------------
  virtual bool render() override;

  //------------------------------------------------------------------
  //! \brief Iterate on scene nodes for their display.
  //------------------------------------------------------------------
  void observeNode(SceneNode const& node) const;

private:

  SceneGraph* m_graph = nullptr;
};

// *****************************************************************
//! \brief a CubicRobot is an robot made of cubes. A CubicRobot is a
//! node of the scene graph. A robot is made of a head, body, legs
//! and arms which are also a node of the scene graph. To make it
//! simple we create a single 3D cube model for each nodes but feel
//! free to add more complex 3D objects.
// *****************************************************************
class CubicRobot: public SceneNode
{
public:

  CubicRobot(VAOPtr cube, const char *name);
  ~CubicRobot()
  {
    DEBUG("---------------- destroy CubicRobot -----------------");
  }

  virtual void update(float const dt) override;

private:

  SceneNodePtr m_body;
  SceneNodePtr m_head;
  SceneNodePtr m_leftArm;
  SceneNodePtr m_rightArm;
  SceneNodePtr m_leftLeg;
  SceneNodePtr m_rightLeg;
  float degreesRotated = 0.0f;
};

// *****************************************************************
//! \brief Display a scere graphe made of 3 moving robots. Each robots
//! are nodes of the scere graph. Each element of robots is also a
//! part of the scene graph.
// *****************************************************************
class GLExample02
  : public IGLWindow,
    public ISceneGraphRenderer<GLVAO, float, 3u>
{
public:

  GLExample02();
  ~GLExample02();

private:

  virtual void onWindowSizeChanged(float const width,
                                   float const height) override;
  virtual bool setup() override;
  virtual bool draw() override;
  bool CreateCube();
  virtual void drawSceneNode(GLVAO& vao, Matrix44f const& transformation) override;

private:

  GLVertexShader    vs;
  GLFragmentShader  fs;
  VAOPtr            m_cube;
  GLProgram         m_prog;
  SceneGraph        m_scenegraph;
  GLImGUI           m_gui;
};

#endif // EXAMPLE_02_HPP
