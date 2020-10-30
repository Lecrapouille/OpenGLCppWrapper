#ifndef GLCONTEXT_HPP
#  define GLCONTEXT_HPP

#  include <GL/glew.h>
#  include "Common/Exception.hpp"

namespace GL {

// ***********************************************************************************************
//! \brief This macro will declare a class OpenGLException derived from Exception.
// ***********************************************************************************************
DECLARE_EXCEPTION(Exception, BaseException)

namespace Context {

bool isCreated();
void setCreated(bool const v = true);

} // namespace Context

#  ifdef CHECK_OPENGL
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
#    define glCheck(expr) expr; GL::checkError(__FILE__, __LINE__, #expr);
#  else
#    define glCheck(expr) expr;
#  endif

} // namespace GL

#endif
