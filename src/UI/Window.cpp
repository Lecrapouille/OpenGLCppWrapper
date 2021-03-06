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

#include "UI/Window.hpp"
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <cassert>

namespace glwrap
{

//------------------------------------------------------------------------------
//! \brief Callback triggered when GLFW failed.
//------------------------------------------------------------------------------
__attribute__((__noreturn__))
static void on_GLFW_error(int /*errorCode*/, const char* msg)
{
  throw OpenGLException(msg);
}

//------------------------------------------------------------------------------
//! \brief Display the GPU memory consumption in human readable format
//------------------------------------------------------------------------------
static void display_gpu_memory()
{
  static size_t previous_gpu_mem = 0_z;
  size_t current_gpu_mem = GPUMemory();

  if (previous_gpu_mem != current_gpu_mem)
    {
      previous_gpu_mem = current_gpu_mem;

      std::cout << "Estimated GPU memory usage: "
                << current_gpu_mem << " bytes"
                << std::endl;
    }
}

//------------------------------------------------------------------------------
//! \brief Static function allowing to "cast" a function pointer to a
//! method pointer. This function is triggered when the mouse is
//! moved.
//------------------------------------------------------------------------------
static void on_mouse_moved(GLFWwindow* obj, double xpos, double ypos)
{
  static double m_lastX = 0.0;
  static double m_lastY = 0.0;
  static bool m_firstMouse = true;

  assert(nullptr != obj);
  IGLWindow* window = static_cast<IGLWindow*>(glfwGetWindowUserPointer(obj));
  window::Mouse& mouse = window->mouse();

  mouse.position = Vector2g(xpos, ypos);

  if (m_firstMouse)
    {
      m_lastX = xpos;
      m_lastY = ypos;
      m_firstMouse = false;
    }

  // Reversed since y-coordinates go from bottom to top
  mouse.displacement = Vector2g(xpos - m_lastX, m_lastY - ypos);

  m_lastX = xpos;
  m_lastY = ypos;

  window->onMouseMoved(mouse);
}

//------------------------------------------------------------------------------
//! \brief Static function allowing to "cast" a function pointer to a
//! method pointer. This function is triggered when the mouse button
//! is scrolled.
//------------------------------------------------------------------------------
static void on_mouse_scrolled(GLFWwindow* obj, double xoffset, double yoffset)
{
  assert(nullptr != obj);
  IGLWindow* window = static_cast<IGLWindow*>(glfwGetWindowUserPointer(obj));
  window::Mouse& mouse = window->mouse();

  mouse.scroll = Vector2g(xoffset, yoffset);
  window->onMouseScrolled(mouse);
}

//------------------------------------------------------------------------------
//! \brief Static function allowing to "cast" a function pointer to a
//! method pointer. This function is triggered when the mouse has been pressed.
//------------------------------------------------------------------------------
static void on_mouse_button_pressed(GLFWwindow* obj, int button, int action, int /*mods*/)
{
  assert(nullptr != obj);
  IGLWindow* window = static_cast<IGLWindow*>(glfwGetWindowUserPointer(obj));
  window::Mouse& mouse = window->mouse();

  mouse.button = static_cast<window::ButtonType>(button);
  mouse.pressed = (action == GLFW_PRESS); // else GLFW_RELEASE
  window->onMouseButtonPressed(mouse);
}

//------------------------------------------------------------------------------
//! \brief
//------------------------------------------------------------------------------
static void on_keyboard_event(GLFWwindow* obj, int key, int /*scancode*/, int action, int /*mods*/)
{
  assert(nullptr != obj);
  IGLWindow* window = static_cast<IGLWindow*>(glfwGetWindowUserPointer(obj));
  if (action == GLFW_PRESS)
    window->onSetKeyAction(static_cast<size_t>(key), window::KEY_PRESS);
  else if (action == GLFW_RELEASE)
    window->onSetKeyAction(static_cast<size_t>(key), window::KEY_RELEASE);
}

//------------------------------------------------------------------------------
//! \brief Static function allowing to "cast" a function pointer to
//! a method pointer. This function is triggered when the window
//! has been resized.
//------------------------------------------------------------------------------
static void on_window_resized(GLFWwindow* obj, int width, int height)
{
  assert(nullptr != obj);
  IGLWindow* window = static_cast<IGLWindow*>(glfwGetWindowUserPointer(obj));
  if (unlikely(0 >= width)) { width = 1; }
  if (unlikely(0 >= height)) { height = 1; }
  window->setWindowSize(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
}

//------------------------------------------------------------------------------
IGLWindow::IGLWindow(uint32_t const width, uint32_t const height, const char *title)
  : m_width(width),
    m_height(height),
    m_title(title),
    m_lastKeys(GLFW_KEY_LAST + 1),
    m_currentKeys(GLFW_KEY_LAST + 1)
{

  DEBUG("%s", "============= OpenGL Context creation ==========================================");

  if (unlikely(nullptr == m_title)) { m_title = ""; }
  if (unlikely(0u == m_width)) { m_width = 1u; }
  if (unlikely(0u == m_height)) { m_height = 1u; }

    // Initialise GLFW
  glfwSetErrorCallback(on_GLFW_error);
  if (!glfwInit())
    throw OpenGLException("Failed to initialize GLFW");

  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  // Open a window and create its OpenGL context
  m_main_window = glfwCreateWindow(static_cast<int>(m_width),
                                   static_cast<int>(m_height),
                                   m_title, nullptr, nullptr);
  if (nullptr == m_main_window)
    throw OpenGLException("Failed to open GLFW window");

  glfwMakeContextCurrent(m_main_window);
  glfwSwapInterval(1); // Enable vsync

  // Initialize GLEW
  glewExperimental = GL_TRUE; // stops glew crashing on OSX :-/
  if (GLEW_OK != glewInit())
    throw OpenGLException("Failed to initialize GLFW");

  // Print out some info about the graphics drivers
  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

  // Make sure OpenGL version 3.2 API is available
  if (!GLEW_VERSION_3_2)
    throw OpenGLException("OpenGL 3.2 API is not available!");

  setContextCreated();

  DEBUG("%s", "============= OpenGL Context created ===========================================");
}

//------------------------------------------------------------------------------
IGLWindow::~IGLWindow()
{
  if (nullptr != m_main_window)
    glfwDestroyWindow(m_main_window);
  glfwTerminate();
}

//------------------------------------------------------------------------------
void IGLWindow::hideMouseCursor()
{
  if (likely(nullptr != m_main_window))
    {
      m_mouse.visible = false;
      glfwSetInputMode(m_main_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

//------------------------------------------------------------------------------
void IGLWindow::showMouseCursor()
{
  if (likely(nullptr != m_main_window))
    {
      m_mouse.visible = true;
      glfwSetInputMode(m_main_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

//------------------------------------------------------------------------------
void IGLWindow::setWindowSize(uint32_t const width, uint32_t const height)
{
  m_width = width;
  m_height = height;

  if (unlikely(0u == m_width)) { m_width = 1u; }
  if (unlikely(0u == m_height)) { m_height = 1u; }

  // Callback to be implemented by the derived class
  onWindowSizeChanged();
}

//------------------------------------------------------------------------------
void IGLWindow::computeFPS()
{
  static uint32_t nbFrames = 0u;
  double currentTime = glfwGetTime();
  m_deltaTime = static_cast<float>(currentTime - m_lastFrameTime);
  m_lastFrameTime = currentTime;
  ++nbFrames;

  if (currentTime - m_lastTime >= 1.0)
    {
      // If last prinf() was more than 1sec ago printf and reset
      std::ostringstream oss;
      m_fps = nbFrames;
      int ms_by_frame = static_cast<int>(1000.0f / static_cast<float>(m_fps));
      oss << '[' << m_fps << " FPS, " << ms_by_frame << " ms] " << m_title;
      glfwSetWindowTitle(m_main_window, oss.str().c_str());
      nbFrames = 0u;
      m_lastTime += 1.0;
    }
}

//------------------------------------------------------------------------------
static window::Event operator&(window::Event lhs, window::Event rhs)
{
  return static_cast<window::Event>(
        static_cast<std::underlying_type<window::Event>::type>(lhs) &
        static_cast<std::underlying_type<window::Event>::type>(rhs));
}

//------------------------------------------------------------------------------
// \fixme callbacksOn(Event::All) + callbacksOn(Event::None) => callbacks are
// not removed.
void IGLWindow::enableCallbacks(window::Event const events)
{
  if ((events & window::Event::MouseMove) != window::Event::None)
    glfwSetCursorPosCallback(m_main_window, on_mouse_moved);
  if ((events & window::Event::MouseScroll) != window::Event::None)
    glfwSetScrollCallback(m_main_window, on_mouse_scrolled);
  if ((events & window::Event::MouseButton) != window::Event::None)
    glfwSetMouseButtonCallback(m_main_window, on_mouse_button_pressed);
  if ((events & window::Event::Keyboard) != window::Event::None)
    glfwSetKeyCallback(m_main_window, on_keyboard_event);
}

//------------------------------------------------------------------------------
bool IGLWindow::start()
{
  // Save the class address to "cast" function callback into a method
  // callback
  glfwSetWindowUserPointer(m_main_window, this);

  // I/O callbacks
  glfwSetFramebufferSizeCallback(m_main_window, on_window_resized);

  // Ensure we can capture keyboard being pressed below
  glfwSetInputMode(m_main_window, GLFW_STICKY_KEYS, GL_TRUE);

  for (size_t i = GLFW_KEY_SPACE; i <= GLFW_KEY_LAST; ++i)
    m_lastKeys[i] = m_currentKeys[i] = window::KEY_RELEASE;

  // Flush OpenGL errors before using this function on real OpenGL
  // routines else a fake error is returned on the first OpenGL
  // routines while valid.
  glCheck();

  // Init OpenGL states of the user application
  DEBUG("%s", "============= SETUP ============================================================");

  try
    {
      if (unlikely(!setup()))
        {
          ERROR("Failed setting-up graphics");
          onSetupFailed();
          return false;
        }

      // Force refreshing computation made when window changed
      onWindowSizeChanged();
    }
  catch (const OpenGLException& e)
    {
      ERROR("Caught exception during setting-up graphics: '%s'",
            e.message().c_str());
      onSetupFailed();
      return false;
    }

  // init FPS
  m_lastTime = glfwGetTime();
  m_lastFrameTime = m_lastTime;
  m_fps = 0;

  // Draw OpenGL scene
  return loop();
}

//------------------------------------------------------------------------------
bool IGLWindow::loop()
{
  do
    {
      DEBUG("%s", "============= LOOP =============================================================");

      display_gpu_memory();
      computeFPS();

      try
        {
          if (likely(false == draw()))
            {
              ERROR("Failed drawing graphics");
              onDrawFailed();
              return false;
            }
        }
      catch (const OpenGLException& e)
        {
          ERROR("Caught exception during drawing graphics: '%s'",
                e.message().c_str());
          onDrawFailed();
          return false;
        }

      // Swap buffers
      glfwSwapBuffers(m_main_window);

      // Update window events (input etc)
      {
        glfwPollEvents();
        const std::lock_guard<std::mutex> lock(m_mutex);
        onKeyboardEvent();
        for (size_t i = GLFW_KEY_SPACE; i <= GLFW_KEY_LAST; ++i)
          m_lastKeys[i] = m_currentKeys[i];
      }
    }
  // Check if the ESC key was pressed or the window was closed
  // Note: use keyPressed() not isKeyDown() because if Event::Keyboard is not we
  // no longer can halt the window.
  while ((GLFW_PRESS != glfwGetKey(m_main_window, GLFW_KEY_ESCAPE)) &&
         (0 == glfwWindowShouldClose(m_main_window)));
  return true;
}

} // namespace glwrap
