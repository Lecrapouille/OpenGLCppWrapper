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

#ifndef OPENGLCPPWRAPPER_GLWINDOW_HPP
#  define OPENGLCPPWRAPPER_GLWINDOW_HPP

// *****************************************************************************
//! \file GLWindow.hpp manages a window and its i/o for drawing OpenGL scenes.
// *****************************************************************************

#  include "OpenGL.hpp"
#  include <GLFW/glfw3.h>

namespace glwrap
{

// *****************************************************************************
//! \class GLWindow GLWindow.hpp
//!
//! \brief GLWindow manages a window for OpenGL.
// *****************************************************************************
class IGLWindow
{
public:

  //------------------------------------------------------------------
  //! \brief Dummy constructor. Does not start the OpenGL context by
  //! security. To do it call start()
  //------------------------------------------------------------------
  IGLWindow(uint32_t const width = 1024u, uint32_t const height = 768u,
            const char *title = "");

  //------------------------------------------------------------------
  //! \brief Destructor. Release the OpenGL context.
  //------------------------------------------------------------------
  virtual ~IGLWindow();

  //------------------------------------------------------------------
  //! \brief Start the OpenGL context and starts the rendering loop.
  //------------------------------------------------------------------
  bool start();

  //------------------------------------------------------------------
  //! \brief Hide mouse cursor.
  //------------------------------------------------------------------
  inline void hideMouseCursor() const
  {
    if (nullptr != m_window)
    {
      glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
  }

  //------------------------------------------------------------------
  //! \brief Return the delta time (in ms) with.
  //------------------------------------------------------------------
  inline float dt() const
  {
    return m_deltaTime;
  }

  //------------------------------------------------------------------
  //! \brief Return the address of the GLFW window.
  //------------------------------------------------------------------
  inline GLFWwindow *window()
  {
    return m_window;
  }

  //------------------------------------------------------------------
  //! \brief Return the current width of the window.
  //------------------------------------------------------------------
  inline uint32_t width() const { return m_width; }

  //------------------------------------------------------------------
  //! \brief Return the current height of the window.
  //------------------------------------------------------------------
  inline uint32_t height() const { return m_height; }

  //------------------------------------------------------------------
  //! \brief Change the position of the window.
  //------------------------------------------------------------------
  void setWindowSize(uint32_t const width, uint32_t const height);

  //------------------------------------------------------------------
  //! \brief Check if the keyboard has been pressed.
  //------------------------------------------------------------------
  inline bool keyPressed(const int key) const
  {
    if (nullptr != m_window)
      return GLFW_PRESS == glfwGetKey(m_window, key);
    return false;
  }

  //------------------------------------------------------------------
  //! \brief Callback when the mouse has been moved. Default behavior
  //! is to do nothing.
  //------------------------------------------------------------------
  virtual void onMouseMoved(const double /*xpos*/, const double /*ypos*/)
  {}

  //------------------------------------------------------------------
  //! \brief Callback when the mouse has been scrolled. Default behavior
  //! is to do nothing.
  //------------------------------------------------------------------
  virtual void onMouseScrolled(const double /*xoffset*/, const double /*yoffset*/)
  {}

private:

  //------------------------------------------------------------------
  //! \brief Compute and display in in the window title the number of
  //! frame per seconds (FPS). Needed for knowing the delta time between
  //! two draw cycles.
  //------------------------------------------------------------------
  void computeFPS();

  //------------------------------------------------------------------
  //! \brief Callback when the window has its size changed
  //! \param width is never <= 0
  //! \param height is never <= 0
  //------------------------------------------------------------------
  virtual void onWindowSizeChanged(const float width, const float height) = 0;

  //------------------------------------------------------------------
  //! \brief Add here all stuffs concerning the init of your 3D game.
  //! \return false for aborting start(), else true for continuing.
  //------------------------------------------------------------------
  virtual bool setup() = 0;

  //------------------------------------------------------------------
  //! \brief Add here all stuffs painting your 3D world to be
  //! displayed. This method is called by the start() method.
  //! \return false for halting start(), else return true for continuing.
  //------------------------------------------------------------------
  virtual bool draw() = 0;

  //------------------------------------------------------------------
  //! \brief Main loop for displaying the 3D world. Call draw().
  //------------------------------------------------------------------
  virtual bool loop();

  //------------------------------------------------------------------
  //! \brief Release your 3D models from memory created by setup().
  //! This method is not called by the destructor because of it's virtual.
  //------------------------------------------------------------------
  virtual void release()
  {
    /* By default no 3D resources has to released */
  }

private:

  double m_lastTime = 0.0;
  double m_lastFrameTime = 0.0;
  int m_fps = 0;
  float m_deltaTime = 0.0f;

  uint32_t m_width;
  uint32_t m_height;
  const char *m_title;
  GLFWwindow *m_window = nullptr;
};

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_GLWINDOW_HPP
