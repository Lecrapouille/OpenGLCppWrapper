#ifndef OPENGLCPPWRAPPER_DEBUG_HPP
#  define OPENGLCPPWRAPPER_DEBUG_HPP

#  include "NonCppStd.hpp"
#  include <stdio.h>
#  include <stdarg.h>
#  include <stdlib.h>
#  include <cstring>

namespace glwrap
{

//------------------------------------------------------------------------------
//! \brief give the file name with its extension from a given path
//------------------------------------------------------------------------------
std::string file_name(std::string const& path);

//------------------------------------------------------------------------------
//! \brief
//------------------------------------------------------------------------------
PRINTFLIKE(4, 5)
void errout(const char* type, const char* file, const uint32_t line, const char* format, ...);

#  ifdef ENABLE_DEBUG
#    define DEBUG_HELPER(format, ...) errout("DEBUG", __FILE__, __LINE__, format "%s", __VA_ARGS__)
#    define DEBUG(...) DEBUG_HELPER(__VA_ARGS__, "")
#  else
#    define DEBUG(format, ...)
#  endif

#  ifndef ERROR
#    define ERROR_HELPER(format, ...) errout("ERROR", __FILE__, __LINE__, format "%s", __VA_ARGS__)
#    define ERROR(...) ERROR_HELPER(__VA_ARGS__, "")
#  endif

} // namespace glwrap

#endif // OPENGLCPPWRAPPER_DEBUG_HPP
