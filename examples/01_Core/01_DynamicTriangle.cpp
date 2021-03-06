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

#include "01_DynamicTriangle.hpp"
#include <iostream>

DynamicTriangle::DynamicTriangle()
  : m_prog("Prog"),
    m_mesh("VAO_triangle")
{
  std::cout << "Hello DynamicTriangle" << std::endl;
}

DynamicTriangle::~DynamicTriangle()
{
  std::cout << "Bye DynamicTriangle" << std::endl;
}

//------------------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------------------
void DynamicTriangle::onWindowSizeChanged()
{
  // Make the viewport matches the new window dimensions.
  glCheck(glViewport(0, 0, width<int>(), height<int>()));
}

//------------------------------------------------------------------------------
//! \brief Make the instropection of VAO and program shader.
//! \note more methods are avalaible in next examples.
//------------------------------------------------------------------------------
void DynamicTriangle::debug()
{
  // Display the list of VBOs hold by the VAO
  {
    std::vector<std::string> vbo_names;
    size_t count = m_mesh.getVBONames(vbo_names);
    std::cout << "VAO '" << m_mesh.name() << "' has "
              << count << " VBO: " << std::endl;
    for (auto& it: vbo_names)
      {
        std::cout << "  '" << it << "'" << std::endl;
      }
  }

  // Display the list of Uniforms hold by the program
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

//------------------------------------------------------------------------------
//! \brief Vertices of the triangle.
//------------------------------------------------------------------------------
static std::vector<Vector2f> initial_position =
{
  Vector2f(-1.0f, -1.0f), Vector2f(1.0f, -1.0f), Vector2f(0.0f, 1.0f)
};

//------------------------------------------------------------------------------
//! \brief Load vertex and fragment shaders. Create a VAO and fill its VBO
//! (position) and uniform (color).
//------------------------------------------------------------------------------
bool DynamicTriangle::setup()
{
  m_vertex_shader.fromString("#version 330 core                        \n"
                             "in vec2      position;                   \n"
                             "void main() {                            \n"
                             "  gl_Position = vec4(position, 0.0, 1.0);\n"
                             "}");
  m_fragment_shader.fromString("#version 330 core                      \n"
                               "uniform vec3 color;                    \n"
                               "out vec4 fragColor;                    \n"
                               "void main() {                          \n"
                               "  fragColor = vec4(color, 1.0);        \n"
                               "}");

  if (!m_prog.attachShaders(m_vertex_shader, m_fragment_shader).compile())
    {
      std::cerr << "Failed compiling OpenGL program. Reason was '"
                << m_prog.getError() << "'" << std::endl;
      return false;
    }

  // Init shader uniforms with a RGB color
  m_prog.vector3f("color") = Vector3f(1.0f, 0.0f, 0.0f);

  // Create VBOs inside the VAO.
  m_prog.bind(m_mesh);

  // Fill VBOs of the VAO: init triangle vertex positions.
  m_mesh.vector2f("position") = initial_position;

  // Helper for debugging states of your program
  debug();

  return true;
}

//------------------------------------------------------------------------------
//! \brief Paint our scene. Here we are using the delta time to
//------------------------------------------------------------------------------
bool DynamicTriangle::draw()
{
  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT));

  // Use time for dynamicaly change values
  static float time = 0.0f;
  time += dt();
  const float c = cosf(time);
  const float ct = c * 0.5f + 0.5f;
  const float st = sinf(time) * 0.5f + 0.5f;

  // Change vertices position over time. Dirty CPU data are transmitted to GPU.
  GLVertexBuffer<Vector2f>& position = m_mesh.vector2f("position");
  position = initial_position;
  position *= c;

  // Change color over time. Dirty CPU data are transmitted to GPU.
  m_prog.vector3f("color") = Vector3f(st, ct, 0.0f);

  // Draw the VAO bound to the shader program. Dirty data are transfered to GPU.
  m_prog.draw(m_mesh, Mode::TRIANGLES, 0, 3);

  return true;
}
