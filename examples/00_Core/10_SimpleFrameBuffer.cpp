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

#include "10_SimpleFrameBuffer.hpp"

//------------------------------------------------------------------
//! \file This example draw a quad in a framebuffer.
//!
//! \note This example takes its inspiration from
//! the Glumpy project gloo-framebuffer.py example.
//------------------------------------------------------------------

//------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------
void GLExample10::onWindowSizeChanged()
{
  // Make sure the viewport matches the new window dimensions.
  glCheck(glViewport(0, 0, width<int>(), height<int>()));
}

//------------------------------------------------------------------
//! \brief Init your scene.
//------------------------------------------------------------------
bool GLExample10::setup()
{
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // First program. Note use '"' and '\n' chars.
  m_vs_plane.fromString("#version 330 core                       \n"
                        "in vec2 position;                       \n"
                        "void main() {                           \n"
                        "  gl_Position = vec4(position,0.0,1.0); \n"
                        "}");
  m_fs_plane.fromString("#version 330 core                       \n"
                        "out vec4 fragColor;                     \n"
                        "void main() {                           \n"
                        "  fragColor = vec4(10,10,10,1);         \n"
                        "}");
  if (!m_prog_plane.attachShaders(m_vs_plane, m_fs_plane).compile())
    {
      std::cerr << "Failed compiling OpenGL program. Reason was '"
                << m_prog_plane.getError() << "'" << std::endl;
      return false;
    }

  // Second program. Note, here, we use C++11 raw string litterals
  // instead of concatening '"' and '\n' chars.
  m_vs_screen.fromString(R"GLSL(#version 330 core
                         in vec2 position;
                         out vec2 v_texcoord;
                         void main() {
                           gl_Position = vec4(0.85*position,0.0,1.0);
                           v_texcoord = 0.85*position;
                         })GLSL");
  m_fs_screen.fromString(R"GLSL(#version 330 core
                         uniform sampler2D screenTexture;
                         in vec2 v_texcoord;
                         out vec4 fragColor;
                         void main() {
                           vec4 color = texture(screenTexture, v_texcoord);
                           fragColor = color / vec4(20,20,20,1);
                         })GLSL");
  if (!m_prog_screen.attachShaders(m_vs_screen, m_fs_screen).compile())
    {
      std::cerr << "Failed compiling OpenGL program. Reason was '"
                << m_prog_screen.getError() << "'" << std::endl;
      return false;
    }

  // Bind VAOs
  m_prog_plane.bind(m_plane);
  m_plane.vector2f("position") =
    {
      Vector2f(-1,-1), Vector2f(-1,+1), Vector2f(+1,-1), Vector2f(+1,+1)
    };
  m_prog_screen.bind(m_screen);
  m_screen.vector2f("position") =
    {
      Vector2f(-1,-1), Vector2f(-1,+1), Vector2f(+1,-1), Vector2f(+1,+1)
    };

  // Framebuffer
  m_fbo.resize(width<uint32_t>(), height<uint32_t>());
  m_fbo.createColorTexture(m_screen.texture2D("screenTexture"));

  return true;
}

//------------------------------------------------------------------
//! \brief Paint our scene.
//------------------------------------------------------------------
bool GLExample10::draw()
{
  // First pass: draw to the framebuffer texture
  m_fbo.render(0u, 0u, width<uint32_t>(), height<uint32_t>(), [this]() {
      glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
      glCheck(glClear(GL_COLOR_BUFFER_BIT));
      m_prog_plane.draw(m_plane, Mode::TRIANGLE_STRIP, 0, 4);
    });

  // Second pass: draw to the screen
  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT));
  m_prog_screen.draw(m_screen, Mode::TRIANGLE_STRIP, 0, 4);

  return true;
}
