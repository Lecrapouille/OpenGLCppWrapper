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

#include "00_SimpleTriangle.hpp"
#include <iostream>

//------------------------------------------------------------------------------
// \brief GLObjects instance have a name. This name is usually used for logs and
// debug purpose such as GL*Shader, GLProgram, GLVAO, GLTexture* but some
// GLObjects have name used as key for hash maps. In this case these instance
// are internally (privately used) and their name managed automatically such
// GLVBO, GLAttributes, GLSamplers).
//------------------------------------------------------------------------------
SimpleTriangle::SimpleTriangle()
  : m_prog("Prog"),
    m_mesh("VAO_triangle")
{
  std::cout << "Hello SimpleTriangle" << std::endl;
}

SimpleTriangle::~SimpleTriangle()
{
  std::cout << "Bye SimpleTriangle" << std::endl;
}

//------------------------------------------------------------------------------
//! \brief Load vertex and fragment shaders. Create a VAO and fill
//! its VBO (position).
//------------------------------------------------------------------------------
bool SimpleTriangle::setup()
{
  // Load vertex and fragment shaders (GLSL code) as string. There are two type
  // of strings: -- classic string but you need to add explicitly chars such as
  // '"' '\n' -- C++11 raw string litterals avoid to add '\n' chars. In this
  // function, we will show the two ways. In next examples you will see how to
  // load shader from file.

  // Load vertex shader from a classic string.
  m_vertex_shader.fromString("#version 330 core                        \n"
                             "in vec2      position;                   \n"
                             "void main() {                            \n"
                             "  gl_Position = vec4(position, 0.0, 1.0);\n"
                             "}");

  // Load fragment shader from C++11 raw string.
  m_fragment_shader.fromString(R"GLSL(#version 330 core
                               out vec4 fragColor;
                               void main() {
                                 fragColor = vec4(1, 0, 0, 1);
                               })GLSL");

  // You have to compile and link shaders into the class GLProgram. If shaders
  // contained syntax errors the compile() will return false and errors can be
  // shown with gerError(). If your OpenGL has been compiled with success, the
  // GLProgram will create its own list of shader uniforms, attributes, and
  // samplers from shaders. In this example, shader only use the attribute
  // position.
  if (!m_prog.attachShaders(m_vertex_shader, m_fragment_shader).compile())
    {
      std::cerr << "Failed compiling OpenGL program. Reason was '"
                << m_prog.getError() << "'" << std::endl;
      return false;
    }

  // To initialize your 3D model, you have to use a VAO and bind it to the
  // desired GLProgram. This last, helped with its internal list of shader
  // attributes and samplers, will populate VBOs and textures inside the VAO
  // instance. Shader attributes and samplers give their name to VBOs and
  // textures. In our example, the VAO will have a single VBO named position.
  m_prog.bind(m_mesh);

  // Fill VBOs of the VAO: init triangle vertex positions. Note "position" shall
  // refer to the variable position in the GLSL code. If you rename it in the
  // sahder you will have to rename here too. Beware name are case sensitive.
  m_mesh.vector2f("position") =
    { Vector2f(-1.0f, -1.0f), Vector2f(1.0f, -1.0f), Vector2f(0.0f, 1.0f) };

  return true;
}

//------------------------------------------------------------------------------
//! \brief Paint our scene. Here we are using the delta time to
//------------------------------------------------------------------------------
bool SimpleTriangle::draw()
{
  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT));

  // Render a triangle from 3 vertices, starting from the first (0th).
  m_prog.draw(m_mesh, Mode::TRIANGLES, 0, 3);

  return true;
}
