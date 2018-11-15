# OpenGL Wrapper API Architecture

This API allows to wrap OpenGL Core API (>= 3.3) within C++
classes. It has been greatly inspired by the Python library
[GLumpy](https://glumpy.github.io/),

**Warning:** Do not confuse OpenGL Core (>= 3.3) with OpenGL Legacy (<=
3.2). This API does not manage OpenGL Legacy.

**Warning:** *this API is currently in gestation. This document is the
expected final API behavior, when reading this line, the current
version may not yet be totally compliant with this page. If you notice
something wrong, please contribute by reporting either a bug or a
fix !*

## OpenGL terminology

Basic OpenGL objects offered to the developers are the following:

* Vertex Buffer Object (VBO) is a portion of GPU memory in which we
store data like vertices, textures, normals, colors.

* Element Buffer Object (EBO) is an buffer indexing vertices in the
aim to save GPU memory by replacing duplicated information by an
index. Usually compression factor: a 3D coordinate in float is
replaced by an uint8 identifier.

* Vertex Array Object (VAO) is an object referencing several VBO (and
up to one EBO) and telling OpenGL how to interpret data. VAOs were
optional in traditional OpenGL version but they are now mandatory.
VAO they avoid for each display of the scene to transfer data from CPU
into VBOs and prevent telling to OpenGL how to interpret data.

* VAO links to VBO is named attribute locations are also an OpenGL
object.

* VAO can be seen has the identifier of a 3D model. For example in a 3D
scene showing two house, and one car: you have to create two VAOs: one
for the car, one for the house. When displaying the two house we just
display the same object at two different location.

* Shader is a program that can be run by the GPU with a syntax near to
the C language.

* Program is a set a several compiled shaders (at least vertex and
fragment). Once compiled, only the program is used, shaders are no
longer useful and usually detached from the memory while the program
is used.

* Uniform, attributes are variables in shaders. Attributes are data
transitioning in graphics pipeline (mainly vertices, colors). While
uniforms are constant over all shaders.

* Attribute locations: is not because your shader attribute is named
'position' that is enough to make the link with the your VBO named
'position'. You have to tell OpenGL the link between the VAO and the
VBO.

* RenderBuffer: OpenGL can either paint in the screen buffer or in a
texture. The last option allows to create sophisticate 2D effect by
injecting the texture into a second program shader that will apply a
more complex image processing before displaying to result into either
the screen or to a new texture (for a 2nd process for
example). Different kind of renderer buffers can be: DepthBuffer (the
Z coordinate of object are converted into a grey texture) or
ColorBuffer or StencilBuffer (for cutting regions of the scene).

## OpenGL Object class

The following table allows to understand that OpenGL routines for any
kind of OpenGL objects (VAO, VBO ...) can be grouped into six main
functions: creation/relase, activation/deactivation, setup/update.
[Click here](https://github.com/Lecrapouille/OpenGLCppWrapper/blob/master/doc/CodeSheet.txt)
to see in raw format the table.

|              | create()             | setup()                                       | activate()                                       | update()        | deactivate()               | release()                      | handle   | target                                                   |
|--------------|----------------------|-----------------------------------------------|--------------------------------------------------|-----------------|----------------------------|--------------------------------|----------|----------------------------------------------------------|
| VAO          | glGenVertexArrays    | -                                             | glBindVertexArray                                | -               | glBindVertexArray          | glDeleteVertexArrays           | create() | -                                                        |
| VBO          | glGenBuffers         | glBufferData                                  | glBindBuffer                                     | glBufferSubData | glBindBuffer               | glDeleteBuffers                | create() | GL_ARRAY_BUFFER                                          |
| EBO          | glGenBuffers         | glBufferData                                  | glBindBuffer                                     | glBufferSubData | glBindBuffer               | glDeleteBuffers                | create() | GL_ELEMENT_ARRAY_BUFFER                                  |
| Shader       | glCreateShader       | GlShaderSource, GlCompileShader               | -                                                | -               | -                          | glDeleteShader                 | create() | GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER |
| Program      | glCreateProgram      | GlAttachShader, GlLinkProgram, GlDetachShader | glUseProgram                                     | -               | glUseProgram               | GlDeleteProgram,GlDetachShader | create() | -                                                        |
| Uniform      | glGetUniformLocation | -                                             | -                                                | -               | -                          | -                              | create() | -                                                        |
| Attribute    | glGetAttribLocation  | -                                             | GlEnableVertexAttribArray, GlVertexAttribPointer | -               | glDisableVertexAttribArray | -                              | create() | -                                                        |
| RenderBuffer | glGenRenderbuffers   | -                                             | GlBindRenderbuffer, GlRenderbufferStorage        | -               | glBindRenderbuffer         | -                              | create() | GL_RENDERBUFFER                                          |
| Texture      | glGenTextures        | glTexParameterf                               | glBindTexture                                    |                 | glBindTexture              | glDeleteTextures               | create() |                                                          |
| Texture1D    |                      | glTexImage1D                                  |                                                  | glTexSubImage1D |                            |                                |          | GL_TEXTURE_1D                                            |
| Texture2D    |                      | glTexImage2D                                  |                                                  | glTexSubImage2D |                            |                                |          | GL_TEXTURE_2D                                            |
| TextureDepth |                      | glTexImage2D                                  |                                                  | glTexSubImage2D |                            |                                |          | GL_TEXTURE_2D                                            |
| Texture3D    |                      | glTexImage2D                                  |                                                  | glTexSubImage2D |                            |                                |          | GL_TEXTURE_CUBE_MAP                                      |

API routines are:

* `create()` creates an OpenGL object in the GPU. An OpenGL `handle`
is returned which is mainly a reference on the GPU memory that the CPU
can use. Instead of letting the CPU manipulating an address from GPU,
OpenGL simply returns an identifier (1, 2 ...). Note that 0 has a
particular meaning and used for unbinding an Object (more details
below).

* `release()` allows to liberate OpenGL Object from the GPU memory.

* `setup()` allows to set parameters the object (like done for
  textures, Shader, Program).

* `activate()` allows to bind `handle` to OpenGL and `deactivate()` to
  unbind (0 is used instead of handle). What does binding mean ?
  OpenGL can only operate with a single object for each kind of object
  (a single VAO is used at a time, as well as a single VBO,
  Program ...). You can see that like a global variable inside the
  OpenGL state machine that you change with bind functions. The main
  advantage is that do not have to pass this object identifier in all
  OpenGL functions. They all refer to binded objects (this would also
  avoid to OpenGL to get more than one binded objects). The drawback
  with this method, is that you can easily forget to bind/unbind
  identifiers.

These routines are pure virtual methods in the interface class named
IGLObject. Any OpenGL Object is a class inheriting from IGLObject has
to implement all of these methods.

The following picture shows the inheritance of OpenGL classes.

![alt tag](https://github.com/Lecrapouille/OpenGLCppWrapper/blob/master/doc/uml/opengl_inhe.png)

The table is a summary of which OpenGL functions is called by the
concrete methods (implementation). For example the setup for Program
needs to call different OpenGL functions (for our API).

If we think well (GLumpy did it for me ;-) we can hide the presence of
these methods to the developer: indeed they are private methods in
IGLObject: they are acceded by two public method: begin() and end()
which call them inside a predefined order. It's the [template method
pattern](https://www.amazon.com/First-Design-Patterns-Elisabeth-Freeman/dp/0596007124)
permitting to encapsulate algorithms.

Here is the pseudo code of the begin() method:
```
begin()
{
  if (need_create) { need_create = create() || return; }
  activate();
  if (need_setup) { need_setup = setup() || return; }
  if (need_update) { need_update = update(); }
}

end()
{
  deactivate();
}
```

**Note:** This is a pseudo code, the real performs more checks, for
example if a method fails (for example OpenGL context is not yet
created) it can be redone later.

need_create, need_setup, need_update are private states of IGLObject:
they remember what method to call or not to call anymore (for example
creation is made once and setup is usually made once, except if the
developer desire to change its behavior).

## Pending Data

The main idea of GLumpy, that is API also uses is to allow the
developer to modify data located in the CPU. Modified data are set as
"dirty" and set in "pending" attempt to be transferred to the GPU for
their display. This is the role of the update() method. Pending data
are particularly useful for VBOs, textures and uniforms.

As a consequence you can work with a not ready OpenGL context. VBOs
are an array of data. CPU side, the API maintains for these object
index of the contiguous portion of dirty data. These data are send at
the very last moment to GPU through the update(); method.

Let suppose the following VBO array storing three consecutive 3D
position (x,y,z). Let suppose there is no current pending data.

| 0 | 0 | 0 | 1 | 1 | 1 | 2 | 2 | 2 | PendingData={,} |
|---|---|---|---|---|---|---|---|---|-----------------|

Let change the y position of the 1st position with the value 42 which is now "dirty".

| 0 | 42 | 0 | 1 | 1 | 1 | 2 | 2 | 2 | PendingData={1,1} |
|---|----|---|---|---|---|---|---|---|-------------------|

Pending data is now referring to the first position of the array.

Now let change the z position of the 2nd position with the value 43 which is now "dirty".

| 0 | 42 | 0 | 1 | 1 | 43 | 2 | 2 | 2 | PendingData={1,5} |
|---|----|---|---|---|----|---|---|---|-------------------|

Now all data from position 1 to 5 are considered as dirty. Pending
data is now referring to the first and fifth position of the array.

Now let change the x position of the 1st position with the value 44 which is now "dirty".

| 44 | 42 | 0 | 1 | 1 | 43 | 2 | 2 | 2 | PendingData={0,5} |
|----|----|---|---|---|----|---|---|---|-------------------|

Now all data from position 0 to 5 are considered as dirty. Pending
data is now referring to the zeroth and fifth position of the array.

Let suppose that VBO::update() is now called. All dirty data (position
0 to 5) are flushed to the GPU, there is no more dirty data and
pending data indices are cleared.

| 44 | 42 | 0 | 1 | 1 | 43 | 2 | 2 | 2 | PendingData={,} |
|----|----|---|---|---|----|---|---|---|-----------------|

Flushing bulk of data, with in our case, with some data not set as
dirty, seems not an optimized way of doing things. This is true but
this allows to reduce the number of OpenGL routines to call. In
addition, we can imagine a more sophisticated method in the case where
order of element is not important: in this case you can simply swap
position of the dirty element with the first non dirty element.

## Pseudo code of an OpenGL program

Init phase:

* First, write your fragment and vertex shaders either in a
  std::string or from a text file.

* Load the script into one of GLShader class: GLFragmentShader,
  GLVertesShader or GLGeometryShader with method fromFile() or
  fromString() to a GLProgram. A GLShader is a class storing the GLSL
  code and allowing to compile it. GLumpy allows to process it and
  macro process it for replacing/injecting piece of code. This API
  will not do this because you can use [M4 macro  code](https://en.wikipedia.org/wiki/M4_(computer_language))
  to reach the same behavior.

* Optionally add a geometry shader to the GLProgram. A GLProgram is
  the main class of this API. First it links all shaders and then
  creates a list of GLUniform, GLAttribute. The method compile() is
  just an alias to the begin() method but with a more explicit name
  and is optional.

* You have to create a GLVAO which will manages a set of GLVBOs. VBOs
  are created in the VAO when this one is binded to a GLProgram. Call
  the method GLProgram::createVAO(VAO&) to init has much as VAO as
  needed. Technically, VBOs and VAOs are not necessary yet created in
  the GPU: they will soon or later.

* For each VAO created, fill their attached VBOs with data (depending
  on your shader: vertices position, colors, textures) use the exact
  name of attributes written in shaders. VBOs have now pending data
  ready to be transmitted to the GPU.

* As help you can use GLProgram::attributeNames() to get the list of
  VBO names: they have the same name than attributes. VAO has also the
  same method for its VBOs: VBONames().

* GLProgram has methods like compiled(), hasErrored(), error(), shaderNames(),
  failedShaders(), uniformNames() to help you debugging.

Draw phase:

- For each VAO, make it used by the GLProgram with GLProgram::bind()
  then call the GLProgram::draw().


The following picture summarizes the composition of OpenGL classes
described in steps seen previously. The Client is the class
representing your application.

![alt tag](https://github.com/Lecrapouille/OpenGLCppWrapper/blob/master/doc/uml/opengl_compo.png)

## VBO attributes offset and stride

This API does not allow the developper to change them. Therefore their
values is always 0. Contrary to GLumpy which uses Numpy which natively
manages offset and stride and where VBO can be see as a std::map of
arrays refereed by a name (so a VBO can be seen as a container of
sub-VBOs) in this API when VBOs are created they are simply a single
array with no views.

In conclusion you cannot pass a single array mixing for example
vertices, colors and texture. You have to init the three created VBOs
with three arrays.
