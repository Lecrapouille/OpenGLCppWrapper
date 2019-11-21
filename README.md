# OpenGLCppWrapper

## What is OpenGLCppWrapper ?

OpenGLCppWrapper is a C++11 wrapper API for writing OpenGL `Core Profile` applications in few lines of code. It hides for you the complexity of calling in the correct order OpenGL functions and passing correct parameters on them. You'll have just to deal with writing your OpenGL shaders (GLSL language), this API will compile them and instantiate class wrapping VBOs, textures. As a consequence, you just have to load your desired textures and fill data in VBOs (vertices position, texture coordinates, normals ..) and nothing more! This API manages automatically OpenGL object lifetime as well as of the transfer of modified CPU data (from your C++ space memory point of view) into GPU data (to graphics card memory point of view).

**Warning:** Do not confuse Core Profile OpenGL (>= 2.2) with Legacy OpenGL (<= 2.1). This API does not manage OpenGL Legacy but manages OpenGL version >= 3.0.

**Warning:** This library is not a game engine but just an oriented object wrapper for OpenGL that can be used in a game engine. This API, contrary to other C++ OpenGL API really uses object-oriented and does not just content to give a C++ flavor name to OpenGL routines. It hides for you all the complexity!

## Why another C++ wrapper for OpenGL?

Short answer: I disliked others API I found on Github! Long answer:

* As a beginner of OpenGL Core, I always get confused by the order of creation/binding of OpenGL objects. With OpenGLCppWrapper, you'll do not have to really know what is OpenGL VBOs, VAOs, framebuffer, shader attributes, ... and in what order they have to be created or bound. What you probably prefer to deal with questions like: "what texture fit the best to my model?" or "What is the list of triangles for my model?" but certainly not low-level issue questions like "is mytexyure.jpg bound to GL_TEXTURE0 or GL_TEXTURE1 ?".

* C++ APIs I saw on GitHub just give a C++ flavor'ish taste to OpenGL C functions. None of them have real class inheritance over OpenGL objects and encapsulate routines. As a consequence, you still have to deal with the correct order of calling routines. The only API demarking to others was [Glumpy](https://github.com/glumpy/glumpy) but written in Python.

* OpenGLCppWrapper takes the opposite direction over OpenGL tutorials: it does not start with a list of triangles, to push them in VBOs and finish coding with shaders (and depending on which OpenGL version, uses or not a VAO). This API does the inverse: it starts from shaders and hides for you the generation of VBOs, attributes, uniforms, texture sampler. You have just to request from the shader program to create for you a new 3D object (aka VAO) that you have to init (VBOs, textures) with data you wanted: vertices, color, texture position.

* As a consequence, the number of public interfaces of OpenGLCppWrapper is reduced as much as possible. Only OpenGL constants mandatory used in parameters of public methods are wrapped by C++ enum for using the strong type checking during the compilation of the project, avoiding to pass 'carrots' while 'potatoes' were expected.

* This API does not only content to initialize VBOs during the init phase, but it also allows you to manipulate your 3D scene data during the run-time (meshes, color ...) from the CPU: the API will transmit automatically to the GPU all modified elements for their display. This is, of course, less performant that manipulating them from shaders code, but this offers you extra flexibility (for scientific tools for example).

* As drawbacks, this API is less cache friendly than calling directly your OpenGL routines (lot of if-then-else, smart pointers and dynamic cast). This is the price of hiding the complexity and staying generic.

## How to use this API? Explained 'Hello-world' example

Here is the pseudo-C++ code of what you will have to write for many of your 3D scenes. All [Examples and tutorials](https://github.com/Lecrapouille/OpenGLCppWrapper/blob/master/examples/README.md) included with this library follow this pseudo-code. You can compare the length of their code with any similar OpenGL tutorial.

In this document, I consider that the reader knows the basic OpenGL code. There is nothing particularly difficult with this tutorial because everything is largely explained in the first section of any OpenGL tutorials findable on the internet.

Before showing C++ code with this API, we have to write a basic fragment and vertex shader (named `my_fragment_shader.glsl` and `my_vertex_shader.glsl`). As explained before, this API has been conceived to develop OpenGL applications by starting with their shaders.

##### my_fragment_shader.glsl:

In this fragment shader, we simply pick the color of the texture (texel) and mix it with a uniform (global) color.

```
01: #version 330 core
02:
03: uniform sampler2D texID; // Texture sampler
04: in vec2 texUV;           // Texture coordinate
05: in vec4 texColor;        // from vertex shader
06: out vec4 fragColor;
07:
08: void main()
09: {
10:     fragColor = texture(texID, texUV) * texColor;
11: }
```

##### my_vertex_shader.glsl:

In this vertex shader, we pass to the fragment shader the texel and the global color. We applied to vertices position a [Model-View-Projection matrix](http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices) (change of basis). Again this kind of basic shaders is largely described on the internet. For the fun, we applied an additional scaling factor.

```
01: #version 330 core
02:
03: uniform float scale;     // Global scaling
04: uniform mat4 model;      // Model matrix
05: uniform mat4 view;       // View matrix
06: uniform mat4 projection; // Projection matrix
07: uniform vec4 color;      // Global color
08: in vec3 position;        // Vertex position
09: in vec2 UV;              // Vertex texture coordinates
10: out vec2 texUV;          // Interpolated fragment texture coordinates
11: out vec4 texColor;       // Interpolated fragment color (out)
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

##### main.cpp:

Here the interesting part of this document: using OpenGLCppWrapper API. The code is written in pseudo-C++ code to be less boring to read. Explanations come after.

```
01: GLProgram prog;
02: GLVAO vao1, vao2;
03:
04: // Init phase:
05: {
06:      // Load shaders code
07:      GLVertexShader vertex_shader.loadFromFile("my_vertex_shader.glsl");
08:      GLFragmentShader fragment_shader.loadFromFile("my_fragment_shader.glsl");
09:
10:      // Compile and link shaders into a GLProgram
11:      if (!prog.attachShaders(vertex_shader, fragment_shader).compile())
12:      {
13:            std::cerr << "failed compiling OpenGL program. Reason was '" << m_prog.getError() << "'" << std::endl;
14:            return false;
15:      }
16:
17:     // Init shader uniforms
18:     prog.scalarf("scale") = 1.0f;
19:     prog.vector4f("color") = Vector4f(0.8f, 0.2f, 0.8f, 0.8f);
20:
21:     // Make vao1 be the active VAO for prog
22:     prog.bind(vao1);
23:
24:     // Define a 3D model (for example a cube)
25:     vao1.vector3f("position") = { Vector3f(...) ... Vector3f(...) };
26:     vao1.vector2f("UV") = { Vector2f(...) ... Vector2f(...) };
27:
28:     // Apply a 2D texture to the 3D model
29:     if (!vao1.texture2D("texID").load("my_texture1.jpg"))
30:          return false;
31:
32:     // Make vao2 be the active VAO for prog
33:     prog.bind(vao2);
34:
35:     // Define another 3D model (for example a plane) with different texture
36:     vao2.vector3f("position") = { ... };
37:     vao2.vector2f("UV") = { ... };
38:     vao2.texture2D("texID").load("my_texture2.jpg")
39: }
40:
41: // Runtime phase:
42: {
43:     // Draw model 1 with implicit first and count vertices
44:     prog.matrix44f("model") = { ... };
45:     prog.bind(vao1);
46:     prog.draw(Mode::TRIANGLES);
47:
48:     // Draw model 2 with explicit first and count vertices
49:     prog.matrix44f("model") = { ... };
50:     prog.bind(vao2);
51:     prog.draw(Mode::TRIANGLES, 0, 36);
52:
53:     // Draw model 1 (alternative way)
54:     prog.draw(vao1, Mode::TRIANGLES, 0, 36);
55:
56:     // Draw model 2: (alternative way)
57:     prog.draw(vao2, Mode::TRIANGLES);
58:
59:     // Example of drawing a model with vertices index (EBO)
60:     GLIndexBuffer<uint32_t> indices = { ... };
61:     prog.draw(vao3, Mode::POINTS, indices);
62: }
63:
64: // Camera management
65: {
66:     prog.matrix44f("projection") = { ... };
67:     prog.matrix44f("view") = { ... };
68: }
```

### Explanations

Init phase: This code is usually made once and in the `setup()` method (for example called by a GLWindow).

* lines 01-02 and 07-08:
GL*x*Shader, GLProgram, and GLVAO are some OpenGL objects that the library is exposing to the developer (where *x* is referring either to Fragment or Vertex or Geometry). A GL*x*Shader holds the GLSL code source and compile it (and in future will allow adding other macro processing features). A GLProgram holds at least two GLxShaders (vertex and fragment) and links them. Once compiled, a GLProgram holds internally and in privacy locations of shader uniforms, shader attributes and texture samplers. A GLVAO can be considered as the OpenGL handler to your 3D model: it is an interface between CPU data and GPU data by holding VBOs and textures. You can modify data of textures and data of VBOs: the API will transfer modified values to your graphical card.

* lines 07-08:
You have to load and compile your GLSL code given as vertex and fragment shader (and optionally geometry shader). You can either refer the file path with `loadFromFile(std::string const&)` or directly pass the code as a string with `fromString(std::string const&)`.

* lines 11-15:
You have to compile and link shaders into the class GLProgram. `compile()` is optional (if not declared, the shader compilation will be just postponed). If shaders contained syntax errors the `compile()` will return `false` and errors can be shown with `gerError()`. If your OpenGL has been compiled with success, the GLProgram will create its own list of shader uniforms, attributes, and samplers from shaders. In this example, they are `texID, position, UV, texColor, scale, model, view, projection` ...

* **Note:** You can repeat lines 07-15 to add more GLPrograms and load more shaders code.

* lines 18-19:
Shader uniforms can be initialized directly because they act like constant values inside shaders. Shader attributes and shader samplers need a bound VAO to be initialized (explained here after). Shader uniforms data are reachable through methods like `scalarf(const char*)`, `vector3f(const char*)`, `matrix44f(const char*)`, ... using the name of the shader attribute or shader sampler (respect the case). Note that `f` means float and `i` means integer.

* line 22 and 33:
To initialize your 3D model, you have to use a VAO and bind it to the desired GLProgram This last, helped with its internal list of shader attributes and samplers, will populate VBOs and textures inside the VAO instance. Shader attributes and samplers give their name to VBOs and textures. In our example, `vao1` and `vao2` will have a VBO named `position` and `UV` and a texture named `texID`. Their data are reachable through methods like `scalarf(const char*)`, `vector3f(const char*)`, `matrix44f(const char*)` or `texture2D(const char*)`, ... using the name of the shader attribute or shader sampler (respect the case).

* line 46, 51:
GLProgram reserves a predefined number of elements in VBO if this information is given either within the GLProgram constructor or with the method `setInitVBOSize(size_t)`. VBOs and textures are instantiate automatically and only once during the first binding. On the next binding nothing are created. Once VAO has been bind to a GLProgram it cannot be bound to another GLProgram (an exception would be thrown to prevent this unexpected behavior). Binding a VAO makes it be the single activable VAO to this GLProgram. As consequence it replaces the older bound VAO.

* lines 25-29:
Textures and VBOs shall be initialized. Textures are uniforms but they are filled like attributes (need a bind VAO). Textures dimension depends on the sampler used in your GLSL code: the can be 1D, 2D, 3D or Cube. You can load jpeg, BMP, png, TGA ... files and for 2D textures, you have to use `load(const char*)`. VBO is a GPU buffer for storing data of your 3d models (vertex position, texture coordinates, normals ...). All VBOs shall have the same size. Automatic checks of their sizes are made.

* line 33-38:
You can create as many VAOs as desired. The number of VAOs only depends on how many 3D models you desire to create. You have to redo the same steps for filling VBOs and textures. In this example, two VAOs are bound the same shader program but you can add more shader programs. For example, see my examples using framebuffers;

Run-time phase: This code is usually called in the `draw()` method (for example called by a GLWindow) for rendering your 3D scene.

* line 45:
To draw a 3D model, you have to bind the VAO to its GLProgram. Note that the second time that `prog.bind(vao1)` is called VBOs and textures are not again allocated. Binding a VAO can be explicit like done here or passed directly in the draw methods. Binding a VAO makes it replaced the older bound.

* line 45-57:
Show different ways to draw models (with/without the number of vertices indicated, with/without index buffer, with/without the VAO...). You can draw triangles, striped triangles, lines, points or different types of primitives offered by OpenGL.

* line 44, 49:
Depending on your GLSL code, if you use [Model View Projection matrices](http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/) (See any OpenGL tutorials concerning how this works). Use the model matrix to place your models inside the scene. Because in Core OpenGL, routines like `glPushMatrix()` and `glPopMatrix()` no longer exist, this API proposes to use a scene graph for more complex scenes (See my examples/ directory to know how to do it concretely.).

* lines 64-68:
In this example, we defined matrices for the model view projection. The matrix `"projection"` is usually updated when the user changes the dimension of the GUI window (`onWindowSizeChanged(uint32 const width, uint32 const height)` of GLWindow and in `setup()`). The `"view"` matrix is updated when your camera is moved (see Legacy OpenGL `glLookAt` routine). Finally, the `"model"` matrix is used for placing models over the world (so to be placed before any `GLProgram::draw` methods).

**Note:** `Mode::TRIANGLES` is a strong type alias for `GL_TRIANGLES`. For all public methods needing an OpenGL GLint or GLenum types I replaced them by a C++ `enum class` (see GLEnum.hpp file for other types). This allows detecting bugs with the wrong type like trying to pass GL_DYNAMIC_DRAW instead of GL_TRIANGLES. Note that, inside private classes, I keep using GL_XXX names because they are not exposed to the public API.

**Note:** a GLSL coding rule suggests to prefix uniform names by "u_" and attribute names "a_". This is not mandatory for this API. The only requirement, for the moment, is to use the same name both for uniform and attribute is not managed by the API (and I'm not sure GLSL can be compiled anyway).

**Note:** Contrary to Glumpy, where GLProgram can modify both textures, uniforms, and attributes, in this API GLProgram only modifies uniforms and GLVAO modifies attributes (named VBO) or textures.

**Note:** C++ does not give the same flexibility than python for class typing. C++ is a strongly typed language (for classes). An example is C++ does not allow a container of template classes. Therefore in Glumpy, you will write `prog['position'] =` for setting either a shader attribute or a shader uniform, while with my API you cannot reach the same concise code. You'll have to explicit the template type and write `vao.VBO<Vector3f>("position") =` for attributes and `prog.uniform<float>("scale") =` for uniforms. Also, be careful that if you pass the wrong template type, an exception will be thrown telling you that the attribute/uniform name is known but the type is not available.

**Help wanted for a better C++ design allowing to shorten number of characters to type for setting attributes and uniforms**: `prog.uniform<Vector3f>("position") =` is 35 char sized for C++ while `prog['position'] =` is 18 char sized for Python (so 50% less). For the moment I can hide the misery of template with wrapping methods: for example instead of `prog.attribute<Vector3f>("position") =` I'll write `prog.vector3f("position") =` (so 27 chars). Same idea for VAO. In addition I use dynamic_cast which is not the best solution ever. Some clues to improv this are welcome.

### Multiple objects: multiple VBOs vs. multiple VAOs

If you want to paint two cubes (let say for this example), you do not have to create two VAOs (one by cube) but just a single VAO and do the following during the run-time process:

* Add [Model View Projection](http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/) matrices in your vertex shader.
* Bind your VAO to your GLProgram.
* Use the Movable class to place (translate, rotate, scale) your first cube in the scene: this will create a 4x4 Matrix.
* Apply this matrix to your shader uniform matrix named "model" of the Model View Projection.
* Draw the first cube.
* Use again the Movable for placing the second cube: redo the three previous steps but this time, draw the second cube (update movable, apply a matrix to uniform, draw).
* See my [examples](https://github.com/Lecrapouille/OpenGLCppWrapper/blob/master/examples/README.md) directory to know how to do it concretely.

With this method, for this example, you will save GPU memory by avoiding storing an additional VAO and all its VBOs needed for drawing a cube.

### What else in OpenGLCppWrapper?

Not shown in this readme but this API can manage framebuffer and cubic, 3D or 1D texture. It also includes features that are not directly in relation to wrapping OpenGL routines. It allows:

* to estimate the GPU memory usage.
* to use Matrix and Vector classes (like glm library).
* to create movable objects allowing to combine transformations like rotation, translation, projection on a model.
* to create a 'scene graph' of objects: a graph where nodes contain a transformation matrix relative to the current node and its parent. Note that the matrix is mandatory for each node of the scene graph while the 3d model is optional: this allows us to add invisible transformations.
* to wrap the initialization of [Dear ImGui](https://github.com/ocornut/imgui) an API for developing GUI in immediate mode (using just if then else instead of callbacks/signal-slot/observers methods like done with GTK+ or Qt).
* to wrap OpenGL window and offers to the user the init, run-time phase and callbacks like keyboard and windows resizing.

See my [examples](https://github.com/Lecrapouille/OpenGLCppWrapper/blob/master/examples/README.md) directory to know how to do it concretely.

### API Documentation and examples

* [Code Architecture](https://github.com/Lecrapouille/OpenGLCppWrapper/blob/master/doc/Architecture.md)
* [Examples and tutorials](https://github.com/Lecrapouille/OpenGLCppWrapper/blob/master/examples/README.md)
* Doxygen documention can be generated by typing `make doc`.

### Work in progress

__Please help contributing !__

Initially, this library was inside the SimTaDyn code source. I moved it to this repo [OpenGLCppWrapper](https://github.com/Lecrapouille/OpenGLCppWrapper) because I think people could be more interested in an OpenGL API than a GIS application.

This project is still in alpha version but it's enough stable for making complete 3D and dynamic scenes in a few lines of C++ code. **Warning:** By alpha I mean that API interface is not yet frozen since I've not reached a stable and mature code. See [Work to be finished](https://github.com/Lecrapouille/OpenGiCppWrapper/issues/1)

I also truly believe that describing the code [architecture](https://github.com/Lecrapouille/OpenGLCppWrapper/blob/master/doc/Architecture.md) is something really important as well for developers than users.

You can contribute by reporting issues, fixing my poor English, adding more examples, create pull requests, testing it for Windows (I do not have Windows) ...

## Compilation/Installation

### Prerequisite

This project depends on my GitHub Makefile project (used for compiling all my GitHub projects) automatically cloned as git submodule [MyMakefile](https://github.com/Lecrapouille/MyMakefile).

##### Ubuntu, Debian:

```sh
sudo apt-get update && apt-get install libglew-dev libglfw3-dev libbz2-dev
```

##### Mac OS X:
```sh
brew install glfw glew
```

Optionally if you want to write/launch unit tests, install `crpcut`
```sh
add-apt-repository ppa:jonlar/crpcut
sudo apt-get update && apt-get install crpcut crpcut-dev
```

### Compilation

Works for Linux. Should work for OS X. Not working for Windows (need help on completing the Makefile).

To compile the API and its examples:
```sh
git clone --recurse-submodules https://github.com/Lecrapouille/OpenGLCppWrapper.git --depth=1
cd cd OpenGLCppWrapper
./build.sh
```

For compiling the API `build.sh` will do:
```sh
make download-external-libs
make compile-external-libs
make -j4
```

A `build/` folder shall have been created containing the compiled and runnable files. Two libraries (one static the second dynamic) shall also be present. You can use them for your project.

**Note:** make commands `download-external-libs` and `compile-external-libs` plays the same role than a recursive git clone (which I do not like it so I only use it for my makefiles). They will download, compile (but not install) GitHub libraries like: Backward, SOIL and Dear IMgui.

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

Multiple versions of this library can co-exist (thanks to their version number).
```sh
cd OpenGLCppWrapper
make download-external-libs
make compile-external-libs
make -j4
sudo make install
```

This will install:
* in `/usr/lib`: the static and shared libraries libOpenGLCppWrapper.
* in `/usr/include/openglcppwrapper-<version>`: all headers files (hpp).
* in `/usr/lib/pkgconfig`: a pkg confile file for linking this API to your project.
* in `/usr/share/OpenGLCppWrapper/<version>/`: documentation, examples and other files.

Check the presence of libraries in your system:
```sh
cd /usr/lib
ls -la libOpenGLCppWrapper*
```
Or better:
```
pkg-config openglcppwrapper --libs
```

### Unit tests

```sh
cd tests
make coverage -j4
```

If all tests passed the coverage report is created in `doc/coverage/index.html` and is opened automatically.

## How to use OpenGLCppWrapper in your project?

Add the include in your cpp file: `# include <OpenGLCppWrapper/OpenGLCppWrapper.hpp>`

In debug mode, you can use the following macros:
* `-DCHECK_OPENGL` for checking bad parameters passed to OpenGL routines.
* `-DENABLE_DEBUG` for activating console logs for debugging.

Compile using the pkg-config tool to add compilation flags to GCC or clang:
* CFLAGS=\`pkg-config openglcppwrapper --cflags\`
* LDFLAGS=\`pkg-config openglcppwrapper --libs\`
* Note: in pkg-config instead of openglcppwrapper which refers to the last installed version,
you can specify your desired version (for example: openglcppwrapper-0.6).

Example with a simple file main.cpp:
```
g++ -W -Wall --std=c++11 main.cpp -o prog `pkg-config openglcppwrapper --cflags --libs`
```

## Credits

This API is largely inspired by projects such as:
* [Glumpy](https://github.com/glumpy/glumpy) python project.
* [opengl4csharp](https://github.com/giawa/opengl4csharp) C# project.

### Why I decided to make this API?

* The first need from this API is that contrary to legacy OpenGL, learning modern OpenGL (aka OpenGL core), is very difficult for beginners.

* When working on my personal project
[SimTaDyn](https://github.com/Lecrapouille/SimTaDyn) I was looking for an OpenGL wrapper in C++. The first need was to hide the OpenGL boilerplate. I looked for such a kind of OpenGL library on GitHub but I founded none which allowed me to write OpenGL scenes in few lines. The second need from this API is that OpenGL does not manage dynamic size VBOs easily (as far as I know). I needed to edit maps with an arbitrary number of nodes and arcs which is problematic with fixed size VBO (OpenGL is made for games and characters are rendering with almost the same number of vertices (if we except tessellation with geometry shaders).

* APIs like [ogl](https://github.com/opengl-tutorials/ogl) or [OOGL](https://github.com/Overv/OOGL) or [gl](https://github.com/acdemiralp/gl) are wrong (even with thousands of stars) because there is too much code like `glBindBuffer`, `BindAttribute(... 3, 42 * sizeof(float))` or `glVertexAttribPointer`, `program.set_uniform(program.uniform_location` that I absolutely do not want to see in a high-level code.

* [Glumpy](https://github.com/glumpy/glumpy) was the exact OpenGL wrapper I was looking for! Unfortunately written in Python! I had to make its portage into C++. I also discovered later [opengl4csharp](https://github.com/giawa/opengl4csharp) written in C# which is quite similar to Glumpy but I think some designs are less good. So finally I found no good C++ OpenGL wrapper API.

**Note: OpenGLCppWrapper is not a full-feature portage of Glumpy or opengl4csharp**

* The first reason is due to the fact that C++ is a strongly typed language that does not offer the same flexibility than python. For example in C++: storing template classes in the same container is not possible.

* The second reason is, maybe I'm totally wrong, that Glumpy does not use by default VAOs (because it uses OpenGL < 3.3) but access to VBOs through OpenGL program attributes. This also perturbs me because in the case of you have several models (therefore several VAOs) to paint this seems problematic to draw them. In my API only VAOs have to be created. They hold VBOs and texture once bound to GLProgram. The result is equivalent to Glumpy but the internal code is different. See this file
[differences](https://github.com/Lecrapouille/OpenGLCppWrapper/blob/master/doc/Differences.md) for more differences.
