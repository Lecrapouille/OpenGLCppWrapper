This folder contains several examples of concrete OpenGL applications using this API.
These examples are inspired by several tutorials and academic examples. See associated credits.

As explained in the README.md file all these examples follow the global code structure:

Init phase:
- Attach fragment and vertex shaders to a shader program then compile them.
- Bind a VAO to a shader program. This makes the program create automatically all instances of VBOs and textures inside the VAO. Program is guided by shader attributes found in compiled shaders.
- The developer fills shader uniforms.
- The developer fills VBOs and textures.

Render phase:
- The developer fills shader uniforms.
- Ask the shader program to draw the bound VAO.

Notes:
- For drawing multiple 3D models repeat this operation several times: one 3d model is handled by a VAO and its VBOs and textures.
- You can create several shader programs. But you cannot mix already bound VAOs to a different program shader.

Example 00:
===========

This example creates a dummy OpenGL window reacting to some IO like keyboard press event, mouse movement, mouse scrolling, window resize event, get the delta time from the previously displayed frame.

Example 01:
===========

Use the previous example and display a simple DearImGUI widget.

Example 02:
===========

This example draws a dynamic triangle. Vertices are changed dynamically by the CPU: they are automatically transferred to the GPU. The program also changes the color of the triangle by writting in a shader uniforms variable.

Example 03:
===========

This example paints a multi-textured triangle. The goal of this example is to show how to use multiple textures. The code source of this example has been inspired by the following tutorial
[Youtube OpenGL 3D Game Tutorial 17: Multitexturing](https://www.youtube.com/watch?v=-kbal7aGUpk&list=PLRIWtICgwaX0u7Rf9zkZhLoLuZVfUksDP&index=17)
made by ThinMatrix.

Example 04:
===========

This example shows how the CPU writes on texture. The dirty portion of the texture is automatically transferred to the GPU.

Example 05:
===========

This example paints two cubes (same model => same VAO) placed on a floor (second model => second VAO). The first cube is spinning while the second does not move. Models are textured and we applied different color on textures. We use model-view-projection matrices to place objects inside the world (model matrice) and place a fixed camera (view-projection).

The goal of this example is to show how to draw several models from the same VAO (instead of creating several instances of VAO). This is possible thanks to model-view-projection matrices that we modify through shader uniforms.

Example 06:
===========

Plot a sphere made of points. The goal of this tutorial is to show how to draw a VAO using a vertices index (EBO).

The code source of this example has been inspired by the following code source: https://github.com/invor/simplestGraphRendering

Example 07:
===========

This example paints skybox. The goal of this example is to show how to use cubic textures.

The code source of this example has been inspired by the following document
https://learnopengl.com/Advanced-OpenGL/Cubemaps

Example 08:
===========

This example paints a terrain made with a 3d texture. The goal of this example is to show how to use 3d textures.

The code source of this example has been inspired by the following academic document:
http://www.mathematik.uni-marburg.de/~thormae/lectures/graphics1/graphics_7_1_eng_web.html#1
and code source (Legacy OpenGL) https://www.uni-marburg.de/fb12/en/researchgroups/grafikmultimedia/lectures/graphics

Example 09:
===========

This example paints a scene graph made of 3 moving robots using a scenegraph which is a graph (in our case a tree) where each node contains 2 things: a 4x4 transformation matrix (to be applied as a change of basis) and an optional 3D model. The transformation matrix (translation, rotation, scaling) is relative to the parent node. The model is optional because sometimes we may want a more complex transformation implying several nodes.

The scene is made of 3 robots (three nodes) and each of them is made of several nodes (head, arms, legs ...) We dynamically change matrices of robots for moving them. The DearIMGui lib is used for displaying the
graph.

The code source of this example has been inspired by the following document: "New Castle University, Tutorial 6: Scene Graphs" https://research.ncl.ac.uk/game/mastersdegree/graphicsforgames/scenegraphs/Tutorial%206%20-%20Scene%20Graphs.pdf

Example 10:
===========

This example paints a 2d quad using the framebuffer. The goal of this example is to show how to use a simple framebuffer (texture only).

The code source of this example has been inspired by the following code: Glumpy gloo-framebuffer.py

Example 11:
===========

This example paints a 3d cube using the framebuffer and then some post processing effect is applied to the scene. The goal of this example is to show how to use a basic framebuffer (texture + depth test).

The code source of this example has been inspired by the following document: https://learnopengl.com/Advanced-OpenGL/Framebuffers and http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/
