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

#include "debug.hpp"

#include "00_Window/00_BasicWindow.hpp"
#include "00_Window/01_BasicWindowIOEvents.hpp"
#include "00_Window/02_BasicWindowImGui.hpp"
#include "01_Core/00_ColorfulTriangle.hpp"
#include "01_Core/01_DynamicTriangle.hpp"
#include "01_Core/02_TexturedTriangle.hpp"
#include "01_Core/03_MultiTexturedTriangle.hpp"
#include "01_Core/04_IndexedQuad.hpp"
#include "01_Core/05_RotatingQuad.hpp"
#include "01_Core/06_IndexedSphere.hpp"

#include <iostream>
#include <stdlib.h>
#include <limits.h>

#define MAX_EXAMPLES 27
#define WIDTH 800
#define HEIGHT 600

//------------------------------------------------------------------------------
__attribute__((__noreturn__))
static void usage(char *argv[])
{
    std::cout << "Usage: " << std::endl
              << "  " << argv[0] << " <integer>" << std::endl;
    std::cout << "Where: <integer> is the example id (0 .. " << MAX_EXAMPLES << "): " << std::endl;
    std::cout << "Window:" << std::endl;
    std::cout << "  0: " << BasicWindow::info() << std::endl;
    std::cout << "  1: " << BasicWindowIOEvents::info() << std::endl;
    std::cout << "  2: " << BasicWindowImGui::info() << std::endl;
    std::cout << "Core API/OpenGL wrapper:" << std::endl;
    std::cout << "  3: " << ColorfulTriangle::info() << std::endl;
    std::cout << "  4: " << DynamicTriangle::info() << std::endl;
    std::cout << "  5: " << TexturedTriangle::info() << std::endl;
    std::cout << "  6: " << MultiTexturedTriangle::info() << std::endl;
    std::cout << "  7: " << IndexedQuad::info() << std::endl;
    std::cout << "  8: " << RotatingQuad::info() << std::endl;
    std::cout << "  9: " << IndexedSphere::info() << std::endl;


    /*
    std::cout << "  7: Render a textured box without model-view-project (MVP) transformation." << std::endl;
    std::cout << "  8: Render a rotating textured Box using MVP transformation." << std::endl;
    std::cout << "  9: Render multiple objects (VAO vs VBO)." << std::endl;
    std::cout << " 10: Render a sphere using an indexed VBO." << std::endl;
    std::cout << " 11: Render a terrain built from a texture 3D." << std::endl;
    std::cout << " 12: Render a basic skybox (texture cube)." << std::endl;
    std::cout << " 13: Render a rotating box inside a skybox and controled by a camera." << std::endl;
    std::cout << " 14: Render a 2D black screen from framebuffer" << std::endl;
    std::cout << " 15: Render a box an apply a 2D post-production effect." << std::endl;
    std::cout << " 16: Run a complex shader found on https://shaderfrog.com." << std::endl;
    std::cout << " 17: Basic light." << std::endl;
    std::cout << "Scene Graph:" << std::endl;
    std::cout << " 18: Display a geometry primitive with a basic material." << std::endl;
    std::cout << " 19: Display a geometry primitive with a depth material." << std::endl;
    std::cout << " 20: Display a geometry primitive with a normal material." << std::endl;
    std::cout << " 21: Show different scene nodes." << std::endl;
    std::cout << " 22: Show different way to create a scene graph." << std::endl;
    std::cout << " 23: Show a scene graph with fog." << std::endl;
    std::cout << " 24: Show a tree with alpha testing." << std::endl;
    std::cout << " 25: Show a scene graph with lights." << std::endl;
    std::cout << "Examples from ThreeJs:" << std::endl;
    std::cout << " 26: Cones pointing to a moving cube." << std::endl;*/
    exit(EXIT_FAILURE);
}

//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    // Need the id of the desired example
    if (argc <= 1)
    {
        std::cerr << "Missing example id !" << std::endl;
        usage(argv);
    }

    // Get the id of the example
    char *endptr;
    long int id = strtol(argv[1], &endptr, 10);
    if (endptr == argv[1] || *endptr != '\0' ||
        ((id == LONG_MIN || id == LONG_MAX) && errno == ERANGE))
    {
        std::cerr << "'" << argv[1] << "' is not a valid example id !" << std::endl;
        usage(argv);
    }

    GLApplication app;
    try
    {
        switch (id)
        {
        case 0:
            app.create<BasicWindow>(WIDTH, HEIGHT, "BasicWindow");
            break;
        case 1:
            app.create<BasicWindowIOEvents>(WIDTH, HEIGHT, "IO events");
            break;
        case 2:
            app.create<BasicWindowImGui>(WIDTH, HEIGHT, "DearImGui");
            break;
        case 3:
            app.create<ColorfulTriangle>(WIDTH, HEIGHT, "Colorful Triangle");
            break;
        case 4:
            app.create<DynamicTriangle>(WIDTH, HEIGHT, "Dynamic Triangle");
            break;
        case 5:
            app.create<TexturedTriangle>(WIDTH, HEIGHT, "Textured Triangle");
            break;
        case 6:
            app.create<MultiTexturedTriangle>(WIDTH, HEIGHT, "Multi Textured Triangle");
            break;
        case 7:
            app.create<IndexedQuad>(WIDTH, HEIGHT, "EBO Quad");
            break;
        case 8:
            app.create<RotatingQuad>(WIDTH, HEIGHT, "Rotating Quad");
            break;
        case 9:
            app.create<IndexedSphere>(WIDTH, HEIGHT, "EBO Sphere");
            break;
        default:
            std::cerr << "Incorrect example id !" << std::endl;
            std::cerr << "'" << argv[1] << "' is not a valid example id !" << std::endl;
            usage(argv);
            return EXIT_FAILURE;
        }
        return app.start() ? EXIT_SUCCESS : EXIT_FAILURE;
    }
    catch (const GL::Exception& e)
    {
        std::cerr << "Caught exception from constructors: " << e.message() << std::endl;
        return EXIT_FAILURE;
    }
}
