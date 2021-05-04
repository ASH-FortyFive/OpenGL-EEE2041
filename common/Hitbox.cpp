#include <Hitbox.h>
#define toRads 0.0174532925199f

//! Constructors and Destructors 
Hitbox::Hitbox()
{
}

Hitbox::Hitbox(Vector3f newCentre, float d, float h, float w)
{
	obb.centrePoint = newCentre;
	obb.extents.x = d/2;
	obb.extents.y = h/2;
	obb.extents.z = w/2;

	loadHitbox();
}



Hitbox::~Hitbox()
{
}


void Hitbox::changeHitbox(Vector3f newCentre, float d, float h, float w)
{

	loadHitbox();
}

void Hitbox::loadHitbox()
{
	//! Calculates the 'first point', in the (F)ront (B)ottom and (L)eft of the rectangle.
	Vector3f FBL = Vector3f(
		obb.centrePoint.x - obb.extents.x,
		obb.centrePoint.y - obb.extents.y,
		obb.centrePoint.z - obb.extents.z
	);

	Vector3f forward 	= obb.localAxis[0] * -2*obb.extents.x;
	Vector3f up 		= obb.localAxis[1] * -2*obb.extents.y;
	Vector3f right 		= obb.localAxis[2] * -2*obb.extents.z;
	
	corner[0] = (FBL);
	corner[1] = (FBL + right);	
	corner[2] = (FBL + up);
	corner[3] = (FBL + up + right);

	corner[4] = (FBL + forward);
	corner[5] = (FBL + forward + right);
	corner[6] = (FBL + forward + up);
	corner[7] = (FBL + forward + up + right);
}

void Hitbox::Test()
{

}

Vector3f Hitbox::trueCentre()
{
	Vector3f trueCentre;
	for (int i(0); i < 8; i++)
	{
		Vector3f point = corner[i] * modelMatrix;
		trueCentre.x = trueCentre.x + point.x;
		trueCentre.y = trueCentre.y + point.y;
		trueCentre.z = trueCentre.z + point.z;
	}

	trueCentre = trueCentre / 8;

	return trueCentre;
}

void Hitbox::Draw(MasterShader shader, Matrix4x4 ModelMatrix)
{
	modelMatrix = ModelMatrix; 

	for (int i(0); i < 8; i++)
	{
		Vector3f point = corner[i] * ModelMatrix;
		
		transformedVertexPositionData[i*3] 		= point.x;
		transformedVertexPositionData[i*3+1]	= point.y;
		transformedVertexPositionData[i*3+2]	= point.z;
	}
	

    //Create Vertex Buffer and upload data
	glGenBuffers(1, &vertexPositionBuffer);  
	glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(transformedVertexPositionData), transformedVertexPositionData, GL_STATIC_DRAW);

    //Create Vertex Index Buffer
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertexIndexData) * sizeof(unsigned int), vertexIndexData, GL_STATIC_DRAW);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glUseProgram(shader.ID);

	glUniform3f(shader.ColourUniformLocation, 0,0,0);

	ModelMatrix.toIdentity();

	glUniformMatrix4fv(
        shader.ModelMatrixUniformLocation,
        1,
        false,
        ModelMatrix.getPtr());

	// Enable and bind vertex position buffer to vertex position attributes 
    // step 1: enable the OpenGL vertex attribute array ‘VertexPositionAttribute’
	glEnableVertexAttribArray(shader.vertexPositionAttribute);

	glVertexAttribPointer(
		shader.vertexPositionAttribute,// The attribute we want to configure
		3,                      // size
		GL_FLOAT,               // type
		GL_FALSE,               // normalized?: GL_FALSE=don’t normalize
		0,                      // stride
		(void*)0                // array buffer offset
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

	// Draw the triangle
	glDrawElements(
		GL_TRIANGLES,      // mode
		36,    				// count
		GL_UNSIGNED_INT,   // type
		(void*)0           // element array buffer offset
	);
	
	//step 5: disable the OpenGL vertex attribute array ‘VertexPositionAttribute’
	glDisableVertexAttribArray(shader.vertexPositionAttribute);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//=============================================================//
}

bool Hitbox::doCollsions(Hitbox& hb)
{
	Vector3f axis[15];

	Matrix4x4 rotationMatrix = modelMatrix;
	float * rotationMatrixValues = rotationMatrix.getPtr();
	rotationMatrixValues[14] = 0; rotationMatrixValues[13] = 0; rotationMatrixValues[12] = 0;

	Matrix4x4 rotationMatrixB = hb.modelMatrix;
	float * rotationMatrixValuesB = hb.modelMatrix.getPtr();
	rotationMatrixValuesB[14] = 0; rotationMatrixValuesB[13] = 0; rotationMatrixValuesB[12] = 0;

	axis[0] = obb.localAxis[0] * rotationMatrix;
	axis[1] = obb.localAxis[1] * rotationMatrix;
	axis[2] = obb.localAxis[2] * rotationMatrix;


	axis[3] = hb.obb.localAxis[0] * rotationMatrixValuesB;
	axis[4] = hb.obb.localAxis[1] * rotationMatrixValuesB;
	axis[5] = hb.obb.localAxis[2] * rotationMatrixValuesB;


	for(int i(0); i < 6; i++)
	{
		std::cout << "Axis " << i << ": " << axis[i] << "("<< axis[i].length() << ")"<<std::endl;
		drawLine(obb.centrePoint * modelMatrix, obb.centrePoint * modelMatrix + axis[i]);
	}

	std::cout << "===========================================" <<std::endl;

	return true;
	/*
	for(int i(0); i < 15; i++)
	{
		std::cout << "Axis " << i << ": " << axis[i] <<std::endl;
		drawLine(obb.centrePoint * modelMatrix, axis[i]);
	}	*/
	std::cout << "===========================================" <<std::endl;

}

bool Hitbox::againstAxis(Vector3f axis, Hitbox& a, Hitbox& b)
{
	
}

bool Hitbox::drawLine(Vector3f v1, Vector3f v2)
{
	glBegin(GL_LINES);
  	glVertex3f(v1.x, v1.y, v1.z);
  	glVertex3f(v2.x, v2.y, v2.z);
	glEnd();
}