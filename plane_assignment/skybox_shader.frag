#version 120

uniform samplerCube cubemape;

varying vec2 uv;

void main()
{
	gl_FragColor = texture(skyboxVertices, uv);
}
