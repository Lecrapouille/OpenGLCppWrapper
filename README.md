# OpenGLCppWrapper

A C++11 OpenGL Core (3.3) wrapper in the aim to write OpenGL code
within few lines. This API is largely inspired by the
[GLumpy](https://github.com/glumpy/glumpy) python project.

## Why another C++ wrapper for OpenGL ?

Short answer: I disliked others API I found on github ! Long answer:

* C++ APIs I saw on github just give a C++ flavor'ish taste to OpenGL
  functions. None of them have real class inheritance over OpenGL objects.

* This API allows to write 3D scenes in really few lines of C++ code
  with almost no knowledge about OpenGL. You just have to understand
  the OpenGL shader language: GLSL.

* Public interfaces of this API are strictly reduced to minimum while
  allowing to do all the wanted job.

* As a beginner of OpenGL Core, I always get confused by the order of
  creation/binding of OpenGL objects. With this API you do not have to
  really know what are OpenGL VBOs, VAOs, attributes, ... and in what
  order they have to be created or binded. This API is here to hide
  all the boilerplate for you.

* This API takes the opposite direction over OpenGL tutorials: it does
  not start with a list of triangles, to push them in VBOs and finish
  coding with shaders and depending on which OpenGL version VAO. This
  API does the inverse: it starts from shaders and hide
  for you the generation of VBOs, attributes, uniforms, texture
  sampler. You have just to request from the shader to create for you
  a new 3D object (aka VAO) that you will init with data you wanted
  (aka VBO): vertices, color, texture position.

* This API does not only content to init VBOs during the init phase,
  it also allows you to manipulate your 3D scene data during the
  run-time (meshes, color ...) from the CPU: the API will transmit
  automatically to the GPU all modified elements for their display.

* So there is something

## How I decided to make it ?

* The first need from this API is that contrary to legacy OpenGL,
  learning modern OpenGL (aka OpenGL core), is very difficult for
  beginners.

* When workign on my personal project
  [SimTaDyn](https://github.com/Lecrapouille/SimTaDyn) I was looking
  for an OpenGL wrapper in C++. The first need was to hide OpenGL
  boilerplate. I looked for a such kind of OpenGL library on github
  but I founded none which allowed me to write OpenGL scenes in few
  lines. The second need from this API is that OpenGL does not manage
  dynamic size VBOs easily (as far as I know). I needed to edit maps
  with an arbitrary number of nodes and arcs which is problematic with
  fixed size VBO (OpenGL is made for games and characters are
  rendering with almost the same number of vertices (if we except
  teslations with geometry shaders).

* [GLumpy](https://github.com/glumpy/glumpy) was the OpenGL wrapper I
  was looking for but unfortunately it is written in Python so I had
  to make the portage into C++ and here the creation of this API !
  **Note: this is not a full-feature portage:**
  * The first reason is due to the fact that C++ is a strongly typed
  language which does not offer the same flexibility than python. For
  examples: storing template classes in the same container like Numpy
  views is difficult.
  * The second reason is that GLumpy does not use by default VAOs but
  access to VBOs through OpenGL program attributes, which also
  perturbs me because in the case of you have several models
  (therefore several VAOs) in the scene to paint this library seems
  less well conceived to draw them but thanks to typage flexibility of
  Python, attributes of the shader can be bind to VAOs. In my API only
  VAOs have to be created and only them can be binded to GLProgram. The
  result is equivalent to GLumpy the code is a little different.
  * There are also some minor changes: See this file
  [differences](https://github.com/Lecrapouille/OpenGLCppWrapper/blob/master/doc/Differences.md)
  for more information. Mains changes are the begin() code which adds
  more checks. OpenGL handles type are not very consistent in the
  Core API this can makes compilation warnings. This is why IGLObject
  is a template class. I moved code from create() to setup() like for
  VBOs this allows to remove some checks that GLumpy does. I create a
  GLSampler class inheriting from GLUniform which does exist in
  GLumpy.

## Work in progress

__Please help contributing !__

I'm moving this library outside SimTaDyn into a new github repo
[OpenGLCppWrapper](https://github.com/Lecrapouille/OpenGLCppWrapper).
I think people are more interested by OpenGL than GIS. This API is
still in alpha version but allows to draw some basic 3D dynamic scenes
in few lines of code. I also truly believe that describing the code
[architecture](https://github.com/Lecrapouille/OpenGLCppWrapper/blob/master/doc/Architecture.md)
is something really important as well for developers than users.

## What can you reach with this API

Here is the pseudo C++ code you'll have to write for obtaining a basic
3D scene. Please compare the length of this code with any "hello
triangle" OpenGL tutorial. Explanations come after the code !

```
GLProgram prog;
GLVAO vao1, vao2;

// Init phase:
{
  // Load shaders
  GLVertexShader   vertex_shader.loadFromFile("my_vertex_shader.glsl");
  GLFragmentShader fragment_shader.loadFromFile("my_fragment_shader.glsl");

  // Compile and link shaders into a GLProgram
  if (!prog.attachShaders(vertex_shader, fragment_shader).compile())
    exit(-1);

  // Init model 1 (for example a cube)
  prog.bind(vao1);
  prog.attribute<Vector3f>("a_position") = { ... };

  // Init model 2 (for example a plane)
  prog.bind(vao2);
  prog.attribute<Vector3f>("a_position") = { ... };

  // Init uniforms
  prog.uniform<float>("u_scale") = 1.0f;
}

// Runtime phase:
{
  // Draw model 1
  prog.bind(vao1);
  prog.draw(GL_TRIANGLES, 0, 36);

  // Draw model 2
  prog.bind(vao2);
  prog.draw(GL_TRIANGLES);
}
```

#### Explanations

Init phase:
* You have to load and compile your vertex and fragment shader (GLSL
  language).
* You have to link them into the class GLProgram.
* If your OpenGL has been compiled with success, it will create the
  list of uniform and attributes used in shaders. In this example
  the attribute named "a_position" because in this example in our
  shader code we have `attribute vec2 a_texcoord`.
* You create a dummy VAO and bind it to your GLProgram this last will
  populate VBOs inside your VAO class. VBOs have a name associated
  with attributes names. In our example a VBO named "a_texcoord".
* You have to init all VBOs with vertices, colors, texture
  position ... (this depends on your GLSL code). All VBOs size shall
  be the same.
* You can create as many VAOs as desired. The number of VOAs depends
  on how many 3D models you desire to create.

Run-time phase:
* You have to bind the VAO to be painted to its GLProgram.
* Call one of the different GLProgram draw() methods (with/without
  index buffer, ...)
* Redo the two previous steps for drawing other VAOs.

**Note:** a GLSL coding rule suggest to prefix uniform names bu "u_"
and attribute names "a_". This is not mandatory for this API. The only
requirement, for the moment, is to use the same name both for uniform
and attribute is not managed by the API (and I'm not sure GLSL can be
compiled anyway).

**Note:** C++ does not give the same flexibility than python for class
typing. C++ is a strongly typing language (for classes).  An example
is C++ does not allow container of template classes. Therefore in
GLumpy you will write `prog['a_position'] =` for setting either an
attribute or an uniform, while with this API, for the moment, you
cannot reaches the same concise code. You'll have to explicit the
template type and write `prog.attribute<Vector3f>("a_position") =` for
attributes and `prog.uniform<float>("u_scale") =` for uniforms. Also,
be careful that if you pass the wrong template type, an exception will
be thrown telling you that the attribute/uniform name is known but the
type is not available. **Help wanted for a better design**

#### Multiple objects

If you want to paint, let say two cubes for this example, you do not
have to create two VAOs but just one and do the follow in your the
run-time procedure:

* Add a [Model View Projection](http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/)
  in your vertex shader.
* Bind your VAO to your GLProgram.
* Use the Movable class to place (translate, rotate, scale) your first
  cube: this will create a 4x4 Matrix.
* Apply this matrix to your shader uniform matrix named "model".
* Draw the first cube.
* Use again the Movable for placing the second cube: redo the three previous
  steps, but this time, draw the second cube.
* See my examples to know how to do it concretely.

### What else ?

This API also includes features which are not directly in relation
with wrapping OpenGL function. It allows:
* to use Matrix and Vector classes (like glm library).

* to create movable object allowing to combine transformations like
  rotation, translation, projection on an object.
* to create a 'scene graph' of objects: a graph where nodes contains a
  transformation matrix to the parent node and a 3d model. Note: the
  matrix is mandatory while the 3d model is optional: this allows to
  create hidden transformation.
* to wrap the init of [Dear ImGui](https://github.com/ocornut/imgui)
  an API fort developing GUI in immediate mode (using just if then
  else instead of callbacks/signal-slot/observers methods like done
  with GTK+ or Qt).
* to wrap OpenGL window and offers to user the init, run-time phase
  and callbacks like keyboard and windows resizing.

## Compilation/Installation

### Prerequisite

Ubuntu, Debian:

```sh
sudo apt-get update && apt-get install libglew-dev libglfw3-dev
```

Mac OS X:
```sh
brew install glfw glew
```

Optionally if you want to write/launch unit tests, install `crpcut`
```sh
add-apt-repository ppa:jonlar/crpcut
sudo apt-get update && apt-get install crpcut crpcut-dev
```

### Compilation

To compile the API:
```sh
git clone https://github.com/Lecrapouille/OpenGLCppWrapper.git --depth=1
cd OpenGLCppWrapper
make download-external-libs
make compile-external-libs
make -j4
```

A `build/` directly shall have been created containing the compiled
files. Two libraries (one static the second dynamic) shall be
present. You can use them for your project.

**Note:** make commands `download-external-libs` and `compile-external-libs`
plays the same role than a recursive git clone (I do not like this git
feature).

To compile API examples:
```sh
cd OpenGLCppWrapper/examples
make -j4
```

Run examples:
```sh
./build/OpenGLExamples
```

### Installation

```sh
cd OpenGLCppWrapper
make -j4
sudo make install
```

This will install:
* in /usr/lib: all static and shared libraries libOpenGLCppWrapper
* in /usr/share/OpenGLCppWrapper/<version>: documentation

Check the presence of libraries in your system:
```sh
cd /usr/lib
ls -la libOpenGLCppWrapper*
```

### Unit tests

```sh
cd tests
make coverage -j4
```

If all tests passed the coverage report is created in `doc/coverage/index.html` and is opened automaticaly.

## API Documentation

* [Work to be finished](https://github.com/Lecrapouille/OpenGLCppWrapper/issues/1)
* [Architecture](https://github.com/Lecrapouille/OpenGLCppWrapper/blob/master/doc/Architecture.md)

## Credits

* https://github.com/glumpy/glumpy
