#version 330 core

// ----------------------------------------------------------------------------
// Data from C++ program
// ----------------------------------------------------------------------------

// Vertex position
in vec3 position;
// Vertex texture coordinates
in vec3 UV;
// Model-View-Projection matrix
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// ----------------------------------------------------------------------------
// Data to fragment program
// ----------------------------------------------------------------------------
out struct v2f_s
{
   // Interpolated fragment texture coordinates
   vec3 UV;
} v2f;

// ----------------------------------------------------------------------------
// Vertex program
// ----------------------------------------------------------------------------
void main()
{
    // To fragment program
    v2f.UV = UV;

    // Final position
    gl_Position = projection * view * model * vec4(position, 1.0);
}
