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

#  include "OpenGL/Context.hpp"
#  include "Math/Vector.hpp"
#  include <GLFW/glfw3.h>
#  include <mutex>

namespace window
{
  static bool const KEY_PRESS = true;
  static bool const KEY_RELEASE = false;

  //! \brief Button type
  enum ButtonType
    {
     LEFT = GLFW_MOUSE_BUTTON_LEFT,
     RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
     MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE,
     ONE = GLFW_MOUSE_BUTTON_1,
     TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT
    };

  //! \brief Structure holding mouse states used by IGLWindow callbacks.
  struct Mouse
  {
    //! \brief position (x and y coordinate)
    Vector2g position;
    //! \brief displacement (delta position)
    Vector2g displacement;
    //! \brief scrolling
    Vector2g scroll;
    //! \brief is cursor visible ?
    bool visible;
    //! \brief button pressed or released ?
    bool pressed;
    //! \brief which button was pressed or released.
    ButtonType button;
  };

  //! \brief Define IO callbacks for IGLWindow
  enum class Event : unsigned
  {
    //! \brief No callback
    None = 0x0,
    //! \brief Keyboard event (pressed and released)
    Keyboard = 0x01,
    //! \brief Mouse button event (pressed and released)
    MouseButton = 0x2,
    //! \brief Mouse motion event (X and Y axis)
    MouseMove = 0x4,
    //! \brief Mouse scroll event
    MouseScroll = 0x8,
    //! \brief All events
    All = 0xFFFF
  };

  constexpr Event operator|(Event lhs, Event rhs)
  {
    return static_cast<Event>(
        static_cast<std::underlying_type<Event>::type>(lhs) |
        static_cast<std::underlying_type<Event>::type>(rhs));
  }

} // namespace window

// *****************************************************************************
//! \class GLWindow GLWindow.hpp
//!
//! \brief GLWindow manages a window for OpenGL. This class is not copyable.
// *****************************************************************************
class IGLWindow : private NonCopyable
{
public:

  //----------------------------------------------------------------------------
  //! \brief Dummy constructor. Does not start the OpenGL context by
  //! security. To do it call start()
  //----------------------------------------------------------------------------
  IGLWindow(uint32_t const width = 1024u, uint32_t const height = 768u,
            const char *title = "");

  //----------------------------------------------------------------------------
  //! \brief Destructor. Release the OpenGL context.
  //----------------------------------------------------------------------------
  virtual ~IGLWindow();

  //----------------------------------------------------------------------------
  //! \brief Enable IO callbacks (ie mouse boutton, mouse scroll, mouse
  //! motion, keyboard).
  //----------------------------------------------------------------------------
  void enableCallbacks(window::Event const events = window::Event::All);

  //----------------------------------------------------------------------------
  //! \brief Start the OpenGL context and starts the rendering loop.
  //----------------------------------------------------------------------------
  bool start();

  //----------------------------------------------------------------------------
  //! \brief Hide and grab the mouse cursor.
  //----------------------------------------------------------------------------
  void hideMouseCursor();

  //----------------------------------------------------------------------------
  //! \brief Make the cursor visible.
  //----------------------------------------------------------------------------
  void showMouseCursor();

  //----------------------------------------------------------------------------
  //! \brief Return the delta time (in ms) with.
  //----------------------------------------------------------------------------
  inline float dt() const { return m_deltaTime; }

  //----------------------------------------------------------------------------
  //! \brief Return the number of frame per seconds.
  //----------------------------------------------------------------------------
  inline uint32_t fps() const { return m_fps; }

  //----------------------------------------------------------------------------
  //! \brief Return the address of the GLFW window.
  //----------------------------------------------------------------------------
  inline GLFWwindow *window() { return m_main_window; }

  //----------------------------------------------------------------------------
  //! \brief Return the structure holding mouse states.
  //! \note this method shall not be called directly.
  //----------------------------------------------------------------------------
  inline window::Mouse& mouse() { return m_mouse; }

  //----------------------------------------------------------------------------
  //! \brief Return the current width of the window.
  //----------------------------------------------------------------------------
  template<typename T>
  inline T width() const { return static_cast<T>(m_width); }

  //----------------------------------------------------------------------------
  //! \brief Return the current height of the window.
  //----------------------------------------------------------------------------
  template<typename T>
  inline T height() const { return static_cast<T>(m_height); }

  //----------------------------------------------------------------------------
  //! \brief Change the position of the window.
  //----------------------------------------------------------------------------
  void setWindowSize(uint32_t const width, uint32_t const height);

//private: // FIXME

  //----------------------------------------------------------------------------
  //! \brief Callback when a key of the keyboard has been pressed or released.
  //----------------------------------------------------------------------------
  inline void onSetKeyAction(size_t const key, bool const action)
  {
    const std::lock_guard<std::mutex> lock(m_mutex);
    m_currentKeys[key] = action;
  }

  //----------------------------------------------------------------------------
  //! \brief Callback when a key of the keyboard has been pressed or released.
  //----------------------------------------------------------------------------
  virtual void onKeyboardEvent()
  {}

  //----------------------------------------------------------------------------
  //! \brief Callback when the mouse has been moved. Default behavior
  //! is to do nothing.
  //----------------------------------------------------------------------------
  virtual void onMouseMoved(window::Mouse const& /*mouse*/)
  {}

  //----------------------------------------------------------------------------
  //! \brief Callback when the mouse has been scrolled. Default behavior
  //! is to do nothing.
  //----------------------------------------------------------------------------
  virtual void onMouseScrolled(window::Mouse const& /*mouse*/)
  {}

  //----------------------------------------------------------------------------
  //! \brief Callback when the mouse has been pressed. Default behavior
  //! is to do nothing.
  //----------------------------------------------------------------------------
  virtual void onMouseButtonPressed(window::Mouse const& /*mouse*/)
  {}

public:

  //----------------------------------------------------------------------------
  //! \brief
  //----------------------------------------------------------------------------
  inline bool isFullscreen() const
  {
    return (m_main_window != nullptr) &&
      (glfwGetWindowMonitor(m_main_window) != nullptr);
  }

  inline bool isKeyDown(size_t const key)
  {
    return window::KEY_PRESS == m_currentKeys[key];
  }

  inline bool isKeyUp(size_t const key)
  {
    return window::KEY_RELEASE == m_currentKeys[key];
  }

  inline bool wasKeyPressed(size_t const key)
  {
    return window::KEY_PRESS == m_currentKeys[key] &&
      window::KEY_RELEASE == m_lastKeys[key];
  }

  inline bool wasKeyReleased(size_t const key)
  {
    return window::KEY_RELEASE == m_currentKeys[key] &&
      window::KEY_PRESS == m_lastKeys[key];
  }

private:

  //----------------------------------------------------------------------------
  //! \brief Compute and display in in the window title the number of
  //! frame per seconds (FPS). Needed for knowing the delta time between
  //! two draw cycles.
  //----------------------------------------------------------------------------
  void computeFPS();

  //----------------------------------------------------------------------------
  //! \brief Callback when the window has its size changed
  //! \param width is never <= 0
  //! \param height is never <= 0
  //----------------------------------------------------------------------------
  virtual void onWindowSizeChanged()
  {}

  //----------------------------------------------------------------------------
  //! \brief Add here all stuffs concerning the init of your 3D game.
  //! \return false for aborting start(), else true for continuing.
  //----------------------------------------------------------------------------
  virtual bool onSetup() = 0;

  //----------------------------------------------------------------------------
  //! \brief Callback triggered when the method setup() failed. By default this
  //! method does nothing.
  //----------------------------------------------------------------------------
  virtual void onSetupFailed() {};

  //----------------------------------------------------------------------------
  //! \brief Add here all stuffs painting your 3D world to be
  //! displayed. This method is called by the start() method.
  //! \return false for halting start(), else return true for continuing.
  //----------------------------------------------------------------------------
  virtual bool onDraw() = 0;

  //----------------------------------------------------------------------------
  //! \brief Callback triggered when the method draw() failed. By default this
  //! method does nothing.
  //----------------------------------------------------------------------------
  virtual void onDrawFailed() {};

  //----------------------------------------------------------------------------
  //! \brief Main loop for displaying the 3D world. Call draw().
  //----------------------------------------------------------------------------
  virtual bool loop();

private:

  //! \brief
  double m_lastTime = 0.0;
  //! \brief
  double m_lastFrameTime = 0.0;
  //! \brief Frames Per Seconds
  uint32_t m_fps = 0;
  //! \brief
  float m_deltaTime = 0.0f;
  //! \brief Windows current width
  uint32_t m_width;
  //! \brief Windows current height
  uint32_t m_height;
  //! \brief Windows title
  const char *m_title = nullptr;
  //! \brief window::Mouse states
  window::Mouse m_mouse;
  //! \brief GLF window context
  GLFWwindow *m_main_window = nullptr;

  //! \brief Keyboard map
  std::vector<char> m_lastKeys;
  std::vector<char> m_currentKeys;
  mutable std::mutex m_mutex;
};

#endif // OPENGLCPPWRAPPER_GLWINDOW_HPP
