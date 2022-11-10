# OpenGLCppWrapper

## What is OpenGLCppWrapper Library ?

[OpenGLCppWrapper](https://github.com/Lecrapouille/OpenGLCppWrapper) is a
C++11 wrapper for writing OpenGL `Core Profile` applications in few lines of
code.

**Warning:** Do not confuse Core Profile OpenGL (>= 2.2) with Legacy OpenGL (<=
2.1). This API does not manage OpenGL Legacy, but manages OpenGL version 3.3.

This API is split in two parts:

- The first API part (functional), greatly been inspired by the
  [Glumpy](https://github.com/glumpy/glumpy) project, is object-oriented based
  does not just content to give a C++ flavor name to OpenGL routines but hides
  for you the complexity of calling in the correct order OpenGL functions as
  well as passing correct parameters to them. You, as developer, will have just
  to deal writing your OpenGL shaders (GLSL language), asking to
  OpenGLCppWrapper to compile them, and filling data to populated VBOs (such as
  vertices position, texture coordinates, normals ...) or loading the desired
  textures image (png, jpeg) to populated samplers. Nothing more complex indeed,
  OpenGLCppWrapper will instantiate classes wrapping OpenGL VAOs, VBOs, texture,
  framebuffers once shaders have been compiled ... It also manages OpenGL object
  lifetime: it makes automatic the transfer of "dirty" CPU data (from your C++
  space memory point of view) to the graphics card memory (VBOs, textures).

- The second API part (not functional, rework in progress), greatly been
  inspired by the [three.js](https://github.com/mrdoob/three.js) project gives
  you more abstraction, by offering some basic functionalities such as geometric
  primitives, materials, camera, lights and scene graph. This part will simplify
  you creating OpenGL scenes.

**Warning:** This library is not a game engine but just an oriented object
wrapper for OpenGL that can be used for a game engine. As drawbacks, this API is
less cache friendly than calling directly your OpenGL routines (due to a lot of
if-then-else, smart pointers and dynamic cast). This is the price of hiding the
complexity and staying generic.

### Table Of Contents

* [Installation](https://github.com/Lecrapouille/OpenGLCppWrapper/blob/master/doc/Install.md)
* [Examples](https://github.com/Lecrapouille/OpenGLCppWrapper/blob/master/examples/Examples.md)
* [Software Architecture](https://github.com/Lecrapouille/OpenGLCppWrapper/blob/master/doc/Architecture.md)
* [Credits](https://github.com/Lecrapouille/OpenGLCppWrapper/blob/master/doc/Credits.md)
