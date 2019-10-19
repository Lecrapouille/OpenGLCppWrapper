#version 150

precision highp float;
precision highp int;

attribute vec3 position;
attribute vec2 uv;

varying vec2 vUv;
varying vec3 vPosition;
varying vec3 vNormal;

void main()
{
  vUv = uv;
  gl_Position = vec4(position, 1.0);//projection * view * model * vec4(position, 1.0);
}
