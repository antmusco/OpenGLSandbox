#version 130

precision highp float;

uniform mat4 fullTransformMatrix;

attribute vec3 position;
attribute vec3 color;

varying vec3 outColor;

void main()
{
	vec4 v = vec4(position, 1.0);
	gl_Position = fullTransformMatrix * v;
	outColor = color;
}
