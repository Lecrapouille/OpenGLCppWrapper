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

#include "GLWindow.hpp"
#include <stdexcept>
#include <iostream>
#include <sstream>

namespace glwrap
{

//------------------------------------------------------------------
//! \brief Callback triggered when GLFW failed.
//------------------------------------------------------------------
__attribute__((__noreturn__))
static void on_error(int /*errorCode*/, const char* msg)
{
  throw OpenGLException(msg);
}

//------------------------------------------------------------------
//! \brief Static function allowing to "cast" a pointer to function to
//! pointer method. This function is triggered when the mouse moved.
//------------------------------------------------------------------
static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
  IGLWindow* obj = (IGLWindow*) glfwGetWindowUserPointer(window);
  if (likely(nullptr != obj))
    {
      obj->onMouseMoved(xpos, ypos);
    }
}

//------------------------------------------------------------------
//! \brief Static function allowing to "cast" a pointer to function to
//! pointer method. This function is triggered when the mouse button
//! is scrolled.
//------------------------------------------------------------------
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  IGLWindow* obj = (IGLWindow*) glfwGetWindowUserPointer(window);
  if (likely(nullptr != obj))
    {
      obj->onMouseScrolled(xoffset, yoffset);
    }
}

//------------------------------------------------------------------
//! \brief Static function allowing to "cast" a pointer to function
//! to pointer method. This function is triggered when the window
//! has been resized.
//------------------------------------------------------------------
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  IGLWindow* obj = (IGLWindow*) glfwGetWindowUserPointer(window);
  if (likely(nullptr != obj))
    {
      if (unlikely(0u >= width)) { width = 1u; }
      if (unlikely(0u >= height)) { height = 1u; }
      obj->setWindowSize(static_cast<uint32_t>(width),
                         static_cast<uint32_t>(height));
    }
}

//------------------------------------------------------------------
IGLWindow::IGLWindow(uint32_t const width, uint32_t const height, const char *title)
  : m_width(width),
    m_height(height),
    m_title(title)
{
  hasCreatedContext() = false;

  if (unlikely(nullptr == m_title)) { m_title = ""; }
  if (unlikely(0u == m_width)) { m_width = 1u; }
  if (unlikely(0u == m_height)) { m_height = 1u; }
}

//------------------------------------------------------------------
IGLWindow::~IGLWindow()
{
  if (hasCreatedContext())
    {
      glfwTerminate();
    }
}

//------------------------------------------------------------------
void IGLWindow::setWindowSize(uint32_t const width, uint32_t const height)
{
  m_width = width;
  m_height = height;

  if (unlikely(0u == m_width)) { m_width = 1u; }
  if (unlikely(0u == m_height)) { m_height = 1u; }

  // Calbback to be implemented by the derived class
  onWindowSizeChanged(static_cast<float>(width),
                      static_cast<float>(height));
}

//------------------------------------------------------------------
void IGLWindow::computeFPS()
{
  static int nbFrames = 0;
  double currentTime = glfwGetTime();
  m_deltaTime = static_cast<float>(currentTime - m_lastFrameTime);
  m_lastFrameTime = currentTime;
  ++nbFrames;

  if (currentTime - m_lastTime >= 1.0)
    {
      // If last prinf() was more than 1sec ago printf and reset
      std::ostringstream oss;
      m_fps = nbFrames;
      int ms_by_frame = static_cast<int>(1000.0 / static_cast<double>(m_fps));
      oss << '[' << m_fps << " FPS, " << ms_by_frame << " ms] " << m_title;
      glfwSetWindowTitle(m_window, oss.str().c_str());
      nbFrames = 0;
      m_lastTime += 1.0;
    }
}

//------------------------------------------------------------------
bool IGLWindow::start()
{
  if (hasCreatedContext())
    {
      std::cerr << "Warning you called twice start(). "
                << "OpenGL context already created"
                << std::endl;
      goto l_runtime;
    }

  // Initialise GLFW
  glfwSetErrorCallback(on_error);
  if (!glfwInit())
    {
      std::cerr << "Failed to initialize GLFW" << std::endl;
      return false;
    }

  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  hasCreatedContext() = true;

  // Open a window and create its OpenGL context
  m_window = glfwCreateWindow(m_width, m_height, m_title, nullptr, nullptr);
  if (nullptr == m_window)
    {
      std::cerr << "Failed to open GLFW window" << std::endl;
      return false;
    }
  glfwMakeContextCurrent(m_window);
  glfwSwapInterval(1); // Enable vsync

  // Initialize GLEW
  glewExperimental = GL_TRUE; // stops glew crashing on OSX :-/
  if (GLEW_OK != glewInit())
    {
      std::cerr << "Failed to initialize GLFW" << std::endl;
      return false;
    }

  // Print out some info about the graphics drivers
  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

  // Make sure OpenGL version 3.2 API is available
  if (!GLEW_VERSION_3_2)
    {
      std::cerr << "OpenGL 3.2 API is not available." << std::endl;
      return false;
    }

  // Save the class address to "cast" function callback into a method
  // callback
  glfwSetWindowUserPointer(m_window, this);

  // I/O callbacks
  glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
  glfwSetCursorPosCallback(m_window, mouse_callback);
  glfwSetScrollCallback(m_window, scroll_callback);
  // Ensure we can capture keyboard being pressed below
  glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);

l_runtime:

  // Flush OpenGL errors before using this function on real OpenGL
  // routines else a fake error is returned on the first OpenGL
  // routines while valid.
  glCheck();

  int res;
  try
    {
      // Init OpenGL states of the user application
      res = setup();
      if (likely(res))
        {
          // init FPS
          m_lastTime = glfwGetTime();
          m_lastFrameTime = m_lastTime;
          m_fps = 0;

          // Draw OpenGL scene
          res = loop();
        }
      else
        {
          std::cerr << "Failed setting-up graphics" << std::endl;
        }
    }
  catch (const OpenGLException& e)
    {
      std::cerr << "Caught exception: " << e.message() << std::endl;
      res = false;
    }

  // Release the memory
  release();

  return res;
}

//------------------------------------------------------------------
bool IGLWindow::loop()
{
  do
    {
      computeFPS();
      if (likely(false == draw()))
        {
          std::cerr << "Aborting" << std::endl;
          return false;
        }
      // Swap buffers
      glfwSwapBuffers(m_window);
      glfwPollEvents();
    }
  // Check if the ESC key was pressed or the window was closed
  while (!keyPressed(GLFW_KEY_ESCAPE) && (0 == glfwWindowShouldClose(m_window)));
  return true;
}

} // namespace glwrap
