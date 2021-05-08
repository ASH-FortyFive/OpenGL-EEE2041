#include "Vector.h"
#include <Camera.h>

//! Constructors and Destructors 
Camera::Camera(): position(1.0,1.0,0.0)
{
	std::cerr << "No Target to Follow" << std::endl;
    ProjectionMatrix.perspective(90,1.0,0.01,100.0);
}
Camera::Camera(Vector3f posOffset, Vector3f tarOffset, Vector3f pos) : positionOffset(posOffset), targetOffset(tarOffset), position(pos)
{
    ProjectionMatrix.perspective(90,1.0,0.01,100.0);
}
Camera::~Camera()
{}

//! Perspective
void Camera::updateShader(MasterShader shader)
{
	glUseProgram(shader.ID);

    glUniformMatrix4fv(
		shader.ProjectionUniformLocation,	//Uniform location
		1,							//Number of Uniforms
		false,						//Transpose 
		ProjectionMatrix.getPtr());	//Pointer to ModelViewMatrixValues

	glUniformMatrix4fv(
        shader.ViewMatrixUniformLocation,
        1,
        false,
        ViewMatrix.getPtr());
}

//! Accessors
Vector3f Camera::getPosition()
{
    return position;
}
Vector3f Camera::getDirection()
{
    return direction;
}


//! Moves Camera
void Camera::move(Vector3f increment)
{
	position = position + increment;
}

//! Follows Given Object
void Camera::follow(Vector3f playerForward, Vector3f playerRotation, Vector3f playerPosition, Vector3f posOffset, Vector3f targetOffset)
{
	//! Not quite right
	/*
	if(rot.z > 90.0f && rot.z < 270.0f)
	{
		GlobalUp = Vector3f(0,-1,0);
	}
	else
	{
		GlobalUp = Vector3f(0,1,0);
	}
	*/
	
	targetRight 			= targetRight.normalise(targetRight.cross(GlobalUp, playerForward));
	targetUp 				= targetUp.cross(playerForward,targetRight);
	relativePosOffset 		= playerForward * posOffset.x + targetUp * posOffset.y + targetRight * posOffset.z;
	//Vector3f relativeTargetOffset 	= playerForward * targetOffset.x + targetUp * targetOffset.y + targetRight * targetOffset.z;


	target 		= playerPosition;// + relativeTargetOffset;
	position 	= playerPosition + relativePosOffset;

	direction 	= (position-playerPosition);
}

//! Main Function that Updates the Camera Position
void Camera::followUpdate(Vector3f playerForward, Vector3f playerRotation, Vector3f playerPosition)
{

	//std::cout << "In of Cam: " <<player.getMeshCentroid() << std::endl;

	follow(playerForward, playerPosition, playerPosition, positionOffset, targetOffset);

	ViewMatrix.lookAt(
		position,
		target,
		GlobalUp
	);
}