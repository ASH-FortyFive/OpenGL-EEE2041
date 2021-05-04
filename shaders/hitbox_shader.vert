#version 120
attribute vec3 aVertexPosition;

uniform mat4x4 ModelMatrix_uniform;
uniform mat4x4 ViewMatrix_uniform;
uniform mat4x4 ProjMatrix_uniform;


void main()
{
	gl_Position =  ProjMatrix_uniform * ViewMatrix_uniform * ModelMatrix_uniform * vec4(aVertexPosition, 1.0);
}

