#include "Vector.h"


//!
Vector3f Vector3f::operator-(Vector3f rhs)
{
	Vector3f result;
	result.x = this->x - rhs.x;
	result.y = this->y - rhs.y;
	result.z = this->z - rhs.z;
	return result;
}

//!
Vector3f Vector3f::operator+(Vector3f rhs)
{
	Vector3f result;
	result.x = this->x + rhs.x;
	result.y = this->y + rhs.y;
	result.z = this->z + rhs.z;
	return result;
}

//!
Vector3f  Vector3f::operator/(float rhs)
{
	Vector3f result;
	result.x = this->x / rhs;
	result.y = this->y / rhs;
	result.z = this->z / rhs;
	return result;	
}

//!
Vector3f  Vector3f::operator*(float rhs)
{
	Vector3f result;
	result.x = this->x * rhs;
	result.y = this->y * rhs;
	result.z = this->z * rhs;
	return result;	
}

//!
float Vector3f::length()
{
	return sqrt(x*x + y*y + z*z);
}

//!
Vector3f Vector3f::normalise(Vector3f v)
{
	return v / v.length();	
}

//! Cross product of two 3D vectors
Vector3f Vector3f::cross(Vector3f v1, Vector3f v2)
{
	Vector3f result;
	result.x = v1.y * v2.z - v2.y* v1.z;
	result.y = v1.z * v2.x - v2.z* v1.x;
	result.z = v1.x * v2.y - v2.x* v1.y;
	return result;	
}

//! Dot Product of two 3D vectors
float Vector3f::dot(Vector3f v1, Vector3f v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

//====================================================== 
//! A function for transforming 3D vectors based on a 4x4 Matrix
//  Maybe flawed both conceputally and in execution 
Vector3f  Vector3f::operator*(Matrix4x4 rhs)
{
	Vector3f result;
	float * values = rhs.getPtr();
	result.x = this->x * values[0]  + this->y * values[4]  + this->z * values[8]  + values[12];
	result.y = this->x * values[1]  + this->y * values[5]  + this->z * values[9]  + values[13];
	result.z = this->x * values[2]  + this->y * values[6]  + this->z * values[10] + values[14];
	return result;	
}

std::ostream& operator<<(std::ostream& out, const Vector3f& vec)
{
	out << vec.x << " : " << vec.y << " : " << vec.z;
	return out;
}

//======================================================

//! Vector4f Functions
Vector4f Vector4f::operator-(Vector4f rhs)
{
	Vector4f result;
	result.x = this->x - rhs.x;
	result.y = this->y - rhs.y;
	result.z = this->z - rhs.z;
	result.w = this->w - rhs.w;
	return result;
}

//!
Vector4f Vector4f::operator+(Vector4f rhs)
{
	Vector4f result;
	result.x = this->x + rhs.x;
	result.y = this->y + rhs.y;
	result.z = this->z + rhs.z;
	result.w = this->w + rhs.w;
	return result;
}

//!
Vector4f  Vector4f::operator/(float rhs)
{
	Vector4f result;
	result.x = this->x / rhs;
	result.y = this->y / rhs;
	result.z = this->z / rhs;
	result.w = this->w / rhs;
	return result;	
}

//!
Vector4f  Vector4f::operator*(float rhs)
{
	Vector4f result;
	result.x = this->x * rhs;
	result.y = this->y * rhs;
	result.z = this->z * rhs;
	result.w = this->w * rhs;
	return result;	
}

//!
float Vector4f::length()
{
	return sqrt(x*x + y*y + z*z + w*w);
}

//!
Vector4f Vector4f::normalise(Vector4f v)
{
	return v / v.length();	
}

//! Special (and possibly bad and also not required as we will just use 3D vectors)
//!
Vector4f Vector4f::operator*(Matrix4x4 rhs)
{
	float * values = rhs.getPtr();
	Vector4f result;
	result.x = this->x * values[0]  + this->y * values[1]  + this->z * values[2]  + this->w * values[3];
	result.y = this->x * values[4]  + this->y * values[5]  + this->z * values[6]  + this->w * values[7];
	result.z = this->x * values[8]  + this->y * values[9]  + this->z * values[10] + this->w * values[11];
	result.w = this->x * values[12] + this->y * values[13] + this->z * values[14] + this->w * values[15];
	return result;	
}