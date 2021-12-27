//=====================================================================
// OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
// Copyright 2018-2021 Quentin Quadrat <lecrapouille@gmail.com>
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

#ifndef OPENGLCPPWRAPPER_GLAPPLICATION_HPP
#  define OPENGLCPPWRAPPER_GLAPPLICATION_HPP

#  include "UI/Window.hpp"
#  include <list>
#  include <iostream>
#  include <mutex>

// *****************************************************************************
//! \brief GLWindow manages several OpenGL windows. This class is not copyable.
// *****************************************************************************
class GLApplication : private NonCopyable
{
public:

    //--------------------------------------------------------------------------
    //! \brief initialize GLFW as OpenGL Core 3.3. No windows are created. Call
    //! create() as many as desired windows. Then call start().
    //--------------------------------------------------------------------------
    GLApplication();

    //--------------------------------------------------------------------------
    //! \brief Destructor. Release the GLFW context. Virtual because of virtual
    //! methods.
    //--------------------------------------------------------------------------
    ~GLApplication();

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    static GLApplication& application()
    {
        return *m_application;
    }

    //--------------------------------------------------------------------------
    //! \brief For each windows attached to this instance, call their init()
    //! methods and loop for ever on their update() methods. The loop is broken
    //! once one of the windows ends (error, closed, ...).
    //!
    //! \pre Need at least one window to be functional. So be sure that create()
    //! has been called before.
    //!
    //! \return true on success (or windows closed). Return false when one of
    //! the window failed.
    //--------------------------------------------------------------------------
    bool start();

    //--------------------------------------------------------------------------
    //! \brief Return the current windows and set its OpenGL context as current.
    //! \pre Need at least one window to be functional. Else an assert is
    //! triggered. So be sure that create() has been called before.
    //--------------------------------------------------------------------------
    GLWindow& getCurrentWindow()
    {
        assert(m_current != nullptr && "No windows have been created");
        return *m_current;
    }

    //--------------------------------------------------------------------------
    //! \brief Set the windows as current and its OpenGL conext as current.
    //--------------------------------------------------------------------------
    void setCurrentWindow(GLWindow& win)
    {
        assert(win.m_context != nullptr && "The windows has no OpenGL conext");
        glfwMakeContextCurrent(win.m_context);
        m_current = &win;
    }

    //--------------------------------------------------------------------------
    //! \brief Create a new OpenGL window. Does not start the OpenGL context by
    //! security.
    //! \tparam W: class deriving from GLWindow
    //--------------------------------------------------------------------------
    template<class W>
    size_t create(uint32_t const width, uint32_t const height, const char *title)
    {
        static_assert(std::is_base_of<GLWindow, W>::value,
                      "Template W is not derived class from GLWindow");

        // Save current active context info so we can restore it after the
        // creation of the new window.
        GLWindow& context = getCurrentWindow();

        m_windows.push_back(std::make_unique<W>(
                             std::max(16u, width), std::max(16u, height),
                             (title == nullptr) ? "" : title));

        setCurrentWindow(*(m_windows.back().get()));
        glfwSwapInterval(1); // Enable vsync
        GL::Context::setCreated();
        initGlew();

        // Restore context
        setCurrentWindow(context);

        // Return the index
        return m_windows.size() - 1u;
    }

    //--------------------------------------------------------------------------
    //! \brief stqtic method initializing Glew
    //--------------------------------------------------------------------------
    static void initGlew()
    {
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK)
            throw GL::Exception("Failed to initialize GLFW");

        // Print out some info about the graphics drivers
        std::cout << std::endl;
        std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
        std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
        std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
        std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
        std::cout << std::endl;

#  pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wold-style-cast"

        // Make sure OpenGL version 3.3 API is available
        if (!GLEW_VERSION_3_3)
            throw GL::Exception("OpenGL 3.3 API is not available!");

#   pragma GCC diagnostic pop
    }

private:

    static GLApplication* m_application;
    //! \brief List of windows attached to the application.
    std::list<std::unique_ptr<GLWindow>> m_windows;
    //! \brief Current OpenGL context.
    GLWindow* m_current;
    //! \brief Protect m_windows.
    // FIXME std::mutex m_mutex;
};

#endif // OPENGLCPPWRAPPER_GLAPPLICATION_HPP
