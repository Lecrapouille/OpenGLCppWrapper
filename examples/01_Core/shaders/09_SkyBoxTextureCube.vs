#version 330 core

// ----------------------------------------------------------------------------
// Data from C++ program
// ----------------------------------------------------------------------------

// Vertex position
in vec3 position;
// Vertex texture coordinates
in vec2 UV;
// View-Projection matrices (no Model matrix needed here)
uniform mat4 projection;
uniform mat4 view;

// ----------------------------------------------------------------------------
// Data to fragment program
// ----------------------------------------------------------------------------
out struct v2f_s
{
   // Interpolated fragment texture coordinates
   vec2 UV;
} v2f;

// ----------------------------------------------------------------------------
// Vertex program
// ----------------------------------------------------------------------------
void main()
{
    // To fragment program
    v2f.UV = UV;

    // Final position
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}
