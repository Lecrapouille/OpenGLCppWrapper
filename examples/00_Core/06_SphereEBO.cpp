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

#include "06_SphereEBO.hpp"

//------------------------------------------------------------------
//! \file this example paints a sphere made of points. For drawing
//! the VAO we pass an index of vertices.
//!
//! \note This example takes its inspiration from
//! https://github.com/invor/simplestGraphRendering
//------------------------------------------------------------------

//------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------
void GLExample06::onWindowSizeChanged()
{
  // Note: height is never zero !
  float ratio = width<float>() / height<float>();

  // Make sure the viewport matches the new window dimensions.
  glCheck(glViewport(0, 0, width<int>(), height<int>()));

  m_prog.matrix44f("projection") =
    matrix::perspective(maths::toRadian(60.0f), ratio, 0.1f, 10.0f);
}

//------------------------------------------------------------------
//! \brief Create a VBO and a EBO (Element Buffer Object).
//------------------------------------------------------------------
void GLExample06::createSphere()
{
  constexpr float radius = 2.0f;
  constexpr uint32_t NbPointsLon = 100u;
  constexpr uint32_t NbPointsLat = 100u;
  constexpr float stepLon = 360.0f / static_cast<float>(NbPointsLon);
  constexpr float stepLat = 180.0f / static_cast<float>(NbPointsLat);
  constexpr float PI = 3.141592653589793238462643383279502884197169399375105820f;
  float latitude = -90.0f;
  float longitude = -180.0f;

  // Bind the VAO to the program. This will populate VBOs.
  // Get the reference of the desired VBO because vector3f()
  // is not a faster method.
  m_prog.bind(m_sphere);
  auto& positions = m_sphere.vector3f("position");

  positions.reserve(NbPointsLon * NbPointsLat);
  for (uint32_t i = 0; i < NbPointsLon; ++i)
    {
      for (uint32_t j = 0; j < NbPointsLat; ++j)
        {
          float lat = (PI / 180.0f) * latitude;
          float lon = (PI / 180.0f) * longitude;
          float lat_sin = sinf(lat);
          float lon_sin = sinf(lon);
          float lat_cos = cosf(lat);
          float lon_cos = cosf(lon);

          positions.append(Vector3f(lon_sin * lat_cos * radius,
                                    lat_sin * radius,
                                    lat_cos * lon_cos * radius));

          longitude += stepLon;
        }
      latitude += stepLat;
    }

  // Create the EBO (index for vertices) and return its reference.
  // Fill the index with values.
  GLIndexBuffer32& indices = m_sphere.index32();
  indices.reserve(NbPointsLon * NbPointsLat);
  uint32_t i = NbPointsLon * NbPointsLat;
  while (i--)
    {
      indices.append(i);
    }
}

//------------------------------------------------------------------
//! \brief Init your scene.
//------------------------------------------------------------------
bool GLExample06::setup()
{
  // Enable some OpenGL states
  glCheck(glEnable(GL_DEPTH_TEST));

  // Load vertex and fragment shaders with GLSL code.
  m_vertex_shader.fromFile("shaders/06_SphereEBO.vs");
  m_fragment_shader.fromFile("shaders/06_SphereEBO.fs");

  // Compile the shader program
  if (!m_prog.attachShaders(m_vertex_shader, m_fragment_shader).compile())
    {
      std::cerr << "Failed compiling OpenGL program. Reason was '"
                << m_prog.getError() << "'" << std::endl;
      return false;
    }

  // Init Model-View matrices (shader uniforms). Note that projection
  // matrix is init inside onWindowSizeChanged() which is called just
  // after this method.
  m_prog.matrix44f("model") = m_transformable.transform();
  m_prog.matrix44f("view") =
    matrix::lookAt(Vector3f(3,3,3), Vector3f(1,1,1), Vector3f(0,1,0));

  // Create the sphere
  createSphere();

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

  // Draw the sphere using the EBO
  m_prog.draw(m_sphere, Mode::POINTS, m_sphere.index32());

  return true;
}
