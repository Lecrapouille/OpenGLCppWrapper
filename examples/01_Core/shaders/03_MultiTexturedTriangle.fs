// This shader code has been inspired by the Youtube OpenGL 3D Game Tutorial 17: Multitexturing
// made by ThinMatrix https://youtu.be/-kbal7aGUpk

#version 330 core

// ----------------------------------------------------------------------------
// Data from C++ program
// ----------------------------------------------------------------------------

// Texture samplers
uniform sampler2D backgroundTexture;
uniform sampler2D rTexture;
uniform sampler2D gTexture;
uniform sampler2D bTexture;
uniform sampler2D blendMap;

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
    vec2 tiledCoords = v2f.UV;

    vec4 blendMapColour = texture(blendMap, v2f.UV);
    float backTextureAmount = 1.0 - (blendMapColour.r + blendMapColour.g + blendMapColour.b);
    vec4 backgroundTextureColour = texture(backgroundTexture, tiledCoords) * backTextureAmount;

    vec4 rTextureColour = texture(rTexture, tiledCoords) * blendMapColour.r;
    vec4 gTextureColour = texture(gTexture, tiledCoords) * blendMapColour.g;
    vec4 bTextureColour = texture(bTexture, tiledCoords) * blendMapColour.b;

    // Final color
    fragColor = backgroundTextureColour + (rTextureColour + gTextureColour + bTextureColour);
}
