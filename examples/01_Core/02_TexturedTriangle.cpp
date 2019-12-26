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

#include "02_TexturedTriangle.hpp"
#include <iostream>

TexturedTriangle::TexturedTriangle()
  : m_prog("Prog"),
    m_mesh("VAO_triangle")
{
  std::cout << "Hello TexturedTriangle" << std::endl;
}

TexturedTriangle::~TexturedTriangle()
{
  std::cout << "Bye TexturedTriangle" << std::endl;
}

//------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------
void TexturedTriangle::onWindowSizeChanged()
{
  // Make the viewport matches the new window dimensions.
  glCheck(glViewport(0, 0, width<int>(), height<int>()));
}

//------------------------------------------------------------------
//! \brief Make the instropection of VAO and program shader.
//! \note From the previous example we add sampler lists.
//------------------------------------------------------------------
void TexturedTriangle::debug()
{
  // TODO add
  // getFailedShaders, getFailedTextures (or NotLoaded), getAttributeNames

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

  // Display the list of Samplers hold by the program
  {
    std::vector<std::string> sampler_names;
    size_t count = m_prog.getSamplersNames(sampler_names);
    std::cout << "Prog '" << m_prog.name() << "' has "
              << count << " samplers: " << std::endl;
    for (auto& it: sampler_names)
      {
        // TODO "for " << it->filename()
        std::cout << "  '" << it << "'" << std::endl;
      }
  }
}

//------------------------------------------------------------------
//! \brief
//------------------------------------------------------------------
bool TexturedTriangle::loadTextures()
{
  GLTexture2D& texture = m_mesh.texture2D("texID");
  texture.interpolation(TextureMinFilter::LINEAR, TextureMagFilter::LINEAR);
  texture.wrap(TextureWrap::MIRRORED_REPEAT);
  return texture.load("textures/hazard.png");
}

//------------------------------------------------------------------
//! \brief Load vertex and fragment shaders. Create a VAO and fill
//! its VBOs (vertex and texture position). Load all textures.
//------------------------------------------------------------------
bool TexturedTriangle::setup()
{
  // Load vertex and fragment shaders with GLSL code.
  m_vertex_shader.fromFile("01_Core/shaders/02_TexturedTriangle.vs");
  m_fragment_shader.fromFile("01_Core/shaders/02_TexturedTriangle.fs");

  // Compile the shader program
  if (!m_prog.attachShaders(m_vertex_shader, m_fragment_shader).compile())
    {
      std::cerr << "Failed compiling OpenGL program. Reason was '"
                << m_prog.getError() << "'" << std::endl;
      return false;
    }

  // Create VBOs of the VAO.
  m_prog.bind(m_mesh);

  // Fill VBOs of the VAO: init triangle vertex positions.
  m_mesh.vector3f("position") =
    {
      Vector3f(-1.0f, -1.0f, 0.0f),
      Vector3f(1.0f, -1.0f, 0.0f),
      Vector3f(0.0f, 1.0f, 0.0f)
    };

  // Fill VBOs of the VAO: init triangle texture positions.
  m_mesh.vector2f("UV") =
    {
      Vector2f(0.0f, 0.0f),
      Vector2f(1.0f, 0.0f),
      Vector2f(0.5f, 1.0f)
    };

  // Repeat the texture motif
  m_mesh.vector2f("UV") *= 4.0f;

  // Fill Load texture files
  if (!loadTextures())
    {
      // In case of failure show which textures has failed.
      std::vector<std::string> list;
      size_t count = m_mesh.getFailedTextures(list);
      std::cerr << "Failed loading " << count << " textures:" << std::endl;
      for (auto& it: list)
        {
          std::cerr << " " << it;
        }
      std::cerr << std::endl;
      return false;
    }

  // Helper for debugging states of your program
  debug();

  return true;
}

//------------------------------------------------------------------
//! \brief Paint our scene. Here we are using the delta time to
//------------------------------------------------------------------
bool TexturedTriangle::draw()
{
  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT));

  m_prog.draw(m_mesh, Mode::TRIANGLES, 0, 3);

  return true;
}
