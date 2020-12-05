#version 330 core

// ----------------------------------------------------------------------------
// Data from C++ program
// ----------------------------------------------------------------------------

in vec3        position;    // Vertex position
in vec2        UV;          // Vertex texture coordinates
uniform float  scale;       // Global scaling
uniform mat4   model;       // Model matrix
uniform mat4   view;        // View matrix
uniform mat4   projection;  // Projection matrix
uniform vec4   color;       // Global color

// ----------------------------------------------------------------------------
// Data to fragment program
// ----------------------------------------------------------------------------
out struct v2f_s
{
   vec2 UV;         // Interpolated fragment texture coordinates
   vec4 color;      // Interpolated fragment color (out)
} v2f;

// ----------------------------------------------------------------------------
// Vertex program
// ----------------------------------------------------------------------------
void main()
{
    // To fragment program
    v2f.UV = UV;
    v2f.color = color;

    // Final position
    gl_Position = projection * view * model * vec4(scale * position, 1.0);
}
