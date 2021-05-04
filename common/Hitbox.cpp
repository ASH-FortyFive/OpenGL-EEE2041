#include <Hitbox.h>
#define toRads 0.0174532925199f

//! Constructors and Destructors 
Hitbox::Hitbox()
{
}

Hitbox::Hitbox(Vector3f newCentre, float w, float h, float d): centre(newCentre), height(h), width(w), debth(d)
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

	Vector3f forward(width,0,0);
	Vector3f up(0,height,0);
	Vector3f right(0,0,debth);
	

	corners.push_back(FBL);
	corners.push_back(FBL + right);
	corners.push_back(FBL + up);
	corners.push_back(FBL + up + right);

	corners.push_back(FBL + forward);
	corners.push_back(FBL + forward + right);
	corners.push_back(FBL + forward + up);
	corners.push_back(FBL + forward + up + right);
}


void Hitbox::Test()
{
	int j(0);
	for (std::vector<Vector3f>::const_iterator i = corners.begin(); i != corners.end(); ++i)
	{
		std::cout << *i;
	}

	std::cout << std::endl;

	std::cout << "============================================="<< std::endl;
}

void Hitbox::Draw(MasterShader shader, Matrix4x4 ModelMatrix)
{

	int j(0);
	for (std::vector<Vector3f>::iterator i = corners.begin(); i != corners.end(); ++i)
	{
		Vector3f point;

		point = *i * ModelMatrix;
		
		transformedVertexPositionData[j*3] 		= point.x;
		transformedVertexPositionData[j*3+1]	= point.y;
		transformedVertexPositionData[j*3+2]	= point.z;

		j++;
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

bool Hitbox::doCollsions(Hitbox& hg)
{
	return false;
}