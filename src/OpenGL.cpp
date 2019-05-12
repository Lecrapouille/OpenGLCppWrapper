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

#include "OpenGL.hpp"

namespace glwrap
{

//! This macro will generate code for members.
IMPLEMENT_EXCEPTION(OpenGLException, Exception, "OpenGL Exception")

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
      errout("GLERR", filename, line, "Failed executing '%s'. Reason is '%s'",
             expression, error);
    }
}

} // namespace glwrap
