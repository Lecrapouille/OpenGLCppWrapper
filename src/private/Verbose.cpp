//=====================================================================
// OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of OpenGLCppWrapper.
//
// OpenGLCppWrapper is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with OpenGLCppWrapper.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#include "Verbose.hpp"

namespace glwrap
{

//------------------------------------------------------------------------------
std::string file_name(std::string const& path)
{
  std::string::size_type pos = path.find_last_of("\\/");
  if (pos != std::string::npos)
    return path.substr(pos + 1, std::string::npos);
  return path;
}

//------------------------------------------------------------------------------
PRINTFLIKE(4, 5)
void errout(const char* type, const char* file, const uint32_t line, const char* format, ...)
{
  va_list args;
  va_start(args, format);

  fprintf(stderr, "%s:%s:%u ", type, file_name(file).c_str(), line);
  vfprintf(stderr, format, args);
  fprintf(stderr, "\n");

  va_end(args);
}

} // namespace glwra
