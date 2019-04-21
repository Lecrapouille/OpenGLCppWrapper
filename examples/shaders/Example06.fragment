#version 330 core

in  vec3 texUV;     // Texture coordinate
out vec4 fragColor;
uniform sampler3D tex3d;

void main()
{
    fragColor = texture(tex3d, texUV.xyz);
}
