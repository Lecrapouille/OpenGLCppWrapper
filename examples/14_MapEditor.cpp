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

namespace utils
{
  template<class T>
  static T map(T const value, T const start1, T const stop1, T const start2, T const stop2)
  {
    T mapped = start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
    return mapped;
  }
} // namespace utils

//------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------
void GLExample14::onWindowSizeChanged(float const width, float const height)
{
  // Note: height is never zero !
  float ratio = width / height;

  // Make sure the viewport matches the new window dimensions.
  glCheck(glViewport(0, 0, static_cast<int>(width), static_cast<int>(height)));

  std::cout << "New Window dimension "
    << static_cast<int>(width) << " x " << static_cast<int>(height)
    << ". Ratio is " << ratio << std::endl;
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
  m_vertex_shader.fromFile("shaders/13_MapEditor.vs");
  m_fragment_shader.fromFile("shaders/13_MapEditor.fs");

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
void GLExample14::onMouseButtonPressed(int const button, int const action)
{
  if ((button == GLFW_MOUSE_BUTTON_LEFT) && (action == GLFW_PRESS))
    {
      float x = utils::map(m_mouse.x, 0.0f, static_cast<float>(width()), -1.0f, 1.0f);
      float y = utils::map(static_cast<float>(height()) - m_mouse.y, 0.0f, static_cast<float>(height()), -1.0f, 1.0f);

      // Equivalent to: m_actions[m_action][m_brush](x, y);
      (this->*m_execute)(x, y);
    }
}

// --------------------------------------------------------------
//! \brief Mouse event
// --------------------------------------------------------------
void GLExample14::onMouseMoved(double const xpos, double const ypos)
{
  m_mouse.x = static_cast<float>(xpos);
  m_mouse.y = static_cast<float>(ypos);
}
