## Core OpenGL Tutorials

This group of tutorials will show you how to create, manipulate and render OpenGL
objects. These examples show the goal of OpenGLCppWrapper: from given program shaders
(fragment, vertex), OpenGLCppWrapper will create for you OpenGL objects: VAO, VBO,
EBO, textures (1D, 2D, 3D, Cube), framebuffer. You only have to fill data to them (vertices,
colors, texture coordinates, load JPEG picture ...) and manipulate these data from CPU:
all modified data will be transfered to GPU.

Example 00:
===========

This a template example. It hold how you can create shaders from C++ string, compile them,
and fill your first vertices and color to display a triangle.

Example 01:
===========

Following the previous example, this example show you that you can modify vertices of your
triangles from CPU side and modified vertices are transfered to GPU.

Example 02:
===========

Following the previous example. This example shows you how to
apply it a texture. Like for vertices the texture is both side CPU and GPU side.

Example 03:
===========

Following the previous example, the triangle is now a square. This example shows you how to
apply it several textures to the square. This can be used for texturing landscape where you
use 3 different terrain textures and the 4th texture is used to blend textures.

Example 04:
===========

Show you how to use indexed VBO (aka EBO or Element Buffer Object) for rendering a square.

Example 05:
===========

Following the previous example but make the cube rotate. This example shows you how to apply
Model-View-Projection matrices.

Example 06:
===========

Follow the Example 04. Show you how to use indexed VBO for rendering a sphere with dots instead of triangles.

Example 07:
===========

Follow Example 05. Show you how to render several objects from the same VAO.

Example 08:
===========

Show how to use 3D texture. Here for rendering a 3d terrain.

Example 09:
===========

Show how to use Cube texture. Here for rendering a skybox.

Example 10:
===========

Show post production effect using a framebuffer.

Example 11:
===========

Show a complex shader example.

Example 12:
===========

Show basic light implementation
