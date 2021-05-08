#include "Vector.h"
#include <Camera.h>
#define bias 0.2f //For Spring Function

//! Constructors and Destructors 
Camera::Camera(): position(1.0,1.0,0.0)
{
	std::cerr << "No Target to Follow" << std::endl;
    ProjectionMatrix.perspective(110,1.0,0.01,100.0);
}
Camera::Camera(Vector3f posOffset, Vector3f tarOffset, Vector3f pos) : positionOffset(posOffset), targetOffset(tarOffset), position(pos)
{
    ProjectionMatrix.perspective(110,1.0,0.01,100.0);
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
void Camera::follow(Matrix4x4 &playerMatrix, Vector3f playerAxis[3],  Vector3f playerPosition, Vector3f posOffset, Vector3f targetOffset)
{
	//! Calculates where the camera should be
	Vector3f targetRight 			= targetRight.cross (Vector3f(0,1,0), playerAxis[0]);
	Vector3f targetUp 				= targetUp.cross	(playerAxis[0],targetRight); 

	

	Vector3f relativePosOffset 		= playerAxis[0] * posOffset.x 		+ targetUp * posOffset.y 	+ targetRight * posOffset.z;
	Vector3f relativeTargetOffset 	= playerAxis[0] * targetOffset.x 	+ targetUp * targetOffset.y + targetRight * targetOffset.z; 


	Vector3f updated_target 		= playerPosition + relativeTargetOffset;
	//! Lags it behind a little, looks nicer
	target							= target * (1.0f-bias) +
									  updated_target * (bias);
	position						= playerPosition + relativePosOffset;

	direction 	= direction.normalise(playerPosition-position); 

	//! Prevents Camera from Flipping
	/*
	if(std::to_string(camRight.y)[0] == '-')
	{
		std::cout << "Flippen" << std::endl;
		GlobalUp = Vector3f(0,-1,0);
	}
	else
	{
		GlobalUp = Vector3f(0, 1,0);
	}
	*/
}

//! Main Function that Updates the Camera Position
void Camera::followUpdate(Matrix4x4 &playerMatrix, Vector3f playerAxis[3], Vector3f playerPosition)
{



	//std::cout << "In of Cam: " <<player.getMeshCentroid() << std::endl;

	follow(playerMatrix, playerAxis, playerPosition, positionOffset, targetOffset);

	ViewMatrix.lookAt(
		position,
		target,
		GlobalUp
	);
}