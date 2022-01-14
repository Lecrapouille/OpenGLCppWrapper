# OpenGLCppWrapper

## What is OpenGLCppWrapper Library ?

[OpenGLCppWrapper](https://github.com/Lecrapouille/OpenGLCppWrapper) is a
C++11 wrapper for writing OpenGL `Core Profile` applications in few lines of
code.

**Warning:** Do not confuse Core Profile OpenGL (>= 2.2) with Legacy OpenGL (<=
2.1). This API does not manage OpenGL Legacy but manages OpenGL version 3.3.

This API is made in two parts:

- the first part hides for you the complexity of calling in the correct order
  OpenGL functions as well as passing correct parameters to them. This API uses
  object-oriented and does not just content to give a C++ flavor name to OpenGL
  routines. You, as developer, will have just to deal with writing your OpenGL
  shaders (GLSL language) ask to OpenGLCppWrapper to compile
  them. OpenGLCppWrapper will instantiate classes wrapping OpenGL VAOs, VBOs,
  texture, framebuffers ... Then, you will just have to write the C++ code
  filling your 3d models by calling your desired textures and loading data for
  filling your VBOs (such as vertices position, texture coordinates, normals ..)
  and which and when 3D model to paint. Nothing more complex! This API also
  manages OpenGL object lifetime: it makes the transfer of "dirty" CPU data
  (from your C++ space memory point of view) to the graphics card memory (VBOs,
  textures). This part has been greatly been inspired by the
  https://github.com/glumpy/glumpy API.

- the second part (work in progress) gives you more abstraction, by offering
  some basic functionalities such as geometric primitives, materials, camera,
  lights and scene graph. This part has been greatly been inspired by the
  https://github.com/mrdoob/three.js/ API.

**Warning:** This library is not a game engine but just an oriented object
wrapper for OpenGL that can be used for a game engine. As drawbacks, this API is
less cache friendly than calling directly your OpenGL routines (due to a lot of
if-then-else, smart pointers and dynamic cast). This is the price of hiding the
complexity and staying generic.

### Table Of Contents

* [Installation](https://github.com/Lecrapouille/OpenGLCppWrapper/blob/master/doc/Install.md)
* [Examples](https://github.com/Lecrapouille/OpenGLCppWrapper/blob/master/examples/Examples.md)
* [Code Architecture](https://github.com/Lecrapouille/OpenGLCppWrapper/blob/master/doc/Architecture.md)
* [Credits](https://github.com/Lecrapouille/OpenGLCppWrapper/blob/master/doc/Credits.md)

### Gerrit as code review

https://review.gerrithub.io/admin/repos/Lecrapouille/OpenGLCppWrapper,general
