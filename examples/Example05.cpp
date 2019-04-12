#include "Example05.hpp"
#include <math.h>

//! \file this example paints a cubemap texture inspired by https://learnopengl.com/Advanced-OpenGL/Cubemaps

//------------------------------------------------------------------
//! \brief Callback when the window changed its size.
//------------------------------------------------------------------
void GLExample05::onWindowSizeChanged(const float width, const float height)
{
  // Note: height is never zero !
  float ratio = width / height;

  m_progCube.matrix44f("projection") =
    matrix::perspective(maths::radians(50.0f), ratio, 0.1f, 10.0f);
  m_progSkyBox.matrix44f("projection") = m_progCube.matrix44f("projection");
}

//------------------------------------------------------------------
//! \brief Init your scene.
//------------------------------------------------------------------
bool GLExample05::setup()
{
  DEBUG("%s", "Setup");

  // Enable some OpenGL states
  glCheck(glEnable(GL_DEPTH_TEST));
  glCheck(glDepthFunc(GL_LESS));
  glCheck(glDisable(GL_BLEND));
  glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  // --- Create a skybox

  // Load from ASCII file the vertex sahder (vs) as well the fragment shader
  vs1.fromFile("shaders/Example05_skybox.vertex");
  fs1.fromFile("shaders/Example05_skybox.fragment");

  // Compile shader as OpenGL program. This one will instanciate all OpenGL objects for you.
  if (!m_progSkyBox.attachShaders(vs1, fs1).compile())
    {
      std::cerr << "failed compiling OpenGL program. Reason was '"
                << m_progSkyBox.error() << "'" << std::endl;
      return false;
    }

  // Binding empty VAO to OpenGL program will make it be populated
  // with all VBOs needed.
  m_progSkyBox.bind(m_skybox);

  // Now we have to fill VBOs with data: here vertices. Because in
  // vertex shader a_position is vect3 we have to cast to Vector3f.
  m_skybox.vector3f("aPos") =
    {
      Vector3f(-1.0f,  1.0f, -1.0f),
      Vector3f(-1.0f, -1.0f, -1.0f),
      Vector3f(1.0f, -1.0f, -1.0f),
      Vector3f(1.0f, -1.0f, -1.0f),
      Vector3f(1.0f,  1.0f, -1.0f),
      Vector3f(-1.0f,  1.0f, -1.0f),

      Vector3f(-1.0f, -1.0f,  1.0f),
      Vector3f(-1.0f, -1.0f, -1.0f),
      Vector3f(-1.0f,  1.0f, -1.0f),
      Vector3f(-1.0f,  1.0f, -1.0f),
      Vector3f(-1.0f,  1.0f,  1.0f),
      Vector3f(-1.0f, -1.0f,  1.0f),

      Vector3f(1.0f, -1.0f, -1.0f),
      Vector3f(1.0f, -1.0f,  1.0f),
      Vector3f(1.0f,  1.0f,  1.0f),
      Vector3f(1.0f,  1.0f,  1.0f),
      Vector3f(1.0f,  1.0f, -1.0f),
      Vector3f(1.0f, -1.0f, -1.0f),

      Vector3f(-1.0f, -1.0f,  1.0f),
      Vector3f(-1.0f,  1.0f,  1.0f),
      Vector3f(1.0f,  1.0f,  1.0f),
      Vector3f(1.0f,  1.0f,  1.0f),
      Vector3f(1.0f, -1.0f,  1.0f),
      Vector3f(-1.0f, -1.0f,  1.0f),

      Vector3f(-1.0f,  1.0f, -1.0f),
      Vector3f(1.0f,  1.0f, -1.0f),
      Vector3f(1.0f,  1.0f,  1.0f),
      Vector3f(1.0f,  1.0f,  1.0f),
      Vector3f(-1.0f,  1.0f,  1.0f),
      Vector3f(-1.0f,  1.0f, -1.0f),

      Vector3f(-1.0f, -1.0f, -1.0f),
      Vector3f(-1.0f, -1.0f,  1.0f),
      Vector3f(1.0f, -1.0f, -1.0f),
      Vector3f(1.0f, -1.0f, -1.0f),
      Vector3f(-1.0f, -1.0f,  1.0f),
      Vector3f(1.0f, -1.0f,  1.0f)
    };

  // Add 6 textures to the sky box
  if (!m_skybox.texture3D("skybox").load(CubeMap::POSITIVE_X, "textures/right.jpg")) return false;
  if (!m_skybox.texture3D("skybox").load(CubeMap::NEGATIVE_X, "textures/left.jpg")) return false;
  if (!m_skybox.texture3D("skybox").load(CubeMap::POSITIVE_Y, "textures/top.jpg")) return false;
  if (!m_skybox.texture3D("skybox").load(CubeMap::NEGATIVE_Y, "textures/bottom.jpg")) return false;
  if (!m_skybox.texture3D("skybox").load(CubeMap::POSITIVE_Z, "textures/front.jpg")) return false;
  if (!m_skybox.texture3D("skybox").load(CubeMap::NEGATIVE_Z, "textures/back.jpg")) return false;

  // --- Create a cube

  // Load from ASCII file the vertex sahder (vs) as well the fragment shader
  vs2.fromFile("shaders/Example05_cubemap.vertex");
  fs2.fromFile("shaders/Example05_cubemap.fragment");

  // Compile shader as OpenGL program. This one will instanciate all OpenGL objects for you.
  if (!m_progCube.attachShaders(vs2, fs2).compile())
    {
      std::cerr << "failed compiling OpenGL program. Reason was '"
                << m_progCube.error() << "'" << std::endl;
      return false;
    }

  // Binding empty VAO to OpenGL program will make it be populated
  // with all VBOs needed.
  m_progCube.bind(m_cube);

  // Now we have to fill VBOs with data: here vertices. Because in
  // vertex shader a_position is vect3 we have to cast to Vector3f.
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

  // Now we have to fill VBOs with data: here texture coordinates.
  // Because in vertex shader a_texcoord is vect2 we have to cast
  // to Vector2f.
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

  // Add a texture to the cube
  if (!m_cube.texture2D("texture1").load("textures/path.png")) return false;

  // --- Init uniforms

  float ratio = static_cast<float>(width()) / (static_cast<float>(height()) + 0.1f);
  m_progCube.matrix44f("projection") =
    matrix::perspective(maths::radians(50.0f), ratio, 0.1f, 10.0f);
  m_progSkyBox.matrix44f("projection") = m_progCube.matrix44f("projection");

  return true;
}

//------------------------------------------------------------------
//! \brief Paint our scene.
//------------------------------------------------------------------
bool GLExample05::draw()
{
  // Clear OpenGL color and depth buffers.
  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  // --------------------------------------------------------------
  // --- First: draw scene as normal
  // --------------------------------------------------------------

  m_progCube.matrix44f("model") = Matrix44f(matrix::Identity);
  Matrix44f view = m_camera.GetViewMatrix();
  m_progCube.matrix44f("view") = view;

  glCheck(glDepthFunc(GL_LESS));
  m_progCube.draw(m_cube, Primitive::TRIANGLES, 0, 36);

  // --------------------------------------------------------------
  // --- Draw skybox as last.
  // --------------------------------------------------------------

  // remove translation from the view matrix
  m_progSkyBox.matrix44f("view") = Matrix44f(Matrix33f(view));

  // Note: change depth function so depth test passes when values are
  // equal to depth buffer's content
  glCheck(glDepthFunc(GL_LEQUAL));
  m_progSkyBox.draw(m_skybox, Primitive::TRIANGLES, 0, 36);

  // --------------------------------------------------------------
  // --- Key pressed
  // --------------------------------------------------------------
  float deltaTime = dt();
  if (keyPressed(GLFW_KEY_W))
    m_camera.ProcessKeyboard(FORWARD, deltaTime);
  if (keyPressed(GLFW_KEY_S))
    m_camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (keyPressed(GLFW_KEY_A))
    m_camera.ProcessKeyboard(LEFT, deltaTime);
  if (keyPressed(GLFW_KEY_D))
    m_camera.ProcessKeyboard(RIGHT, deltaTime);

  return true;
}

// --------------------------------------------------------------
// --- Mouse event
// --------------------------------------------------------------
void GLExample05::onMouseMoved(const double xpos, const double ypos)
{
  static float lastX = 0.0f;
  static float lastY = 0.0f;
  static bool firstMouse = true;

  if (firstMouse)
    {
      lastX = xpos;
      lastY = ypos;
      firstMouse = false;
    }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

  lastX = xpos;
  lastY = ypos;
  m_camera.ProcessMouseMovement(xoffset, yoffset);
}
