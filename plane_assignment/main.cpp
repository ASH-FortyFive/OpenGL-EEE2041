//!Includes
#include <GL/glew.h>
#include <GL/glut.h>
#include <Shader.h>
#include <iostream>
#include <Matrix.h>
#include <Mesh.h>
#include <Vector.h>
#include <math.h>
#include <string>
#include <Texture.h>

#include <Player.h>
#include <Camera.h>
#include <Model.h>
#include <Skybox.h>



//!Function Prototypes
void initShader();
void initGLUTFunctions();
void display(void);
void keyboard(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void handleKeys();
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void Timer(int value);
void initTexture(std::string filename, GLuint & textureID);

//! Temp Functions
void initTemp();	
void VectorPrinter(Vector3f vec);

//Lights

//! Screen size
int screenWidth   	        = 1920;
int screenHeight   	        = 1080;

//! Array of key states
bool keyStates[256];

//! Global Variables
	// Shader IDs
GLuint shaderProgramID;				// Shader Program ID
GLuint skyboxShaderID;
	// Shaders
GLuint vertexPositionAttribute;		// Vertex Position Attribute Location
GLuint vertexNormalAttribute;		// Vertex Normal Attribute Location
GLuint vertexColourAttribute;		// Vertex Colour Attribute Location
//GLuint ColourUniformLocation;		// Colour Uniform Location
	// Shaders - Textures
GLuint vertexTexcoordAttribute;		// Vertex Texture Coordiante Attribute Location
GLuint TextureMapUniformLocation;	// Texture Map Location
	// For Transformations (and Camera)
Matrix4x4 ModelViewMatrix;		  	// ModelView Matrix (Not Used)
GLuint MVMatrixUniformLocation;		// ModelView Matrix Uniform
	// For Camera
Matrix4x4 ProjectionMatrix;		  	// Projection Matrix
GLuint ProjectionUniformLocation;	// Projection Matrix Uniform Location
	// Unknown
GLuint TimeUniformLocation; 		// Unsure
	// For Lighting
GLuint LightPositionUniformLocation;                // Light Position Uniform   
GLuint AmbientUniformLocation;
GLuint SpecularUniformLocation;
GLuint SpecularPowerUniformLocation;

//! Light Settings
Vector3f lightPosition;				                // Light Position 
//Vector3f colour;					                // Colour Variable 
	//Material Properties
Vector3f ambient    = Vector3f(0.1,0.1,0.1);
Vector3f specular   = Vector3f(1.0,1.0,1.0);
float specularPower = 10.0;


//! Loaded Models
Model groundPlane;
Model ring;
Model plane;

GLuint texture;
GLuint texture1;
GLuint texture2;

GLuint new_vertexPositionAttribute;
GLuint new_MVMatrixUniformLocation;
GLuint new_ProjectionUniformLocation;

//! Bools for Toggles
bool PentaToggle = false;
bool WireFrame = false;

Vector3f CamLocation = Vector3f(4.0,4.0,4.0);
Vector3f CamLookAt = Vector3f(2.0,2.0,2.0);

// Time
float t_global = 0.0;

//! Main Program Entry
int main(int argc, char** argv)
{	
    glutInit(&argc, argv); //Init GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH); //Set Display Mode
    glutInitWindowSize(screenWidth, screenHeight); //Set Window Size
    glutInitWindowPosition(200, 200); // Window Position
    glutCreateWindow("Plane Assignment"); //Create Window

	// Init GLEW
	if (glewInit() != GLEW_OK) 
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	initGLUTFunctions();

    //Init Key States to false;    
    for(int i = 0 ; i < 256; i++)
        keyStates[i] = false;
    
    //Sets up shader program
    initShader();

	// Creates Basics Shapes for Testing
	initTemp();
	
	glClearColor(0.3,0.3,0.3,1.0);

    //Enter main loop
    glutMainLoop();

	//Clean-Up
    glDeleteProgram(shaderProgramID);

    return 0;
}

//! Sets up the windows and GLUT
void initGLUTFunctions()
{
 	glutDisplayFunc(display); //Set Display function
	
	glutKeyboardFunc(keyboard); //Set Keyboard Interaction Functions
	glutKeyboardUpFunc(keyUp); 
	
	glutMouseFunc(mouse); //Set Mouse Interaction Functions
	glutPassiveMotionFunc(motion);
	glutMotionFunc(motion);

    glutTimerFunc(100,Timer, 0); //Start start timer function after 100 milliseconds

	glEnable(GL_TEXTURE_2D); //Enables 2D textures in OpenGl
	glEnable(GL_DEPTH_TEST); //Enables Depth Test to ensure things render in the correct order
}

//! Loads and Sets Up the Shaders
void initShader()
{
	//! Loads shader from file
    shaderProgramID = Shader::LoadFromFile("shader.vert","shader.frag");
	skyboxShaderID = Shader::LoadFromFile("skybox_shader.vert","skybox_shader.frag");
    
	//! For Positions and Transforms
    // Gets handle for vertex position buffer
	vertexPositionAttribute 		= glGetAttribLocation(shaderProgramID, "aVertexPosition");
	// Gets handel for vertex normal buffer
	vertexNormalAttribute 			= glGetAttribLocation(shaderProgramID,    "aVertexNormal"); 
	//Gets Matrix Uniform Location, for camera and transform
	MVMatrixUniformLocation 		= glGetUniformLocation(shaderProgramID,"MVMatrix_uniform");
	//Gets Projection Matrix Uniform location, for projection
	ProjectionUniformLocation 		= glGetUniformLocation(shaderProgramID,"ProjMatrix_uniform");

 	//! For Colours (NOT WORKING)
    //Colour Uniform Location
	//ColourUniformLocation = glGetUniformLocation(shaderProgramID, "Colour_uniform");

	//! For Textures
	// Get a handle for our texture coordinate buffer	
	vertexTexcoordAttribute 		= glGetAttribLocation(shaderProgramID,"aVertexTexcoord");
	//Gets Texture Map Uniform location
	TextureMapUniformLocation 		= glGetUniformLocation(shaderProgramID,"TextureMap_uniform");

	//! For Lighting
	LightPositionUniformLocation	= glGetUniformLocation(shaderProgramID, "LightPosition_uniform"); 
	AmbientUniformLocation 			= glGetUniformLocation(shaderProgramID, "Ambient_uniform"); 
	SpecularUniformLocation 		= glGetUniformLocation(shaderProgramID, "Specular_uniform"); 
	SpecularPowerUniformLocation 	= glGetUniformLocation(shaderProgramID, "SpecularPower_uniform"); 

	std::cout 	<< LightPositionUniformLocation << " : "
				<< AmbientUniformLocation << ": "
				<< SpecularUniformLocation << ": "
				<< SpecularPowerUniformLocation << std::endl;

	//! Unknown
	//Get Time Uniform Location
	TimeUniformLocation = glGetUniformLocation(shaderProgramID,"t_uniform");


	//! Multiple Shader Testing
	new_vertexPositionAttribute 	= glGetAttribLocation(skyboxShaderID, "aVertexPosition");
	new_MVMatrixUniformLocation 	= glGetUniformLocation(skyboxShaderID,"MVMatrix_uniform");
	new_ProjectionUniformLocation 	= glGetUniformLocation(skyboxShaderID,"ProjMatrix_uniform");
}

void initTemp()
{
	//! Init Light
	//Set colour variable and light position
	//colour = Vector3f(1.0,0.0,0.0);
	lightPosition= Vector3f(10.0,10.0,10.0);


	ModelHelper::initTexture("../models/plane1.bmp", texture);
	ModelHelper::initTexture("../models/grass.bmp", texture1);
	ModelHelper::initTexture("../models/Crate.bmp", texture2);

	plane.loadOBJ("../models/plane1.obj", TextureMapUniformLocation, texture);

	groundPlane.loadOBJ("../models/ground.obj", TextureMapUniformLocation, texture1);

	ring.loadOBJ("../models/torus.obj", TextureMapUniformLocation, texture2);

	ring.rotate(90.0f, Vector3f(0.0f,0.0f,1.0f));
	ring.translate(Vector3f(0.0f,2.0f,0.0f));
	ring.setScale(1.5f);

	plane.translate(Vector3f(3.0f,2.0f,0.0f));

	groundPlane.translate(Vector3f(0.0f,0.0f,0.0f));
	groundPlane.setScale(10.0f);
}	

//! Display Loop
void display(void)
{
    //Handle keys
    handleKeys();

	//Set Viewport
	//glViewport(0,0,screenWidth, screenHeight);
	
	// Clear the screen
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//Use shader which were initialised in initShader()
	glUseProgram(shaderProgramID);

	//! Time
	t_global += 0.001;
	glUniform1f(TimeUniformLocation, t_global);

	//! Camera and Projection
	ProjectionMatrix.perspective(90,1.0,0.01,100.0);

	glUniformMatrix4fv(
		ProjectionUniformLocation,	//Uniform location
		1,							//Number of Uniforms
		false,						//Transpose 
		ProjectionMatrix.getPtr());	//Pointer to ModelViewMatrixValues

		//Camera	
	ModelViewMatrix.lookAt(
		CamLocation,
		CamLookAt,
		Vector3f(0, 1, 0 )
	);
	
	//Set modelview matrix uniform
	glUniformMatrix4fv(	
		MVMatrixUniformLocation,  	//Uniform location
		1,							//Number of Uniforms
		false,						//Transpose Matrix
		ModelViewMatrix.getPtr());	//Pointer to ModelViewMatrixValues


	//! Lighting
	glUniform3f(LightPositionUniformLocation, lightPosition.x,lightPosition.y,lightPosition.z);
    glUniform4f(AmbientUniformLocation, ambient.x, ambient.y, ambient.z, 1.0);
    glUniform4f(SpecularUniformLocation, specular.x, specular.y, specular.z, 1.0);
    glUniform1f(SpecularPowerUniformLocation, specularPower);

	plane.Draw(ModelViewMatrix,MVMatrixUniformLocation, vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);

	groundPlane.Draw(ModelViewMatrix,MVMatrixUniformLocation, vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);

	//plane.Draw(ModelViewMatrix, MVMatrixUniformLocation, vertexPositionAttribute);


	glUseProgram(skyboxShaderID);

	glUniformMatrix4fv(
		new_ProjectionUniformLocation,	//Uniform location
		1,							//Number of Uniforms
		false,						//Transpose 
		ProjectionMatrix.getPtr());	//Pointer to ModelViewMatrixValues

	//ring.Draw(ModelViewMatrix, MVMatrixUniformLocation, vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);	

	ring.Draw(ModelViewMatrix, new_MVMatrixUniformLocation, new_vertexPositionAttribute);

	//Unuse Shader
	glUseProgram(0);

    //Swap Buffers and post redisplay
	glutSwapBuffers();
	glutPostRedisplay();
}

//! Keyboard Interaction
void keyboard(unsigned char key, int x, int y)
{
	//Quits program when esc is pressed
	if (key == 27)	//esc key code
	{
		exit(0);
	}
	else if(key == 'x')
    {
		plane.scale(-0.1);
		std::cout << "shrink" << std::endl;
    }
	else if (key == 'X')
	{
		std::cout << "grow" << std::endl;
		plane.scale(0.1);
	}
	else if (key == 'p')
	{
		plane.printMatrix();
	}
	else if (key == 'c')
	{
		std::cout <<"Cam: Location" << std::endl;
		VectorPrinter(CamLocation);
		std::cout <<"Cam: LookAt" << std::endl;
		VectorPrinter(CamLookAt);
	}
	else if (key == 'g')
	{
		PentaToggle = !PentaToggle;
	}
	else if(key == 'z' || key == 'Z')
	{
		if(WireFrame){
			//If On Toggle Off
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		} else{
			//If Off Toggle On
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		WireFrame = !WireFrame;
	}
	/*
	else
	{
		std::cout << key << "" <<std::endl;
	}
	*/

    //Set key status
    keyStates[key] = true;
}

//! Handle key up situation
void keyUp(unsigned char key, int x, int y)
{
    keyStates[key] = false;
}

//! Handle Keys
void handleKeys()
{
    //Keys are handled here
	if(keyStates['w'])
    {
		plane.translate(Vector3f(0.0f,0.01f,0.0f));
    }
	if (keyStates['s'])
	{
		plane.translate(Vector3f(0.0f,-0.01f,0.0f));
	}
	if(keyStates['a'])
    {
		plane.translate(Vector3f(-0.01f,0.0f,0.0f));
    }
	if (keyStates['d'])
	{
		plane.translate(Vector3f(0.01f,0.0f,0.0f));
	}
	if(keyStates['q'])
    {
		plane.rotate(1.0f, Vector3f(0.0f,0.0f,1.0f));
    }
	if (keyStates['e'])
	{
		plane.rotate(-1.0f, Vector3f(0.0f,0.0f,1.0f));
	}
	if (keyStates['i'])
	{
		CamLocation.y += 0.1f;
		CamLookAt.y += 0.1f;
	}
	if (keyStates['k'])
	{
		CamLocation.y -= 0.1f;
		CamLookAt.y -= 0.1f;
	}
	if (keyStates['j'])
	{
		CamLocation.x -= 0.1f;
		CamLookAt.x -= 0.1f;
	}
	if (keyStates['l'])
	{
		CamLocation.x += 0.1f;
		CamLookAt.x += 0.1f;
	}
	if (keyStates['o'])
	{
		CamLocation.z -= 0.1f;
		CamLookAt.z -= 0.1f;
	}
	if (keyStates['u'])
	{
		CamLocation.z += 0.1f;
		CamLookAt.z += 0.1f;
	}
}

//! Mouse Interaction
void mouse(int button, int state, int x, int y)
{

}

//! Motion
void motion(int x, int y)
{
    
}

//! Timer Function
void Timer(int value)
{
	// Do something
    
    //Call function again after 10 milli seconds
	glutTimerFunc(10,Timer, 0);
}

//! Debug Print Function
void VectorPrinter(Vector3f vec)
{
	std::cout << "X:[" <<vec.x << "]\n";
	std::cout << "Y:[" <<vec.y << "]\n";
	std::cout << "Z:[" <<vec.z << "]\n";
}