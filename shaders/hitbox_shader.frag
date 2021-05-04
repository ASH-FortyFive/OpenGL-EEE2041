#version 120

uniform vec3 Colour_uniform;

void main()
{
	gl_FragColor = vec4(Colour_uniform,1.0) + vec4(1.0,0.0,0.0,1.0);
}
