//=====================================================================
// OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of OpenGLCppWrapper.
//
// OpenGLCppWrapper is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with OpenGLCppWrapper.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef GLWINDOW_HPP_
#  define GLWINDOW_HPP_

#  include <GL/glew.h>
#  include <GLFW/glfw3.h>
#  include "GLException.hpp"

// **************************************************************
//! \brief
// **************************************************************
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
  //! \brief Destructor. Release the OpenGL context. Call the virtual
  //! methode stop().
  //------------------------------------------------------------------
  virtual ~IGLWindow();

  //------------------------------------------------------------------
  //! \brief Start the OpenGL context and starts the rendering loop.
  //------------------------------------------------------------------
  bool start();

  //------------------------------------------------------------------
  //! \brief Return the delta time (in ms) with.
  //------------------------------------------------------------------
  inline float dt() const
  {
    return m_deltaTime;
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  inline GLFWwindow *obj()
  {
    return m_window;
  }

  inline uint32_t width() const { return m_width; }
  inline uint32_t height() const { return m_height; }
  inline void setWindowSize(uint32_t const width, uint32_t const height)
  {
    m_width = width;
    m_height = height;

    // Calbback to be implemented by the derived class
    onWindowSizeChanged(static_cast<float>(width),
                        static_cast<float>(height));
  }

private:

  //------------------------------------------------------------------
  //! \brief Private function needed by dt(). Compute and display in
  //! in the window title the number of frame per seconds (FPS).
  //------------------------------------------------------------------
  void FPS();

  //------------------------------------------------------------------
  //! \brief Callback when the window has its size changed
  //! \param width is never <= 0
  //! \param height is never <= 0
  //------------------------------------------------------------------
  virtual void onWindowSizeChanged(const float width, const float height) = 0;

  //------------------------------------------------------------------
  //! \brief Virtual method. Add here all stuffs concerning the init
  //! of your 3D game.
  //! \return false for aborting start(), else true for continuing.
  //------------------------------------------------------------------
  virtual bool setup() = 0;

  //------------------------------------------------------------------
  //! \brief Virtual method. Add here all stuffs painting your 3D
  //! world to be displayed. This method is called by the udpdate()
  //! method.
  //! \return false for halting update(), else true for continuing.
  //------------------------------------------------------------------
  virtual bool draw() = 0;

  //------------------------------------------------------------------
  //! \brief Main loop for displaying the 3D world. Call draw().
  //------------------------------------------------------------------
  void update();

  //------------------------------------------------------------------
  //! \brief Virtual method. Release your 3D models from memory.
  //! Called by the destructor.
  //------------------------------------------------------------------
  virtual void release();

private:

  double m_lastTime;
  double m_lastFrameTime;
  int m_fps;
  float m_deltaTime;

  uint32_t m_width;
  uint32_t m_height;
  const char *m_title;
  GLFWwindow *m_window = nullptr;
};

#endif /* GLWINDOW_HPP_ */
