//=====================================================================
// OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
// Copyright 2018-2020 Quentin Quadrat <lecrapouille@gmail.com>
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
#include "01_Core/03_MultiTexturedSquare.hpp"
#include "01_Core/04_IndexedQuad.hpp"
#include "01_Core/05_RotatingQuad.hpp"
#include "01_Core/06_IndexedSphere.hpp"
#include "01_Core/07_MultipleObjects.hpp"
#include "01_Core/08_TerrainTexture3D.hpp"
#include "01_Core/09_SkyBoxTextureCube.hpp"
#include "01_Core/10_SkyBoxShape.hpp"
#include "01_Core/11_ComplexShader.hpp"

/*#include "01_Core/10_PostProdFrameBuffer.hpp"
#include "01_Core/12_BasicLighting.hpp"*/

#include "02_Scene/00_DepthMaterial.hpp"

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
    std::cout << "Window API:" << std::endl;
    std::cout << "  0: " << BasicWindow::info() << std::endl;
    std::cout << "  1: " << BasicWindowIOEvents::info() << std::endl;
    std::cout << "  2: " << BasicWindowImGui::info() << std::endl;
    std::cout << "OpenGL wrapper API:" << std::endl;
    std::cout << "  3: " << ColorfulTriangle::info() << std::endl;
    std::cout << "  4: " << DynamicTriangle::info() << std::endl;
    std::cout << "  5: " << TexturedTriangle::info() << std::endl;
    std::cout << "  6: " << MultiTexturedSquare::info() << std::endl;
    std::cout << "  7: " << IndexedQuad::info() << std::endl;
    std::cout << "  8: " << RotatingQuad::info() << std::endl;
    std::cout << "  9: " << IndexedSphere::info() << std::endl;
    std::cout << " 10: " << MultipleObjects::info() << std::endl;
    std::cout << " 11: " << TerrainTexture3D::info() << std::endl;
    std::cout << " 12: " << SkyBoxTextureCube::info() << std::endl;
    std::cout << " 13: " << SkyBoxShape::info() << std::endl;
    std::cout << " 14: " << ComplexShader::info() << std::endl;
    /*std::cout << " 13: " << PostProdFrameBuffer::info() << std::endl;
    std::cout << " 15: " << BasicLighting::info() << std::endl;*/
    std::cout << "SceneGraph API:" << std::endl;
    std::cout << " 16: " << ShapeDepthMaterial::info() << std::endl;
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
            app.create<MultiTexturedSquare>(WIDTH, HEIGHT, "Multi Textured Triangle");
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
        case 10:
            app.create<MultipleObjects>(WIDTH, HEIGHT, "Multiple Objects");
            break;
        case 11:
            app.create<TerrainTexture3D>(WIDTH, HEIGHT, "Terrain 3D Texture");
            break;
        case 12:
            app.create<SkyBoxTextureCube>(WIDTH, HEIGHT, "SkyBox Cube Texture");
            break;
        case 13:
            app.create<SkyBoxShape>(WIDTH, HEIGHT, "Shape inside a skybox");
            break;
        case 14:
            app.create<ComplexShader>(WIDTH, HEIGHT, "Complex Shader");
            break;
        /*case 14:
            app.create<PostProdFrameBuffer>(WIDTH, HEIGHT, "PostProd FrameBuffer");
            break;
 */
        case 16:
            app.create<ShapeDepthMaterial>(WIDTH, HEIGHT, "Depth Material");
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
