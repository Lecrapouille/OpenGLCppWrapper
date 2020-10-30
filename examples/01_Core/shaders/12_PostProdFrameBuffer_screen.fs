#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D screenTexture;
uniform float time;

void main()
{
    FragColor = vec4(texture(screenTexture, TexCoords + 0.005*vec2(sin(time+1024.0*TexCoords.x),cos(time+768.0*TexCoords.y)) ).xyz, 1.0);
}
