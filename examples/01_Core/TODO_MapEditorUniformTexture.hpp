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

#ifndef EXAMPLE_14_MAP_EDITOR_HPP
#  define EXAMPLE_14_MAP_EDITOR_HPP

#  include <OpenGLCppWrapper/OpenGLCppWrapper.hpp>
#  include <iostream>
#  include <map>

using namespace glwrap;
using namespace glwrap::window;

class GLExample14: public IGLWindow
{
public:

  GLExample14()
    : m_vao_nodes("VAO_nodes"),
      m_vao_arcs("VAO_arcs"),
      m_vao_zones("VAO_zones"),
      m_prog("Prog", 1024_z),
      m_from(0.0f, 0.0f)
  {}

  ~GLExample14()
  {}

protected:

  typedef void(GLExample14::*execute_t)(float, float);

  enum class Brush { Node, Arc, Zone };
  enum class Action { Insertion, Deletion, Moving };

  virtual void onMouseButtonPressed(Mouse const& mouse) override;
  virtual void onMouseMoved(Mouse const& mouse) override;
  virtual void onKeyboardEvent() override;
  virtual void onWindowSizeChanged() override;
  virtual bool setup() override;
  virtual bool draw() override;

  void SelectTool();
  //void NoAction(float const, float const) { }
  void InsertNode(float const x, float const y);
  void DeleteNode(float const x, float const y);
  void MoveNode(float const x, float const y);
  void InsertArc(float const x, float const y);
  void DeleteArc(float const x, float const y);
  void MoveArc(float const x, float const y);
  void InsertZone(float const x, float const y);
  void DeleteZone(float const x, float const y);
  void MoveZone(float const x, float const y);

private:

  Brush              m_brush = Brush::Node;
  Action             m_action = Action::Insertion;
  Mouse              m_mouse;
  GLVertexShader     m_vertex_shader;
  GLFragmentShader   m_fragment_shader;
  GLVAO              m_vao_nodes;
  GLVAO              m_vao_arcs;
  GLVAO              m_vao_zones;
  GLProgram          m_prog;

  execute_t          m_execute = &GLExample14::InsertNode;
  bool               m_arcing = false;
  //bool               m_zoning = false;
  Vector2f           m_from;

  std::map<Action, std::map<Brush, execute_t>> m_actions =
  {
    { Action::Insertion,
      {
        { Brush::Node, &GLExample14::InsertNode },
        { Brush::Arc,  &GLExample14::InsertArc  },
        { Brush::Zone, &GLExample14::InsertZone },
      }
    },
    { Action::Deletion,
      {
        { Brush::Node, &GLExample14::DeleteNode },
        { Brush::Arc,  &GLExample14::DeleteArc  },
        { Brush::Zone, &GLExample14::DeleteZone },
      }
    },
    { Action::Moving,
      {
        { Brush::Node, &GLExample14::MoveNode },
        { Brush::Arc,  &GLExample14::MoveArc  },
        { Brush::Zone, &GLExample14::MoveZone },
      }
    }
  };
};

#endif // EXAMPLE_14_MAP_EDITOR_HPP
