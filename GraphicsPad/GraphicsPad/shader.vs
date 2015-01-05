#version 130

precision highp float;

uniform mat4 fullTransformMatrix;

attribute vec3 modelPosition;
attribute vec3 modelColor;

varying vec3 outColor;

void main()
{
	vec4 v = vec4(modelPosition, 1.0);
	gl_Position = fullTransformMatrix * v;
	outColor = modelColor;
}
