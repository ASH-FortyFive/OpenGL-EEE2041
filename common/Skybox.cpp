#include <Skybox.h>

//! Constructors and Destructors
Skybox::Skybox()
{}
Skybox::~Skybox()
{}


//! Sets up Buffer Shader
void Skybox::Init(GLuint TextureMapUniformLocation, std::string * texture_faces)
{
    //! Binds Textures for all 6 Faces
    for(int i(0); i < 6; i++)
    {
        ModelHelper::initTexture(texture_faces[i], cubeTextureID[i]);
        cube[i].loadOBJ("../models/ground.obj", TextureMapUniformLocation, cubeTextureID[i]);
    } 


    /*
    std::string skyboxPaths[6] = {
    "../models/skybox/right.bmp",
    "../models/skybox/left.bmp",
    "../models/skybox/top.bmp",
    "../models/skybox/bottom.bmp",
    "../models/skybox/front.bmp",
    "../models/skybox/back.bmp"};
    */

    //! Rotates Cubes Correctly
    cube[0].setRotation (Vector3f(90.0f,0.0f,90.0f));
    cube[1].setRotation (Vector3f(90.0f,180.0f,90.0f));
    cube[3].setRotation (Vector3f(0.0f,0.0f,180.0f));
    cube[4].setRotation (Vector3f(0.0f,0.0f,90.0f));
    cube[5].setRotation (Vector3f(0.0f,180.0f,90.0f));

    makeBox();
}

void Skybox::makeBox()
{
    cube[0].translate(0.0f,0.0f,1.0f);
    cube[1].translate(0.0f,0.0f,-1.0f);
    cube[2].translate(0.0f,1.0f,0.0f);
    cube[3].translate(0.0f,-1.0f,0.0f);
    cube[4].translate(-1.0f,0.0f,0.0f);
    cube[5].translate(1.0f,0.0f,0.0f);
}

//! Main Function of the Class, draws the skybox and adds textures
void Skybox::Draw(Vector3f camPos,Matrix4x4 ModelViewMatrix, GLuint MVMatrixUniformLocation, GLuint vertexPositionAttribute, GLuint vertexNormalAttribute, GLuint vertexTexcoordAttribute)
{
    Matrix4x4 RotModelViewMatrix(ModelViewMatrix);

    float* mvmPtr = RotModelViewMatrix.getPtr();

	//mvmPtr[3] = 0;
	//mvmPtr[7] = 0;
	//mvmPtr[11] = 0;


    
    cube[0].setPosition(camPos);
    cube[1].setPosition(camPos); 
    cube[2].setPosition(camPos);
    cube[3].setPosition(camPos);
    cube[4].setPosition(camPos);
    cube[5].setPosition(camPos);
    
    makeBox();

    for(int i(0); i < 6; i++)
    {
        cube[i].Draw(RotModelViewMatrix,MVMatrixUniformLocation, vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);
    }
}
