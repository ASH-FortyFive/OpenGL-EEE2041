#include "Model.h"

Model::Model() : 
    scaleFactor(1.0f), 
    position(0.0f,0.0f,0.0f),  
    rotationAxis(0.0f,0.0f,1.0f), 
    rotationAngle(0.0f), 
    colour(0.5f, 0.5f, 0.5f)
{
    ModelMatrix.toIdentity();
}

Model::Model(Vector3f initalPos, float initalSize, Vector3f initalRot, float initalAngle, Vector3f initalColour) : 
    scaleFactor(initalSize), 
    position(initalPos),  
    rotationAxis(initalRot), 
    rotationAngle(initalAngle), 
    colour(initalColour)
{
    ModelMatrix.toIdentity();
}

Model::~Model()
{

}


//! Load the .OBJ File, mostly using parents function, but adds the matrix uniform used for the translations and the camera
bool Model::loadOBJ(std::string filename, GLuint _TextureMapUniformLocation, GLuint texture)
{
    //! Uses Parent loadFunction
    Mesh::loadOBJ(filename);

    //! Adds the Uniforms
    TextureMapUniformLocation = _TextureMapUniformLocation; //TextureMapUniform for Textures
    //! Adds the ID for the Texture
    textureID = texture;

    std::cout << "TextureID: " << textureID << std::endl;

    //! Returns true for correctly loaded texture
    return true;
}

//! Main Function of the Class, draws the model (using parent function) and adds transforms and textures
void Model::Draw(Matrix4x4 ModelViewMatrix, GLuint MVMatrixUniformLocation, GLuint vertexPositionAttribute, GLuint vertexNormalAttribute, GLuint vertexTexcoordAttribute)
{
    //! Applies Basic Colours
    /*
    if(colourUniformLocation != -1)
    {
        glUniform4f(colourUniformLocation, colour.x, colour.y,  colour.z , 1.0f);
    }*/


    //! Applies All Transforms, may be possible to reduce the number of operations
    ModelMatrix = ModelMatrix * ModelViewMatrix; 
    ModelMatrix.translate(position.x, position.y, position.z);
    ModelMatrix.rotate(rotationAngle, rotationAxis.x, rotationAxis.y, rotationAxis.z);
    ModelMatrix.scale(scaleFactor, scaleFactor, scaleFactor);

    glUniformMatrix4fv(
        MVMatrixUniformLocation,
        1,
        false,
        ModelMatrix.getPtr());

    //! Sends the Correct Texture to Shader

    //! Applies Basic Colours
    /*
    if(colourUniformLocation != -1)
    {
        glUniform4f(colourUniformLocation, 0.1f, 0.1f, 0.1f, 1.0f);
    }*/

    //! Loads the Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glUniform1i(TextureMapUniformLocation, 0);

    //! Uses Parents Draw Function after applying Transforms and Texture
    Mesh::Draw(vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);
    
    //! Resets for Next Model (MIGHT EVENTUALLY BE OBSOLETE) 
    ModelMatrix.toIdentity();
    glUniformMatrix4fv(
        MVMatrixUniformLocation,
        1,
        false,
        ModelMatrix.getPtr());
    
}


//! All the Mutator Functions, both incrementing and setting (Currently Lacks Error Checking & Rotation may be changed to Quaternions)
    // Changes Scale of Model in Wold
void Model::scale(float increment)
{
    scaleFactor += increment; 
}
void Model::setScale(float newScale)
{
    scaleFactor = newScale; 
}
    // Changes Position of Model in World
void Model::translate(Vector3f increment)
{
    position = position + increment;
}
void Model::setPosition(float x, float y, float z)
{
    position = Vector3f(x,y,z);
}
    // Changes Rotation Values (MAY REQUIRE A REDO)
void Model::rotate(float increment, Vector3f newAxis)
{
    rotationAxis = newAxis;
    rotationAngle += increment;
}
void Model::setRotation(float newAngle, Vector3f newAxis)
{
    rotationAxis = newAxis;
    rotationAngle = newAngle;
}
    // Changes Colour (Currently Broken)
void Model::setColour(float R, float G, float B)
{
   
     if(R >= 0.0f || R <= 1.0f ||   // Red
        G >= 0.0f || G <= 1.0f ||   // Green
        B >= 0.0f || B <= 1.0f)      // Blue
    {
        colour = Vector3f(R,G,B);
    }else
    {
        std::cerr << "Invalid Colour Set" << std::endl;
    }
}

//! DEBUGS FUNCTIONS (DISABLE IN FINAL BUILD)
//! Debug Function for Checking Rotation
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

//! Helper Function
//!  Loads Texture based on string into given textureID 
void ModelHelper::initTexture(std::string filename, GLuint & textureID)
{
	//Generate texture and bind
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
    
	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 

	//Get texture Data
	int width, height;
	char* data;
	Texture::LoadBMP(filename, width, height, data);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);

    //Cleanup data - copied to GPU
    delete[] data;
}


