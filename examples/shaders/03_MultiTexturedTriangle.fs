// Copyright Youtube OpenGL 3D Game Tutorial 17: Multitexturing
// by ThinMatrix
// https://www.youtube.com/watch?v=-kbal7aGUpk&list=PLRIWtICgwaX0u7Rf9zkZhLoLuZVfUksDP&index=17

#version 330 core

in  vec2 texUV;     // Texture coordinate
out vec4 fragColor;
uniform sampler2D backgroundTexture;
uniform sampler2D rTexture;
uniform sampler2D gTexture;
uniform sampler2D bTexture;
uniform sampler2D blendMap;

void main()
{
    vec2 tiledCoords = texUV;

    vec4 blendMapColour = texture(blendMap, texUV);
    float backTextureAmount = 1.0 - (blendMapColour.r + blendMapColour.g + blendMapColour.b);
    vec4 backgroundTextureColour = texture(backgroundTexture, tiledCoords) * backTextureAmount;

    vec4 rTextureColour = texture(rTexture, tiledCoords) * blendMapColour.r;
    vec4 gTextureColour = texture(gTexture, tiledCoords) * blendMapColour.g;
    vec4 bTextureColour = texture(bTexture, tiledCoords) * blendMapColour.b;
    vec4 totalColour = backgroundTextureColour + (rTextureColour + gTextureColour + bTextureColour);

    fragColor = totalColour;
}
