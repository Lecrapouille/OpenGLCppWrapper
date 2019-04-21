#include "Example03.hpp"
#include <math.h>

//! \file this example paints two cubes (same model) placed on a floor
//! (second model). The fisrt cube is turning while the second is
//! fixed. Models are textured and we applied different color on
//! textures.

//------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------
void GLExample03::onWindowSizeChanged(const float width, const float height)
{
  // Note: height is never zero !
  float ratio = width / height;

  // Make sure the viewport matches the new window dimensions; note that width and
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);

  m_prog.matrix44f("projection") =
    matrix::perspective(maths::radians(50.0f), ratio, 0.1f, 10.0f);
}

void GLExample03::createSphere()
{
  constexpr float radius = 2.0f;
  constexpr int NbPointsLon = 50;
  constexpr int NbPointsLat = 100;
  constexpr float stepLon = 360.0f / static_cast<float>(NbPointsLon);
  constexpr float stepLat = 180.0f / static_cast<float>(NbPointsLat);
  constexpr float PI = 3.141592653589793238462643383279502884197169399375105820f;

  float latitude = -90.0f;
  float longitude = -180.0f;

  auto& positions = m_vao.vector3f("position");
  positions.reserve(NbPointsLon * NbPointsLat);
  m_indices.reserve(NbPointsLon * NbPointsLat);

  for (int i = 0; i < NbPointsLon; ++i)
    {
      for (int j = 0; j < NbPointsLat; ++j)
        {
          float lat = (PI / 180.0f) * latitude;
          float lon = (PI / 180.0f) * longitude;
          float lat_sin = sin(lat);
          float lon_sin = sin(lon);
          float lat_cos = cos(lat);
          float lon_cos = cos(lon);

          positions.append(Vector3f(lon_sin * lat_cos * radius,
                                    lat_sin * radius,
                                    lat_cos * lon_cos * radius));

          longitude += stepLon;
        }
      latitude += stepLat;
    }

 for (int i = 0; i < NbPointsLon * NbPointsLat; ++i)
    {
      m_indices.append(i);
    }
}

//------------------------------------------------------------------
//! \brief Init your scene.
//------------------------------------------------------------------
bool GLExample03::setup()
{
  DEBUG("%s", "Setup");

  // Enable some OpenGL states
  glCheck(glEnable(GL_DEPTH_TEST));
  glCheck(glDepthFunc(GL_LESS));
  glCheck(glDisable(GL_BLEND));
  glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  // Load from ASCII file the vertex sahder (vs) as well the fragment shader
  vs.fromFile("shaders/Example03.vertex");
  fs.fromFile("shaders/Example03.fragment");

  // Compile shader as OpenGL program. This one will instanciate all OpenGL objects for you.
  if (!m_prog.attachShaders(vs, fs).compile())
    {
      std::cerr << "failed compiling OpenGL program. Reason was '"
                << m_prog.error() << "'" << std::endl;
      return false;
    }

  // --- Create a sphere

  // Binding empty VAO to OpenGL program will make it be populated
  // with all VBOs needed.
  m_prog.bind(m_vao);

  // Now we have to fill VBOs with data: here vertices. Because in
  // vertex shader position is vect3 we have to cast to Vector3f.
  createSphere();

  // --- Init OpenGL shader uniforms
  float ratio = static_cast<float>(width()) / (static_cast<float>(height()) + 0.1f);
  m_prog.matrix44f("projection") =
    matrix::perspective(maths::radians(50.0f), ratio, 0.1f, 10.0f);

  m_prog.matrix44f("model") = m_movable.transform();
  m_prog.matrix44f("view") =
    matrix::lookAt(Vector3f(3,3,3), Vector3f(1,1,1), Vector3f(0,1,0));

  // -- Perform some debug
  DEBUG("%s", "Instropection:");
  std::vector<std::string> vbos = m_vao.VBONames();
  for (auto& it: vbos)
    {
      std::cout << "VAO has VBO named '" << it << "'" << std::endl;
    }

  // TODO Check if everything is ok (attrib/uniform are set, prog compiled ...)

  // We have terminated creating our 3D scene, we can now paint it.
  DEBUG("%s", "GLExample03::draw");
  return true;
}

//------------------------------------------------------------------
//! \brief Paint our scene.
//------------------------------------------------------------------
bool GLExample03::draw()
{
  // Clear OpenGL color and depth buffers.
  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  // --- Draw the turning cube and apply to it a "pinkished" coloration.

  // Important: bind the VAO to the OpenGL shader to let it know to
  // OpenGL which one to paint. Contrary to bind() in setup(), his
  // time, VBOs are not populated !
  m_prog.bind(m_vao);

  // Paint the 36 verties (aka nodes) constituing a cube
  m_prog.draw(Primitive::POINTS, m_indices);

  return true;
}
