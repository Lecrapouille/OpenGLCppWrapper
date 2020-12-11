#version 330 core

// ----------------------------------------------------------------------------
// Data from C++ program
// ----------------------------------------------------------------------------

// Vertex position
in vec2 position;
// Vertex texture coordinates
in vec2 UV;

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
    gl_Position = vec4(position.x, position.y, 0.0, 1.0);
}
