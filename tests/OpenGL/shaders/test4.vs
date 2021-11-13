#version 330 core

in vec2 position;
in vec3 color;

out struct v2f_s { vec3 color; } v2f;

void main() {
  v2f.color = color;
  gl_Position = vec4(position, 0.0, 1.0);
}
