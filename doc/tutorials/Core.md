# OpenGLCppWrapper Tutorial

## How to use this API? An explained 'Hello-world' example

This section will treat on how to use this API. This is a small tutorial explaining the template for your projects. Indeed, the pseudo-C++ code shown in this tutorial will be the same for many of your 3D applications that you will make. As proof, I made numerous [examples](../../examples/README.md) included with the code source of this library, which follow the pseudo-code of this tutorial. My examples are greatly inspired by several OpenGL tutorials found on the internet (see their README for credits). You can compare the length of the code.

In this document, I consider that the reader has basic knowledge of modern OpenGL (which is my case). There is nothing particularly difficult with this tutorial because everything is already largely explained in the first chapters of any OpenGL tutorials foundable on the internet.

Before showing the C++ pseudo-code using OpenGLCppWrapper, we have, first, to write a basic fragment and vertex shader named `my_fragment_shader.glsl` and `my_vertex_shader.glsl`. OpenGLCppWrapper has been conceived to develop OpenGL applications by starting with their shaders and finish by filling VBOs. This is the opposite way of thinking compared to the way tutorials make you learn OpenGL.

##### my_fragment_shader.glsl:

In this fragment shader, we simply pick the color of the texture (texel) and mix it with a uniform color. Uniforms are kind of global variables, while `in` and `out` are the inputs and outputs of the OpenGL pipeline connecting shaders to framebuffers.

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

In this vertex shader, we pass to the fragment shader the texel and the global color. We applied to vertices position a [Model-View-Projection matrix](http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices) (change of basis). For the fun, we applied an additional scaling factor. Again, this kind of basic shaders is largely described on the internet.

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

Initialization phase: This code is usually made once and in the `setup()` method (for example called by a GLWindow).

* lines 01-02 and 07-08:
GL*x*Shader, GLProgram, and GLVAO are some OpenGL objects that OpenGLCppWrapper is exposing to the developer (where *x* is referring either to Fragment or Vertex or Geometry). A GL*x*Shader holds the GLSL code source and compile it (in future it will may offer macro processing features). A GLProgram holds at least two GLxShaders (vertex and fragment) and links them. Once compiled, a GLProgram holds internally and in privacy locations of shader uniforms, shader attributes and texture samplers. A GLVAO can be considered as the OpenGL handler to your 3D model: it is an interface between CPU data and GPU data by holding VBOs and textures. You can modify data of textures and data of VBOs: the API will transfer modified values to your graphical card.

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
* Use the Transformable class to place (translate, rotate, scale) your first cube in the scene: this will create a 4x4 Matrix.
* Apply this matrix to your shader uniform matrix named "model" of the Model View Projection.
* Draw the first cube.
* Use again the Transformable for placing the second cube: redo the three previous steps but this time, draw the second cube (update transformable, apply a matrix to uniform, draw).
* See my [examples](../../examples/README.md) directory to know how to do it concretely.

With this method, for this example, you will save GPU memory by avoiding storing an additional VAO and all its VBOs needed for drawing a cube.

### What else in OpenGLCppWrapper?

Not yet shown in tutorials but OpenGLCppWrapper can manage framebuffer and cubic, 3D, 2D or 1D textures, multi-textures. It also includes features that are not directly in relation to wrapping OpenGL routines. It allows:

* to estimate the GPU memory usage.
* Make some security checks like detecting uninitilized VBOs, textures or VBO mismatch number of elements.
* (work in progress) TrueType fonts.
* to use Matrix and Vector classes (like glm library).
* to create transformable objects allowing to combine transformations like rotation, translation, projection on a model.
* to create a 'scene graph' of objects: a graph where nodes contain a transformation matrix relative to the current node and its parent. Note that the matrix is mandatory for each node of the scene graph while the 3d model is optional: this allows us to add invisible transformations.
* to wrap the initialization of [Dear ImGui](https://github.com/ocornut/imgui) an API for developing GUI in immediate mode (using just if then else instead of callbacks/signal-slot/observers methods like done with GTK+ or Qt).
* to wrap OpenGL window and offers to the user the init, run-time phase and callbacks like keyboard and windows resizing.

See my [examples](../../examples/README.md) directory to know how to do it concretely.
