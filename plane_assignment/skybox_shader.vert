#version 120
attribute vec3 aVertexPosition;

uniform mat4x4 MVMatrix_uniform;
uniform mat4x4 ProjMatrix_uniform;

void main()
{
	gl_Position = ProjMatrix_uniform * MVMatrix_uniform * vec4(aVertexPosition, 1.0);
}

