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
#include "Maths.hpp"

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
void GLExample06::onWindowSizeChanged(const float width, const float height)
{
  // Note: height is never zero !
  float ratio = width / height;

  // Make sure the viewport matches the new window dimensions.
  glCheck(glViewport(0, 0, width, height));

  m_prog.matrix44f("projection") =
    matrix::perspective(maths::radians(50.0f), ratio, 0.1f, 10.0f);
}

//------------------------------------------------------------------
//! \brief
//------------------------------------------------------------------
void GLExample06::createSphere()
{
  constexpr float radius = 2.0f;
  constexpr int NbPointsLon = 100;
  constexpr int NbPointsLat = 100;
  constexpr float stepLon = 360.0f / static_cast<float>(NbPointsLon);
  constexpr float stepLat = 180.0f / static_cast<float>(NbPointsLat);
  constexpr float PI = 3.141592653589793238462643383279502884197169399375105820f;

  float latitude = -90.0f;
  float longitude = -180.0f;

  m_prog.bind(m_sphere);
  auto& positions = m_sphere.vector3f("position");
  positions.reserve(NbPointsLon * NbPointsLat);
  m_indices.clear();
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
      std::cerr << "failed compiling OpenGL program. Reason was '"
                << m_prog.getError() << "'" << std::endl;
      return false;
    }

  // Init shader uniforms
  float ratio = static_cast<float>(width()) / (static_cast<float>(height()) + 0.1f);
  m_prog.matrix44f("projection") =
    matrix::perspective(maths::radians(50.0f), ratio, 0.1f, 10.0f);
  m_prog.matrix44f("model") = m_movable.transform();
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

  // Draw the sphere
  m_prog.draw(m_sphere, Primitive::POINTS, m_indices);

  return true;
}
