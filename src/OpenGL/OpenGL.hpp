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

#ifndef OPENGLCPPWRAPPER_OPENGL_HPP
#  define OPENGLCPPWRAPPER_OPENGL_HPP

// *****************************************************************************
//! \file OpenGL.hpp OpenGL routines.
// *****************************************************************************

#  include "OpenGL/GLEnum.hpp"
#  include "Common/GPUMemory.hpp"
#  include "Common/Exception.hpp"
#  include "Math/Maths.hpp"
#  include <atomic>

namespace glwrap
{

// ***********************************************************************************************
//! \brief This macro will declare a class OpenGLException derived from Exception.
// ***********************************************************************************************
DECLARE_EXCEPTION(OpenGLException, Exception)

//! \brief Return true if the OpenGL context has been created else false.
bool isContextCreated();

//! \brief Set the state of the OpenGL context creation.
//! \param[in] v the new state: true for a created OpenGL context, false else.
//! \fixme Make this function private.
void setContextCreated(bool const v = true);

//----------------------------------------------------------------------------
//! \brief Allow to detect if the last OpenGL command succeeded or failed.
//! In the case of failure an error is displayed on console and/or logged.
//!
//! \note do not use this function directly but use the macro glCheck.
//!
//! \param filename the file path where the OpenGL routine was called.
//! \param line the line where the OpenGL routine was called.
//! \param expression the line content where the OpenGL routine was called.
//----------------------------------------------------------------------------
void checkError(const char* filename, const uint32_t line, const char* expression);

//----------------------------------------------------------------------------
//! Macro encapsuling the OpenGL command and the fault checker.
//----------------------------------------------------------------------------
#  ifdef CHECK_OPENGL
#    define glCheck(expr) expr; glwrap::checkError(__FILE__, __LINE__, #expr);
#  else
#    define glCheck(expr) expr;
#  endif

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_OPENGL_HPP
