# Compilation/Installation

OpenGLCppWrapper is mainly made for Linux but it also works for OS X (while I
can hardly test on it because I have to borrow a MacBook).  OpenGLCppWrapper is
yet working for Windows because I do not have windows to develop with.

## Prerequisite

This project depends on:
* [MyMakefile](https://github.com/Lecrapouille/MyMakefile) my GitHub Makefile
  project used for compiling all my GitHub projects. It's automatically cloned
  as a git submodule. You have nothing to do except not to forget to add the
  option `--recurse-submodules` when you git clone the project.
* [OpenGLCppWrapper-data](https://github.com/Lecrapouille/OpenGLCppWrapper-data)
  this repo contains all textures needed for examples. This repo is
  automatically downloaded the Makefile. You have nothing to do.
* [SOIL](https://github.com/kbranigan/Simple-OpenGL-Image-Library.git) this
  library is used for loading picture files (jpeg, png ...). This library is
  downloaded by the Makefile rule `download-external-libs`.
* [Dear IMgui](https://github.com/ocornut/imgui) this library is used for adding
  widgets to your applications. This library is downloaded by the Makefile rule
  `download-external-libs`.
* [backward-cpp](https://github.com/bombela/backward-cpp), this library is used
  for displaying the stack when a crash occured. It is used on debug mode. This
  library is downloaded by the Makefile rule `download-external-libs`.
* glfw glew libraries for OpenGL.

##### Ubuntu, Debian:

```sh
sudo apt-get update && apt-get install libglew-dev libglfw3-dev libbz2-dev
```

##### Mac OS X:
```sh
brew install glfw glew
```

##### Optional:

Optionally, if you want to help developing, want to write/launch unit tests,
install `crpcut` (only runnable with Linux)

```sh
add-apt-repository ppa:jonlar/crpcut
sudo apt-get update && apt-get install crpcut crpcut-dev
```

## Compilation of the API

To download the project, its external libraries and compile the API with its examples:
```sh
git clone --recurse-submodules https://github.com/Lecrapouille/OpenGLCppWrapper.git --depth=1
cd cd OpenGLCppWrapper
make download-external-libs
make compile-external-libs
make
```

**note:** you can add `-j4` or `-j8` for faster the compiler where 4 or 8
depends on the number of core your CPU have.

If you are a developper `make download-external-libs` and `make
compile-external-libs` has to be called once or when you want to upgrade the
libraries (they follow the master branch). These commands play the same role
than a recursive git clone: they will download, compile (but not install) GitHub
libraries: Backward, SOIL and Dear IMgui. You have to do it once (or for
updating). Why? I hate git submodules: it always make things painful. `repo`
with its manifests is a bit overkill, so I prefer Makefile rules or script
shell and this gave me good result with Travis-CI.

After `make` a `build/` folder shall have been created containing the compiled
and runnable files. Two libraries (one static the second dynamic) shall also be
present. You can use them for your project. You can type `sudo make install` to
install them on your system.

If, after that, you want to modify code source, just do `make`. You can type
`make help` for displaying rules.

## Compilation of examples

To compile API examples:
```sh
cd examples/00_Core
make
```

A `build/` folder shall have been created containing the compiled and runnable
files. Run examples:
```sh
./build/OpenGLExamples
```

Will display the list of possible examples. Type `./build/OpenGLExamples 1` for
example for running the 1st example.

## Installation

Multiple versions of this library can coexist thanks to their versioning number.
After the compilation, just type:

```sh
cd OpenGLCppWrapper
sudo make install
```

This will install:
* in `/usr/lib`: the static and shared libraries libOpenGLCppWrapper.
* in `/usr/include/openglcppwrapper-<version>`: all headers files (hpp).
* in `/usr/lib/pkgconfig`: a pkg confile file for linking this API with your future projects.
* in `/usr/share/OpenGLCppWrapper/<version>/`: documentation, examples and other files.

If you do not like the default location, Pass to Makefile options `DESTDIR`,
`PREFIX` and `BINDIR` or directly edit the file `.makefile/Makefile.header`
(note: touching a makefile will force to recompile the whole project).

Check the presence of libraries in your system:
```sh
cd /usr/lib
ls -la libOpenGLCppWrapper*
```
Or better:
```
pkg-config openglcppwrapper --libs
```

## Developpers

### How to use OpenGLCppWrapper in your project?

Add the include in your cpp file: `# include <OpenGLCppWrapper/OpenGLCppWrapper.hpp>`

In debug mode, you can use the following macros:
* `-DCHECK_OPENGL` for checking bad parameters passed to OpenGL routines.
* `-DENABLE_DEBUG` for activating console logs for debugging.

Compile using the pkg-config tool to add compilation flags to GCC or clang:
* `CFLAGS=\`pkg-config openglcppwrapper --cflags\``
* `LDFLAGS=\`pkg-config openglcppwrapper --libs\``
* Note: in pkg-config instead of `openglcppwrapper` which refers to the last installed version,
you can specify your desired version (for example: `openglcppwrapper-0.6`).

Example with a simple file main.cpp:
```
g++ -W -Wall --std=c++11 main.cpp -o prog `pkg-config openglcppwrapper --cflags --libs`
```

Example with Makefile given in [../tests/BasicProject](../tests/BasicProject)

### What code to write in my project ?

* [Tutorials](https://github.com/Lecrapouille/OpenGLCppWrapper/blob/master/doc/tutorials/Core.md)
* [Examples](https://github.com/Lecrapouille/OpenGLCppWrapper/blob/master/examples/README.md)
* [Code Architecture](https://github.com/Lecrapouille/OpenGLCppWrapper/blob/master/doc/Architecture.md)

### Doxygen

Documentation of the code source can be found [here](https://lecrapouille.github.io/OpenGLCppWrapper.github.io/). It can be localy generated as `doc/html/index.html` by typing `make doc`.

### Unit tests

```sh
cd OpenGLCppWrapper/tests
make coverage
```

If all tests passed, a coverage report is created inside `doc/coverage/` and the
`index.html` is opened automatically.
