#include "Vector.h"
#include <Model.h>
#define toRads 0.0174532925199f

void Model::test()
{
   
}

//! Constructors and Destructors 
Model::Model() : 
    scaleFactor(1.0f), 
    position(0.0f,0.0f,0.0f),  
    rotationAngles(0.0f, 0.0f, 0.0f)
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

    //! Returns true for correctly loaded texture
    return true;
}

bool Model::loadHitbox(std::string filename)
{
    hitboxPath = filename;
    return reloadHitbox();
}

bool Model::reloadHitbox()
{
    hitboxes.clear();

    std::fstream newFile;
    newFile.open(hitboxPath, std::ios::in);
    if(!newFile.good())
	{
		std::cerr<<"Failed to open file at "<<hitboxPath<<std::endl;
		return false;
	}

    int numOfBoxes, typeID;
    newFile >> numOfBoxes;
    Hitbox::OBB newOBB;
    Hitbox::hbType type;

    for(int i(0); i < numOfBoxes; i++)
    {
        if(newFile.eof())
        {
            newFile.close();
            return false;
        }
        
        newFile >> newOBB.centrePoint.x >> newOBB.centrePoint.y >> newOBB.centrePoint.z;
        newFile >> newOBB.extents[0]    >> newOBB.extents[1]    >> newOBB.extents[2];
        newFile >> typeID;

        switch(typeID)
        {
            case 0:
            {
                type = Hitbox::Obstacle;
                break;
            }
            case 1:
            {
                type = Hitbox::Target;
                break;
            }
            case 2:
            {
                type = Hitbox::Main;
                break;
            } 
        }

        //new Hitbox(newOBB);
        hitboxes.push_back(new Hitbox(newOBB, type));
    }

}

void Model::changeTexture(GLuint texture)
{
    textureID = texture;
}

//! Main Function of the Class, draws the model (using parent function) and adds transforms and textures
void Model::Draw(MasterShader shader)
{
    glUseProgram(shader.ID);
    //! Applies All Transforms
    ModelMatrix = getMatrix();                                 
    
    for(auto hitbox : hitboxes)
    {
        hitbox->ModelMatrix = ModelMatrix;
    }

    //! Sends Model Matrix to Shader
    glUniformMatrix4fv(
        shader.ModelMatrixUniformLocation,
        1,
        false,
        ModelMatrix.getPtr());

    //! Loads the Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glUniform1i(TextureMapUniformLocation, 0);

    //! Uses Parents Draw Function after applying Transforms and Texture
    Mesh::Draw(shader.vertexPositionAttribute, shader.vertexNormalAttribute, shader.vertexTexcoordAttribute);
    
    //! Resets for Next Model (MIGHT EVENTUALLY BE OBSOLETE) 
    ModelMatrix.toIdentity();
    glUniformMatrix4fv(
        shader.ModelMatrixUniformLocation,
        1,
        false,
        ModelMatrix.getPtr());
}

//! Main Function of the Class, draws the model (using parent function) and adds transforms and textures
void Model::DrawHitboxes(MasterShader shader)
{   
    for(auto hitbox : hitboxes)
    {
        hitbox->Draw(shader);
    }
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
    setPosition(position + increment);
}
    // Overload for floats
void Model::translate(float x, float y, float z)
{
    translate(Vector3f(x,y,z));
}
void Model::setPosition(float x, float y, float z)
{
    setPosition(Vector3f(x,y,z));
}
void Model::setPosition(Vector3f pos)
{
    position = pos;
}
    // Changes Rotation Values 
void Model::rotate(Vector3f increment)
{
    setRotation(Vector3f(
        rotationAngles.x + increment.x,
        rotationAngles.y + increment.y,
        rotationAngles.z + increment.z
    ));
}
void Model::setRotation(Vector3f newAngle)
{
    
    rotationAngles.x =fmod(newAngle.x + 360.0f, 360.0f);
    rotationAngles.y =fmod(newAngle.y + 360.0f, 360.0f);
    rotationAngles.z =fmod(newAngle.z + 360.0f, 360.0f);
}

//! Accessors
Vector3f Model::facing()
{
    Vector3f rotationRadians(rotationAngles.z * toRads, rotationAngles.y * toRads, rotationAngles.z * toRads);

    float x =  cos(rotationRadians.z) * cos(rotationRadians.y);
    float y =  sin(rotationRadians.z); 
    float z = -cos(rotationRadians.z) * sin(rotationRadians.y);

    return rotationRadians.normalise(Vector3f(x,y,z));
}

Matrix4x4 Model::getMatrix()
{
    Matrix4x4 matrix;
    matrix.translate(position.x, position.y, position.z);  // Translates
    matrix.rotate(rotationAngles.y, 0.0f, 1.0f, 0.0f);     // Rotate around y
    matrix.rotate(rotationAngles.z, 0.0f, 0.0f, 1.0f);     // Rotate around z
    matrix.rotate(rotationAngles.x, 1.0f, 0.0f, 0.0f);     // Rotate around x               
    matrix.scale(scaleFactor, scaleFactor, scaleFactor);   // Scales
    return matrix;
}

Vector3f Model::getRotiation()
{
    return rotationAngles;
}



//! Gets Centre
Vector3f Model::getMeshCentroid()
{
    //Vector3f centroid(Mesh::getMeshCentroid());
    //return centroid + position;
    return position;
}

//! Helper Function
//!  Loads Texture based on string into given textureID 
void ModelHelper::initTexture(std::string filename, GLuint & textureID, GLuint TEXTURE_TYPE)
{
	//Generate texture and bind
	glGenTextures(1, &textureID);
	glBindTexture(TEXTURE_TYPE, textureID);
    
	//Set texture parameters
    glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);	
	glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 

	//Get texture Data
	int width, height;
	char* data;
	Texture::LoadBMP(filename, width, height, data);
	glTexImage2D(TEXTURE_TYPE, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glBindTexture(TEXTURE_TYPE, 0);

    //Cleanup data - copied to GPU
    delete[] data;
}