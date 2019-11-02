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

#include "02_DynamicTriangle.hpp"

//------------------------------------------------------------------
//! \file This example shows how to create a simple colorful triangle
//! and how we can modify VBO data from CPU and how dirty data are
//! automatically transfered to GPU.
//------------------------------------------------------------------

static std::vector<Vector2f> initial_position =
{
  Vector2f(-1.0f, -1.0f), Vector2f(1.0f, -1.0f), Vector2f(0.0f, 1.0f)
};

//------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------
void GLExample02::onWindowSizeChanged()
{
  // Make sure the viewport matches the new window dimensions.
  glCheck(glViewport(0, 0, width<int>(), height<int>()));
}

//------------------------------------------------------------------
//! \brief Make the instropection of VAO and program shader
//------------------------------------------------------------------
void GLExample02::debug()
{
  // VBOs of the VAO
  {
    std::vector<std::string> vbo_names;
    size_t count = m_triangle.getVBONames(vbo_names);
    std::cout << "VAO '" << m_triangle.name() << "' has "
              << count << " VBO: " << std::endl;
    for (auto& it: vbo_names)
      {
        std::cout << "  '" << it << "'" << std::endl;
      }
  }

  // Uniforms of the program
  {
    std::vector<std::string> uniform_names;
    size_t count = m_prog.getUniformNames(uniform_names);
    std::cout << "Prog '" << m_prog.name() << "' has "
              << count << " uniforms: " << std::endl;
    for (auto& it: uniform_names)
      {
        std::cout << "  '" << it << "'" << std::endl;
      }
  }
}

//------------------------------------------------------------------
//! \brief Load vertex and fragment shaders. Create a VAO and fill
//! its VBO (position).
//------------------------------------------------------------------
bool GLExample02::setup()
{
  // Load vertex and fragment shaders with GLSL code. Note, you can
  // use C++11 raw string litterals instead of concatening '"' and
  // '\n' chars. Like fromString(R"GLSL(#version 330 core ... })GLSL");
  m_vertex_shader.fromString("#version 330 core                        \n"
                             "in vec2      position;                   \n"
                             "void main() {                            \n"
                             "  gl_Position = vec4(position, 0.0, 1.0);\n"
                             "}");
  m_fragment_shader.fromString("#vversion 330 core                      \n"
                               "uniform vec3 color;                    \n"
                               "out vec4 fragColor;                    \n"
                               "void main() {                          \n"
                               "  fragColor = vec4(color, 1.0);        \n"
                               "}");

  // Compile the shader program
  if (!m_prog.attachShaders(m_vertex_shader, m_fragment_shader).compile())
    {
      std::cerr << m_prog.getError() << std::endl;
      return false;
    }

  // Init shader uniforms: Global RGB color
  m_prog.vector3f("color") = Vector3f(1.0f, 0.0f, 0.0f);

  // Create VBOs of the VAO.
  m_prog.bind(m_triangle);
  debug();

  // Fill VBOs of the VAO: init triangle vertex positions.
  m_triangle.vector2f("position") = initial_position;

  return true;
}

//------------------------------------------------------------------
//! \brief Paint our scene. Here we are using the delta time to
//------------------------------------------------------------------
bool GLExample02::draw()
{
  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT));

  static float time = 0.0f;
  time += dt();
  const float c = cosf(time);
  const float ct = c * 0.5f + 0.5f;
  const float st = sinf(time) * 0.5f + 0.5f;

  // Change vertices position: dirty CPU data are transmitted to GPU.
  GLVertexBuffer<Vector2f>& position = m_triangle.vector2f("position");
  position = initial_position;
  position *= c;

  // Change color over time.
  m_prog.vector3f("color") = Vector3f(st, ct, 0.0f);

  // Draw the VAO bound to the shader program. Dirty data are
  // transfered to GPU.
  m_prog.draw(m_triangle, Mode::TRIANGLES, 0, 3);

  return true;
}
