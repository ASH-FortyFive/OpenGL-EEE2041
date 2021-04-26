#include "Vector.h"
#include <Camera.h>

//! Constructors and Destructors 
Camera::Camera(): position(1.0,1.0,0.0), target(0.0,0.0,0.0) 
{
    ProjectionMatrix.perspective(90,1.0,0.01,100.0);
}
Camera::Camera(Vector3f pos, Vector3f look) : position(pos), target(look) 
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
		target,
		Vector3f(0, 1, 0 )
	);
}

//! Accessors
Vector3f Camera::getPosition()
{
    return position;
}
Vector3f Camera::getDirection()
{
    return direction.normalise(position-target);
}



//! Moves Camera
void Camera::move(Vector3f increment)
{
	position = position + increment;
	target = target + increment;
}

//! Follows Given Object
void Camera::follow(Vector3f newTarget, Vector3f facing, Vector3f posOffset, Vector3f targetOffset)
{
	Vector3f targetRight = targetRight.normalise(right.cross(GlobalUp, facing));
	Vector3f targetUp = up.cross(facing,targetRight);
	Vector3f relativePosOffset = facing * posOffset.x + targetUp * posOffset.y + targetRight * posOffset.z;
	Vector3f relativeTargetOffset = facing * targetOffset.x + targetUp * targetOffset.y + targetRight * targetOffset.z;

    target 	 = newTarget;// + relativeTargetOffset;
	position = newTarget + relativePosOffset;
	

	//this->update(); 
	
	
}

//! Main Function that Updates the Camera Position
void Camera::update()
{
	//! These are already calcuated with the LookAt function, however we need these values again
	//! Vectors that points (away? from) the camera to the target
	direction = direction.normalise(position-target);
	//! To the Right of Camera
	right = right.normalise(right.cross(GlobalUp, direction));
	//! Up from Camera
	up = up.cross(direction,right);
}

void Camera::debugPrint()
{

}