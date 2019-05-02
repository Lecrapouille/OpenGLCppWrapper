#include "Example08.hpp"
#include <math.h>

//------------------------------------------------------------------
//! \file this example show a simple framebuffer example of a cube
//------------------------------------------------------------------

//------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------
void GLExample08::onWindowSizeChanged(const float width, const float height)
{
  // Note: height is never zero !
  float ratio = width / height;

  // Make sure the viewport matches the new window dimensions; note that width and
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);

  m_prog_cube.matrix44f("projection") =
    matrix::perspective(maths::radians(50.0f), ratio, 0.1f, 10.0f);
}

//------------------------------------------------------------------
//! \brief Init your scene.
//------------------------------------------------------------------
bool GLExample08::setup()
{
  // Enable some OpenGL states
  glCheck(glEnable(GL_DEPTH_TEST));
  glCheck(glDisable(GL_BLEND));
  glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  // First program
  m_vs_screen.fromFile("shaders/Example08_screen.vertex");
  m_fs_screen.fromFile("shaders/Example08_screen.fragment");
  if (!m_prog_screen.attachShaders(m_vs_screen, m_fs_screen).compile())
    {
      std::cerr << "failed compiling OpenGL program. Reason was '"
                << m_prog_screen.error() << "'" << std::endl;
      return false;
    }

  m_prog_screen.bind(m_screen);
  m_screen.vector2f("aPos") =
    {
      Vector2f(1.0f, -1.0f), Vector2f(-1.0f, -1.0f), Vector2f(-1.0f, -1.0f),
      Vector2f(1.0f, -1.0f), Vector2f(-1.0f, -1.0f), Vector2f(1.0f, -1.0f)
    };
  m_screen.vector2f("aTexCoords") =
    {
      Vector2f(-1,-1), Vector2f(-1,+1), Vector2f(+1,-1), Vector2f(+1,+1)
    };

  // Second program
  m_vs_cube.fromFile("shaders/Example08_cube.vertex");
  m_fs_cube.fromFile("shaders/Example08_cube.fragment");
  if (!m_prog_cube.attachShaders(m_vs_cube, m_fs_cube).compile())
    {
      std::cerr << "failed compiling OpenGL program. Reason was '"
                << m_prog_cube.error() << "'" << std::endl;
      return false;
    }

  m_prog_cube.bind(m_cube);
  m_cube.vector3f("aPos") =
    {
      Vector3f(-0.5f, -0.5f, -0.5f),
      Vector3f(0.5f, -0.5f, -0.5f),
      Vector3f(0.5f,  0.5f, -0.5f),
      Vector3f(0.5f,  0.5f, -0.5f),
      Vector3f(-0.5f,  0.5f, -0.5f),
      Vector3f(-0.5f, -0.5f, -0.5f),

      Vector3f(-0.5f, -0.5f,  0.5f),
      Vector3f(0.5f, -0.5f,  0.5f),
      Vector3f(0.5f,  0.5f,  0.5f),
      Vector3f(0.5f,  0.5f,  0.5f),
      Vector3f(-0.5f,  0.5f,  0.5f),
      Vector3f(-0.5f, -0.5f,  0.5f),

      Vector3f(-0.5f,  0.5f,  0.5f),
      Vector3f(-0.5f,  0.5f, -0.5f),
      Vector3f(-0.5f, -0.5f, -0.5f),
      Vector3f(-0.5f, -0.5f, -0.5f),
      Vector3f(-0.5f, -0.5f,  0.5f),
      Vector3f(-0.5f,  0.5f,  0.5f),

      Vector3f(0.5f,  0.5f,  0.5f),
      Vector3f(0.5f,  0.5f, -0.5f),
      Vector3f(0.5f, -0.5f, -0.5f),
      Vector3f(0.5f, -0.5f, -0.5f),
      Vector3f(0.5f, -0.5f,  0.5f),
      Vector3f(0.5f,  0.5f,  0.5f),

      Vector3f(-0.5f, -0.5f, -0.5f),
      Vector3f(0.5f, -0.5f, -0.5f),
      Vector3f(0.5f, -0.5f,  0.5f),
      Vector3f(0.5f, -0.5f,  0.5f),
      Vector3f(-0.5f, -0.5f,  0.5f),
      Vector3f(-0.5f, -0.5f, -0.5f),

      Vector3f(-0.5f,  0.5f, -0.5f),
      Vector3f(0.5f,  0.5f, -0.5f),
      Vector3f(0.5f,  0.5f,  0.5f),
      Vector3f(0.5f,  0.5f,  0.5f),
      Vector3f(-0.5f,  0.5f,  0.5f),
      Vector3f(-0.5f,  0.5f, -0.5f),
    };

  m_cube.vector2f("aTexCoords") =
    {
      Vector2f(0.0f, 0.0f),
      Vector2f(1.0f, 0.0f),
      Vector2f(1.0f, 1.0f),
      Vector2f(1.0f, 1.0f),
      Vector2f(0.0f, 1.0f),
      Vector2f(0.0f, 0.0f),

      Vector2f(0.0f, 0.0f),
      Vector2f(1.0f, 0.0f),
      Vector2f(1.0f, 1.0f),
      Vector2f(1.0f, 1.0f),
      Vector2f(0.0f, 1.0f),
      Vector2f(0.0f, 0.0f),

      Vector2f(1.0f, 0.0f),
      Vector2f(1.0f, 1.0f),
      Vector2f(0.0f, 1.0f),
      Vector2f(0.0f, 1.0f),
      Vector2f(0.0f, 0.0f),
      Vector2f(1.0f, 0.0f),

      Vector2f(1.0f, 0.0f),
      Vector2f(1.0f, 1.0f),
      Vector2f(0.0f, 1.0f),
      Vector2f(0.0f, 1.0f),
      Vector2f(0.0f, 0.0f),
      Vector2f(1.0f, 0.0f),

      Vector2f(0.0f, 1.0f),
      Vector2f(1.0f, 1.0f),
      Vector2f(1.0f, 0.0f),
      Vector2f(1.0f, 0.0f),
      Vector2f(0.0f, 0.0f),
      Vector2f(0.0f, 1.0f),

      Vector2f(0.0f, 1.0f),
      Vector2f(1.0f, 1.0f),
      Vector2f(1.0f, 0.0f),
      Vector2f(1.0f, 0.0f),
      Vector2f(0.0f, 0.0f),
      Vector2f(0.0f, 1.0f),
    };

  // Apply textures
  if (!m_cube.texture2D("texture1").load("textures/path.png")) return false;

  // Camera
  float ratio = static_cast<float>(width()) / (static_cast<float>(height()) + 0.1f);
  m_prog_cube.matrix44f("projection") =
    matrix::perspective(maths::radians(50.0f), ratio, 0.1f, 10.0f);
  m_prog_cube.matrix44f("model") = Matrix44f(matrix::Identity);
  m_prog_cube.matrix44f("view") =
    matrix::lookAt(Vector3f(0.75, -0.75, 0.75), Vector3f(0.0, 0.0, 0.0), Vector3f(0.0, 0.0, 1.0));

  // Framebuffer
  m_fbo.resize(width(), height());
  m_screen.texture2D("screenTexture") = m_fbo.createColorTexture();
  m_fbo.createDepthBuffer();

  return true;
}

//------------------------------------------------------------------
//! \brief Paint our scene.
//------------------------------------------------------------------
bool GLExample08::draw()
{
  // First pass: draw to the framebuffer texture
  m_fbo.render(0, 0, width(), height(), [this]() {
      glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
      glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
      glEnable(GL_DEPTH_TEST);
      m_prog_cube.draw(m_cube, Primitive::TRIANGLES, 0, 36);
    });

  // Second pass: draw to the screen
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);
  m_prog_screen.draw(m_screen, Primitive::TRIANGLE_STRIP, 0, 6);

  return true;
}
