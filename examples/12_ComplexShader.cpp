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

#include "12_ComplexShader.hpp"
#include "Maths.hpp"

void GLExample12::onWindowSizeChanged(const float width, const float height)
{
  // Make sure the viewport matches the new window dimensions.
  glCheck(glViewport(0, 0, static_cast<int>(width), static_cast<int>(height)));
}

//------------------------------------------------------------------
//! \brief Initialize shader and create a quad.
//------------------------------------------------------------------
bool GLExample12::setup()
{
  // Load vertex and fragment shaders with GLSL code.
  m_vertex_shader.fromFile("shaders/12_ComplexShader.vs");
  m_fragment_shader.fromFile("shaders/12_ComplexShader.fs");

  // Compile the shader program
  if (!m_prog.attachShaders(m_vertex_shader, m_fragment_shader).compile())
    {
      std::cerr << "failed compiling OpenGL program. Reason was '"
                << m_prog.getError() << "'" << std::endl;
      return false;
    }

  // Create a quad.
  m_prog.bind(m_quad);

  // Fill VBOs of the VAO: init quad vertex positions.
  m_quad.vector3f("position") =
    {
      Vector2f(1.0f, 1.0f), Vector2f(1.0f, -1.0f),
      Vector2f(-1.0f, -1.0f), Vector2f(-1.0f, 1.0f)
    };

  // Fill VBOs of the VAO: init texture positions.
  m_quad.vector2f("uv") =
    {
      Vector2f(1.0f, 1.0f), Vector2f(1.0f, 0.0f),
      Vector2f(0.0f, 0.0f), Vector2f(0.0f, 1.0f)
    };

  // Vertices index of the quad
  m_quad.index32() =
    {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
    };

  settings();

  return true;
}

//------------------------------------------------------------------
//! \brief Initialize seetings of the shader.
//------------------------------------------------------------------
void GLExample12::settings()
{
  m_prog.vector3f("color") = Vector3f(1.0f, 1.0f, 1.0f);
  m_prog.scalarf("speed") = 0.0001f;
  m_prog.scalarf("brightness") = 0.0018f;
  m_prog.scalarf("distfading") = 0.7f;
  m_prog.scalarf("twinkleSpeed") = 200.0f;
}

//------------------------------------------------------------------
//! \brief Paint our scene. Here we are using the delta time to
//------------------------------------------------------------------
bool GLExample12::draw()
{
  static float time = 0.0f;
  time += dt();

  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT));

  m_prog.scalarf("time") = time;
  m_prog.draw(m_quad, Mode::TRIANGLES, m_quad.index32());

  return true;
}
