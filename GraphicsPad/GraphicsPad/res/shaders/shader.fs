#version 130

precision highp float;

varying vec3 outColor;
varying vec2 outTexCoord;

uniform sampler2D texture;

void main()
{
	gl_FragColor = texture2D (texture, outTexCoord);
	//gl_FragColor = vec4(outColor, 1.0);
}
