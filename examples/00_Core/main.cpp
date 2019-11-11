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

#include "00_EmptyWindowWithIOEvents.hpp"
#include "01_EmptyWindowWithImgui.hpp"
#include "02_DynamicTriangle.hpp"
#include "03_MultiTexturedTriangle.hpp"
#include "05_VAOvsVBO.hpp"
#include "06_SphereEBO.hpp"
#include "07_SkyBoxTextureCube.hpp"
#include "08_TerrainTexture3D.hpp"
#include "09_SceneGraph.hpp"
#include "10_SimpleFrameBuffer.hpp"
#include "11_PostProdFrameBuffer.hpp"
#include "12_ComplexShader.hpp"
#include "13_Lighting.hpp"
#include "14_MapEditor.hpp"
#include "15_Geometry.hpp"
#include <iostream>
#include <map>
#include <stdlib.h>
#include <limits.h>

#define MAX_EXAMPLES 15

//------------------------------------------------------------------------------
__attribute__((__noreturn__))
static void usage(char *argv[])
{
  std::cout << "Usage: " << std::endl
            << "  " << argv[0] << " <integer>" << std::endl;
  std::cout << "Where: <integer> is the example id (0 .. " << MAX_EXAMPLES << "): " << std::endl;
  std::cout << "  0: Create an empty window reacting to IO events." << std::endl;
  std::cout << "  1: Create an empty window with dear a Daear IMGui widget." << std::endl;
  std::cout << "  2: Draw a triangle moving and blinking." << std::endl;
  std::cout << "  3: Draw a multi textured triangle." << std::endl;
  std::cout << "  4: TODO" << std::endl;
  std::cout << "  5: Multiple VAOs versus multiple VBOs." << std::endl;
  std::cout << "  6: Draw a sphere using VBO and EBO." << std::endl;
  std::cout << "  7: Draw a cylinder inside a skybox (cubic texture)." << std::endl;
  std::cout << "  8: Create a terrain using a 3d texture." << std::endl;
  std::cout << "  9: Draw 3 moving robots using a scene graph." << std::endl;
  std::cout << " 10: Draw a 2D square in a framebuffer." << std::endl;
  std::cout << " 11: Draw a cube in a framebuffer." << std::endl;
  std::cout << " 12: Universe Nursery" << std::endl;
  std::cout << " 13: Light + material" << std::endl;
  std::cout << " 14: Basic Map editor" << std::endl;
  std::cout << " 15: Geometry and SceneGraph" << std::endl;
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

  std::unique_ptr<IGLWindow> win;
  switch (id)
    {
    case 0:
      win = std::make_unique<GLExample00>();
      break;
    case 1:
      win = std::make_unique<GLExample01>();
      break;
    case 2:
      win = std::make_unique<GLExample02>();
      break;
    case 3:
      win = std::make_unique<GLExample03>();
      break;
    case 4:
      std::cerr << "Coming soon" << std::endl;
      //win = std::make_unique<GLExample04>();
      return EXIT_FAILURE;
      //break;
    case 5:
      win = std::make_unique<GLExample05>();
      break;
    case 6:
      win = std::make_unique<GLExample06>();
      break;
    case 7:
      win = std::make_unique<GLExample07>();
      break;
    case 8:
      win = std::make_unique<GLExample08>();
      break;
    case 9:
      win = std::make_unique<GLExample09>();
      break;
    case 10:
      win = std::make_unique<GLExample10>();
      break;
    case 11:
      win = std::make_unique<GLExample11>();
      break;
    case 12:
      win = std::make_unique<GLExample12>();
      break;
    case 13:
      win = std::make_unique<GLExample13>();
      break;
    case 14:
      win = std::make_unique<GLExample14>();
      break;
    case 15:
      win = std::make_unique<GLExample15>();
      break;
    default:
      std::cerr << "Incorrect example id !" << std::endl;
      std::cerr << "'" << argv[1] << "' is not a valid example id !" << std::endl;
      usage(argv);
      return EXIT_FAILURE;
    }

  return win->start() ? EXIT_SUCCESS : EXIT_FAILURE;
}
