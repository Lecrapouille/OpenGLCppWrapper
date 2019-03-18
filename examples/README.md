Example 01:
===========

This example paints two cubes (same model) placed on a floor
(second model). The fisrt cube is turning while the second is
fixed. Models are textured and we applied different color on
textures. Two options: draw the scene just with VBO or VBO + EBO.

Example 02:
===========

This example paints a scene graph made of 3 moving robots.
A scene graph is a graph (in our case a tree) where each nodes
contains 2 things: an 4x4 transformation matrix to be applied to an
optional 3D object. The transformation matrix (translation, rotation,
scaling) is relative to the parent node.

The scene is made of 3 robots (three nodes) and each of them is made
of several nodes (head, arms, legs ...) We dynamicaly change matrices
of robot for moving them. DearIMGui is used for displaying the graph.

Example 03:
===========

Plot a sphere of points (VBO + EBO). Comes from
https://github.com/invor/simplestGraphRendering

Example 04:
===========

Draw a multi-textured triangle like shown in the [Youtube OpenGL 3D Game Tutorial 17: Multitexturing](https://www.youtube.com/watch?v=-kbal7aGUpk&list=PLRIWtICgwaX0u7Rf9zkZhLoLuZVfUksDP&index=17)
made by ThinMatrix.
