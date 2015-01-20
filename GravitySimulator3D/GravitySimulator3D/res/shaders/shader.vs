#version 130

precision highp float;

uniform mat4 modelToProjectionMatrix;
uniform mat4 modelToWorldMatrix;

attribute vec4 modelPosition;
attribute vec3 modelColor;
attribute vec3 modelNormal;
attribute vec2 modelTexCoord;

varying vec4 outPosition;
varying vec3 outColor;
varying vec2 outTexCoord;
varying vec3 outNormal;

void main()
{
	gl_Position = modelToProjectionMatrix * modelPosition;
	outPosition = modelToWorldMatrix * modelPosition;

	outTexCoord = modelTexCoord;

	outColor = modelColor;

	outNormal = normalize(vec3(modelToWorldMatrix * vec4(modelNormal, 0.0)));
}
