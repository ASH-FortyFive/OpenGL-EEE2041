#include <Hitbox.h>
#define toRads 0.0174532925199f

//! Constructors and Destructors 
Hitbox::Hitbox()
{
	

}

Hitbox::~Hitbox()
{

}

void Hitbox::loadHitboxes(Vector3f centre, float h, float w, float t)
{

    /*
    GLfloat vertexPositionData[24] = 
	{ 
		 x,  y,  z, //0 Bottom,front, left
		 x,  y,  z+, //1 Bottom,front, right
		 x+, y,  z, //2 Bottom,back,  left
         x+, y,  z+, //3 Bottom,back,  right
         x,  y+, z, //4 Top,   front, left
         x,  y+, z+, //5 Top,   front, right
         x+, y+, z, //6 Top,   back,  left
         x+, y+, z+  //7 Top,   back,  right
	};
    

    	{ 
		 x,  y,  z,  000 = 0
		 x,  y,  z+, 001 = 1
		 x,  y+, z,  010 = 2
         x,  y+, z+, 011 = 3
         x+, y,  z,  100 = 4
         x+, y,  z+, 101 = 5
         x+, y+, z,  110 = 6
         x+, y+, z+  111 = 7
                     
	};
    
    for(int i(0); i < 24; i++)
    {   
        vertexPositionData[24]
    }

    vertexPositionData[0] = 1;
    vertexPositionData[1] = 2;
    vertexPositionData[0] = 1;

    vertexPositionData[1] = 2;
    vertexPositionData[0] = 1;
    vertexPositionData[1] = 2;
    
    vertexPositionData[0] = 1;
    vertexPositionData[1] = 2;
    vertexPositionData[0] = 1;
    
    vertexPositionData[1] = 2;
    vertexPositionData[0] = 1;
    vertexPositionData[1] = 2;
    
    vertexPositionData[0] = 1;
    vertexPositionData[1] = 2;
    vertexPositionData[0] = 1;
    
    vertexPositionData[1] = 2;
    vertexPositionData[0] = 1;
    vertexPositionData[1] = 2;

    vertexPositionData[0] = 1;
    vertexPositionData[1] = 2;
    vertexPositionData[0] = 1;
    
    vertexPositionData[1] = 2;
    vertexPositionData[0] = 1;
    vertexPositionData[1] = 2;

    //Vertex Data
	*/


	//Create Vertex Buffer and upload data
	glGenBuffers(1, &vertexPositionBuffer);  
	glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositionData), vertexPositionData, GL_STATIC_DRAW);

    //Create Vertex Index Buffer
	

	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertexIndexData) * sizeof(unsigned int), vertexIndexData, GL_STATIC_DRAW);
}

void Hitbox::Test()
{
    
    vertexPositionData[1] += 1;
    vertexPositionData[4] += 1;
    vertexPositionData[7] += 1;

    vertexPositionData[10] += 1;
    vertexPositionData[13] += 1;
    vertexPositionData[16] += 1;
    
    vertexPositionData[19] += 1;
    vertexPositionData[22] += 1;

    
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