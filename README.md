# OpenGLCppWrapper

## What is OpenGLCppWrapper ?

OpenGLCppWrapper is a C++11 wrapper API for writing OpenGL `Core
Profile` applications in few lines of code. It hides for you the
complexity of calling in the correct order OpenGL functions. In
addition you start writing with OpenGL shaders (GLSL language) and you
finish with filling data in VBOs. This API also manages object life
time (construction, destruction) and allows you to manipulate and
change GPU data directly from your C++ memory space (CPU): dirty
data are automatically transmitted to the graphic card memory (GPU).

**Warning:** Do not confuse Core Profile OpenGL (>= 2.2) with Legacy
OpenGL (<= 2.1).  This API does not manage OpenGL Legacy but manages
OpenGL version >= 3.0.

**Warning:** This library is not a game engine but just an oriented
object wrapper for OpenGL that can be used in a game engine. This API,
contrary to others C++ OpenGL API really use object oriented and does
not just content to give a C++ flavor'ish name to OpenGL routines.

## Why another C++ wrapper for OpenGL ?

Short answer: I disliked others API I found on github ! Long answer:

* As a beginner of OpenGL Core, I always get confused by the order of
  creation/binding of OpenGL objects. With this API you do not have to
  really know what are OpenGL VBOs, VAOs, attributes, ... and in what
  order they have to be created or bound. This API is here to hide
  all the boilerplate for you.

* C++ APIs I saw on github just give a C++ flavor'ish taste to OpenGL
  C functions. None of them have real class inheritance over OpenGL
  objects and encapsulate routines. The only one demarking to other
  was [Glumpy](https://github.com/glumpy/glumpy) but written in
  Python.

* This API allows to write 3D scenes in really few lines of C++ code
  with almost no knowledge about OpenGL. You just have to understand
  the OpenGL shader language: GLSL. From these shaders, the API will
  instantiate for you all OpenGL objects (VBOs, textures ...).

* Public interfaces of OpenGL Objects are reduced as much as possible.

* This API takes the opposite direction over OpenGL tutorials: it does
  not start with a list of triangles, to push them in VBOs and finish
  coding with shaders and depending on which OpenGL version VAO. This
  API does the inverse: it starts from shaders and hide for you the
  generation of VBOs, attributes, uniforms, texture sampler. You have
  just to request from the shader to create for you a new 3D object
  (aka VAO) that you will init with data you wanted (aka VBO):
  vertices, color, texture position.

* This API does not only content to initialize VBOs during the init
  phase, it also allows you to manipulate your 3D scene data during
  the run-time (meshes, color ...) from the CPU: the API will transmit
  automatically to the GPU all modified elements for their display.

## How to use this API ? Explained Hello-world example

Here is the pseudo C++ code of what you will have to write for many of
your 3D scenes (All [Examples and
tutorials](https://github.com/Lecrapouille/OpenGLCppWrapper/blob/master/examples/README.md)
included in this library follow this pseudo code). You can compare the
length of this code with any "hello textured triangle" OpenGL
tutorial. I explain the code just after showing it.

In this document I consider that the reader knows basic OpenGL code at
least what explained in basic tutorials.

Before showing C++ code with this API, we have to write a basic
fragment and vertex shader (named `my_fragment_shader.glsl` and
my_vertex_shader.glsl`). As explained before this API has been
conceived to develop OpenGL applications by starting with their
shaders.

##### my_fragment_shader.glsl

In this vertex shader, we pick the color of the texture (texel) and
mix it with an uniform (global) color. There is nothing particularly
difficult with this fragment shader because it's largely explained in
OpenGL tutorials findable on internet.

```
01: #version 330 core
02:
03: uniform sampler2D  texID;     // Texture sampler
04: in  vec2           texUV;     // Texture coordinate
05: in  vec4           texColor;  // from vertex shader
06: out vec4           fragColor;
07:
08: void main()
09: {
10:     fragColor = texture(texID, texUV) * texColor;
11: }
```

##### my_vertex_shader.glsl

In this vertex shader, we pass to the fragment shader the texel and
the global color. We applied to vertices position a
[Model-View-Projection matrix](http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices)
(change of basis). Again these kind of basic shaders are largely
described on the web. For the fun we applied an additional scaling
factor.

```
01: #version 330 core
02:
03: uniform float  scale;         // Global scaling
04: uniform mat4   model;         // Model matrix
05: uniform mat4   view;          // View matrix
06: uniform mat4   projection;    // Projection matrix
07: uniform vec4   color;         // Global color
08: in vec3        position;      // Vertex position
09: in vec2        UV;            // Vertex texture coordinates
10: out vec2       texUV;         // Interpolated fragment texture coordinates
11: out vec4       texColor;      // Interpolated fragment color (out)
12:
13: void main()
14: {
15:     // Varying variables
16:     texUV = UV;
17:     texColor = color;
18:
19:     // Final position
20:     gl_Position = projection * view * model * vec4(scale * position, 1.0);
21: }
```

##### main.cpp

Here the most funny part of this document : using this API. The code
is written in pseudo C++ code to be less boring to read. Explanations
come here after.

```
01: GLProgram prog;
02: GLVAO vao1, vao2;
03:
04: // Init phase:
05: {
06:   // Load shaders code
07:   GLVertexShader   vertex_shader.loadFromFile("my_vertex_shader.glsl");
08:   GLFragmentShader fragment_shader.loadFromFile("my_fragment_shader.glsl");
09:
10:   // Compile and link shaders into a GLProgram
11:   if (!prog.attachShaders(vertex_shader, fragment_shader).compile()) {
12:     std::cerr << "failed compiling OpenGL program. Reason was '" << m_prog.error() << "'" << std::endl;
13:     return false;
14:   }
15:
16:   // Init uniforms
17:   prog.scalarf("scale") = 1.0f;
18:   prog.vector4f("color") = Vector4f(0.8f, 0.2f, 0.8f, 0.8f);
19:
20:   // vao1 is now the active VAO for prog
21:   prog.bind(vao1);
22:
23:   // Define a 3D model (for example a cube)
24:   vao1.vector3f("position") = { Vector3f(...) ... Vector3f(...) };
25:   vao1.vector2f("UV") = { Vector2f(...) ... Vector2f(...) };
26:
27:   // Apply a 2D texture to the 3D model
28:   if (!vao1.texture2D("texID").load("my_texture1.jpg"))
29:     return false;
30:
31:   // vao2 is now the active VAO for prog
32:   prog.bind(vao2);
33:
34:   // Define an other 3D model (for example a plane) with different texture
35:   vao2.vector3f("position") = { ... };
36:   vao2.vector2f("UV") = { ... };
37:   vao2.texture2D("texID").load("my_texture2.jpg")
38: }
39:
40: // Runtime phase:
41: {
42:   // Draw model 1 with implicit first and count vertices
43:   prog.bind(vao1);
44:   prog.draw(Primitive::TRIANGLES);
45:
46:   // Draw model 2 with explicit first and count vertices
47:   prog.bind(vao2);
48:   prog.draw(Primitive::TRIANGLES, 0, 36);
49:
50:   // Draw model 1 (alternative way)
51:   prog.draw(vao1, Primitive::TRIANGLES, 0, 36);
52:
53:   // Draw model 2: (alternative way)
54:   prog.draw(vao2, Primitive::TRIANGLES);
55:
56:   // Example of drawing a model with vertices index (EBO)
57:   GLIndexBuffer<uint32_t> indices = { ... };
58:   prog.draw(vao3, Primitive::POINTS, indices);
59: }
60:
61: // Camera management
62: {
63:   prog.matrix44f("projection") = { ... };
64:   prog.matrix44f("view") = { ... };
65:   prog.matrix44f("model") = { ... };
66: }
```

### Explanations

Init phase:

* lines 01-02 and 07-08: GL*x*Shader, GLProgram and GLVAO are some
  OpenGL objects that the library is exposing to the developer (where
  *x* is referring either to Fragment or Vertex or Geometry). A
  GL*x*Shader holds the GLSL code source and compile it (and in future
  will allow to add other macro processing features). A GLProgram
  holds at least two GLxShaders (vertex and fragment) and link
  them. Once compiled, a GLProgram holds locations of shader uniforms
  and shader attributes. GLVAO can be considered as the OpenGL handler
  to your 3D model: it is an interface between CPU data and GPU data
  by holding VBOs and textures. You can modify data of textures and
  VBOs: the API will transfer modified values to your graphical card.

* lines 07-08: You have to load and compile your GLSL code given as
  vertex and fragment shader (and optionally geometry shader). You can
  either refer the file path with `loadFromFile` or directly pass the
  code as string with `fromString`.

* line 11-14: You have to compile and link shaders into the class
  GLProgram. `compile()` is optional (if not declared, the shader
  compilation will be just postponed). If shaders contained syntax
  errors the compile() will failed and errors can be shown with
  `error()`.

* line 15: If your OpenGL has been compiled with success, the
  GLProgram will create its own list of uniforms and attributes from
  shaders. In this example they are `texID, position, UV, texColor,
  scale, model, view, projection` ...

* line 17-18: Uniforms can be initialized directly because they acts
  like constant values inside shaders. Attributes needs a bound VAO
  to the GLProgram (which is made line 21).

* line 21: To initialize attributes you have to create a VAO and bind
  it to your GLProgram. This last will allocate VBOs and textures
  inside the VAO instance. Textures can be 1D, 2D, 3D. VBO are GPU
  buffer for storing data of your 3d models (vertex position, Vertex
  texture coordinates, normals ...)  GLProgram can also reserve some
  extra memory if this information is given either with the
  constructor or with the method `setInitVBOSize`. VBO creation is
  made automatically (and only once) during the first binding (next
  binding will not create VBOs or textures). Shader attributes will
  give their name to VBOs. In our example `vao1` and `vao2` will have
  a VBO named `position`, `UV` and texture named `texID`. Once VAO has
  been bind to a GLProgram it cannot be used by an other GLProgram
  (else an exception would be thrown to prevent errors).

* line 24-25: You have to initialize all VBOs with vertices, colors,
  texture position ... (which depends on your GLSL code). All VBOs
  shall have the same size. **TODO** automatic check will be added.

* line 28-29: Textures are uniforms but they are filled like
  attributes (need a bind VAO). We use the `load` method to store the
  data inside the bind VAO. Different format of files is managed
  (jpeg, bmp, png, tga ...).

* line 32-37: You can create as many VAOs as desired. The number of
  VAOs only depends on how many 3D models you desire to create. You
  have to redo the same steps for filling VBOs and textures.

Run-time phase:

* line 43: to draw a 3D model, you have to bind the VAO to its
  GLProgram. Note that the second time that `prog.bind(vao1)` is
  called VBOs and textures are not again allocated.

* line 44-58: show different way to draw models (with/without number
  of vertices indicated, with/without index buffer, ...). You can draw
  triangles, lines, points and more primitives.

* line 63-65: In this example, we have to defined matrices for the
  model view projection. The matrix `"projection"` is usually updated
  when the user change the dimension of the GUI window. The `"view"`
  matrix is updated when your camera is moved (see Legacy OpenGL
  lookAt routine). Finally the `"model"` matrix is used for placing
  models over the world (so to be placed before any `GLProgram::draw`
  methods.

**Note:** `Primitive::TRIANGLES` is a strong type alias for
GL_TRIANGLES. For all public methods needing an OpenGL GLint or GLenum
types I replaced them by a C++ `enum class` (see GLEnum.hpp file for
other types). This allows to detect bugs with wrong type like trying to
pass GL_DYNAMIC_DRAW instead of GL_TRIANGLES. Note that internally in
classes, I keep using GL_XXX names because they cannot be access
outside the API.

**Note:** a GLSL coding rule suggest to prefix uniform names bu "u_"
and attribute names "a_". This is not mandatory for this API. The only
requirement, for the moment, is to use the same name both for uniform
and attribute is not managed by the API (and I'm not sure GLSL can be
compiled anyway).

**Note:** Contrary to Glumpy where GLProgram can modify both textures,
uniforms and attributes, in this API GLProgram only modifies uniforms
and GLVAO only modifies attributes (named VBO) or textures.

**Note:** C++ does not give the same flexibility than python for class
typing. C++ is a strongly typing language (for classes).  An example
is C++ does not allow container of template classes. Therefore in
Glumpy you will write `prog['position'] =` for setting either an
attribute or an uniform, while with my API you cannot reaches the
same concise code. You'll have to explicit the template type and write
`vao.VBO<Vector3f>("position") =` for attributes and
`prog.uniform<float>("scale") =` for uniforms. Also, be careful that
if you pass the wrong template type, an exception will be thrown
telling you that the attribute/uniform name is known but the type is
not available.

**Help wanted for a better C++ design allowing to shorten number of
characters to type for setting attributes and uniforms**:
`prog.uniform<Vector3f>("position") =` is 35 char sized for C++
while `prog['position'] =` is 18 char sized for Python (so 50%
less). For the moment I can hide the misery of template with wrapping
methods: for example instead of `prog.attribute<Vector3f>("position")
=` I'll write `prog.vector3f("position") =` (so 27 chars). Same idea
for VAO.

### Multiple objects: multiple VBOs vs. multiple VAOs

If you want to paint two cubes (let say for this example), you do not
have to create two VAOs (one by cube) but just a single VAO and do the
follow during the run-time process:

* Add a [Model View Projection](http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/)
  in your vertex shader.
* Bind your VAO to your GLProgram.
* Use the Movable class to place (translate, rotate, scale) your first
  cube in the scene: this will create a 4x4 Matrix.
* Apply this matrix to your shader uniform matrix named "model" of the Model View Projection.
* Draw the first cube.
* Use again the Movable for placing the second cube: redo the three
  previous steps but this time, draw the second cube (update movable,
  apply matrix to uniform, draw).
* See my examples/ directory to know how to do it concretely.

With this method, for this example, you will save GPU memory by
avoiding storing an additional VAO and all its VBOs needed for drawing
a cube.

### What else in this API ?

Not shown in this readme but this API can manage framebuffer and cubic,
3D and 1D textures. See examples for more informations.

This API also includes features which are not directly in relation
with wrapping OpenGL routines. It allows:

* to use Matrix and Vector classes (like glm library).
* to create movable objects allowing to combine transformations like
  rotation, translation, projection on an model.
* to create a 'scene graph' of objects: a graph where nodes contains a
  transformation matrix relative to the current node and its
  parent. Note that the matrix is mandatory for each node of the scene
  graph while the 3d model is optional: this allows to add invisible
  transformations.
* to wrap the initialization of [Dear ImGui](https://github.com/ocornut/imgui)
  an API fort developing GUI in immediate mode (using just if then
  else instead of callbacks/signal-slot/observers methods like done
  with GTK+ or Qt).
* to wrap OpenGL window and offers to user the init, run-time phase
  and callbacks like keyboard and windows resizing.

### API Documentation

* [Code Architecture](https://github.com/Lecrapouille/OpenGLCppWrapper/blob/master/doc/Architecture.md)
* [Examples and tutorials](https://github.com/Lecrapouille/OpenGLCppWrapper/blob/master/examples/README.md)

### Work in progress

__Please help contributing !__

Initially this library was inside the SimTaDyn code source. I moved it
to this repo [OpenGLCppWrapper](https://github.com/Lecrapouille/OpenGLCppWrapper)
because I think people are more interested by an OpenGL API than a GIS application.

This project is still in alpha version but allows to draw some basic 3D
and dynamic scenes in few lines of code. **Warning:** Interfaces are
not yet frozen since I've not reached a stable and mature code. [Work
to be finished](https://github.com/Lecrapouille/OpenGLCppWrapper/issues/1)

I also truly believe that describing the code
[architecture](https://github.com/Lecrapouille/OpenGLCppWrapper/blob/master/doc/Architecture.md)
is something really important as well for developers than users.

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
./build.sh
```

Which is equivalent to:
```sh
git clone --recurse-submodules https://github.com/Lecrapouille/OpenGLCppWrapper.git --depth=1
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
make download-resources
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

If all tests passed the coverage report is created in `doc/coverage/index.html` and is opened automatically.

### Verbose

To activate console logs for debugging, see in `src/Makefile` and in
`examples/Makefile` gcc flags `-DERROR -UDEBUG` and set to `-DERROR -DDEBUG`.

## Credits

This API is largely inspired by projects such as:
* [Glumpy](https://github.com/glumpy/glumpy) python project.
* [opengl4csharp](https://github.com/giawa/opengl4csharp) C# project.

### Why I decided to make this API ?

* The first need from this API is that contrary to legacy OpenGL,
  learning modern OpenGL (aka OpenGL core), is very difficult for
  beginners.

* When working on my personal project
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

* [Glumpy](https://github.com/glumpy/glumpy) was the OpenGL wrapper I
  was looking for but unfortunately it is written in Python so I had
  to make the portage into C++ and here the creation of this API !
  **Note: this is not a full-feature portage:**
  * The first reason is due to the fact that C++ is a strongly typed
  language which does not offer the same flexibility than python. For
  examples: storing template classes in the same container like Numpy
  views is difficult.
  * The second reason is that Glumpy does not use by default VAOs but
  access to VBOs through OpenGL program attributes, which also
  perturbs me because in the case of you have several models
  (therefore several VAOs) in the scene to paint this library seems
  less well conceived to draw them but thanks to typage flexibility of
  Python, attributes of the shader can be bind to VAOs. In my API only
  VAOs have to be created and only them can be bound to GLProgram. The
  result is equivalent to Glumpy the code is a little different.
  * There are also some minor changes: See this file
  [differences](https://github.com/Lecrapouille/OpenGLCppWrapper/blob/master/doc/Differences.md)
  for more information. Mains changes are the begin() code which adds
  more checks. OpenGL handles type are not very consistent in the
  Core API this can makes compilation warnings. This is why IGLObject
  is a template class. I moved code from create() to setup() like for
  VBOs this allows to remove some checks that Glumpy does. I create a
  GLSampler class inheriting from GLUniform which does exist in
  Glumpy.
