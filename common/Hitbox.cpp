#include <Hitbox.h>
#define toRads 0.0174532925199f

//! Constructors and Destructors 
Hitbox::Hitbox()
{
}

Hitbox::Hitbox(Vector3f newCentre, float d, float h, float w)
{
	obb.centrePoint = newCentre;
	obb.extents[0] = d/2;
	obb.extents[1]= h/2;
	obb.extents[2] = w/2;

	loadHitbox();
}


Hitbox::~Hitbox()
{
	
}

void Hitbox::loadHitbox()
{
	//! Calculates the 'first point', in the (F)ront (B)ottom and (L)eft of the rectangle.
	Vector3f FBL = Vector3f(
		obb.centrePoint.x - obb.extents[0],
		obb.centrePoint.y - obb.extents[1],
		obb.centrePoint.z - obb.extents[2]
	);

	Vector3f forward 	= obb.localAxis[0] * -2*obb.extents[0];
	Vector3f up 		= obb.localAxis[1] * -2*obb.extents[1];
	Vector3f right 		= obb.localAxis[2] * -2*obb.extents[2];
	
	corner[0] = (FBL);
	corner[1] = (FBL + right);	
	corner[2] = (FBL + up);
	corner[3] = (FBL + up + right);

	corner[4] = (FBL + forward);
	corner[5] = (FBL + forward + right);
	corner[6] = (FBL + forward + up);
	corner[7] = (FBL + forward + up + right);
}

void Hitbox::Draw(MasterShader shader)
{
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
	Vector3f v; // Used for .dot, .cross, and .normalise

	//! Removes the translation element from the modelMatrixes
	Matrix4x4 rotationMatrix = ModelMatrix;
	float * rotationMatrixValues = rotationMatrix.getPtr();
	rotationMatrixValues[14] = 0; rotationMatrixValues[13] = 0; rotationMatrixValues[12] = 0;

	Matrix4x4 rotationMatrixB = hb.ModelMatrix;
	float * rotationMatrixValuesB = rotationMatrixB.getPtr();
	rotationMatrixValuesB[14] = 0; rotationMatrixValuesB[13] = 0; rotationMatrixValuesB[12] = 0;


	//! Creates the first 6th Axis to check (directions of both hitboxes)
	Vector3f axis[6];

	axis[0] = v.normalise(obb.localAxis[0] * rotationMatrix);
	axis[1] = v.normalise(obb.localAxis[1] * rotationMatrix);
	axis[2] = v.normalise(obb.localAxis[2] * rotationMatrix);


	axis[3] = v.normalise(hb.obb.localAxis[0] * rotationMatrixB);
	axis[4] = v.normalise(hb.obb.localAxis[1] * rotationMatrixB);
	axis[5] = v.normalise(hb.obb.localAxis[2] * rotationMatrixB);

	//! Creates the project extensions
	Vector3f projectedExtends[2][3];

	for(int i(0); i < 3; i++)
	{
		projectedExtends[0][i] = axis[i] * obb.extents[i];
		projectedExtends[1][i] = axis[i + 3] * hb.obb.extents[i];
	}

	//! Distance between the box centres
	Vector3f T = hb.obb.centrePoint * hb.ModelMatrix - obb.centrePoint * ModelMatrix;
	
	//! Checks if first 6 axis pass the test
	for(int i(0); i < 6; i++)
	{
		if(againstAxis(axis[i], T , projectedExtends))
		{
			return false;
		}
	}

	//! Creates the next 9 axis and test after creation (doing so optmizes code slightly)
	for(int i(0); i < 3; i++)
	{
		for(int j(0); j < 3; j++)
		{	
			if(againstAxis(v.normalise(v.cross(axis[i], axis[3+j])), T , projectedExtends))
			{
				return false;
			}
 		}
	}

	return true;

}

bool Hitbox::againstAxis(Vector3f axis, Vector3f distance, Vector3f pE[2][3])
{
	return 
		abs(axis.dot(distance, axis)) > 
		abs(axis.dot(pE[0][0], axis)) + 
		abs(axis.dot(pE[0][1], axis)) +
		abs(axis.dot(pE[0][2], axis)) +
		abs(axis.dot(pE[1][0], axis)) + 
		abs(axis.dot(pE[1][1], axis)) +
		abs(axis.dot(pE[1][2], axis))
	;
}

void Hitbox::drawLine(Vector3f v1, Vector3f v2)
{
	glBegin(GL_LINES);
  	glVertex3f(v1.x, v1.y, v1.z);
  	glVertex3f(v2.x, v2.y, v2.z);
	glEnd();
}
