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
#include "OpenGL.hpp"
#include <stdexcept>
#include <iostream>
#include <sstream>

__attribute__((__noreturn__))
static void on_error(int /*errorCode*/, const char* msg)
{
  throw std::runtime_error(msg);
}

static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
  IGLWindow* obj = (IGLWindow*) glfwGetWindowUserPointer(window);

  if (nullptr == obj) return ;
  obj->onMouseMoved(xpos, ypos);
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  IGLWindow* obj = (IGLWindow*) glfwGetWindowUserPointer(window);

  if (nullptr == obj) return ;
  if (width < 0) { width = 1; }
  if (height < 0) { height = 1; }

  obj->setWindowSize(width, height);
}

IGLWindow::IGLWindow(uint32_t const width, uint32_t const height, const char *title)
  : m_width(width),
    m_height(height),
    m_title(title)
{
  opengl::hasCreatedContext() = false;
}

/* Close OpenGL window and terminate GLFW */
IGLWindow::~IGLWindow()
{
  if (opengl::hasCreatedContext())
    {
      release();
      glfwTerminate();
    }
}

void IGLWindow::release()
{
  /* By default no 3D resources has to released */
}

void IGLWindow::FPS()
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

bool IGLWindow::start()
{
  if (opengl::hasCreatedContext())
    {
      std::cerr << "Warning you called twice start(). "
                << "OpenGL context already created"
                << std::endl;
      goto l_update;
    }

  int res;

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

  // Open a window and create its OpenGL context
  m_window = glfwCreateWindow(m_width, m_height, m_title, nullptr, nullptr);
  if (nullptr == m_window)
    {
      std::cerr << "Failed to open GLFW window" << std::endl;
      glfwTerminate();
      return false;
    }
  glfwMakeContextCurrent(m_window);
  glfwSwapInterval(1); // Enable vsync

  // Initialize GLEW
  glewExperimental = GL_TRUE; //stops glew crashing on OSX :-/
  if (GLEW_OK != glewInit())
    {
      std::cerr << "Failed to initialize GLFW" << std::endl;
      glfwTerminate();
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
      glfwTerminate();
      return false;
    }

  // Save the class address to "cast" function callback into a method
  // callback
  glfwSetWindowUserPointer(m_window, this);
  glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
  glfwSetCursorPosCallback(m_window, mouse_callback);
  //glfwSetScrollCallback(m_window, scroll_callback);

  // Ensure we can capture keyboard being pressed below
  glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);
  try
    {
      // This is an awful hack but this is to be sure to flush OpenGL
      // errors before using this function on real OpenGL routines else a
      // fake error is returned on the first OpenGL routines while valid.
      glGetError();
      res = setup();
    }
  catch (const OpenGLException& e)
    {
      std::cerr << e.message() << std::endl;
      res = false;
    }

  if (false == res)
    {
      std::cerr << "Failed setting-up graphics" << std::endl;
      glfwTerminate();
      return res;
    }

  // init FPS
  m_lastTime = glfwGetTime();
  m_lastFrameTime = m_lastTime;
  m_fps = 0;

  opengl::hasCreatedContext() = true;

l_update:
  update();
  return true;
}

void IGLWindow::update()
{
  try
    {
      do
        {
          FPS();
          if (false == draw())
            {
              std::cerr << "Aborting" << std::endl;
              return ;
            }
          // Swap buffers
          glfwSwapBuffers(m_window);
          glfwPollEvents();
        }
      // Check if the ESC key was pressed or the window was closed
      while ((GLFW_PRESS != glfwGetKey(m_window, GLFW_KEY_ESCAPE)) &&
             (0 == glfwWindowShouldClose(m_window)));
    }
  catch (const OpenGLException& e)
    {
      std::cerr << e.message() << std::endl;
    }
}
