// Copyright: https://learnopengl.com/

#version 330 core

// ----------------------------------------------------------------------------
// Data from C++ program
// ----------------------------------------------------------------------------

// Vertex position
in vec3 position;
// Vertex texture coordinates
in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// ----------------------------------------------------------------------------
// Data to fragment program
// ----------------------------------------------------------------------------
out struct v2f_s
{
   vec3 position;
   vec3 normal;
} v2f;

// ----------------------------------------------------------------------------
// Vertex program
// ----------------------------------------------------------------------------
void main()
{
    // To fragment program
    v2f.position = vec3(model * vec4(position, 1.0));
    v2f.normal = mat3(transpose(inverse(model))) * normal;

    // Final position
    gl_Position = projection * view * vec4(v2f.position, 1.0);
}
