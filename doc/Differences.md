# Code divergences with Glumpy/gloo

OpenGLCppWrapper is not a C++ portage of gloo part of Glumpy (written in Python) while OpenGLCppWrapper is largely inspired by Glumpy, there are internal differences shown by this page.

##### gpudata.py vs. PendingContainer.hpp and PendingData.hpp:

OpenGLCppWrapper does not use a C++ portage of numpy (like done with https://github.com/dpilger26/NumCpp). It use a simple std::vector.

##### globject.py vs. GLObject.hpp

`activate()` and `deactivate()` have been renamed to `begin()` and `end()` in OpenGLCppWrapper in memory of `glBegin()` and `glEnd()`

OpenGLCppWrapper does more checks in the `begin()`. And because in OpenGL API types are not very consistent producing compilation warnings with g++ or clang++. OpenGL `handles` are template.

OpenGLCppWrapper forces to give a name to each object. This is used for debugging in logs but also for hash tables.

##### variable.py vs. GLLocation.hpp

Because in C++ storing heterogeneous objects in vectors or maps is not possible, the class hierarchy is different. Template methods for setting shader uniforms data replace python arrays.

A major difference exists: OpenGLCppWrapper uses GLSampler which does not exist in Glumpy. Sampler holds the texture id and acts both a shader uniform and a shader attribute. They are referred to as uniform inside GLSL but in OpenGLCppWrapper VAO to holds textures and therefore shall be initialized like shader attributes. Glumpy only manage

##### shader.py vs. GLShaders.hpp

OpenGLCppWrapper does not manage snippets. I think tools like GNU M4 can do the job. OpenGLCppWrapper GLShaders only holds the GLSL code source and manage includes of other GLSL files. Contrary to Glumpy it does not detect and clean unused uniforms or attributes because in OpenGL Core 3.3 it already does the job. Some minor difference exist: OpenGLCppWrapper compile shaders in `setup()` and not in `update()`.

##### program.py vs. GLProgram.hpp

Some minor difference exist: OpenGLCppWrapper link shaders in `setup()` and not in `update()`.

Major differences exist: OpenGLCppWrapper bind VAO and draw explicit or implicit VAO. When a VAO is bound to a GLProgram, VBOs and textures are populated inside the VAO by the GLPogram.
Also once a VAO has been bound to a GLProgram it cannot be bound to a different GLProgram: this will produce an exception.

In OpenGLCppWrapper, contrary to Glumpy, a GLProgram controls the bound VAO. By "control" I mean it can change VAO states. This violates OO design but thanks to this GLProgram activate VBOs and textures hold by the VAO.

##### array.py vs. GLVAO.hpp

OpenGLCppWrapper holds VBO and textures. The list of VBOs and textures are populated by the GLProgram when a VAO is bound to the GLProgram. This is a completely different behavior compared to Glumpy. In my opinion, this allows easily to create several textured VAO from the same GLProgram. My Python knowledge is not good and maybe I did not understand well the states of Glumpy which seems to store texture and VBOs inside the GLProgram. So sharing 2 VAO from the same shader program is not possible.

In OpenGLCppWrapper, modifying a shader uniform is made from the GLProgram, while modifying a VBO or texture is made from the bound VAO. In Glumpy all is made directly from the shader program.

In OpenGLCppWrapper, modifying a VBO makes the VAO forced to check if all its VBO have the same size.

##### texture.py vs GLTextures.hpp

OpenGLCppWrapper uses SOIL library and implement `load()` methods for loading jpeg, bmp ... files. It use a structure TextureOptions for its setup. OpenGLCppWrapper call less glBindTexture in `setup` and `update`.

OpenGLCppWrapper manages 3D textures but Glumpy not. OpenGLCppWrapper manages Cube texture differently: it holds 6 2D textures and calls their methods (`update`, `setup` ...).

##### framebuffer.py vs. GLFrameBuffers.hpp

Because of Python weak type, Glumpy manages better heterogeneous types like texture or render buffer.

In the opposite, OpenGLCppWrapper uses lambda function for rendering a scene inside the active framebuffer, hiding so tedious code like `begin()`, `end()` or `glViewPort()`.

# Code divergences with opengl4csharp

* opengl4csharp explicitly creates VBOs and attach them to the VAO. This part is hidden in OpenGLCppWrapper when VAO is bound to a shader program.
* opengl4csharp explicitly binds textures. This part is also hidden in OpenGLCppWrapper by the `draw()` methods.
