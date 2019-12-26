# Tutorial and Examples

In this folder, you will see multiple examples on how to use
OpenGLCppWrapper. They are ordered to follow each other with an increasing
difficulty. Theses examples are split into three categories:
- 00_Window: for everything concerning the window holding the OpenGL context,
  its input/output events (keyboard, mouse events) and 2D widgets.
- 01_Core: for everything concerning wrapping OpenGL routines allowing to render
  basic scenes.
- 02_Scene: for everything concerning scene graph which is a layer upper 01_Core
  allowing to construct OpenGL scenes without digging too much in shaders.

This folder contains several examples inspired by the work of other people:
tutorials,academic document ... I added links to original codes or/and documents.

## How to build these examples

```
cd OpenGLCppWrapper/examples/
make -j8
./build/OpenGLExamples
./build/OpenGLExamples 0
```

## 00_Window

Example 00:
===========

This example creates a dummy OpenGL window, no more!

Example 01:
===========

This example creates a dummy OpenGL window reacting to I/O such keyboard
pressed/released event, mouse button click, mouse movement, mouse scrolling,
window resize event, get the delta time from the previously displayed frame.

Example 02:
===========

This example shows how to initialize [Dear
ImGui](https://github.com/ocornut/imgui) context to create widgets and buttons
inside your OpenGL window. In this example, a simple button is added for
changing the background color.

## 01_Core

All examples in this section follow the guiding line explained in the document
[tutorials](https://github.com/Lecrapouille/OpenGLCppWrapper/blob/master/doc/tutorials/Core.md). For
summarizing it:

Initialization phase:
- Attach fragment and vertex shaders to a shader program and compile them.
- Bind a VAO to a shader program. This makes the program create automatically
  all instances of VBOs and textures inside the VAO. Program is guided by shader
  attributes and uniform variable names found in compiled shaders. Note that in
  the case you defined a variable but do not use it, OpenGL may consider it as
  dead variable and therefore this API will not create the associate VBO.
- The developer fills shader uniforms and VBO.
- The developer refer to desired textures.

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

Example 03:
===========

This example uses the most simple shader code for drawing a triangle in a single
color.

Example 04:
===========

This example draws a dynamic triangle. Vertices positions are changed
dynamically from the CPU: modified positions are automatically transferred to
the GPU. The program also changes the color of the triangle.

Example 05:
===========

This example draws a textured triangle.

Example 06:
===========

This example renders a multi-textured triangle. The goal of this example is to
show how to apply multiple textures on the same mesh. The code source of this
example has been inspired by the following original Youtube tutorial [Youtube
OpenGL 3D Game Tutorial 17:
Multitexturing](https://www.youtube.com/watch?v=-kbal7aGUpk&list=PLRIWtICgwaX0u7Rf9zkZhLoLuZVfUksDP&index=17)
made by ThinMatrix. The shader has not been modified for this example.

Example 07:
===========

This example draws a textured cube. It's basically the same code than Example 05
but with more vertices. The displayed box looks like more a square than a cube
because in this example we did not apply a Model-View-Projection (MVP) matrix to
the cube.

Example 08:
===========

This example follows the example 06 but apply a Model-View-Projection
transformation matrix to each vertices. We also apply a rotation to the cube to
make it spin.

Example 09:
===========

The goal of this example is to show how to draw several models from the same VAO
(instead of creating several instances of VAO). This example paints two cubes
(same model => same VAO) placed on a floor (second model => second VAO). The
first cube is spinning while the second does not move. Models are textured and
we applied different color on textures. We use the model matrix of the MVP
transformation to place objects inside the world.

Example 10:
===========

Render a sphere made of points (instead of triangles). The goal of this tutorial
is to show how to draw a VAO using a VBO and its vertices index (EBO).

The code source of this example has been inspired by the following original code
source: https://github.com/invor/simplestGraphRendering

Example 11:
===========

This example renders a terrain made from a 3D texture. The goal of this example
is to show how to use 3d textures.

The code source of this example has been inspired by the following academic
document:
http://www.mathematik.uni-marburg.de/~thormae/lectures/graphics1/graphics_7_1_eng_web.html#1
and the original code source (Legacy OpenGL) can be downloaded here:
https://www.uni-marburg.de/fb12/en/researchgroups/grafikmultimedia/lectures/graphics

Example 12:
===========

This example render skybox. The goal of this example is to show how to use cubic
textures. In this example we do not focus on the camera.

The code source of this example has been inspired by the following original
document and code source: https://learnopengl.com/Advanced-OpenGL/Cubemaps

Example 13:
===========

This example follows the example 12 and render a rotating box and a camera to
allow the user to displace inside the scene.

Example 14:
===========

This example paints a 2D quad using a framebuffer. The goal of this example is
to show how to use a simple framebuffer (texture only).

The code source of this example has been inspired by the following code: Glumpy
https://github.com/glumpy/glumpy/blob/master/examples/gloo-framebuffer.py

Example 15:
===========

This example paints a cube using the framebuffer and then apply some post
processing 2D effect. The goal of this example is to show how to use a basic
framebuffer (texture + depth test).

The code source of this example has been inspired by the following document and
code source: https://learnopengl.com/Advanced-OpenGL/Framebuffers and
http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/

Example 16:
===========

This example shows a complex shader example picked from https://shaderfrog.com
just for testing my API. The shader code comes from Universe Nursery by
andrewray https://shaderfrog.com/app/view/47 The goal of this example was yo
check if my API can make it run.

Example 17:
===========

This example shows how to apply a light and material to a box. This example has
been inspired by the following document and its code source:
http://www.opengl-tutorial.org

## 02_Scene

Since we saw how easily we can create OpenGL scenes, we may want abstract a
little more and do not manage shaders yourself. In addition, we want to give to
objects a relative position transformation between them : when moving one, we
want other objects have their position updated automatically.

In these examples, we will see how to create a scene graph of predefined shapes
using materials in the same way that https://threejs.org/ does.

Example 18:
===========

This example shows how to create a shape using a basic material.

Example 19:
===========

This example shows how to create a shape using a depth material: object have a
black/grey/black color. Farther they are, farther they are darker.

Example 20:
===========

This example shows how to create a shape using a normals material: object have a
color depending on the direction of their face normals.

Example 21:
===========

This example shows different scene nodes and how to attach object between them.

Example 22:
===========

This example shows how different ways to create a scene graph. The scene is made
of 3 robots (three nodes) and each of them is made of several nodes (head, arms,
legs ...) We dynamically change matrices of robots for moving them. The
DearImGui lib is used for debugging the graph.

The code source of this example has been inspired by the following document:
"New Castle University, Tutorial 6: Scene Graphs"
https://research.ncl.ac.uk/game/mastersdegree/graphicsforgames/scenegraphs/Tutorial%206%20-%20Scene%20Graphs.pdf

Example 23:
===========

Reuse the example 22 but add a fog effect.

Example 24:
===========

Reuse the example 22 but add a several lights.
