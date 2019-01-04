#include "Example01.hpp"
#include "Example02.hpp"
#include "Example03.hpp"
#include <iostream>

#define MAX_EXAMPLES 3

static void usage(char *argv[])
{
  std::cout << "Usage: " << std::endl
            << "  " << argv[0] << " <integer>" << std::endl;
  std::cout << "Where: <integer> is the example id (1 .. " << MAX_EXAMPLES << "): " << std::endl;
  std::cout << "  1: Display a 3d scene with differents objects" << std::endl;
  std::cout << "  2: Display a 3d scene graph with imgui debug" << std::endl;
  std::cout << "  3: Display a generated sphere" << std::endl;
  exit(1);
}

int main(int argc, char *argv[])
{
  if (argc <= 1)
    {
      std::cout << "Missing example id !" << std::endl;
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
      win->start();
      break;
    case 2:
      win = std::make_unique<GLExample02>();
      win->start();
      break;
    case 3:
      win = std::make_unique<GLExample03>();
      win->start();
      break;
    default:
      std::cout << "Incorrect example id !" << std::endl;
      usage(argv);
      return -1;
    }

  return 0;
}
