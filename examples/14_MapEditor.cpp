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

#include "14_MapEditor.hpp"

//------------------------------------------------------------------
//! \file This example shows how to create an basic map editor.
//! VAO has a reserved number of elements.
//------------------------------------------------------------------

//------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------
void GLExample14::onWindowSizeChanged()
{
  // Make sure the viewport matches the new window dimensions.
  glCheck(glViewport(0, 0, width<int>(), height<int>()));
}

//------------------------------------------------------------------
//! \brief Init your scene.
//------------------------------------------------------------------
bool GLExample14::setup()
{
  glCheck(glEnable(GL_PROGRAM_POINT_SIZE));
  glCheck(glEnable(GL_DEPTH_TEST));
  glCheck(glDepthFunc(GL_LESS));
  glCheck(glDisable(GL_BLEND));
  glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  // Load vertex and fragment shaders with GLSL code.
  m_vertex_shader.fromFile("shaders/14_MapEditor.vs");
  m_fragment_shader.fromFile("shaders/14_MapEditor.fs");

  // Compile the shader program
  if (!m_prog.attachShaders(m_vertex_shader, m_fragment_shader).compile())
    {
      std::cerr << "failed compiling OpenGL program. Reason was '"
                << m_prog.getError() << "'" << std::endl;
      return false;
    }

  // Success
  return true;
}

//------------------------------------------------------------------
//! \brief Paint our scene. Here we are using the delta time to
//! change the color of the window.
//------------------------------------------------------------------
bool GLExample14::draw()
{
  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  m_prog.vector3f("color") = Vector3f(0.0f, 1.0f, 0.0f);
  m_prog.draw(m_vao_arcs, Mode::LINES);

  m_prog.vector3f("color") = Vector3f(1.0f, 0.0f, 0.0f);
  m_prog.draw(m_vao_nodes, Mode::POINTS);

  m_prog.vector3f("color") = Vector3f(0.0f, 0.0f, 1.0f);
  m_prog.draw(m_vao_zones, Mode::LINE_STRIP);

  if (keyPressed(GLFW_KEY_A))
    {
      std::cout << "Arc mode" << std::endl;
      m_brush = Brush::Arc;
      SelectTool();
    }
  else if (keyPressed(GLFW_KEY_N))
    {
      std::cout << "Nodes mode" << std::endl;
      m_brush = Brush::Node;
      SelectTool();
    }
  else if (keyPressed(GLFW_KEY_Z))
    {
      std::cout << "Zones Mode" << std::endl;
      m_brush = Brush::Zone;
      SelectTool();
    }
  else if (keyPressed(GLFW_KEY_I))
    {
      std::cout << "Insertion mode" << std::endl;
      m_action = Action::Insertion;
      SelectTool();
    }
  else if (keyPressed(GLFW_KEY_D))
    {
      std::cout << "Deletion mode" << std::endl;
      m_action = Action::Deletion;
      SelectTool();
    }
  else if (keyPressed(GLFW_KEY_M))
    {
      std::cout << "Movine mode" << std::endl;
      m_action = Action::Moving;
      SelectTool();
    }

  // Success
  return true;
}

void GLExample14::InsertNode(float const x, float const y)
{
  m_vao_nodes.vector3f("position").append(Vector2f(x, y));
}

void GLExample14::DeleteNode(float const, float const)
{
}

void GLExample14::MoveNode(float const, float const)
{
}

void GLExample14::InsertArc(float const x, float const y)
{
  m_arcing ^= true;
  m_vao_nodes.vector3f("position").append(Vector2f(x, y));
  if (m_arcing)
    {
      m_from = Vector2f(x, y);
    }
  else
    {
      m_vao_arcs.vector3f("position").append(m_from);
      m_vao_arcs.vector3f("position").append(Vector2f(x, y));
    }
}

void GLExample14::DeleteArc(float const, float const)
{
}

void GLExample14::MoveArc(float const, float const)
{
}

void GLExample14::InsertZone(float const x, float const y)
{
  m_vao_nodes.vector3f("position").append(Vector2f(x, y));
  m_vao_zones.vector3f("position").append(Vector2f(x, y));
}

void GLExample14::DeleteZone(float const, float const)
{
}

void GLExample14::MoveZone(float const, float const)
{
}

void GLExample14::SelectTool()
{
  auto it = m_actions.find(m_action);
  if (it != m_actions.end())
    {
      auto itt = it->second.find(m_brush);
      if (itt != it->second.end())
        {
          m_execute = itt->second;
        }
    }
}

// --------------------------------------------------------------
//! \brief Mouse event
// --------------------------------------------------------------
void GLExample14::onMouseButtonPressed(Mouse const& mouse)
{
  if ((mouse.button == ButtonType::LEFT) && (mouse.pressed))
    {
      const float w = width<float>();
      const float h = height<float>();
      const float px = static_cast<float>(mouse.position.x);
      const float py = static_cast<float>(mouse.position.y);
      const float x = maths::lmap(px, 0.0f, w, -1.0f, 1.0f);
      const float y = maths::lmap(h - py, 0.0f, h, -1.0f, 1.0f);

      // Equivalent to: m_actions[m_action][m_brush](x, y);
      (this->*m_execute)(x, y);
    }
}

// --------------------------------------------------------------
//! \brief Mouse event
// --------------------------------------------------------------
void GLExample14::onMouseMoved(Mouse const& /*mouse*/)
{
}
