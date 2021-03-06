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

#include "00_BasicWindow.hpp"
#include <iostream>
#include <stdlib.h>
#include <limits.h>

//------------------------------------------------------------------------------
int main()
{
  std::unique_ptr<IGLWindow> win;
  try
    {
      win = std::make_unique<BasicWindow>();
      return win->start() ? EXIT_SUCCESS : EXIT_FAILURE;
    }
  catch (const OpenGLException& e)
    {
      // Caught OpenGLException from constructors
      ERROR("Caught exception: '%s'", e.message().c_str());
      return EXIT_FAILURE;
    }
}
