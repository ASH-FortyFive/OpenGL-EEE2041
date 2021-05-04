#include "ShaderClass.h"
//! Constructors and Destructors 

MasterShader::MasterShader()
{
    
}

MasterShader::~MasterShader()
{

}

//! Loads the Shader and sets up Locations
void MasterShader::loadShader(std::string vert, std::string frag)
{
    ID = Shader::LoadFromFile(vert,frag);
    //! Transforms
    // Gets handle for vertex position buffer
	vertexPositionAttribute 		= glGetAttribLocation(ID, "aVertexPosition");
	// Gets handel for vertex normal buffer
	vertexNormalAttribute 			= glGetAttribLocation(ID,    "aVertexNormal"); 
	//Gets Matrix Uniform Location, for camera and transform
	ModelMatrixUniformLocation 		= glGetUniformLocation(ID,"ModelMatrix_uniform");
	ViewMatrixUniformLocation 		= glGetUniformLocation(ID,"ViewMatrix_uniform");
	//Gets Projection Matrix Uniform location, for projection
	ProjectionUniformLocation 		= glGetUniformLocation(ID,"ProjMatrix_uniform");

	//! Textures
	// Get a handle for our texture coordinate buffer	
	vertexTexcoordAttribute 		= glGetAttribLocation(ID,"aVertexTexcoord");
	//Gets Texture Map Uniform location
	TextureMapUniformLocation 		= glGetUniformLocation(ID,"TextureMap_uniform");

	//! For Lighting
	LightPositionUniformLocation	= glGetUniformLocation(ID, "LightPosition_uniform"); 
	AmbientUniformLocation 			= glGetUniformLocation(ID, "Ambient_uniform"); 
	SpecularUniformLocation 		= glGetUniformLocation(ID, "Specular_uniform"); 
	SpecularPowerUniformLocation 	= glGetUniformLocation(ID, "SpecularPower_uniform"); 

	//! For Time
	TimeUniformLocation = glGetUniformLocation(ID,"t_uniform");

	//! For Colour
	ColourUniformLocation = glGetUniformLocation(ID,"Colour_uniform");

    std::cout <<    vertexPositionAttribute << " : " <<  vertexNormalAttribute << " : " << ModelMatrixUniformLocation << " : " << ViewMatrixUniformLocation << " : " << ProjectionUniformLocation << " : " << 
                    vertexTexcoordAttribute << " : " << TextureMapUniformLocation << " : " << 
                    LightPositionUniformLocation << " : " << AmbientUniformLocation << " : " << SpecularUniformLocation << " : " << SpecularPowerUniformLocation << std::endl << "Colour: " << ColourUniformLocation << std::endl;
}
