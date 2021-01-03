# Tutorial and Examples

In this folder, you will see multiple examples on how to use
OpenGLCppWrapper. They are ordered to follow each other with an increasing
difficulty. Theses examples are grouped into three categories:
- 00_Window: for everything concerning the window holding the OpenGL context,
  its input/output events (keyboard, mouse events) and 2D widgets using DearImGui.
- 01_Core: for everything concerning wrapping OpenGL routines allowing to render
  basic scenes.
- 02_Scene: for everything concerning manipulating scene tree which is an abstraction
  layer over the OpenGL wrapper layer. Scene tree allows to construct complex scene
  and manipulate objects stored as nodes and where objects have a relative position to
  their parent node.

This folder contains several examples inspired by the work of other people:
tutorials, academic document ... I added links to original codes or/and documents inside
the C++ code.

## How to build these examples

```
cd OpenGLCppWrapper/examples/
make -j8
```

To show the list of examples, type `./build/OpenGLExamples`, else pass as argument the example identifier (integer), ie `./build/OpenGLExamples 0`

## 00_Window

Example 00_BasicWindow:
=======================

This example creates the minimal template OpenGL window class. In this example the background color is changes over time introducing the delta time from the previously displayed frame.

Example 01_BasicWindowIOEvents:
===============================

This example creates a dummy OpenGL window reacting to I/O such keyboard
pressed/released event, mouse button click, mouse movement, mouse scrolling,
window resize event, get the delta time from the previously displayed frame.

Example 02_BasicWindowImGui:
============================

This example shows how to initialize [DearImGui](https://github.com/ocornut/imgui) context
to create a 2D GUI and buttons inside your OpenGL window. In this example, a simple button is added for changing the background color.

## 01_Core

All examples in this section follow the guiding line explained in the document
[tutorials](../doc/tutorials/Core.md). For summarizing it:

Initialization phase:
- Attach fragment and vertex shaders to a shader program and compile them.
- Bind a VAO to a shader program. This makes the program create automatically
  all instances of VBOs and textures inside the VAO. Program is guided by shader
  attributes and uniform variable names found in compiled shaders. Note that in
  the case you defined a variable but do not use it, OpenGL may consider it as
  dead variable and therefore this API will not create the associate VBO.
- The developer fills shader uniforms and VBO.
- The developer refers the path of desired textures.

Render phase:
- The developer may update shader uniforms (when dynamic behavior is desired).
- The developer may update shader attribute but cannot impact on the size of
  VBOs. Modified values are automatically updated to the GPU.
- Ask the shader program to draw the bound VAO.

Notes:
- For drawing multiple 3D models repeat this operation several times: one 3d
  model is handled by a VAO and its VBOs and textures.
- You can create several shader programs. But you cannot mix already bound VAOs
  to a different program shader: once they are bound the link cannot be broken.

Example 00_ColorfulTriangle:
============================

This example uses the most simple shader code for drawing a colorfull triangle.
This introduces how to construct shader from string, compile them and fill VBOs.

Example 01_DynamicTriangle:
===========================

This example draws a dynamic triangle. Vertices positions are changed
dynamically from the CPU: modified positions are automatically transferred to
the GPU. The program also changes the color of the triangle dynamically.

Example 02_TexturedTriangle:
============================

This example draws a textured triangle.

Example 03_MultiTexturedSquare:
===============================

This example renders a multi-textured triangle. The goal of this example is to
show how to blend multiple textures on the same mesh. The code source of this
example has been inspired by the following original Youtube tutorial [Youtube
OpenGL 3D Game Tutorial 17:
Multitexturing](https://www.youtube.com/watch?v=-kbal7aGUpk&list=PLRIWtICgwaX0u7Rf9zkZhLoLuZVfUksDP&index=17)
made by ThinMatrix. The shader code has not been modified for this example.

Example 04_IndexedQuad:
=======================

This example draws a textured box. It's basically the same code than 00_ColorfulTriangle
but with more vertices and introduce indexation of vertices. No Model-View-Projection (MVP) matrix is applied and therefore the box looks like a square. We also draw vertices as
points.

Example 05_RotatingQuad:
========================

This example follows the example 04_IndexedQuad but apply a Model-View-Projection
transformation matrix to each vertices. We also apply a rotation to the cube to
make it spin around an axis.

Example 06_IndexedSphere:
=========================

Render a sphere made of points (instead of triangles). The goal of this tutorial
is to show how to draw a VAO using a VBO and its vertices index (EBO).

The code source of this example has been inspired by the following original code
source: https://github.com/invor/simplestGraphRendering

Example 07_MultipleObjects:
===========================

The goal of this example is to show how to draw several models from the same VAO
(instead of creating several instances of VAO). This example paints three cubes
(2 from the same model => same VAO) placed on a floor (second model => second VAO). The
first cube is spinning while the second does not move. Models are textured and
we applied different color on textures. We use the model matrix of the MVP
transformation to place objects inside the world.

Example 08_TerrainTexture3D:
============================

This example renders a terrain made from a 3D texture. The goal of this example
is to show how to use 3d textures.

The code source of this example has been inspired by the following academic
document:
http://www.mathematik.uni-marburg.de/~thormae/lectures/graphics1/graphics_7_1_eng_web.html#1
and the original code source (Legacy OpenGL) can be downloaded here:
https://www.uni-marburg.de/fb12/en/researchgroups/grafikmultimedia/lectures/graphics

Example 09_SkyBoxTextureCube:
=============================

This example render a skybox and no object inside the skybox. Show how to use cubic
textures. In this example we do not focus on the camera.

The code source of this example has been inspired by the following original
document and code source: https://learnopengl.com/Advanced-OpenGL/Cubemaps

Example 10_SkyBoxShape:
=======================

Follow 09_SkyBoxTextureCube and add a turning box. In this example we do not focus
on the camera.

Example 11_ComplexShader:
=========================

This example shows a complex shader example picked from https://shaderfrog.com
just for testing my API. The shader code comes from Universe Nursery by
andrewray https://shaderfrog.com/app/view/47 The goal of this example was yo
check if my API can make it run.

Example 12_BasicLighting:
=========================

This example shows how to apply a light and material to a box. This example has
been inspired by the following document and its code source:
http://www.opengl-tutorial.org

Example 13_PostProdFrameBuffer:
===============================

This example paints a cube using the framebuffer and then apply some post
processing 2D effect. The goal of this example is to show how to use a basic
framebuffer (texture + depth test).

The code source of this example has been inspired by the following document and
code source: https://learnopengl.com/Advanced-OpenGL/Framebuffers and
http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/

## 02_Scene

Since we saw how easily we can create OpenGL scenes, we may want abstract a
little more and do not manage shaders and shape yourself. In addition, we want to give to
objects a relative position transformation between them : when moving one, we
want child objects attach to them have their position updated automatically.
