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

#include "00_Window/00_BasicWindow.hpp"
#include "00_Window/01_WindowIOEvents.hpp"
#include "00_Window/02_WindowImGui.hpp"

#include "01_Core/00_SimpleTriangle.hpp"
#include "01_Core/01_DynamicTriangle.hpp"
#include "01_Core/02_TexturedTriangle.hpp"
#include "01_Core/03_MultiTexturedTriangle.hpp"
#include "01_Core/04_TexturedBox.hpp"
#include "01_Core/05_MVPTexturedBox.hpp"
#include "01_Core/06_MultipleObjects.hpp"
#include "01_Core/07_IndexedVBO.hpp"
#include "01_Core/08_TerrainTexture3D.hpp"
#include "01_Core/09_SkyBoxTextureCube.hpp"
#include "01_Core/10_SkyBoxShapeCamera.hpp"
#include "01_Core/11_SimpleFrameBuffer.hpp"
#include "01_Core/12_PostProdFrameBuffer.hpp"
#include "01_Core/13_ComplexShader.hpp"
#include "01_Core/14_BasicLighting.hpp"

#include "02_Scene/00_BasicMaterial.hpp"
#include "02_Scene/01_DepthMaterial.hpp"
#include "02_Scene/02_NormalsMaterial.hpp"
#include "02_Scene/03_SceneNodes.hpp"
#include "02_Scene/04_SceneGraph.hpp"
//#include "02_Scene/05_SceneFog.hpp"
//#include "02_Scene/06_SceneLight.hpp"

#include <iostream>
#include <stdlib.h>
#include <limits.h>

#define MAX_EXAMPLES 24

//------------------------------------------------------------------------------
__attribute__((__noreturn__))
static void usage(char *argv[])
{
  std::cout << "Usage: " << std::endl
            << "  " << argv[0] << " <integer>" << std::endl;
  std::cout << "Where: <integer> is the example id (0 .. " << MAX_EXAMPLES << "): " << std::endl;
  std::cout << "Window/UI/IO:" << std::endl;
  std::cout << "  0: Empty window rendering nothing." << std::endl;
  std::cout << "  1: Empty window reacting to IO events." << std::endl;
  std::cout << "  2: Empty window with dear a Dear ImGui widget." << std::endl;
  std::cout << "Core API/OpenGL wrapper:" << std::endl;
  std::cout << "  3: Render a simple mono-color triangle." << std::endl;
  std::cout << "  4: Render a triangle with vertex positions changing over time." << std::endl;
  std::cout << "  5: Render a textured triangle." << std::endl;
  std::cout << "  6: Render a multi-textured triangle." << std::endl;
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
  std::cout << " 24: Show a scene graph with lights." << std::endl;
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
  try
    {
      switch (id)
        {
        case 0:
          win = std::make_unique<BasicWindow>();
          break;
        case 1:
          win = std::make_unique<WindowIOEvents>();
          break;
        case 2:
          win = std::make_unique<WindowImGui>();
          break;
        case 3:
          win = std::make_unique<SimpleTriangle>();
          break;
        case 4:
          win = std::make_unique<DynamicTriangle>();
          break;
        case 5:
          win = std::make_unique<TexturedTriangle>();
          break;
        case 6:
          win = std::make_unique<MultiTexturedTriangle>();
          break;
        case 7:
          win = std::make_unique<TexturedBox>();
          break;
        case 8:
          win = std::make_unique<MVPTexturedBox>();
          break;
        case 9:
          win = std::make_unique<MultipleObjects>();
          break;
        case 10:
          win = std::make_unique<IndexedVBO>();
          break;
        case 11:
          win = std::make_unique<TerrainTexture3D>();
          break;
        case 12:
          win = std::make_unique<SkyBox>();
          break;
        case 13:
          win = std::make_unique<SkyBoxShapeCamera>();
          break;
        case 14:
          win = std::make_unique<SimpleFrameBuffer>();
          break;
        case 15:
          win = std::make_unique<PostProdFrameBuffer>();
          break;
        case 16:
          win = std::make_unique<ComplexShader>();
          break;
        case 17:
          win = std::make_unique<BasicLighting>();
          break;
        case 18:
          win = std::make_unique<ShapeBasicMaterial>();
          break;
        case 19:
          win = std::make_unique<ShapeDepthMaterial>();
          break;
        case 20:
          win = std::make_unique<ShapeNormalsMaterial>();
          break;
        case 21:
          win = std::make_unique<SceneNodes>();
          break;
        case 22:
          win = std::make_unique<RobotsSceneGraph>();
          break;
          /*case 23:
          win = std::make_unique<SceneFog>();
          break;
        case 24:
          win = std::make_unique<SceneLight>();
          break;*/
        default:
          std::cerr << "Incorrect example id !" << std::endl;
          std::cerr << "'" << argv[1] << "' is not a valid example id !" << std::endl;
          usage(argv);
          return EXIT_FAILURE;
        }
      return win->start() ? EXIT_SUCCESS : EXIT_FAILURE;
    }
  catch (const OpenGLException& e)
    {
      // Caught OpenGLException from constructors
      ERROR("Caught exception: '%s'", e.message().c_str());
      return EXIT_FAILURE;
    }
}
