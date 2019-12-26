#version 330 core

in vec3        position;      // Vertex position
in vec2        UV;            // Vertex texture coordinates
out vec2       texUV;         // Interpolated fragment texture coordinates

void main()
{
    // Varying variables
    texUV = UV;

    // Final position
    gl_Position = vec4(position, 1.0);
}
