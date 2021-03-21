#version 120
uniform vec4 Colour_uniform;
uniform sampler2D TexureMap_uniform;

varying vec2 uv;

void main()
{
	gl_FragColor = texture2D(TexureMap_uniform, uv);
}
