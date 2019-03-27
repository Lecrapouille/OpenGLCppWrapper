#include "Example04.hpp"
#include <math.h>

//! \file this example paints a plane with multi-textures. This
//! example https://www.youtube.com/user/ThinMatrix/

//------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------
void GLExample04::onWindowSizeChanged(const float /*width*/, const float /*height*/)
{
}

//------------------------------------------------------------------
//! \brief Init your scene.
//------------------------------------------------------------------
bool GLExample04::setup()
{
  DEBUG("Setup");

  // Enable some OpenGL states
  glCheck(glEnable(GL_DEPTH_TEST));
  glCheck(glDepthFunc(GL_LESS));
  glCheck(glDisable(GL_BLEND));
  glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  // Load from ASCII file the vertex sahder (vs) as well the fragment shader
  vs.fromFile("shaders/Example04.vertex");
  fs.fromFile("shaders/Example04.fragment");

  // Compile shader as OpenGL program. This one will instanciate all OpenGL objects for you.
  if (!m_prog.attachShaders(vs, fs).compile())
    {
      std::cerr << "failed compiling OpenGL program. Reason was '"
                << m_prog.error() << "'" << std::endl;
      return false;
    }

  // --- Create a plane (for the floor)

  // Binding empty VAO to OpenGL program will make it be populated
  // with all VBOs needed.
  m_prog.bind(m_vao);

  // Now we have to fill VBOs with data: here vertices. Because in
  // vertex shader a_position is vect3 we have to cast to Vector3f.
  m_vao.vector3f("position") =
    {
      Vector3f(-1.0f, -1.0f, 0.0f), Vector3f(1.0f, -1.0f, 0.0f), Vector3f(0.0f, 1.0f, 0.0f)
    };

  // Now we have to fill VBOs with data: here texture coordinates.
  // Because in vertex shader a_texcoord is vect2 we have to cast
  // to Vector2f.
  m_vao.vector2f("UV") =
    {
      Vector2f(0.0f, 0.0f), Vector2f(1.0f, 0.0f), Vector2f(0.5f, 1.0f)
    };

  // --- Create textures

  // --- Init VAO textures
  if (!m_vao.texture2D("blendMap").load("textures/blendMap.png")) return false;
  if (!m_vao.texture2D("backgroundTexture").load("textures/grassy2.png")) return false;
  if (!m_vao.texture2D("rTexture").load("textures/mud.png")) return false;
  if (!m_vao.texture2D("gTexture").load("textures/grassFlowers.png")) return false;
  if (!m_vao.texture2D("bTexture").load("textures/path.png")) return false;

  return true;
}

//------------------------------------------------------------------
//! \brief Paint our scene.
//------------------------------------------------------------------
bool GLExample04::draw()
{
  // Clear OpenGL color and depth buffers.
  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  m_prog.draw(m_vao, Primitive::TRIANGLES, 0, 3);

  return true;
}
