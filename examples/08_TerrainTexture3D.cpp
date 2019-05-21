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

#include "08_TerrainTexture3D.hpp"
#include "Maths.hpp"

//------------------------------------------------------------------
//! \file this example paints a 3d lanscape from a 3d texture.
//!
//! \note This example takes its inspiration from
//! http://www.mathematik.uni-marburg.de/~thormae/lectures/graphics1/graphics_7_1_eng_web.html#1
//! and:
//! https://www.uni-marburg.de/fb12/en/researchgroups/grafikmultimedia/lectures/graphics
//------------------------------------------------------------------

//------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------
void GLExample08::onWindowSizeChanged(const float width, const float height)
{
  // Note: height is never zero !
  float ratio = width / height;

  // Make sure the viewport matches the new window dimensions.
  glCheck(glViewport(0, 0, static_cast<int>(width), static_cast<int>(height)));

  m_prog.matrix44f("projection") =
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

  // Load vertex and fragment shaders with GLSL code.
  m_vertex_shader.fromFile("shaders/08_TerrainTexture3D.vs");
  m_fragment_shader.fromFile("shaders/08_TerrainTexture3D.fs");

  // Compile the shader program
  if (!m_prog.attachShaders(m_vertex_shader, m_fragment_shader).compile())
    {
      std::cerr << "failed compiling OpenGL program. Reason was '"
                << m_prog.getError() << "'" << std::endl;
      return false;
    }

  // Init uniforms.
  float ratio = static_cast<float>(width()) / (static_cast<float>(height()) + 0.1f);
  m_prog.matrix44f("projection") =
    matrix::perspective(maths::radians(50.0f), ratio, 0.1f, 10.0f);
  m_prog.matrix44f("model") = Matrix44f(matrix::Identity);;
  m_prog.matrix44f("view") =
    matrix::lookAt(Vector3f(0.75, -0.75, 0.75), Vector3f(0.0, 0.0, 0.0), Vector3f(0.0, 0.0, 1.0));

  // Create the terrain
  return createTerrain();
}

//------------------------------------------------------------------
//! \brief Pasize_t our scene.
//------------------------------------------------------------------
bool GLExample08::draw()
{
  // Clear OpenGL color and depth buffers.
  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  m_prog.draw(m_vao, Mode::TRIANGLE_STRIP, 0, m_nbVertices);

  return true;
}

//------------------------------------------------------------------
//! \brief Init your scene.
//------------------------------------------------------------------
bool GLExample08::createTerrain()
{
  const uint32_t dim = 40;

  m_prog.bind(m_vao);

  // Load all 2D textures into a single bige 3D texture
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
  generateAltitudes(dim);
  loadTerrain(dim);
  return true;
}

//------------------------------------------------------------------
//! \brief Generate terrain altitude.
//------------------------------------------------------------------
void GLExample08::generateAltitudes(const uint32_t dim)
{
  // Create random values
  m_altitudes.resize(dim * dim);

  for (uint32_t r = 0; r < dim * dim; ++r)
    {
      m_altitudes[r] = fabsf(float(rand())) / float(RAND_MAX);
    }

  // Generate smooth m_altitudes values
  std::vector<float> smoothTerrain(dim * dim);
  for (unsigned k = 0; k < 5; ++k)
    {
      float maxVal = 0.0f;
      float minVal = 1.0f;
      for (uint32_t x = 0; x < dim; ++x)
        {
          for (uint32_t y = 0; y < dim; ++y)
            {
              if (x == 0 || x == dim - 1)
                m_altitudes[x * dim + y] = 0.0f;
              else if (y == 0 || y == dim - 1)
                m_altitudes[x * dim + y] = 0.0f;
              else
                {
                  float a = 0.0f;
                  uint32_t counter = 0;
                  for (uint32_t s = 0; s <= 2; ++s)
                    {
                      for(uint32_t r = 0; r <= 2; ++r)
                        {
                          a += m_altitudes[(x + s - 1) * dim + (y + r - 1)];
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
      for (uint32_t r = 0; r < dim * dim; ++r)
        {
          m_altitudes[r] = (smoothTerrain[r] - minVal) / (maxVal-minVal);
        }
    }
}

//------------------------------------------------------------------
//! \brief Fill VBOs with mesh position and 3d texture position.
//------------------------------------------------------------------
void GLExample08::loadTerrain(const uint32_t dim)
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
                             m_altitudes[(x - 1) * dim + (y - 1)] * texHeight));
          uv.append(Vector3f(float(x) / float(dim),
                             float(y - 1) / float(dim),
                             m_altitudes[x * dim + (y - 1)] * texHeight));
          uv.append(Vector3f(float(x - 1) / float(dim),
                             float(y) / float(dim),
                             m_altitudes[(x - 1) * dim + y] * texHeight));
          uv.append(Vector3f(float(x) / float(dim),
                             float(y) / float(dim),
                             m_altitudes[x * dim + y] * texHeight));

          // Meshes
          pos.append(Vector3f(float(x - 1) / float(dim) -0.5f,
                              float(y - 1) / float(dim) -0.5f,
                              m_altitudes[(x - 1) * dim + (y - 1)] * maxHeight));
          pos.append(Vector3f(float(x) / float(dim) -0.5f,
                              float(y - 1) / float(dim) -0.5f,
                              m_altitudes[x * dim + (y - 1)] * maxHeight));
          pos.append(Vector3f(float(x - 1) / float(dim) -0.5f,
                              float(y) / float(dim) -0.5f,
                              m_altitudes[(x - 1) * dim + y] * maxHeight));
          pos.append(Vector3f(float(x) / float(dim) -0.5f,
                              float(y) / float(dim) -0.5f,
                              m_altitudes[x * dim + y] * maxHeight));

        }
    }
}
