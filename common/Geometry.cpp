#include "Geometry.h"


Geometry::Geometry() : scaleFactor(1.0f), position(0.0f,0.0f,0.0f),  rotationAxis(0.0f,0.0f,1.0f), rotationAngle(0.0f)
{
    ModelMatrix.toIdentity();
}

Geometry::~Geometry()
{

}

void Geometry::draw()
{
    //! Applied All Transforms, may be possible to reduce the number of operations
    ModelMatrix.toIdentity();
    ModelMatrix.translate(position.x, position.y, position.z);
    ModelMatrix.rotate(rotationAngle, rotationAxis.x, rotationAxis.y, rotationAxis.z);
    ModelMatrix.scale(scaleFactor, scaleFactor, scaleFactor);

    glUniformMatrix4fv(
        MVMatrixUniformLocation,
        1,
        false,
        ModelMatrix.getPtr());

    //! Draws the final Geometry
    
    //Position
    glEnableVertexAttribArray(vertexPositionAttribute);
    glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);
    glVertexAttribPointer(
        vertexPositionAttribute,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
    );

    //Colour 
    glEnableVertexAttribArray(vertexColourAttribute);
    glBindBuffer(GL_ARRAY_BUFFER, vertexColourBuffer);
    glVertexAttribPointer(
        vertexColourAttribute,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	// Draw the triangle
	glDrawElements(
		GL_TRIANGLES,       // mode
		numberOfIndex,    				// count
		GL_UNSIGNED_INT,    // type
		(void*)0            // element array buffer offset
	);    
    
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(vertexPositionAttribute);
    glDisableVertexAttribArray(vertexColourAttribute);

    //Resets for Next Model
    /*
    ModelMatrix.toIdentity();

    glUniformMatrix4fv(
        MVMatrixUniformLocation,
        1,
        false,
        ModelMatrix.getPtr());
*/
}

void Geometry::initAttribute(GLuint posLoc, GLuint colLoc, GLuint matLoc)
{
    vertexPositionAttribute = posLoc;
    vertexColourAttribute = colLoc;
    MVMatrixUniformLocation = matLoc;
}

//! Adds all the vertex position and colour data to the buffer
void Geometry::initVerts(GLfloat vertsPosition[], GLfloat vertsColour[], GLuint vertsIndex[], int sizeVerts, int sizeIndex)
{
    //! For draw function
    numberOfIndex = sizeIndex;

    //! Transfers data over, must be a better way to do this
    GLfloat verticesPosition[sizeVerts];
    GLfloat verticesColour[sizeVerts];
    GLuint verticesIndex[sizeIndex];

    for(int i(0); i < sizeVerts; i++)
    {
        verticesPosition[i] = vertsPosition[i];
        verticesColour[i] = vertsColour[i];
    }

    for(int i(0); i < sizeIndex; i++)
    {
        verticesIndex[i] = vertsIndex[i];
    }

    //! Creates Vertex Position Buffer
    glGenBuffers(1, &vertexPositionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);
    glBufferData(  
        GL_ARRAY_BUFFER, 
        sizeof(verticesPosition), 
        verticesPosition, 
        GL_STATIC_DRAW);

    //! Creates Vertex Colour Buffer
    glGenBuffers(1, &vertexColourBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexColourBuffer);
    glBufferData(
        GL_ARRAY_BUFFER, 
        sizeof(verticesColour), 
        verticesColour, 
        GL_STATIC_DRAW);

    //! Creates Vertex Index Buffer
    glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, 
        sizeof(verticesIndex) * sizeof(unsigned int), 
        verticesIndex, 
        GL_STATIC_DRAW);
}

void Geometry::scale(float increment)
{
    scaleFactor += increment; 
}

void Geometry::translate(Vector3f increment)
{
    position = position + increment;
}

void Geometry::rotate(float increment, Vector3f newAxis)
{
    rotationAxis = newAxis;
    rotationAngle += increment;
}


void Geometry::printMatrix()
{
    ModelMatrix.toIdentity();
    ModelMatrix.translate(position.x, position.y, position.z);
    ModelMatrix.rotate(rotationAngle, rotationAxis.x, rotationAxis.y, rotationAxis.z);
    ModelMatrix.scale(scaleFactor, scaleFactor, scaleFactor);
    ModelMatrix.print("MV");
    ModelMatrix.toIdentity();
    std::cout << "scaleFactor: "<< scaleFactor << std::endl;
}

