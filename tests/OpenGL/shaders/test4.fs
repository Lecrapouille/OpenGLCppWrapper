#version 330 core

in struct v2f_s {
  vec3 color;
} v2f;

out vec4 fragColor;

void main() {
  fragColor = vec4(v2f.color, 1);
}
