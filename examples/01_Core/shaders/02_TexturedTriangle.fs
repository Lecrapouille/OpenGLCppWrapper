#version 330 core

uniform sampler2D  texID;     // Texture sampler
in  vec2           texUV;     // Texture coordinate
out vec4           fragColor;

void main()
{
    fragColor = texture(texID, texUV);
}
