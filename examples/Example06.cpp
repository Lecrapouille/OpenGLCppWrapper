#include "Example06.hpp"
#include <math.h>

//------------------------------------------------------------------
//! \file this example paints a 3d lanscape from a 3d texture.
//! It was inspired by http://www.mathematik.uni-marburg.de/~thormae/lectures/graphics1/graphics_7_1_eng_web.html#1
//! and: https://www.uni-marburg.de/fb12/en/researchgroups/grafikmultimedia/lectures/graphics
//------------------------------------------------------------------

//------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------
void GLExample06::onWindowSizeChanged(const float width, const float height)
{
  // Note: height is never zero !
  float ratio = width / height;

  // Make sure the viewport matches the new window dimensions; note that width and
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);

  m_prog.matrix44f("projection") =
    matrix::perspective(maths::radians(50.0f), ratio, 0.1f, 10.0f);
}

//------------------------------------------------------------------
//! \brief Init your scene.
//------------------------------------------------------------------
bool GLExample06::setup()
{
  // Enable some OpenGL states
  glCheck(glEnable(GL_DEPTH_TEST));
  glCheck(glDisable(GL_BLEND));
  glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  // Load from ASCII file the vertex sahder (vs) as well the fragment shader
  vs.fromFile("shaders/Example06.vertex");
  fs.fromFile("shaders/Example06.fragment");

  // Compile shader as OpenGL program. This one will instanciate all OpenGL objects for you.
  if (!m_prog.attachShaders(vs, fs).compile())
    {
      std::cerr << "failed compiling OpenGL program. Reason was '"
                << m_prog.error() << "'" << std::endl;
      return false;
    }

  // Binding empty VAO to OpenGL program will make it be populated
  // with all VBOs needed.
  m_prog.bind(m_vao);

  // Camera
  float ratio = static_cast<float>(width()) / (static_cast<float>(height()) + 0.1f);
  m_prog.matrix44f("projection") =
    matrix::perspective(maths::radians(50.0f), ratio, 0.1f, 10.0f);
  m_prog.matrix44f("model") = Matrix44f(matrix::Identity);;
  m_prog.matrix44f("view") =
    matrix::lookAt(Vector3f(0.75, -0.75, 0.75), Vector3f(0.0, 0.0, 0.0), Vector3f(0.0, 0.0, 1.0));

  // Create textures
  m_vao.texture3D("tex3d").wrap(TextureWrap::CLAMP_TO_BORDER);
  if (!m_vao.texture3D("tex3d").load(
        {
          "textures/deep_water.png",
          "textures/shallow_water.png",
          "textures/shore.png",
          "textures/fields.png",
          "textures/rocks.png",
          "textures/snow.png",
        }))
    {
      return false;
    }

  // Create the terrain
  const int dim = 40;
  buildTerrain(dim);
  drawTerrain(dim);
  return true;
}

//------------------------------------------------------------------
//! \brief Paint our scene.
//------------------------------------------------------------------
bool GLExample06::draw()
{
  // Clear OpenGL color and depth buffers.
  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  m_prog.draw(m_vao, Primitive::TRIANGLE_STRIP, 0, m_nbVertices);

  return true;
}

//------------------------------------------------------------------
//! \brief Generate terrain altitude.
//------------------------------------------------------------------
void GLExample06::buildTerrain(const int dim)
{
  // Create random values
  m_terrain.resize(dim * dim);

  for (int r = 0; r < dim * dim; ++r)
    {
      int rval = rand();
      m_terrain[r] = fabs(float(rval)) / float(RAND_MAX);
    }

  // Generate smooth m_terrain values
  std::vector<float> smoothTerrain(dim * dim);
  for (unsigned k = 0; k < 5; ++k)
    {
      float maxVal = 0.0f;
      float minVal = 1.0f;
      for (int x = 0; x < dim; ++x)
        {
          for (int y = 0; y < dim; ++y)
            {
              if (x == 0 || x == dim - 1)
                m_terrain[x * dim + y] = 0.0f;
              else if (y == 0 || y == dim - 1)
                m_terrain[x * dim + y] = 0.0f;
              else
                {
                  float a = 0.0f;
                  int counter = 0;
                  for (int s = -1; s <= 1; ++s)
                    {
                      for(int r = -1; r <= 1; ++r)
                        {
                          a += m_terrain[(x + s) * dim + (y + r)];
                          ++counter;
                        }
                    }
                  float val = a / float(counter);
                  smoothTerrain[x * dim + y] = val;
                  if (val > maxVal) maxVal = val;
                  if (val < minVal) minVal = val;
                }
            }
        }
      for (int r = 0; r < dim * dim; ++r)
        {
          m_terrain[r] = (smoothTerrain[r] - minVal) / (maxVal-minVal);
        }
    }
}

//------------------------------------------------------------------
//! \brief Fill VBOs with mesh position and 3d texture position.
//------------------------------------------------------------------
void GLExample06::drawTerrain(const int dim)
{
  float maxHeight = 0.2f;
  float texHeight = 0.9f;

  // Cache references to avoid calling the hash function during the loop
  auto& pos = m_vao.vector3f("position");
  auto& uv = m_vao.vector3f("UV");

  // Reserve VBO sizes (optional but avoid to reallocate several times)
  // FIXME: pass m_nbVertices when binding VAO ?
  m_nbVertices = (dim - 1) * (dim - 1) * 4;
  pos.reserve(m_nbVertices);
  uv.reserve(m_nbVertices);

  for (unsigned x = 1; x < dim; ++x)
    {
      for (unsigned y = 1; y < dim; ++y)
        {
          // Texture3D
          uv.append(Vector3f(float(x - 1) / float(dim),
                             float(y - 1) / float(dim),
                             m_terrain[(x - 1) * dim + (y - 1)] * texHeight));
          uv.append(Vector3f(float(x) / float(dim),
                             float(y - 1) / float(dim),
                             m_terrain[x * dim + (y - 1)] * texHeight));
          uv.append(Vector3f(float(x - 1) / float(dim),
                             float(y) / float(dim),
                             m_terrain[(x - 1) * dim + y] * texHeight));
          uv.append(Vector3f(float(x) / float(dim),
                             float(y) / float(dim),
                             m_terrain[x * dim + y] * texHeight));

          // Meshes
          pos.append(Vector3f(float(x - 1) / float(dim) -0.5f,
                              float(y - 1) / float(dim) -0.5f,
                              m_terrain[(x - 1) * dim + (y - 1)] * maxHeight));
          pos.append(Vector3f(float(x) / float(dim) -0.5f,
                              float(y - 1) / float(dim) -0.5f,
                              m_terrain[x * dim + (y - 1)] * maxHeight));
          pos.append(Vector3f(float(x - 1) / float(dim) -0.5f,
                              float(y) / float(dim) -0.5f,
                              m_terrain[(x - 1) * dim + y] * maxHeight));
          pos.append(Vector3f(float(x) / float(dim) -0.5f,
                              float(y) / float(dim) -0.5f,
                              m_terrain[x * dim + y] * maxHeight));

        }
    }
}
