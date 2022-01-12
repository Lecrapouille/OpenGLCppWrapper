//=====================================================================
// OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
// Copyright 2018-2022 Quentin Quadrat <lecrapouille@gmail.com>
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

#include "Editor.hpp"

#include <iostream>
#include <stdlib.h>
#include <limits.h>
#include <thread>

#ifndef WINDOW_INITIAL_WIDTH
#  define WINDOW_INITIAL_WIDTH (800u)
#endif
#ifndef WINDOW_INITIAL_HEIGHT
#  define WINDOW_INITIAL_HEIGHT (600u)
#endif

//------------------------------------------------------------------------------
int main(/*int argc, char *argv[]*/)
{
    std::unique_ptr<GLWindow> app = std::make_unique<Editor>(WINDOW_INITIAL_WIDTH,
        WINDOW_INITIAL_HEIGHT, "OpenGLCppWrapper Editor");
    try
    {
        return app->run() ? EXIT_SUCCESS : EXIT_FAILURE;
    }
    catch (const GL::Exception& e)
    {
        std::cerr << "Aborting! Caught exception: " << e.message() << std::endl;
    }

    return EXIT_FAILURE;
}
