#version 130

precision highp float;

uniform mat4 modelToProjectionMatrix;

attribute vec3 modelPosition;
attribute vec3 modelColor;
attribute vec2 modelTexCoord;

varying vec3 outColor;
varying vec2 outTexCoord;

void main()
{
	vec4 v = vec4(modelPosition, 1.0);
	gl_Position = modelToProjectionMatrix * v;
	outTexCoord = modelTexCoord;
	outColor = modelColor;
}
