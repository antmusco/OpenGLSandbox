#version 130

precision highp float;

uniform sampler2D texture;
uniform vec3 lightSource;
uniform vec4 ambientLight;

varying vec4 outPosition;
varying vec3 outColor;
varying vec2 outTexCoord;
varying vec3 outNormal;

void main()
{
	vec3 worldLight = normalize(lightSource - vec3(outPosition));
	float brightness = clamp(dot(outNormal, worldLight), 0.0, 1.0);
	vec4 diffuseLight = vec4(brightness, brightness, brightness, 1.0);
	gl_FragColor = texture2D (texture, outTexCoord) * (ambientLight + diffuseLight);
}
