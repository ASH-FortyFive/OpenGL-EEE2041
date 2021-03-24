#include "Vector.h"
#include <Camera.h>

//! Constructors and Destructors 
Camera::Camera(): position(4.0,4.0,4.0), lookAt(2.0,2.0,2.0) 
{
    ProjectionMatrix.perspective(90,1.0,0.01,100.0);
}
Camera::Camera(Vector3f pos, Vector3f look) : position(pos), lookAt(look) 
{
    ProjectionMatrix.perspective(90,1.0,0.01,100.0);
}
Camera::~Camera()
{}

//! Perspective
void Camera::setProjection(GLuint _ProjectionUniformLocation)
{
    glUniformMatrix4fv(
		_ProjectionUniformLocation,	//Uniform location
		1,							//Number of Uniforms
		false,						//Transpose 
		ProjectionMatrix.getPtr());	//Pointer to ModelViewMatrixValues
}
void Camera::changeModelView(Matrix4x4& _ModelViewMatrix)
{
    _ModelViewMatrix.lookAt(
		position,
		lookAt,
		Vector3f(0, 1, 0 )
	);
}

//! Follows Given Object
void Camera::follow(Vector3f target, Vector3f offset)
{
    lookAt = target;
    position = lookAt + offset;
}

