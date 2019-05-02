Example 01:
===========

This example paints two cubes (same model = same VAO) placed on a
floor (second model => second VAO). The fisrt cube is turning while
the second is fixed. Models are textured and we applied different
color on textures. This example has two compilation options: either
draw the scene just with VBO or draw the scene with VBO + vertex
indices (EBO).

The goal of this example is to show how to draw several models from
the same VAO (instead of creating X VAOs).

Example 02:
===========

This example paints a scene graph made of 3 moving robots.  A scene
graph is a graph (in our case a tree) where each nodes contains 2
things: an 4x4 transformation matrix to be applied to an optional 3D
object. The transformation matrix (translation, rotation, scaling) is
relative to the parent node.

The scene is made of 3 robots (three nodes) and each of them is made
of several nodes (head, arms, legs ...) We dynamicaly change matrices
of robot for moving them. The DearIMGui lib is used for displaying the
graph.

The code source of this example has been inspired by the following
document: "New Castle University, Tutorial 6: Scene Graphs"
https://research.ncl.ac.uk/game/mastersdegree/graphicsforgames/scenegraphs/Tutorial%206%20-%20Scene%20Graphs.pdf

The goal of this example is to show how to make a scene graph.

Example 03:
===========

Plot a sphere made of points: 1 VAO + 1 VBO + 1 EBO.

The code source of this example has been inspired by the following
code source: https://github.com/invor/simplestGraphRendering

Example 04:
===========

This example paints a multi-textured triangle. The goal of this
example is to show how to use multiple textures.

The code source of this example has been inspired by the following
[Youtube OpenGL 3D Game Tutorial 17: Multitexturing](https://www.youtube.com/watch?v=-kbal7aGUpk&list=PLRIWtICgwaX0u7Rf9zkZhLoLuZVfUksDP&index=17)
made by ThinMatrix.

Example 05:
===========

This example paints skybox. The goal of this example is to show how to
use cubic textures.

The code source of this example has been inspired by the following document
https://learnopengl.com/Advanced-OpenGL/Cubemaps


Example 06:
===========

This example paints a terrain made with a 3d texture. The goal of this
example is to show how to use 3d textures.

The code source of this example has been inspired by the following document:
http://www.mathematik.uni-marburg.de/~thormae/lectures/graphics1/graphics_7_1_eng_web.html#1
and https://www.uni-marburg.de/fb12/en/researchgroups/grafikmultimedia/lectures/graphics

Example 07:
===========

This example paints a 2d quad using framebuffer. The goal of this example is to show how to use minimalist framebuffer.

The code source of this example has been inspired by the following code: Glumpy gloo-framebuffer.py

Example 08:
===========

This example paints a 3d cube using framebuffer. The goal of this example is to show how to a simple framebuffer.

The code source of this example has been inspired by the following document: https://learnopengl.com/Advanced-OpenGL/Framebuffers
