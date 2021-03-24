#include "Vector.h"
#include <Model.h>
#define toRads 0.0174532925199f

//! Constructors and Destructors 
Model::Model() : 
    scaleFactor(1.0f), 
    position(0.0f,0.0f,0.0f),  
    rotationAngles(0.0f, 0.0f, 0.0f), 
    colour(0.5f, 0.5f, 0.5f)
{
    ModelMatrix.toIdentity();
}

Model::Model(Vector3f initalPos, float initalSize, Vector3f initalRot, Vector3f initalAngles, Vector3f initalColour) : 
    scaleFactor(initalSize), 
    position(initalPos),  
    rotationAxis(initalRot), 
    rotationAngles(initalAngles), 
    colour(initalColour)
{
    ModelMatrix.toIdentity();
}

Model::~Model()
{}

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

    //! Applies All Transforms, may be possible to reduce the number of operations
    ModelMatrix = ModelMatrix * ModelViewMatrix;                // Starts by getting
    ModelMatrix.translate(position.x, position.y, position.z);  // Translates
    ModelMatrix.rotate(rotationAngles.x, 1.0f, 0.0f, 0.0f);     // Rotate around x
    ModelMatrix.rotate(rotationAngles.y, 0.0f, 1.0f, 0.0f);     // Rotate around y
    ModelMatrix.rotate(rotationAngles.z, 0.0f, 0.0f, 1.0f);     // Rotate around z
    ModelMatrix.scale(scaleFactor, scaleFactor, scaleFactor);   // Scales

    glUniformMatrix4fv(
        MVMatrixUniformLocation,
        1,
        false,
        ModelMatrix.getPtr());


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
    // Changes Rotation Values 
void Model::rotate(Vector3f increment)
{
    rotationAngles.x = (rotationAngles.x + increment.x);
    rotationAngles.y = (rotationAngles.y + increment.y);
    rotationAngles.z = (rotationAngles.z + increment.z);
}
void Model::setRotation(Vector3f newAngle)
{
    rotationAngles = newAngle;
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

//
Vector3f Model::facing()
{
    Vector3f rotationRadians(rotationAngles.z * toRads, rotationAngles.y * toRads, rotationAngles.z * toRads);

    float x =  cos(rotationRadians.z) * cos(rotationRadians.y);
    float y =  sin(rotationRadians.z); 
    float z = -cos(rotationRadians.z) * sin(rotationRadians.y);

    return Vector3f(x,y,z);
}


//! Gets Centre
Vector3f Model::getMeshCentroid()
{
    Vector3f centroid(Mesh::getMeshCentroid());
    return centroid + position;
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