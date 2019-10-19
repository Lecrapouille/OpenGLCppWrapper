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

#include "07_SkyBoxTextureCube.hpp"

//------------------------------------------------------------------
//! \file this example paints a shape inside a skybox. A skybox is
//! cubic texture simuling a infinite landscape
//!
//! \note This example takes its inspiration from
//! https://learnopengl.com/Advanced-OpenGL/Cubemaps
//------------------------------------------------------------------

//------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------
void GLExample07::onWindowSizeChanged()
{
  // Note: height is never zero !
  float ratio = width<float>() / height<float>();

  // Make sure the viewport matches the new window dimensions.
  glCheck(glViewport(0, 0, width<int>(), height<int>()));

  m_progShape.matrix44f("projection") =
    matrix::perspective(maths::toRadian(50.0f), ratio, 0.1f, 10.0f);
  m_progSkyBox.matrix44f("projection") = m_progShape.matrix44f("projection");
}

// --------------------------------------------------------------
//! \brief Mouse event
// --------------------------------------------------------------
void GLExample07::onMouseMoved(window::Mouse const& mouse)
{
  float const dx = static_cast<float>(mouse.displacement.x);
  float const dy = static_cast<float>(mouse.displacement.y);
  m_camera.ProcessMouseMovement(dx, dy);
}

//------------------------------------------------------------------
//! \brief Create a skybox.
//------------------------------------------------------------------
bool GLExample07::createSkyBox()
{
  // Load from ASCII file the vertex sahder (vs) as well the fragment shader
  vs1.fromFile("shaders/07_SkyBoxTextureCube_skybox.vs");
  fs1.fromFile("shaders/07_SkyBoxTextureCube_skybox.fs");

  // Compile shader as OpenGL program. This one will instanciate all OpenGL objects for you.
  if (!m_progSkyBox.attachShaders(vs1, fs1).compile())
    {
      std::cerr << "failed compiling OpenGL program. Reason was '"
                << m_progSkyBox.getError() << "'" << std::endl;
      return false;
    }

  // Init uniforms.
  float ratio = width<float>() / height<float>();
  m_progSkyBox.matrix44f("projection") =
    matrix::perspective(maths::toRadian(50.0f), ratio, 0.1f, 10.0f);

  // Binding empty VAO to OpenGL program will make it be populated
  // with all VBOs needed.
  m_progSkyBox.bind(m_skybox);

  // Now we have to fill VBOs with data: here vertices. Because in
  // vertex shader a_position is vect3 we have to cast to Vector3f.
  m_skybox.vector3f("aPos") =
    {
       #include "../geometry/cube_position.txt"
    };

  // Add 6 textures to the sky box
  if (!m_skybox.textureCube("skybox").load(CubeMap::POSITIVE_X, "../textures/right.jpg")) return false;
  if (!m_skybox.textureCube("skybox").load(CubeMap::NEGATIVE_X, "../textures/left.jpg")) return false;
  if (!m_skybox.textureCube("skybox").load(CubeMap::POSITIVE_Y, "../textures/top.jpg")) return false;
  if (!m_skybox.textureCube("skybox").load(CubeMap::NEGATIVE_Y, "../textures/bottom.jpg")) return false;
  if (!m_skybox.textureCube("skybox").load(CubeMap::POSITIVE_Z, "../textures/front.jpg")) return false;
  if (!m_skybox.textureCube("skybox").load(CubeMap::NEGATIVE_Z, "../textures/back.jpg")) return false;

  return true;
}

//------------------------------------------------------------------
//! \brief Create a 3D shape (Cone, Pyramid, Cylinder, Tube). See
//! these class like factories for instanciating VAO.
//------------------------------------------------------------------
bool GLExample07::createShape()
{
  // Load from ASCII file the vertex sahder (vs) as well the fragment shader
  vs2.fromFile("shaders/07_SkyBoxTextureCube_shape.vs");
  fs2.fromFile("shaders/07_SkyBoxTextureCube_shape.fs");

  // Compile shader as OpenGL program. This one will instanciate all OpenGL objects for you.
  if (!m_progShape.attachShaders(vs2, fs2).compile())
    {
      std::cerr << "failed compiling OpenGL program. Reason was '"
                << m_progShape.getError() << "'" << std::endl;
      return false;
    }

  // Init uniforms. Set the Z-clipping of the camera to a high value
  // in the aim to create an effect of deep when objects are far away
  // (in contrast of the skybox which never changes its size).
  float ratio = width<float>() / height<float>();
  m_progShape.matrix44f("projection") =
    matrix::perspective(maths::toRadian(50.0f), ratio, 0.1f, 100.0f);

  // Binding empty VAO to OpenGL program will make it be populated
  // with all VBOs needed.
  m_progShape.bind(m_shape);

  // Uncomment to debug triangles
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  const uint32_t slices = 32u;
  const float radius = 1.0f;
  const float height = 1.0f;

  // Create a cylinder with two caps (bottom & top). Merge them into a
  // single VAO.
  Cylinder cylinder(radius, height, slices);
  Circle circle1(radius, slices); circle1.vertices() += Vector3f(0.0f, 0.0f, 0.5f);
  Circle circle2(radius, slices); circle2.vertices() -= Vector3f(0.0f, 0.0f, 0.5f);

  m_shape.vector3f("aPos")
    .append(circle1.vertices())
    .append(circle2.vertices())
    .append(cylinder.vertices());

  m_shape.vector2f("aTexCoords")
    .append(circle1.textures())
    .append(circle2.textures())
    .append(cylinder.textures());

  m_shape.index32()
    .appendIndex(circle1.indices())   // Starting index: 0
    .appendIndex(circle2.indices())   // Starting index: 34
    .appendIndex(cylinder.indices()); // Starting index: 68

  // Repeat the texture motif
  m_shape.vector2f("aTexCoords") *= 2.0f;

  // Add a texture to the cube
  if (!m_shape.texture2D("texture1").load("../textures/path.png")) return false;

  return true;
}

//------------------------------------------------------------------
//! \brief Init your scene.
//------------------------------------------------------------------
bool GLExample07::setup()
{
  // Enable some OpenGL states
  glCheck(glEnable(GL_DEPTH_TEST));
  glCheck(glDepthFunc(GL_LESS));
  glCheck(glDisable(GL_BLEND));
  glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  hideMouseCursor();

  return createShape() && createSkyBox();
}

// --------------------------------------------------------------
//! \brief Draw the shape.
// --------------------------------------------------------------
void GLExample07::drawShape()
{
  m_progShape.matrix44f("model") = Matrix44f(matrix::Identity);
  m_progShape.matrix44f("view") = m_camera.GetViewMatrix();

  // Set depth function back to default
  glCheck(glDepthFunc(GL_LESS));
  m_progShape.draw(m_shape, Mode::TRIANGLES, m_shape.index32());
}

// --------------------------------------------------------------
//! \brief Draw skybox. Should be draw as last.
// --------------------------------------------------------------
void GLExample07::drawSkyBox()
{
  // Remove translation from the view matrix
  Matrix44f view = m_camera.GetViewMatrix();
  m_progSkyBox.matrix44f("view") = Matrix44f(Matrix33f(view));

  // Change depth function so depth test passes when values are equal
  // to depth buffer's content
  glCheck(glDepthFunc(GL_LEQUAL));
  m_progSkyBox.draw(m_skybox, Mode::TRIANGLES, 0, 36);
}

//------------------------------------------------------------------
//! \brief Paint our scene.
//------------------------------------------------------------------
bool GLExample07::draw()
{
  // Clear OpenGL color and depth buffers.
  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  // Draw scene as normal.
  drawShape();
  // Draw skybox as last. Set depth function back to default
  drawSkyBox();

  // Key pressed
  if (keyPressed(GLFW_KEY_W))
    m_camera.ProcessKeyboard(Camera_Movement::FORWARD, dt());
  if (keyPressed(GLFW_KEY_S))
    m_camera.ProcessKeyboard(Camera_Movement::BACKWARD, dt());
  if (keyPressed(GLFW_KEY_A))
    m_camera.ProcessKeyboard(Camera_Movement::LEFT, dt());
  if (keyPressed(GLFW_KEY_D))
    m_camera.ProcessKeyboard(Camera_Movement::RIGHT, dt());

  return true;
}
