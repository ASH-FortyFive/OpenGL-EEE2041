#version 120

uniform vec4 Colour_uniform;

void main()
{
	gl_FragColor = vec4(Colour_uniform);
}
