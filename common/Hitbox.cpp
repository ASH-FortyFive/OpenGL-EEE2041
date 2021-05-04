#include <Hitbox.h>
#define toRads 0.0174532925199f

//! Constructors and Destructors 
Hitbox::Hitbox()
{
}

Hitbox::Hitbox(Vector3f newCentre, float h, float w, float d): centre(newCentre), height(h), width(w), debth(d)
{
	loadHitbox();
}


Hitbox::~Hitbox()
{
}


void Hitbox::changeHitbox(Vector3f newCentre, float h, float w, float d)
{
	centre = newCentre;
	height = h; width = w; debth = d;

	loadHitbox();
}

void Hitbox::loadHitbox()
{
	//! Calculates the 'first point', in the (F)ront (B)ottom and (L)eft of the rectangle.
	Vector3f FBL = Vector3f(
		centre.x - width/2,
		centre.y - height/2,
		centre.z - debth/2
	);

	vertexPositionData[0] = FBL.x;
	vertexPositionData[3] = FBL.x;
	vertexPositionData[6] = FBL.x;
	vertexPositionData[9] = FBL.x;
	vertexPositionData[12] = FBL.x + width;
	vertexPositionData[15] = FBL.x + width;
	vertexPositionData[18] = FBL.x + width;
	vertexPositionData[21] = FBL.x + width;

	vertexPositionData[1] 	= FBL.y;
	vertexPositionData[4] 	= FBL.y;
	vertexPositionData[7] 	= FBL.y + height;
	vertexPositionData[10] 	= FBL.y + height;
	vertexPositionData[13] 	= FBL.y;
	vertexPositionData[16] 	= FBL.y;
	vertexPositionData[19] 	= FBL.y + height;
	vertexPositionData[22] 	= FBL.y + height;

	vertexPositionData[2] 	= FBL.z;
	vertexPositionData[5] 	= FBL.z + debth;
	vertexPositionData[8] 	= FBL.z;
	vertexPositionData[11] 	= FBL.z + debth;
	vertexPositionData[14] 	= FBL.z;
	vertexPositionData[17] 	= FBL.z + debth;
	vertexPositionData[20] 	= FBL.z;
	vertexPositionData[23] 	= FBL.z + debth;
}


void Hitbox::Test()
{
    
}

void Hitbox::Draw(MasterShader shader)
{
    //Create Vertex Buffer and upload data
	glGenBuffers(1, &vertexPositionBuffer);  
	glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositionData), vertexPositionData, GL_STATIC_DRAW);

    //Create Vertex Index Buffer
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertexIndexData) * sizeof(unsigned int), vertexIndexData, GL_STATIC_DRAW);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glUseProgram(shader.ID);

	Matrix4x4 mod;

	glUniformMatrix4fv(
        shader.ModelMatrixUniformLocation,
        1,
        false,
        mod.getPtr());

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