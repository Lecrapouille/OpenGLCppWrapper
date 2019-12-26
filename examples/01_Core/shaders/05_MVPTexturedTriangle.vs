#version 330 core

uniform mat4   model;         // Model matrix
uniform mat4   view;          // View matrix
uniform mat4   projection;    // Projection matrix
in vec3        position;      // Vertex position
in vec2        UV;            // Vertex texture coordinates
out vec2       texUV;         // Interpolated fragment texture coordinates

void main()
{
    // Varying variables
    texUV = UV;

    // Final position
    gl_Position = projection * view * model * vec4(position, 1.0);
}
