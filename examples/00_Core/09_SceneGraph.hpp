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

#ifndef EXAMPLE_09_SCENEGRAPH_HPP
#  define EXAMPLE_09_SCENEGRAPH_HPP

#  include <OpenGLCppWrapper/OpenGLCppWrapper.hpp>

using namespace glwrap;

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
  inline void observeGraph(Node3D_SP const& scene)
  {
    m_scene = scene;
  }

protected:

  //------------------------------------------------------------------
  //! \brief override IGLImGUI method: debug the scenegraph
  //------------------------------------------------------------------
  virtual bool render() override;

  //------------------------------------------------------------------
  //! \brief Iterate on scene nodes for their display.
  //------------------------------------------------------------------
  void observeNode(Node3D const& node);// const;

private:

  Node3D_SP m_scene = nullptr;
};

// *****************************************************************
//! \brief
// *****************************************************************
class Cube: public Node3D
{
public:

  Cube(const char *name)
    : Node3D(name),
      m_prog("GLProgram"),
      m_vao("VAO_cube")
  {
    // Load from ASCII file the vertex sahder (vs) as well the fragment shader
    m_vertex_shader.fromFile("shaders/09_SceneGraph.vs");
    m_fragment_shader.fromFile("shaders/09_SceneGraph.fs");

    // Compile shader as OpenGL program. This one will instanciate all OpenGL
    // objects for you.
    if (m_prog.attachShaders(m_vertex_shader, m_fragment_shader).compile())
      {
        // Init shader uniforms
        m_prog.vector4f("color") = Vector4f(0.2f, 0.2f, 0.2f, 0.2f);

        // Mandatory: bind VAO to program to get
        // it populated of VBOs.
        m_prog.bind(m_vao);

        // Fill the VBO for vertices
        m_vao.vector3f("position") =
          {
             #include "../geometry/cube_position.txt"
          };

        // We do not want a cube centered to (0,0,0).
        m_vao.vector3f("position") += Vector3f(0.0f, 1.0f, 0.0f);

        // Fill the VBO for texture coordiantes
        m_vao.vector2f("UV") =
          {
             #include "../geometry/cube_texture.txt"
          };

        // Create the texture
        m_vao.texture2D("texID").interpolation(TextureMinFilter::LINEAR,
                                               TextureMagFilter::LINEAR);
        m_vao.texture2D("texID").wrap(TextureWrap::CLAMP_TO_EDGE);
        m_vao.texture2D("texID").load("../textures/wooden-crate.jpg");

        float ratio = 1024.0f/728.0f;//width<float>() / height<float>();
        m_prog.matrix44f("projection") =
          matrix::perspective(maths::toRadian(60.0f), ratio, 0.1f, 10000.0f);
        m_prog.matrix44f("view") =
          matrix::lookAt(Vector3f(0.0f, 10.0f, 100.0f), Vector3f(30), Vector3f(0,1,0));
      }
    else
      {
        std::cerr << "failed compiling OpenGL program. Reason was '"
                  << m_prog.getError() << "'" << std::endl;
      }
  }

  virtual bool renderable() const override { return true; }

  virtual void renderer() override
  {
    Matrix44f transform =
      matrix::scale(m_world_transform, Transformable3D::localScale());

    m_prog.matrix44f("model") = transform;
    m_prog.draw(m_vao, Mode::TRIANGLES, 0, 36);

    Node3D::renderer();
  }

  virtual void update(float const dt)
  {
    // Update world transform matrices
    Node3D::update(dt);
  }

  static std::shared_ptr<Cube> create(const char* name)
  {
    return std::make_shared<Cube>(name);
  }

private:

  GLProgram         m_prog;
  GLVertexShader    m_vertex_shader;
  GLFragmentShader  m_fragment_shader;
  GLVAO             m_vao;
};

DECLARE_CLASS(CubicRobot)

// *****************************************************************
//! \brief a CubicRobot is an robot made of cubes. A CubicRobot is a
//! node of the scene graph. A robot is made of a head, body, legs
//! and arms which are also a node of the scene graph. To make it
//! simple we create a single 3D cube model for each nodes but feel
//! free to add more complex 3D objects.
// *****************************************************************
class CubicRobot: public Node3D
{
public:

  CubicRobot(const char *name);
  ~CubicRobot();

  static CubicRobot_SP create(const char *name)
  {
    return std::make_shared<CubicRobot>(name);
  }

  virtual void update(float const dt) override;

  virtual bool renderable() const override { return false; }

  virtual void renderer() override
  {
    Node3D::renderer();
  }

private:

  Node3D_SP m_cube;
  Node3D_SP m_body;
  Node3D_SP m_head;
  Node3D_SP m_leftArm;
  Node3D_SP m_rightArm;
  Node3D_SP m_leftLeg;
  Node3D_SP m_rightLeg;
  float radiansRotated = 0.0f;
};

// *****************************************************************
//! \brief Display a scere graphe made of 3 moving robots. Each robots
//! are nodes of the scere graph. Each element of robots is also a
//! part of the scene graph.
// *****************************************************************
class GLExample09
  : public IGLWindow
{
public:

  GLExample09()
  {}

  ~GLExample09()
  {}

private:

  virtual void onWindowSizeChanged() override;
  virtual bool setup() override;
  virtual bool draw() override;

private:

  Camera    m_camera;
  Node3D_SP m_scene;
  GLImGUI   m_imgui;
};

#endif // EXAMPLE_09_SCENEGRAPH_HPP
