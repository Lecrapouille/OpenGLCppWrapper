#include "Example02.hpp"

//------------------------------------------------------------------
//! \file Display a scere graphe made of 3 moving robots. Each robots
//! are nodes of the scere graph. Each element of robots is also a
//! part of the scene graph.
//------------------------------------------------------------------

CubicRobot::CubicRobot(const char *name, GLVAO& cube)
  : SceneNode(nullptr, name)
{
  LOGI("Cstr CubicRobot");

  // Body
  m_body = new SceneNode(cube, "Body");
  m_body->localScale(Vector3f(10.0f, 15.0f, 5.0f));
  m_body->position(Vector3f(0.0f, 35.0f, 0.0f));
  add(*m_body);

  // Head
  m_head = new SceneNode(cube, "Head");
  m_head->localScale(Vector3f(5.0f));
  m_head->position(Vector3f(0.0f, 30.0f, 0.0f));
  m_body->add(*m_head);

  // Left arm
  m_leftArm = new SceneNode(cube, "Left Arm");
  m_leftArm->localScale(Vector3f(3.0f, -18.0f, 3.0f));
  m_leftArm->position(Vector3f(-12.0f, 30.0f, -1.0f));
  m_body->add(*m_leftArm);

  // Right arm
  m_rightArm = new SceneNode(cube, "Right Arm");
  m_rightArm->localScale(Vector3f(3.0f, -18.0f, 3.0f));
  m_rightArm->position(Vector3f(12.0f, 30.0f, -1.0f));
  m_body->add(*m_rightArm);

  // Left leg
  m_leftLeg = new SceneNode(cube, "Left Leg");
  m_leftLeg->localScale(Vector3f(3.0f, -17.5f, 3.0f));
  m_leftLeg->position(Vector3f(-8.0f, 0.0f, 0.0f));
  m_body->add(*m_leftLeg);

  // Right leg
  m_rightLeg = new SceneNode(cube, "Right Leg");
  m_rightLeg->localScale(Vector3f(3.0f, -17.5f, 3.0f));
  m_rightLeg->position(Vector3f(8.0f, 0.0f, 0.0f));
  m_body->add(*m_rightLeg);
}

//------------------------------------------------------------------
//! \brief Paint the GUI
//------------------------------------------------------------------
void GLImGUI::drawNode(SceneNode_t &node)
{
  if (ImGui::TreeNode(node.m_name.c_str()))
    {
      if (nullptr != node.mesh())
        {
          std::string name("Mesh '" + node.mesh()->name() + "'");
          ImGui::Text(name.c_str());
        }
      else
        {
          ImGui::Text("No Mesh");
        }

      if (ImGui::TreeNode("Transf. Matrix"))
        {
          std::stringstream ss;
          ss << node.worldTransform();
          ImGui::Text(ss.str().c_str());
          ImGui::TreePop();
        }

      if (ImGui::TreeNode("Child"))
        {
          std::vector<SceneNode_t*> const &children = node.children();
          for (auto i: children)
            {
              drawNode(*i);
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
  ImGui::Begin("Hello, world!");

  if (ImGui::TreeNode("Scene graph"))
    {
      if (nullptr != m_root)
        {
          drawNode(*m_root);
        }
      ImGui::TreePop();
    }
  ImGui::Separator();

  ImGui::End();
  return true;
}

//------------------------------------------------------------------
//! \brief Move part of the body of robots
//------------------------------------------------------------------
void CubicRobot::update(float const dt)
{
  LOGI("Robot::update");

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
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------
void GLExample02::onWindowSizeChanged(const float width, const float height)
{
  // Note: height is never zero !
  float ratio = width / height;

  m_prog.uniform<Matrix44f>("u_projection") =
    matrix::perspective(maths::radians(50.0f), ratio, 0.1f, 10.0f);
}

//------------------------------------------------------------------
//! \brief Create three robots and add them in the graph scene.
//------------------------------------------------------------------
GLExample02::GLExample02()
  : m_cube("VAO_cube"),
    m_prog("GLProgram")
{
}

GLExample02::~GLExample02()
{
  LOGD("---------------- quit -----------------");
  std::cout << "Bye" << std::endl;
  delete m_root;
}

void GLExample02::CreateCube()
{
  // Mandatory: bind VAO to program to get
  // it populated of VBOs.
  m_prog.bind(m_cube);

  // Fill the VBO for vertices
  m_prog.attribute<Vector3f>("a_position") =
    {
      //  X     Y     Z

      // bottom
      Vector3f(-1.0f,-1.0f,-1.0f),
      Vector3f( 1.0f,-1.0f,-1.0f),
      Vector3f(-1.0f,-1.0f, 1.0f),
      Vector3f( 1.0f,-1.0f,-1.0f),
      Vector3f( 1.0f,-1.0f, 1.0f),
      Vector3f(-1.0f,-1.0f, 1.0f),

      // top
      Vector3f(-1.0f, 1.0f,-1.0f),
      Vector3f(-1.0f, 1.0f, 1.0f),
      Vector3f( 1.0f, 1.0f,-1.0f),
      Vector3f( 1.0f, 1.0f,-1.0f),
      Vector3f(-1.0f, 1.0f, 1.0f),
      Vector3f( 1.0f, 1.0f, 1.0f),

      // front
      Vector3f(-1.0f,-1.0f, 1.0f),
      Vector3f( 1.0f,-1.0f, 1.0f),
      Vector3f(-1.0f, 1.0f, 1.0f),
      Vector3f( 1.0f,-1.0f, 1.0f),
      Vector3f( 1.0f, 1.0f, 1.0f),
      Vector3f(-1.0f, 1.0f, 1.0f),

      // back
      Vector3f(-1.0f,-1.0f,-1.0f),
      Vector3f(-1.0f, 1.0f,-1.0f),
      Vector3f( 1.0f,-1.0f,-1.0f),
      Vector3f( 1.0f,-1.0f,-1.0f),
      Vector3f(-1.0f, 1.0f,-1.0f),
      Vector3f( 1.0f, 1.0f,-1.0f),

      // left
      Vector3f(-1.0f,-1.0f, 1.0f),
      Vector3f(-1.0f, 1.0f,-1.0f),
      Vector3f(-1.0f,-1.0f,-1.0f),
      Vector3f(-1.0f,-1.0f, 1.0f),
      Vector3f(-1.0f, 1.0f, 1.0f),
      Vector3f(-1.0f, 1.0f,-1.0f),

      // right
      Vector3f(1.0f,-1.0f, 1.0f),
      Vector3f(1.0f,-1.0f,-1.0f),
      Vector3f(1.0f, 1.0f,-1.0f),
      Vector3f(1.0f,-1.0f, 1.0f),
      Vector3f(1.0f, 1.0f,-1.0f),
      Vector3f(1.0f, 1.0f, 1.0f)
    };

  // Contrary to Example01, when I made the
  // first version of the SceneGraph example
  // the cube was not centered. So let see
  // how to translate it.
  m_prog.attribute<Vector3f>("a_position")
    += Vector3f(0.0f, 1.0f, 0.0f);

  // Fill the VBO for texture coordiantes
  m_prog.attribute<Vector2f>("a_texcoord") =
    {
      //  U     V

      // bottom
      Vector2f(0.0f, 0.0f),
      Vector2f(1.0f, 0.0f),
      Vector2f(0.0f, 1.0f),
      Vector2f(1.0f, 0.0f),
      Vector2f(1.0f, 1.0f),
      Vector2f(0.0f, 1.0f),

      // top
      Vector2f(0.0f, 0.0f),
      Vector2f(0.0f, 1.0f),
      Vector2f(1.0f, 0.0f),
      Vector2f(1.0f, 0.0f),
      Vector2f(0.0f, 1.0f),
      Vector2f(1.0f, 1.0f),

      // front
      Vector2f(1.0f, 0.0f),
      Vector2f(0.0f, 0.0f),
      Vector2f(1.0f, 1.0f),
      Vector2f(0.0f, 0.0f),
      Vector2f(0.0f, 1.0f),
      Vector2f(1.0f, 1.0f),

      // back
      Vector2f(0.0f, 0.0f),
      Vector2f(0.0f, 1.0f),
      Vector2f(1.0f, 0.0f),
      Vector2f(1.0f, 0.0f),
      Vector2f(0.0f, 1.0f),
      Vector2f(1.0f, 1.0f),

      // left
      Vector2f(0.0f, 1.0f),
      Vector2f(1.0f, 0.0f),
      Vector2f(0.0f, 0.0f),
      Vector2f(0.0f, 1.0f),
      Vector2f(1.0f, 1.0f),
      Vector2f(1.0f, 0.0f),

      // right
      Vector2f(1.0f, 1.0f),
      Vector2f(1.0f, 0.0f),
      Vector2f(0.0f, 0.0f),
      Vector2f(1.0f, 1.0f),
      Vector2f(0.0f, 0.0f),
      Vector2f(0.0f, 1.0f)
    };
}

//------------------------------------------------------------------
//! Create the cene graph: 3 robots
//------------------------------------------------------------------
bool GLExample02::setup()
{
  LOGI("GLExample02::setup()");

  // Init the context of the DearIMgui library
  if (false == m_gui.setup(*this))
    return false;

  // Enable some OpenGL states
  glCheck(glEnable(GL_DEPTH_TEST));
  glCheck(glDepthFunc(GL_LESS));
  glCheck(glDisable(GL_BLEND));
  glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  // Load from ASCII file the vertex sahder (vs) as well the fragment shader
  vs.fromFile("shaders/Example01.vertex");
  fs.fromFile("shaders/Example01.fragment");

  // Compile shader as OpenGL program. This one will instanciate all OpenGL objects for you.
  if (!m_prog.attachShaders(vs, fs).compile())
    {
      std::cerr << "failed compiling OpenGL program. Reason was '"
                << m_prog.error() << "'" << std::endl;
      return false;
    }

  // Create the texture
  m_prog.uniform<GLTexture2D>("u_texture").interpolation(GL_LINEAR);
  m_prog.uniform<GLTexture2D>("u_texture").wrapping(GL_CLAMP_TO_EDGE);
  if (false == m_prog.uniform<GLTexture2D>("u_texture").load("wooden-crate.jpg"))
    return false;

  // Projection matrices
  float ratio = static_cast<float>(width()) / (static_cast<float>(height()) + 0.1f);
  m_prog.uniform<Matrix44f>("u_projection") =
    matrix::perspective(maths::radians(50.0f), ratio, 0.1f, 10000.0f);
  m_prog.uniform<Matrix44f>("u_view") =
    matrix::lookAt(Vector3f(0.0f, 10.0f, 100.0f), Vector3f(30), Vector3f(0,1,0));

  // Uniforms from the Example01
  m_prog.uniform<float>("u_scale") = 1.0f;
  m_prog.uniform<Vector4f>("u_color") = Vector4f(0.2f, 0.2f, 0.2f, 0.2f);

  // Init VAO and its VBOs.
  CreateCube();

  // Create 3 nodes of the scene graph: 3 robots
  LOGD("Create graph scene");
  m_root = new SceneNode_t("Root");
  m_robot1 = new CubicRobot("CubicRobot1", m_cube);
  m_robot2 = new CubicRobot("CubicRobot2", m_cube);
  m_robot3 = new CubicRobot("CubicRobot3", m_cube);

  // Link nodes of the scene graph
  m_root->add(*m_robot1);
  m_root->add(*m_robot2);
  m_root->add(*m_robot3);

  // Place robots on the scene
  m_robot2->position(Vector3f(30.0f, 0.0f, 0.0f));
  m_robot3->position(Vector3f(60.0f, 0.0f, 0.0f));

  // Show the scene graph in the GUI. Note: this method is not safe
  // against tree reorganisation. This is just for the example !
  m_gui.setNode(*m_root);
  return true;
}

//------------------------------------------------------------------
//! Draw recursively a node from a scene graph
//------------------------------------------------------------------
void GLExample02::drawNode(SceneNode_t &node)
{
  LOGI("Renderer:drawNode '%s'", node.m_name.c_str());

  // Get the 3D model from the node
  GLVAO* vao = node.mesh();

  // 3D models are optional, so do not forget to check against nullptr
  if (nullptr != vao)
    {
      // Apply the matrix making 3D objects moving along the scene
      Matrix44f transform = matrix::scale(node.worldTransform(), node.localScale());
      m_prog.uniform<Matrix44f>("u_model") = transform;

      // Draw the 3D model
      m_prog.bind(*vao);
      m_prog.draw(GL_TRIANGLES, 0, 36);
    }

  // FIXME: This code should not be here but in SceneGraph algorithm
  // TODO: make this code not recursive
  // Recursive iteration of the graph for drawing other node
  std::vector<SceneNode_t*> const &children = node.children();
  for (auto i: children)
    {
      drawNode(*i);
    }
 }

//------------------------------------------------------------------
//! Draw scene graph (made of robots)
//------------------------------------------------------------------
bool GLExample02::draw()
{
  LOGI("GLExample02::draw()");

  if (nullptr == m_root)
    return false;

  // clear everything
  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  // Move robots
  m_root->update(dt());

  // Draw robots
  drawNode(*m_root);

  // Paint the GUI
  if (false == m_gui.draw())
    return false;

  return true;
}
