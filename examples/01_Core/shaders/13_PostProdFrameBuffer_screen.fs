#version 330 core

// ----------------------------------------------------------------------------
// Data from C++ program
// ----------------------------------------------------------------------------

// Texture sampler
uniform sampler2D texID;
uniform float time;
uniform float screen_width;
uniform float screen_height;

// ----------------------------------------------------------------------------
// Data from vertex program
// ----------------------------------------------------------------------------
in struct v2f_s
{
   // Interpolated fragment texture coordinates
   vec2 UV;
} v2f;

// ----------------------------------------------------------------------------
// Fragment program output
// ----------------------------------------------------------------------------
out vec4 fragColor;

// ----------------------------------------------------------------------------
// Fragment program
// ----------------------------------------------------------------------------
void main()
{
    // Final color
    fragColor = vec4(texture(texID, v2f.UV +
                             0.005 * vec2(sin(time + screen_width * v2f.UV.x),
                             cos(time + screen_height * v2f.UV.y))
                            ).xyz, 1.0);
}
