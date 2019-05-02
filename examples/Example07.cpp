#include "Example07.hpp"
#include <math.h>

//------------------------------------------------------------------
//! \file this example draw a quad in a framebuffer. Inspired by
//! the Glumpy gloo-framebuffer.py example.
//------------------------------------------------------------------

//------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------
void GLExample07::onWindowSizeChanged(const float width, const float height)
{
  // Make sure the viewport matches the new window dimensions; note that width and
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

//------------------------------------------------------------------
//! \brief Init your scene.
//------------------------------------------------------------------
bool GLExample07::setup()
{
  glCheck(glEnable(GL_DEPTH_TEST));
  glCheck(glDepthFunc(GL_LESS));
  glCheck(glDisable(GL_BLEND));
  glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
  glCheck(glClearColor(0.0f, 0.0f, 1.0f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  // First program
  m_vs_plane.fromString("#version 330 core\n in vec2 position; void main() { gl_Position = vec4(position,0.0,1.0); }");
  m_fs_plane.fromString("#version 330 core\n void main() { gl_FragColor = vec4(10,10,10,1); }");
  if (!m_prog_plane.attachShaders(m_vs_plane, m_fs_plane).compile())
    {
      std::cerr << "Failed compiling OpenGL program. Reason was '"
                << m_prog_plane.error() << "'" << std::endl;
      return false;
    }

  // Second program
  m_vs_screen.fromString("#version 330 core\n in vec2 position; out vec2 v_texcoord;\n"
"void main() { gl_Position = vec4(0.85*position,0.0,1.0); v_texcoord = 0.85*position; }");
  m_fs_screen.fromString("#version 330 core\n uniform sampler2D texture; in vec2 v_texcoord; out vec4 fragColor;\n"
"void main() { vec4 color = texture2D(texture, v_texcoord); fragColor = color / vec4(20,20,20,1); }");

  if (!m_prog_screen.attachShaders(m_vs_screen, m_fs_screen).compile())
    {
      std::cerr << "Failed compiling OpenGL program. Reason was '"
                << m_prog_screen.error() << "'" << std::endl;
      return false;
    }

  // Bind VAOs
  m_prog_plane.bind(m_plane);
  m_plane.vector2f("position") = { Vector2f(-1,-1), Vector2f(-1,+1), Vector2f(+1,-1), Vector2f(+1,+1) };
  m_prog_screen.bind(m_screen);
  m_screen.vector2f("position") = { Vector2f(-1,-1), Vector2f(-1,+1), Vector2f(+1,-1), Vector2f(+1,+1) };

  // Framebuffer
  m_fbo.resize(width(), height());
  m_screen.texture2D("texture") = m_fbo.createColorTexture();

  return true;
}

//------------------------------------------------------------------
//! \brief Paint our scene.
//------------------------------------------------------------------
bool GLExample07::draw()
{
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  // First pass: draw to the framebuffer texture
  m_fbo.render(0, 0, width(), height(), [this]() {
      glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
      m_prog_plane.draw(m_plane, Primitive::TRIANGLE_STRIP, 0, 4);
    });

  // Second pass: draw to the screen
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
  m_prog_screen.draw(m_screen, Primitive::TRIANGLE_STRIP, 0, 4);

  return true;
}
