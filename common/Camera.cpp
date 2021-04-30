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
		GlobalUp
	);
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
	//target = target + increment;
}

//! Follows Given Object
void Camera::follow(Player player, Vector3f posOffset, Vector3f targetOffset)
{
	Vector3f facing = player.facing();
	Vector3f rot = player.getRotiation();

	if(rot.z > 90.0f && rot.z < 270.0f)
	{
		std::cout << "Flipped" << std::endl;
		GlobalUp = Vector3f(0,-1,0);
	}
	else
	{
		GlobalUp = Vector3f(0,1,0);
	}


	Vector3f targetRight 			= targetRight.normalise(targetRight.cross(GlobalUp, facing));
	Vector3f targetUp 				= targetUp.cross(facing,targetRight);
	Vector3f relativePosOffset 		= facing * posOffset.x + targetUp * posOffset.y + targetRight * posOffset.z;
	Vector3f relativeTargetOffset 	= facing * targetOffset.x + targetUp * targetOffset.y + targetRight * targetOffset.z;



	std::cout << std::fixed << "X:" << rot.x << " Y:" <<  rot.y << " Z:" <<  rot.z <<"\n";


	target 		= player.getMeshCentroid() + relativeTargetOffset;
	position 	= player.getMeshCentroid() + relativePosOffset;

	direction 	= direction.normalise(position-player.getMeshCentroid());
}

//! Main Function that Updates the Camera Position
void Camera::update(Player player, Matrix4x4& _ModelViewMatrix, GLuint _ProjectionUniformLocation)
{

	follow(player, positionOffset, targetOffset);

	changeModelView(_ModelViewMatrix);

	setProjection(_ProjectionUniformLocation);	
}