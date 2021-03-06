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

#include "OpenGL/OpenGL.hpp"

namespace glwrap
{

//! This macro will generate code for members.
IMPLEMENT_EXCEPTION(OpenGLException, Exception, "OpenGL Exception")

//----------------------------------------------------------------------------
//! \brief Return if the OpenGL has been created or has not been
//! created or has failed creating.
//! \defgroup OpenGL OpenGL wrapper
//!
//! \return true if the OpenGL context has been created
//! else return false (not yet created or failed during
//! its creation).
//----------------------------------------------------------------------------
static std::atomic<bool> OpenGLContextCreated{false};

//----------------------------------------------------------------------------
bool isContextCreated()
{
  DEBUG("OpenGL Context >>>>>>>>>>>>>>>>>>>>>>>>>>>>> %u", OpenGLContextCreated.load());
  return OpenGLContextCreated;
}

//----------------------------------------------------------------------------
void setContextCreated(bool const v)
{
  OpenGLContextCreated.store(v);
  DEBUG("OpenGL Context <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< %u", OpenGLContextCreated.load());
}

//----------------------------------------------------------------------------
void throwIfNoOpenGLContext(std::string const& msg)
{
  if (unlikely(!isContextCreated()))
    throw OpenGLException("OpenGL Context is not yet created. " + msg);
}

//----------------------------------------------------------------------------
void throwIfNoOpenGLContext()
{
  if (unlikely(!isContextCreated()))
    throw OpenGLException("OpenGL Context is not yet created");
}

//----------------------------------------------------------------------------
void throwIfOpenGLClassCalledBeforeContext()
{
  throwIfNoOpenGLContext("Make this instance called after GLWindow constructor");
}

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
      errout(stderr, "GLERR:", filename, line, "Failed executing '%s'. Reason is '%s'",
             expression, error);
    }
}

} // namespace glwrap
