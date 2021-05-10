#include "Vector.h"
#include <Camera.h>
#define TPbias 0.4f //For Spring Function

//! Constructors and Destructors 
Camera::Camera(): position(1.0,1.0,0.0)
{
	std::cerr << "No Target to Follow" << std::endl;
    ProjectionMatrix.perspective(110,1.0,0.01,1000.0);
}
Camera::Camera(Vector3f posOffset, Vector3f tarOffset, Vector3f pos) : thirdPersonOffset(posOffset), targetOffset(tarOffset), position(pos)
{
	positionOffset = thirdPersonOffset;
    ProjectionMatrix.perspective(110,1.0,0.01,1000.0);
}
Camera::~Camera()
{}

//! Perspective
void Camera::updateShader(MasterShader shader)
{
	glUseProgram(shader.ID);

    glUniformMatrix4fv(
		shader.ProjectionUniformLocation,	//Uniform location
		1,									//Number of Uniforms
		false,								//Transpose 
		ProjectionMatrix.getPtr());			//Pointer to ModelViewMatrixValues

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


//! First Person
void Camera::togglePOV()
{
	if(firstPerson) //! if FP switch to TP
	{
		positionOffset = firstPersonOffset;
		bias = 1;
	}
	else //! if TP switch to FP
	{
		positionOffset = thirdPersonOffset;
		bias = TPbias;
	}
	firstPerson = !firstPerson;
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
	//Vector3f targetAhead			= targetAhead.cross (Vector3f(0,1,0), targetRight) * 1.0f;

	Vector3f relativePosOffset 		= playerAxis[0] * posOffset.x 		+ GlobalUp * posOffset.y 	 + playerAxis[2] * posOffset.z;
	Vector3f relativeTargetOffset 	= playerAxis[0] * targetOffset.x 	+ playerAxis[1] * targetOffset.y + playerAxis[2] * targetOffset.z; 



	Vector3f updated_target 		= playerPosition + relativeTargetOffset;
	//! Lags it behind a little, looks nicer
	target							= target * (1.0f-bias) +
									  updated_target * (bias);
	position						= playerPosition + relativePosOffset;

	direction 	= direction.normalise(playerPosition-position); 

}

//! Main Function that Updates the Camera Position
void Camera::followUpdate(Matrix4x4 &playerMatrix, Vector3f playerAxis[3], Vector3f playerPosition)
{
	follow(playerMatrix, playerAxis, playerPosition, positionOffset, targetOffset);

	ViewMatrix.lookAt(
		position,
		target,
		GlobalUp
	);
}