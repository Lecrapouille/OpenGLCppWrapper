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

#include "09_SceneGraph.hpp"
#include <sstream>

//------------------------------------------------------------------------------
//! \file Display a scene graphe made of 3 animated robots. Each robots are
//! nodes of the scene graph. Each element of robots is also a part of the scene
//! graph.
//------------------------------------------------------------------------------

CubicRobot::CubicRobot(const char *name)
  : Node3D(name)
{
  DEBUG("Create CubicRobot %s", name);

  // Body
  m_body = Cube::create("Body");
  m_body->localScale(Vector3f(10.0f, 15.0f, 5.0f));
  m_body->position(Vector3f(0.0f, 35.0f, 0.0f));

  // Head
  m_head = Cube::create("Head");
  m_head->localScale(Vector3f(5.0f));
  m_head->position(Vector3f(0.0f, 30.0f, 0.0f));

  // Left arm
  m_leftArm = Cube::create("LeftArm");
  m_leftArm->localScale(Vector3f(3.0f, -18.0f, 3.0f));
  m_leftArm->position(Vector3f(-12.0f, 30.0f, -1.0f));

  // Right arm
  m_rightArm = Cube::create("RightArm");
  m_rightArm->localScale(Vector3f(3.0f, -18.0f, 3.0f));
  m_rightArm->position(Vector3f(12.0f, 30.0f, -1.0f));

  // Left leg
  m_leftLeg = Cube::create("LeftLeg");
  m_leftLeg->localScale(Vector3f(3.0f, -17.5f, 3.0f));
  m_leftLeg->position(Vector3f(-8.0f, 0.0f, 0.0f));

  // Right leg
  m_rightLeg = Cube::create("RightLeg");
  m_rightLeg->localScale(Vector3f(3.0f, -17.5f, 3.0f));
  m_rightLeg->position(Vector3f(8.0f, 0.0f, 0.0f));

  // Robot
  m_body->add(m_head);
  m_body->add(m_leftArm);
  m_body->add(m_rightArm);
  m_body->add(m_leftLeg);
  m_body->add(m_rightLeg);
  this->add(m_body);

  //m_body->debug();
}

CubicRobot::~CubicRobot()
{
  DEBUG("Destroy CubicRobot %s", name().c_str());
}

//------------------------------------------------------------------------------
//! \brief Move element of the robot body
//------------------------------------------------------------------------------
void CubicRobot::update(float const dt)
{
  DEBUG("%s", "Robot::update");

  // Speed: 36 degrees per second
  const float radiansPerSecond = maths::toRadian(36.0f);
  radiansRotated = dt * radiansPerSecond;
  radiansRotated = maths::wrapToPI(radiansRotated);

  rotateY(radiansRotated);
  m_head->rotateY(-radiansRotated);
  m_leftArm->rotateX(-radiansRotated);
  m_rightArm->rotateX(radiansRotated);

  // Update world transform matrices
  Node3D::update(dt);
}

//------------------------------------------------------------------------------
//! \brief Paint the GUI
//------------------------------------------------------------------------------
void GLImGUI::observeNode(Node3D const& node) //const
{
  std::string nodename("Node '" + node.name() + "'");
  ImGui::SetNextTreeNodeOpen(true);
  if (ImGui::TreeNode(nodename.c_str()))
    {
      //VAO const& mesh = node.renderable();
      //if (nullptr != mesh)
      if (node.renderable())
      {
        std::string name("Meshes");// '" + mesh->name() + "'");
          ImGui::TextUnformatted(name.c_str());
        }
      else
        {
          ImGui::Text("Has no meshes");
        }

      ImGui::Text("Transf. Matrix:");
      std::stringstream ss;
      ss << node.worldTransform();
      ImGui::TextUnformatted(ss.str().c_str());

      ss.str("");
      ss << "Has child " << node.children().size() << " Nodes:";
      ImGui::SetNextTreeNodeOpen(true);
      if (ImGui::TreeNode(ss.str().c_str()))
        {
          for (auto const& i: node.children())
            {
              observeNode(*i);
            }
          ImGui::TreePop();
        }
      ImGui::TreePop();
    }
}

//------------------------------------------------------------------------------
//! \brief Paint the GUI
//------------------------------------------------------------------------------
bool GLImGUI::render()
{
  ImGui::SetNextTreeNodeOpen(true);
  ImGui::Begin("Hello, world!");

  if (ImGui::TreeNode("Scene graph"))
    {
      observeNode(*m_scene);
      ImGui::TreePop();
    }
  ImGui::Separator();

  ImGui::End();
  return true;
}

//------------------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------------------
void GLExample09::onWindowSizeChanged()
{
  // Make sure the viewport matches the new window dimensions.
  glCheck(glViewport(0, 0, width<int>(), height<int>()));

  //TODO
  //m_prog.matrix44f("projection") =
  //  m_camera.camera().updateProjectionMatrix();
}

//------------------------------------------------------------------------------
//! Create the cene graph made of 3 robots. Each robot is a scene node.
//------------------------------------------------------------------------------
bool GLExample09::setup()
{
  DEBUG("%s", "Create graph scene");

  // Init the context of the DearIMgui library
  if (false == m_imgui.setup(*this))
    return false;

  // Enable some OpenGL states
  glCheck(glEnable(GL_DEPTH_TEST));
  glCheck(glDepthFunc(GL_LESS));
  glCheck(glDisable(GL_BLEND));
  glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  // Create 3 Scene nodes (robots)
  Node3D_SP robot1 = std::make_shared<CubicRobot>("CubicRobot1");
  Node3D_SP robot2 = std::make_shared<CubicRobot>("CubicRobot2");
  Node3D_SP robot3 = std::make_shared<CubicRobot>("CubicRobot3");

  // Place robots on the scene
  robot2->position(Vector3f(30.0f, 0.0f, 0.0f));
  robot3->position(Vector3f(60.0f, 0.0f, 0.0f));

  // Create the scene
  m_scene = Node3D::create("Root");
  m_scene->add(robot1);
  m_scene->add(robot2);
  m_scene->add(robot3);

  // This is an example for searching a node.
  // Be careful: this is not a robust method: this function does not
  // manage nodes with duplicated identifier: it will halt on the
  // first id.
  {
    if (m_scene->getSibling("fdf") == nullptr)
      std::cout << "nullptr" << std::endl;
    m_scene->getSibling("CubicRobot1")->debug();
    Node3D::get(m_scene, "Body")->debug();
  }

  // Show the scene graph in the GUI. Note: this method is not safe
  // against tree reorganisation. This is just for the example !
  m_imgui.observeGraph(m_scene);

  return true;
}

//------------------------------------------------------------------------------
//! Draw the scene graph (made of robots)
//------------------------------------------------------------------------------
bool GLExample09::draw()
{
  DEBUG("%s", "GLExample09::draw()");

  // clear everything
  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  // Traverse the scene graph for moving robots
  m_scene->update(dt());

  // Traverse the scene graph for drawing robots.
  //Matrix44f view = m_camera.camera().viewMatrix();
  //m_progSkyBox.matrix44f("view") = Matrix44f(Matrix33f(view));
  m_scene->renderer(/*m_camera*/);

  // Paint the GUI
  if (false == m_imgui.draw())
    return false;

  return true;
}
