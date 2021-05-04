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
	Vector3f v; //Ussed because the Vector class is terrible


	Vector3f aU[3], bU[3];
	for (int i(0); i < 3; i++){
		aU[i] = obb.localAxis[i] * modelMatrix;
		bU[i] = hb.obb.localAxis[i] * hb.modelMatrix;
	}

	float aE[3], bE[3];
	aE[0] = obb.extents.x; 		aE[1] = obb.extents.y; 		aE[2] = obb.extents.z;
	bE[0] = hb.obb.extents.x; 	bE[1] = hb.obb.extents.y; 	bE[2] = hb.obb.extents.z;

	float rA, rB;
	float R[3][3], absR[3][3]; //Rotation matrix to bring hb into this objects coordinate space

	for (int i(0); i < 3; i++){
		for (int j(0); j < 3; j++){
			R[i][j] = v.dot(aU[i], bU[j]);
			absR[i][j] = abs(R[i][j]) + 0.0001f;
		}
	}
		
	Vector3f T = (hb.obb.centrePoint * hb.modelMatrix) - (obb.centrePoint * modelMatrix);
	float t[3];
	t[0] = v.dot(T,aU[0]);
	t[0] = v.dot(T,aU[1]);
	t[0] = v.dot(T,aU[2]);

	//To test Collsions we check 15 axis

	//This objects axis - 3
	for(int i(0); i < 3; i++)
	{
		rA = aE[i];
		rB = bE[0] * absR[i][0] + bE[1] * absR[i][1] + bE[2] * absR[i][2];
		if(abs(t[i]) > rA + rB) return false;
	}

	for(int i(0); i < 3; i++)
	{
		rA = aE[0] * absR[0][i] + aE[1] * absR[1][i] + aE[2] * absR[2][i];
		rB = bE[i];
		if(abs(t[0] * R[0][i] + t[1] * R[1][i] + t[2] * R[2][i]) > rA + rB) return false;
	}

	// Test axis L=A0xB0
	rA = aE[1] * absR[2][0] + aE[2] * absR[1][0];
	rB = bE[1] * absR[0][2] + bE[2] * absR[0][1];
	if (abs(t[2] * R[1][0] - t[1] * R[2][0]) > rA + rB) return false;

	// Test axis L=A0xB1
	rA = aE[1] * absR[2][1] + aE[2] * absR[1][1];
	rB = bE[0] * absR[0][2] + bE[2] * absR[0][0];
	if (abs(t[2] * R[1][1] - t[1] * R[2][1]) > rA + rB) return false;

	// Test axis L=A0xB2
	rA = aE[1] * absR[2][2] + aE[2] * absR[2][1];
	rB = bE[0] * absR[0][1] + bE[1] * absR[0][0];
	if (abs(t[2] * R[1][2] - t[1] * R[2][2]) > rA + rB) return false;

	// Test axis L=A1xB0
	rA = aE[0] * absR[2][0] + aE[2] * absR[0][0];
	rB = bE[1] * absR[1][2] + bE[2] * absR[1][1];
	if (abs(t[0] * R[2][0] - t[2] * R[0][0]) > rA + rB) return false;

	// Test axis L=A1xB1
	rA = aE[0] * absR[2][1] + aE[2] * absR[0][1];
	rB = bE[0] * absR[1][2] + bE[2] * absR[1][0];
	if (abs(t[0] * R[2][1] - t[2] * R[0][1]) > rA + rB) return false;

	// Test axis L=A1xB2
	rA = aE[0] * absR[2][2] + aE[2] * absR[0][2];
	rB = bE[0] * absR[1][1] + bE[1] * absR[1][0];
	if (abs(t[0] * R[2][2] - t[2] * R[0][2]) > rA + rB) return false;

	// Test axis L=A2xB0
	rA = aE[0] * absR[1][0] + aE[1] * absR[0][0];
	rB = bE[1] * absR[2][2] + bE[2] * absR[2][1];
	if (abs(t[1] * R[0][0] - t[0] * R[1][0]) > rA + rB) return false;

	// Test axis L=A2xB1
	rA = aE[0] * absR[1][1] + aE[1] * absR[0][1];
	rB = bE[0] * absR[2][2] + bE[2] * absR[2][0];
	if (abs(t[1] * R[0][1] - t[0] * R[1][1]) > rA + rB) return false;

	// Test axis L=A2xB2
	rA = aE[0] * absR[1][2] + aE[1] * absR[0][2];
	rB = bE[0] * absR[2][1] + bE[1] * absR[2][0];
	if (abs(t[1] * R[0][2] - t[0] * R[1][2]) > rA + rB) return false;

	return true;
}