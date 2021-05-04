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

bool Hitbox::doCollsions(Hitbox& hg)
{
	Vector3f otherCentre = hg.trueCentre();
	for(int i(0); i < 8; i++)
	{
		Vector3f line;
		Vector3f point = corner[i] * modelMatrix; 
		Vector3f points = hg.corner[i] * hg.modelMatrix; 
		line = point - points;
		glBegin(GL_LINES);
  		glVertex3f(points.x, points.y, points.z);
  		glVertex3f(point.x, point.y, point.z);
		glEnd();

		//std::cout << i << " = " << line.length() << std::endl;
	}
	//std::cout <<" ========================= " << std::endl;

	return false;
}