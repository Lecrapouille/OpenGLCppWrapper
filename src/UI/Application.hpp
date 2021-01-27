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

// *****************************************************************************
//! \class GLWindow GLWindow.hpp
//!
//! \brief GLWindow manages a window for OpenGL. This class is not copyable.
// *****************************************************************************
class GLApplication : private NonCopyable
{
public:

    GLApplication();

    //--------------------------------------------------------------------------
    //! \brief Destructor. Release the OpenGL context.
    //--------------------------------------------------------------------------
    virtual ~GLApplication();

    //--------------------------------------------------------------------------
    //! \brief
    //--------------------------------------------------------------------------
    bool start();

    static GLWindow* window()
    {
        return m_window;
    }

    static void makeContextCurrent(GLWindow* current)
    {
        if ((current != nullptr) && (current->m_context != nullptr))
        {
            glfwMakeContextCurrent(current->m_context);
            m_window = current;
        }
    }

    //--------------------------------------------------------------------------
    //! \brief Dummy constructor. Does not start the OpenGL context by
    //! security. To do it call start()
    //--------------------------------------------------------------------------
    template<class Window>
    size_t create(uint32_t const width, uint32_t const height, const char *title)
    {
        // Save current active context info so we can restore it later!
        GLWindow* previous_context = window();
        std::unique_ptr<GLWindow> win
                = std::make_unique<Window>(std::max(16u, width),
                                           std::max(16u, height),
                                           (title == nullptr) ? "" : title);

        makeContextCurrent(win.get());
        glfwSwapInterval(1); // Enable vsync
        GL::Context::setCreated();

        initGlew();

        m_windows.push_back(std::move(win));

        // Restore context
        makeContextCurrent(previous_context);

        // Return the index
        return m_windows.size() - 1u;
    }

    //--------------------------------------------------------------------------
    //! \brief
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

    static std::list<std::unique_ptr<GLWindow>> m_windows;
    static GLWindow* m_window; // Current context
};

#endif // OPENGLCPPWRAPPER_GLAPPLICATION_HPP
