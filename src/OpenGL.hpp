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

#ifndef OPENGL_HPP
#  define OPENGL_HPP

#  include <cstdint>

namespace opengl
{
  //! \brief Create an OpenGL context. Call it before drawing
  //! primitives.

  //-------------------------------------------------------------
  //! \brief Return if the OpenGL has been created or has not been
  //! created or has failed creating.
  //-------------------------------------------------------------
  bool& hasCreatedContext();

  //-------------------------------------------------------------
  //! \brief Allow to detect if the last OpenGL command succeeded or
  //! not. In the case of failure an error is displayed and logged.
  //-------------------------------------------------------------
  void  checkError(const char* file, uint32_t line, const char* expression);

  //! Macro encapsuling the OpenGL command and the fault checker.
#  ifdef CHECK_OPENGL
#    define glCheck(expr) expr; opengl::checkError(SHORT_FILENAME, __LINE__, #expr);
#  else
#    define glCheck(expr) expr;
#  endif

} // namespace opengl

#  include "GLImGUI.hpp"
#  include "GLVAO.hpp"
#  include "GLProgram.hpp"

template<class T>
inline void glBegin(T& obj)
{
  obj.begin();
}

template<class T>
inline void glEnd(T& obj)
{
  obj.end();
}

#endif
