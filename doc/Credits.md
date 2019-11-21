# Credits

## Why another C++ wrapper for OpenGL?

Short answer: I disliked others C++ API I found on Github! The interesting
libraries are all made in foreign language for my initial project
[SimTaDyn](https://github.com/Lecrapouille/SimTaDyn).

Long answer:

* Contrary to legacy OpenGL, learning modern OpenGL (aka OpenGL Core Profile),
  is more difficult for beginners.

* As a beginner of OpenGL Core, I always get confused by the order of
  creation/binding of OpenGL objects. With OpenGLCppWrapper, you'll do not have
  to really know what is OpenGL VBOs, VAOs, framebuffer, shader
  attributes, ... and in what order they have to be created or bound. What you
  probably prefer to deal with questions like: "what texture fit the best to my
  model?" or "What is the list of triangles for my model?" but certainly not
  low-level issue questions like "is myTexture.jpg is bound to GL_TEXTURE0 or
  GL_TEXTURE1 ?".

* When working on my personal project
  [SimTaDyn](https://github.com/Lecrapouille/SimTaDyn) I was looking for a C++
  API wrapping OpenGL routines. The first need was to hide the OpenGL
  boilerplate. I looked for such a kind of OpenGL library on GitHub but I found
  none which allowed me to write OpenGL scenes in few lines. The second need, is
  that OpenGL does not manage dynamic size VBOs easily (as far as I know). I
  needed to edit maps with an arbitrary number of nodes and arcs which is
  problematic with fixed size VBO (OpenGL is made for games and characters are
  rendering with almost the same number of vertices (if we except tessellation
  with geometry shaders).

* C++ APIs I saw on GitHub just give a C++ flavor'ish taste to OpenGL C
  functions. None of them have real class inheritance wrapping OpenGL objects
  and encapsulate for hiding the misery of calling routines. As a consequence,
  you still have to deal with the correct order of calling routines.

* APIs like [ogl](https://github.com/opengl-tutorials/ogl) or
  [OOGL](https://github.com/Overv/OOGL) or
  [gl](https://github.com/acdemiralp/gl) are wrong (even with thousands of
  stars) because there is too much code like `glBindBuffer`,
  `BindAttribute(... 3, 42 * sizeof(float))` or `glVertexAttribPointer`,
  `program.set_uniform(program.uniform_location` that I absolutely do not want
  to see in a high-level code.

* I also wanted an API that not only content to initialize VBOs during the init
  phase (meshes, color, textures ...), but that also allows you to manipulate
  your 3D scene data during the runtime from the CPU point of view: the API will
  transmit automatically to the GPU all modified elements for their
  display. This is, of course, less performant that manipulating them from
  shaders code, but this offers you extra flexibility (for scientific tools for
  example).

* The number of public interfaces of OpenGLCppWrapper is reduced as much as
  possible. Only OpenGL constants used as parameters in public methods are
  wrapped by C++ enum for using the strong type checking during the compilation
  of the project, avoiding to pass 'carrots' while 'potatoes' were expected.

* The only API demarking to others was
  [Glumpy](https://github.com/glumpy/glumpy). It was the API I was looking for
  but unfortunately written in Python! I had to make its portage into C++. I
  also discovered later [opengl4csharp](https://github.com/giawa/opengl4csharp),
  written in C#, quite similar to Glumpy but I think some designs are less good.

* I found no good C++ OpenGL wrapper API, so I make my own largely inspired by
  Glumpy which takes the opposite direction over OpenGL tutorials: it does not
  start with a list of triangles, to push them in VBOs and finish coding with
  shaders (and depending on which OpenGL version, uses or not a VAO) it does the
  inverse: it starts from shaders and hides for you the generation of VBOs,
  attributes, uniforms, texture samplers. As a developper, you'll have just to
  request the shader program to instanciate a new 3D model (aka VAO), initialize
  it with data the shader needs: vertices, colors, texture positions. Finally
  paint the model with the bound shader.

## Credits

OpenGLCppWrapper is largely inspired by projects such as:

* OpenGL wrappers:
  * [Glumpy](https://github.com/glumpy/glumpy) Python+Numpy+OpenGL: fast,
  scalable and beautiful scientific visualization
  * [opengl4csharp](https://github.com/giawa/opengl4csharp) OpenGL 4 Bindings
    (partially based on OpenTK) for C#

* Rendering Engine:
  * [three.js](https://github.com/mrdoob/three.js)
  * [scg3](https://github.com/vahlers/scg3) An extensible OpenGL 3 / C++11 scene
  graph library for teaching computer graphics along the programmable pipeline.


OpenGLCppWrapper/src/OpenGL is greatly inspired by Glumpy/gloo but it is not a
full-feature portage of Glumpy or opengl4csharp.

* The first reason is due to the fact that C++ is a strongly typed language that
  does not offer the same flexibility than python. For example in C++ storing
  template classes in the same container is not possible.

* The second reason is, maybe I'm totally wrong, that Glumpy does not use by
  default VAOs (because it uses OpenGL < 3.3) but access to VBOs through OpenGL
  program attributes. This also perturbs me because in the case of you have
  several models (therefore several VAOs) to paint this seems problematic to
  draw them. In my API only VAOs are directly created. They hold VBOs and
  texture once bound to GLProgram. The result is equivalent to Glumpy but the
  internal code is different. See this file
  [differences](https://github.com/Lecrapouille/OpenGLCppWrapper/blob/master/doc/Differences.md)
  for more differences.
