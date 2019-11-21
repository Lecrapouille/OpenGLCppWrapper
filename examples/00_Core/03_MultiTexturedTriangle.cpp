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

#include "03_MultiTexturedTriangle.hpp"

//------------------------------------------------------------------
//! \file This example shows how to create a simple multi-textured
//! triangle. This method allows to create terrain textures.
//!
//! \note This example takes its inspiration from the Youtube video
//! "OpenGL 3D Game Tutorial 17: Multitexturing" made by ThinMatrix
//! https://www.youtube.com/watch?v=-kbal7aGUpk&list=PLRIWtICgwaX0u7Rf9zkZhLoLuZVfUksDP&index=17
//------------------------------------------------------------------

//------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------
void GLExample03::onWindowSizeChanged()
{
  // Make sure the viewport matches the new window dimensions.
  glCheck(glViewport(0, 0, width<int>(), height<int>()));
}

//------------------------------------------------------------------
//! \brief Make the instropection of VAO and program shader
//------------------------------------------------------------------
void GLExample03::debug()
{
  // VBOs of the VAO
  {
    std::vector<std::string> vbos = m_triangle.VBONames();
    std::cout << "VAO '" << m_triangle.name() << "' has "
              << vbos.size() << " VBO: " << std::endl;
    for (auto& it: vbos)
      {
        std::cout << "  '" << it << "'" << std::endl;
      }
  }

  // Uniforms of the program
  {
    std::vector<std::string> uniforms = m_prog.getUniformNames();
    std::cout << "Prog '" << m_prog.name() << "' has "
              << uniforms.size() << " uniforms: " << std::endl;
    for (auto& it: uniforms)
      {
        std::cout << "  '" << it << "'" << std::endl;
      }
  }

  // Textures samplers
  {
    std::vector<std::string> samplers = m_prog.getSamplersNames();
    std::cout << "Prog '" << m_prog.name() << "' has "
              << samplers.size() << " samplers: " << std::endl;
    for (auto& it: samplers)
      {
        std::cout << "  '" << it << "'" << std::endl;
      }
  }
}

//------------------------------------------------------------------
//! \brief Load vertex and fragment shaders. Create a VAO and fill
//! its VBOs (vertex and texture position). Load all textures.
//------------------------------------------------------------------
bool GLExample03::setup()
{
  // Load vertex and fragment shaders with GLSL code.
  m_vertex_shader.fromFile("shaders/03_MultiTexturedTriangle.vs");
  m_fragment_shader.fromFile("shaders/03_MultiTexturedTriangle.fs");

  // Compile the shader program
  if (!m_prog.attachShaders(m_vertex_shader, m_fragment_shader).compile())
    {
      std::cerr << "failed compiling OpenGL program. Reason was '"
                << m_prog.getError() << "'" << std::endl;
      return false;
    }

  // Create VBOs of the VAO.
  m_prog.bind(m_triangle);
  debug();

  // Fill VBOs of the VAO: init triangle vertex positions.
  m_triangle.vector3f("position") =
    {
      Vector3f(-1.0f, -1.0f, 0.0f), Vector3f(1.0f, -1.0f, 0.0f), Vector3f(0.0f, 1.0f, 0.0f)
    };

  // Fill VBOs of the VAO: init triangle texture positions.
  m_triangle.vector2f("UV") =
    {
      Vector2f(0.0f, 0.0f), Vector2f(1.0f, 0.0f), Vector2f(0.5f, 1.0f)
    };

  // Fill textures
  if (!m_triangle.texture2D("blendMap").load("../textures/blendMap.png")) return false;
  if (!m_triangle.texture2D("backgroundTexture").load("../textures/grassy2.png")) return false;
  if (!m_triangle.texture2D("rTexture").load("../textures/mud.png")) return false;
  if (!m_triangle.texture2D("gTexture").load("../textures/grassFlowers.png")) return false;
  if (!m_triangle.texture2D("bTexture").load("../textures/path.png")) return false;

  return true;
}

//------------------------------------------------------------------
//! \brief Paint our scene. Here we are using the delta time to
//------------------------------------------------------------------
bool GLExample03::draw()
{
  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT));

  m_prog.draw(m_triangle, Mode::TRIANGLES, 0, 3);

  return true;
}
