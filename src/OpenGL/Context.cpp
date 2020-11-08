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

#include "OpenGL/Context.hpp"
#include <atomic>
#include <iostream>
#include <cassert>

namespace GL {

//! This macro will generate code for members.
IMPLEMENT_EXCEPTION(Exception, BaseException, "OpenGL Exception")

namespace Context {

//----------------------------------------------------------------------------
//! \brief Return if the OpenGL has been created or has not been
//! created or has failed creating.
//! \defgroup OpenGL OpenGL wrapper
//!
//! \return true if the OpenGL context has been created
//! else return false (not yet created or failed during
//! its creation).
//----------------------------------------------------------------------------
static std::atomic<bool> created{false};

//----------------------------------------------------------------------------
bool isCreated()
{
    static bool singleton = false;

    if (!created)
    {
        if (!singleton)
        {
            std::cerr << "OpenGL context is not created" << std::endl;
            singleton = true;
        }
    }
    else
    {
        singleton = false;
    }
    return created;
}

//----------------------------------------------------------------------------
void setCreated(bool const v)
{
    created.store(v);
}

} // namespace Context

#  ifdef CHECK_OPENGL
//----------------------------------------------------------------------------
void checkError(const char* filename, const uint32_t line, const char* expression)
{
    GLenum id;
    const char* error;

    while ((id = glGetError()) != GL_NO_ERROR)
    {
        switch (id)
        {
        case GL_INVALID_OPERATION:
            error = "GL_INVALID_OPERATION";
            break;
        case GL_INVALID_ENUM:
            error = "GL_INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            error = "GL_INVALID_VALUE";
            break;
        case GL_OUT_OF_MEMORY:
            error = "GL_OUT_OF_MEMORY";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            error = "GL_INVALID_FRAMEBUFFER_OPERATION";
            break;
        default:
            error = "UNKNOWN";
            break;
        }

        // Do not use directly LOG macros because it will catch this
        // filename and its line instead of the faulty file/line which
        // produced the OpenGL error.
        //errout(stderr, "GLERR:", filename, line, "Failed executing '%s'. Reason is '%s'",
        //       expression, error);
        std::cerr << "GLERR: " << filename << " " << line << ": Failed executing "
                  << expression << ". Reason was " << error << std::endl;
        //assert(false);
    }
}
#  endif // CHECK_OPENGL

} // namespace GL
