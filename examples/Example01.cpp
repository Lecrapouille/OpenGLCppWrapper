#include "Example01.hpp"
#include <math.h>

//! \file this example paints two cubes (same model) placed on a floor
//! (second model). The fisrt cube is turning while the second is
//! fixed. Models are textured and we applied different color on
//! textures.

//------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------
void GLExample01::onWindowSizeChanged(const float width, const float height)
{
  // Note: height is never zero !
  float ratio = width / height;

  m_prog.uniform<Matrix44f>("u_projection") =
    matrix::perspective(maths::radians(50.0f), ratio, 0.1f, 10.0f);
}

//------------------------------------------------------------------
//! \brief Init your scene.
//------------------------------------------------------------------
bool GLExample01::setup()
{
  LOGD("Setup");

  // Enable some OpenGL states
  glCheck(glEnable(GL_DEPTH_TEST));
  glCheck(glDepthFunc(GL_LESS));
  glCheck(glDisable(GL_BLEND));
  glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  // Load from ASCII file the vertex sahder (vs) as well the fragment shader
  vs.fromFile("shaders/Example01.vertex");
  fs.fromFile("shaders/Example01.fragment");

  // Compile shader as OpenGL program. This one will instanciate all OpenGL objects for you.
  if (!m_prog.attachShaders(vs, fs).compile())
    {
      std::cerr << "failed compiling OpenGL program. Reason was '"
                << m_prog.error() << "'" << std::endl;
      return false;
    }

  // --- Create a cube

  // Binding empty VAO to OpenGL program will make it be populated
  // with all VBOs needed.
  m_prog.bind(m_vao_quad);

  // Now we have to fill VBOs with data: here vertices. Because in
  // vertex shader a_position is vect3 we have to cast to Vector3f.
  m_prog.attribute<Vector3f>("a_position") =
    {
      //  X     Y     Z

      // bottom
      Vector3f(-1.0f,-1.0f,-1.0f),
      Vector3f( 1.0f,-1.0f,-1.0f),
      Vector3f(-1.0f,-1.0f, 1.0f),
      Vector3f( 1.0f,-1.0f,-1.0f),
      Vector3f( 1.0f,-1.0f, 1.0f),
      Vector3f(-1.0f,-1.0f, 1.0f),

      // top
      Vector3f(-1.0f, 1.0f,-1.0f),
      Vector3f(-1.0f, 1.0f, 1.0f),
      Vector3f( 1.0f, 1.0f,-1.0f),
      Vector3f( 1.0f, 1.0f,-1.0f),
      Vector3f(-1.0f, 1.0f, 1.0f),
      Vector3f( 1.0f, 1.0f, 1.0f),

      // front
      Vector3f(-1.0f,-1.0f, 1.0f),
      Vector3f( 1.0f,-1.0f, 1.0f),
      Vector3f(-1.0f, 1.0f, 1.0f),
      Vector3f( 1.0f,-1.0f, 1.0f),
      Vector3f( 1.0f, 1.0f, 1.0f),
      Vector3f(-1.0f, 1.0f, 1.0f),

      // back
      Vector3f(-1.0f,-1.0f,-1.0f),
      Vector3f(-1.0f, 1.0f,-1.0f),
      Vector3f( 1.0f,-1.0f,-1.0f),
      Vector3f( 1.0f,-1.0f,-1.0f),
      Vector3f(-1.0f, 1.0f,-1.0f),
      Vector3f( 1.0f, 1.0f,-1.0f),

      // left
      Vector3f(-1.0f,-1.0f, 1.0f),
      Vector3f(-1.0f, 1.0f,-1.0f),
      Vector3f(-1.0f,-1.0f,-1.0f),
      Vector3f(-1.0f,-1.0f, 1.0f),
      Vector3f(-1.0f, 1.0f, 1.0f),
      Vector3f(-1.0f, 1.0f,-1.0f),

      // right
      Vector3f(1.0f,-1.0f, 1.0f),
      Vector3f(1.0f,-1.0f,-1.0f),
      Vector3f(1.0f, 1.0f,-1.0f),
      Vector3f(1.0f,-1.0f, 1.0f),
      Vector3f(1.0f, 1.0f,-1.0f),
      Vector3f(1.0f, 1.0f, 1.0f)
    };

  // Now we have to fill VBOs with data: here texture coordinates.
  // Because in vertex shader a_texcoord is vect2 we have to cast
  // to Vector2f.
  m_prog.attribute<Vector2f>("a_texcoord") =
    {
      //  U     V

      // bottom
      Vector2f(0.0f, 0.0f),
      Vector2f(1.0f, 0.0f),
      Vector2f(0.0f, 1.0f),
      Vector2f(1.0f, 0.0f),
      Vector2f(1.0f, 1.0f),
      Vector2f(0.0f, 1.0f),

      // top
      Vector2f(0.0f, 0.0f),
      Vector2f(0.0f, 1.0f),
      Vector2f(1.0f, 0.0f),
      Vector2f(1.0f, 0.0f),
      Vector2f(0.0f, 1.0f),
      Vector2f(1.0f, 1.0f),

      // front
      Vector2f(1.0f, 0.0f),
      Vector2f(0.0f, 0.0f),
      Vector2f(1.0f, 1.0f),
      Vector2f(0.0f, 0.0f),
      Vector2f(0.0f, 1.0f),
      Vector2f(1.0f, 1.0f),

      // back
      Vector2f(0.0f, 0.0f),
      Vector2f(0.0f, 1.0f),
      Vector2f(1.0f, 0.0f),
      Vector2f(1.0f, 0.0f),
      Vector2f(0.0f, 1.0f),
      Vector2f(1.0f, 1.0f),

      // left
      Vector2f(0.0f, 1.0f),
      Vector2f(1.0f, 0.0f),
      Vector2f(0.0f, 0.0f),
      Vector2f(0.0f, 1.0f),
      Vector2f(1.0f, 1.0f),
      Vector2f(1.0f, 0.0f),

      // right
      Vector2f(1.0f, 1.0f),
      Vector2f(1.0f, 0.0f),
      Vector2f(0.0f, 0.0f),
      Vector2f(1.0f, 1.0f),
      Vector2f(0.0f, 0.0f),
      Vector2f(0.0f, 1.0f)
    };

  // --- Create a plane (for the floor)

  // Binding empty VAO to OpenGL program will make it be populated
  // with all VBOs needed.
  m_prog.bind(m_vao_floor);

  // Now we have to fill VBOs with data: here vertices. Because in
  // vertex shader a_position is vect3 we have to cast to Vector3f.
  m_prog.attribute<Vector3f>("a_position") =
    {
      Vector3f(5, -1.5,  5), Vector3f(-5, -1.5,  5), Vector3f(-5, -1.5, -5),
      Vector3f(5, -1.5,  5), Vector3f(-5, -1.5, -5), Vector3f(5, -1.5, -5)
    };

  // Now we have to fill VBOs with data: here texture coordinates.
  // Because in vertex shader a_texcoord is vect2 we have to cast
  // to Vector2f.
  m_prog.attribute<Vector3f>("a_position") *= Vector3f(2.0f, 1.0, 2.0f);
  m_prog.attribute<Vector2f>("a_texcoord") =
    {
      Vector2f(0.0f, 0.0f), Vector2f(1.0f, 0.0f), Vector2f(0.0f, 1.0f),
      Vector2f(1.0f, 0.0f), Vector2f(1.0f, 1.0f), Vector2f(0.0f, 1.0f),
    };

  // --- Create a texture

  // Texture FIXME 1 texture par VAO
  m_prog.uniform<GLTexture2D>("u_texture").interpolation(GL_LINEAR);
  m_prog.uniform<GLTexture2D>("u_texture").wrapping(GL_CLAMP_TO_EDGE);
  if (false == m_prog.uniform<GLTexture2D>("u_texture").load("wooden-crate.jpg"))
    return false;

  // --- Init OpenGL shader uniforms
  m_prog.uniform<float>("u_scale") = 1.0f;

  float ratio = static_cast<float>(width()) / (static_cast<float>(height()) + 0.1f);
  m_prog.uniform<Matrix44f>("u_projection") =
    matrix::perspective(maths::radians(50.0f), ratio, 0.1f, 10.0f);

  m_prog.uniform<Matrix44f>("u_model") = m_movable1.transform();
  m_prog.uniform<Matrix44f>("u_view") =
    matrix::lookAt(Vector3f(3,3,3), Vector3f(0,0,0), Vector3f(0,1,0));

  // -- Perform some debug
  LOGD("Instropection:");
  std::vector<std::string> vbos = m_vao_quad.VBONames();
  for (auto& it: vbos)
    {
      std::cout << "VAO has VBO named '" << it << "'" << std::endl;
    }

  // TODO Check if everything is ok (attrib/uniform are set, prog compiled ...)

  // We have terminated creating our 3D scene, we can now paint it.
  LOGD("GLExample01::draw");
  return true;
}

//------------------------------------------------------------------
//! \brief Paint our scene.
//------------------------------------------------------------------
bool GLExample01::draw()
{
  // Clear OpenGL color and depth buffers.
  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  // Fast and dirty code for spinning our 1st cube.
  static float time = 0.0f;
  time += dt();
  float ct = cosf(time);

  // FIXME: a ameliorer
  // Apply the texture to paint
  if (false == m_prog.uniform<GLTexture2D>("u_texture").load("hazard.png"))
    return false;

  // --- Draw the turning cube and apply to it a "pinkished" coloration.

  // Important: bind the VAO to the OpenGL shader to let it know to
  // OpenGL which one to paint. Contrary to bind() in setup(), his
  // time, VBOs are not populated !
  m_prog.bind(m_vao_quad);

  // Applied the "pinkished" coloration.
  m_prog.uniform<Vector4f>("u_color") = Vector4f(0.8f, 0.2f, 0.8f, 0.8f);

  // Apply a translation and a rotation to our cube to make it spin.
  m_movable1.rotate(4.0f * ct, Vector3f(0, 1, 0));
  m_movable1.position(Vector3f(-1.0f, 0.0f, -1.0f));
  m_prog.uniform<Matrix44f>("u_model") = m_movable1.transform();

  // Paint the 36 verties (aka nodes) constituing a cube
  m_prog.draw(GL_TRIANGLES, 0, 36); // FIXME 0, 36 a cacher

  // --- Draw a fixed cube and apply to it a "darkished" coloration.

  // Important: bind the VAO to the OpenGL shader to let it know to
  // OpenGL which one to paint. Contrary to bind() in setup(), his
  // time, VBOs are not populated !
  m_prog.bind(m_vao_quad);

  // Applied the "pinkished" coloration.
  m_prog.uniform<Vector4f>("u_color") = Vector4f(0.2f, 0.2f, 0.2f, 0.2f);

  // Apply a translation to our cube.
  m_movable2.reset();
  m_movable2.position(Vector3f(3.0f, 0.0f, 0.0f));
  m_prog.uniform<Matrix44f>("u_model") = m_movable2.transform();

  // Paint the 36 verties (aka nodes) constituing a cube
  m_prog.draw(GL_TRIANGLES, 0, 36);

  // --- Draw a floor

  // FIXME: a ameliorer
  // Apply the texture to paint
  if (false == m_prog.uniform<GLTexture2D>("u_texture").load("wooden-crate.jpg"))
    return false;

  // Important: bind the VAO to the OpenGL shader to let it know to
  // OpenGL which one to paint. Contrary to bind() in setup(), his
  // time, VBOs are not populated !
  m_prog.bind(m_vao_floor);

  // Applied a "shadow" coloration.
  m_prog.uniform<Vector4f>("u_color") = Vector4f(0.2f, 0.2f, 0.2f, 0.2f);
  m_movable3.reset();
  m_movable3.position(Vector3f(0.0f, 0.0f, 0.0f));
  m_prog.uniform<Matrix44f>("u_model") = m_movable3.transform();
  m_prog.draw(GL_TRIANGLES, 0, 6);

  return true;
}
