#include "Model.h"


Model::Model() : scaleFactor(1.0f), position(0.0f,0.0f,0.0f),  rotationAxis(0.0f,0.0f,1.0f), rotationAngle(0.0f), colour(0.5f, 0.5f, 0.5f)
{
    ModelMatrix.toIdentity();
}

Model::~Model()
{

}

bool Model::loadOBJ(std::string filename, GLuint MatrixUniformLocation)
{
    if(!Mesh::loadOBJ(filename))
    {
        return -1;
    }
    MVMatrixUniformLocation = MatrixUniformLocation;
}

void Model::Draw(GLuint vertexPositionAttribute, GLuint colourUniformLocation, GLuint vertexNormalAttribute, GLuint vertexTexcordAttribute)
{
    //! Applies Basic Colours
    if(colourUniformLocation != -1)
    {
        glUniform4f(colourUniformLocation, colour.x, colour.y,  colour.z , 1.0f);
    }


    //! Applies All Transforms, may be possible to reduce the number of operations
    ModelMatrix.toIdentity();
    ModelMatrix.translate(position.x, position.y, position.z);
    ModelMatrix.rotate(rotationAngle, rotationAxis.x, rotationAxis.y, rotationAxis.z);
    ModelMatrix.scale(scaleFactor, scaleFactor, scaleFactor);

    glUniformMatrix4fv(
        MVMatrixUniformLocation,
        1,
        false,
        ModelMatrix.getPtr());


    Mesh::Draw(vertexPositionAttribute, vertexNormalAttribute, vertexTexcordAttribute);
    
    

    //Resets for Next Model
    ModelMatrix.toIdentity();

    //! Applies Basic Colours
    if(colourUniformLocation != -1)
    {
        glUniform4f(colourUniformLocation, 0.1f, 0.1f, 0.1f, 1.0f);
    }


    glUniformMatrix4fv(
        MVMatrixUniformLocation,
        1,
        false,
        ModelMatrix.getPtr());
    
}

void Model::scale(float increment)
{
    scaleFactor += increment; 
}

void Model::translate(Vector3f increment)
{
    position = position + increment;
}

void Model::rotate(float increment, Vector3f newAxis)
{
    rotationAxis = newAxis;
    rotationAngle += increment;
}

void Model::setColour(Vector3f newColour)
{
   
     if(newColour.x >= 0.0f || newColour.x <= 1.0f ||   // Red
        newColour.y >= 0.0f || newColour.y <= 1.0f ||   // Green
        newColour.z >= 0.0f || newColour.z <= 1.0f)      // Blue
    {
        colour = newColour;
    }

    
}

void Model::printMatrix()
{
    ModelMatrix.toIdentity();
    ModelMatrix.translate(position.x, position.y, position.z);
    ModelMatrix.rotate(rotationAngle, rotationAxis.x, rotationAxis.y, rotationAxis.z);
    ModelMatrix.scale(scaleFactor, scaleFactor, scaleFactor);
    ModelMatrix.print("MV");
    ModelMatrix.toIdentity();
    std::cout << "scaleFactor: "<< scaleFactor << std::endl;
}

