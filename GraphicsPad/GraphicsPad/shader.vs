#version 130

precision highp float;

attribute vec2 position;
attribute vec3 color;

varying vec3 outColor;

void main()
{
	gl_Position = vec4(position, 0.0, 1.0);
	outColor = color;
}
