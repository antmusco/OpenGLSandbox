#version 130

precision highp float;

varying vec3 outColor;

void main()
{
	gl_FragColor = vec4(outColor, 1.0);
}
