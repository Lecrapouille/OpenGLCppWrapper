#include "Example01.hpp"
#include "Example02.hpp"
#include "Example03.hpp"
#include "Example04.hpp"
#include "Example05.hpp"
#include "Example06.hpp"
#include <iostream>
#include <cstdlib>

#define MAX_EXAMPLES 6

__attribute__((__noreturn__))
static void usage(char *argv[])
{
  std::cout << "Usage: " << std::endl
            << "  " << argv[0] << " <integer>" << std::endl;
  std::cout << "Where: <integer> is the example id (1 .. " << MAX_EXAMPLES << "): " << std::endl;
  std::cout << "  1: Display a 3d scene with differents objects" << std::endl;
  std::cout << "  2: Display a 3d scene graph with imgui debug" << std::endl;
  std::cout << "  3: Display a generated sphere" << std::endl;
  std::cout << "  4: Display a multi-textured plane" << std::endl;
  std::cout << "  5: Display a cubemap texture" << std::endl;
  std::cout << "  6: Display a 3D texture" << std::endl;
  exit(1);
}

int main(int argc, char *argv[])
{
  if (argc <= 1)
    {
      std::cerr << "Missing example id !" << std::endl;
      usage(argv);
    }
  long int example = strtol(argv[1], nullptr, 10);
  if ((0L == example) || (example > MAX_EXAMPLES))
    {
      usage(argv);
    }

  std::unique_ptr<IGLWindow> win;

  switch (example)
    {
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
      win = std::make_unique<GLExample04>();
      break;
    case 5:
      win = std::make_unique<GLExample05>();
      break;
    case 6:
      win = std::make_unique<GLExample06>();
      break;
    default:
      std::cerr << "Incorrect example id !" << std::endl;
      usage(argv);
      return -1;
    }

  return win->start() ? EXIT_SUCCESS : EXIT_FAILURE;
}
