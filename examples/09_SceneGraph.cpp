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
#include "Maths.hpp"
#include <sstream>

//------------------------------------------------------------------
//! \file Display a scere graphe made of 3 moving robots. Each robots
//! are nodes of the scere graph. Each element of robots is also a
//! part of the scene graph.
//------------------------------------------------------------------

CubicRobot::CubicRobot(VAOPtr cube, const char *name)
  : SceneNode(nullptr, name)
{
  DEBUG("%s", "Cstr CubicRobot");

  // Body
  m_body = attach(cube, "Body");
  m_body->localScale(Vector3f(10.0f, 15.0f, 5.0f));
  m_body->position(Vector3f(0.0f, 35.0f, 0.0f));

  // Head
  m_head = m_body->attach(cube, "Head");
  m_head->localScale(Vector3f(5.0f));
  m_head->position(Vector3f(0.0f, 30.0f, 0.0f));

  // Left arm
  m_leftArm = m_body->attach(cube, "Left Arm");
  m_leftArm->localScale(Vector3f(3.0f, -18.0f, 3.0f));
  m_leftArm->position(Vector3f(-12.0f, 30.0f, -1.0f));

  // Right arm
  m_rightArm = m_body->attach(cube, "Right Arm");
  m_rightArm->localScale(Vector3f(3.0f, -18.0f, 3.0f));
  m_rightArm->position(Vector3f(12.0f, 30.0f, -1.0f));

  // Left leg
  m_leftLeg = m_body->attach(cube, "Left Leg");
  m_leftLeg->localScale(Vector3f(3.0f, -17.5f, 3.0f));
  m_leftLeg->position(Vector3f(-8.0f, 0.0f, 0.0f));

  // Right leg
  m_rightLeg = m_body->attach(cube, "Right Leg");
  m_rightLeg->localScale(Vector3f(3.0f, -17.5f, 3.0f));
  m_rightLeg->position(Vector3f(8.0f, 0.0f, 0.0f));
}

//------------------------------------------------------------------
//! \brief Move element of the robot body
//------------------------------------------------------------------
void CubicRobot::update(float const dt)
{
  DEBUG("%s", "Robot::update");

  const GLfloat degreesPerSecond = 1.0f;
  degreesRotated += dt * degreesPerSecond;
  degreesRotated = maths::wrapTo180(degreesRotated);

  rotate(degreesRotated, Vector3f(0.0f, 1.0f, 0.0f));
  m_head->rotate(-degreesRotated, Vector3f(0.0f, 1.0f, 0.0f));
  m_leftArm->rotate(-degreesRotated, Vector3f(1.0f, 0.0f, 0.0f));
  m_rightArm->rotate(degreesRotated, Vector3f(1.0f, 0.0f, 0.0f));
  SceneNode::update(dt);
}

//------------------------------------------------------------------
//! \brief Paint the GUI
//------------------------------------------------------------------
void GLImGUI::observeNode(SceneNode const& node) const
{
  std::string nodename("Node '" + node.id() + "'");
  ImGui::SetNextTreeNodeOpen(true);
  if (ImGui::TreeNode(nodename.c_str()))
    {
      VAOPtr const& mesh = node.renderable();
      if (nullptr != mesh)
        {
          std::string name("Meshes '" + mesh->name() + "'");
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
      ss << "Has child " << node.nbChildren() << " Nodes:";
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

//------------------------------------------------------------------
//! \brief Paint the GUI
//------------------------------------------------------------------
bool GLImGUI::render()
{
  ImGui::SetNextTreeNodeOpen(true);
  ImGui::Begin("Hello, world!");

  if (ImGui::TreeNode("Scene graph"))
    {
      if (nullptr != m_graph)
        {
          auto const& root = m_graph->root();
          if (nullptr != root)
            {
              observeNode(*root);
            }
        }
      ImGui::TreePop();
    }
  ImGui::Separator();

  ImGui::End();
  return true;
}

//------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------
void GLExample09::onWindowSizeChanged(float const width, float const height)
{
  // Note: height is never zero !
  const float ratio = width / height;

  // Make sure the viewport matches the new window dimensions.
  glCheck(glViewport(0, 0, static_cast<int>(width), static_cast<int>(height)));

  m_prog.matrix44f("u_projection") =
    matrix::perspective(maths::radians(50.0f), ratio, 0.1f, 10.0f);
}

//------------------------------------------------------------------
//! \brief Create a cube constituing parts of robots.
//------------------------------------------------------------------
bool GLExample09::CreateCube()
{
  m_cube = std::make_shared<GLVAO>("VAO_cube");

  // Mandatory: bind VAO to program to get
  // it populated of VBOs.
  m_prog.bind(*m_cube);

  // Fill the VBO for vertices
  m_cube->vector3f("position") =
    {
      #include "geometry/cube_position.txt"
    };

  // We do not want a cube centered to (0,0,0).
  m_cube->vector3f("position") += Vector3f(0.0f, 1.0f, 0.0f);

  // Fill the VBO for texture coordiantes
  m_cube->vector2f("UV") =
    {
      #include "geometry/cube_texture.txt"
    };

  // Create the texture
  m_cube->texture2D("texID").interpolation(TextureMinFilter::LINEAR, TextureMagFilter::LINEAR);
  m_cube->texture2D("texID").wrap(TextureWrap::CLAMP_TO_EDGE);
  if (false == m_cube->texture2D("texID").load("textures/wooden-crate.jpg"))
    return false;

  return true;
}

//------------------------------------------------------------------
//! Create the cene graph: 3 robots
//------------------------------------------------------------------
bool GLExample09::setup()
{
  // Init the context of the DearIMgui library
  if (false == m_imgui.setup(*this))
    return false;

  // Enable some OpenGL states
  glCheck(glEnable(GL_DEPTH_TEST));
  glCheck(glDepthFunc(GL_LESS));
  glCheck(glDisable(GL_BLEND));
  glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  // Load from ASCII file the vertex sahder (vs) as well the fragment shader
  m_vertex_shader.fromFile("shaders/09_SceneGraph.vs");
  m_fragment_shader.fromFile("shaders/09_SceneGraph.fs");

  // Compile shader as OpenGL program. This one will instanciate all OpenGL objects for you.
  if (!m_prog.attachShaders(m_vertex_shader, m_fragment_shader).compile())
    {
      std::cerr << "failed compiling OpenGL program. Reason was '"
                << m_prog.getError() << "'" << std::endl;
      return false;
    }

  // Init shader uniforms
  m_prog.scalarf("scale") = 1.0f;
  m_prog.vector4f("color") = Vector4f(0.2f, 0.2f, 0.2f, 0.2f);
  float ratio = static_cast<float>(width()) / (static_cast<float>(height()) + 0.1f);
  m_prog.matrix44f("projection") =
    matrix::perspective(maths::radians(50.0f), ratio, 0.1f, 10000.0f);
  m_prog.matrix44f("view") =
    matrix::lookAt(Vector3f(0.0f, 10.0f, 100.0f), Vector3f(30), Vector3f(0,1,0));

  // Attach 3 robots in the scene graph. Each robot is a scene node.
  DEBUG("%s", "Create graph scene");

  // Init VAO and its VBOs.
  if (!CreateCube())
    return false;

  // Create 3 robots
  SceneNodePtr root = std::make_shared<SceneNode>(nullptr, "root");
  SceneNodePtr robot1 = std::make_shared<CubicRobot>(m_cube, "CubicRobot1");
  SceneNodePtr robot2 = std::make_shared<CubicRobot>(m_cube, "CubicRobot2");
  SceneNodePtr robot3 = std::make_shared<CubicRobot>(m_cube, "CubicRobot3");

  // Link nodes of the scene graph
  m_scenegraph.attach(root);
  root->attach(robot1);
  root->attach(robot2);
  root->attach(robot3);

  // Place robots on the scene
  robot2->position(Vector3f(30.0f, 0.0f, 0.0f));
  robot3->position(Vector3f(60.0f, 0.0f, 0.0f));

  // Show the scene graph in the GUI. Note: this method is not safe
  // against tree reorganisation. This is just for the example !
  m_imgui.observeGraph(m_scenegraph);

  // This is an example for searching a node.
  // Be careful: this is not a robust method: this function does not
  // manage nodes with duplicated identifier: it will halt on the
  // first id.
  std::string key("CubicRobot2");
  SceneNodePtr node = m_scenegraph.findNode(key);
  if (nullptr == node)
    {
      std::cout << "I did not found '" << key << "'" << std::endl;
    }
  else
    {
      std::cout << "I found node " << node << " " << node->id() << std::endl;
    }

  return true;
}

//------------------------------------------------------------------
//! Draw the scene graph (made of robots)
//------------------------------------------------------------------
bool GLExample09::draw()
{
  DEBUG("%s", "GLExample09::draw()");

  // clear everything
  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  // Traverse the scene graph for moving robots
  m_scenegraph.update(dt());

  // Traverse the scene graph for drawing robots.
  // drawScene() will be called for each node.
  m_scenegraph.drawnBy(*this);

  // Paint the GUI
  if (false == m_imgui.draw())
    return false;

  return true;
}

//------------------------------------------------------------------
//! Draw the current Scene node (= draw a part of robots)
//------------------------------------------------------------------
void GLExample09::drawSceneNode(GLVAO& vao, Matrix44f const& transform)
{
  m_prog.matrix44f("model") = transform;

  // Draw the 3D model
  m_prog.draw(vao, Mode::TRIANGLES, 0, 36);
}
