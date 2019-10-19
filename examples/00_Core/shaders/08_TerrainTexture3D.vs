#version 330 core

in vec3        position;      // Vertex position
in vec3        UV;            // Vertex texture coordinates
out vec3       texUV;         // Interpolated fragment texture coordinates

uniform mat4   model;
uniform mat4   view;
uniform mat4   projection;

void main()
{
    // Varying variables
    texUV = UV;

    // Final position
    gl_Position = projection * view * model * vec4(position, 1.0);
}
