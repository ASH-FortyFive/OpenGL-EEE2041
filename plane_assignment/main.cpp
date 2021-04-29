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
#include <iomanip>

#include <Player.h>
#include <Camera.h>
#include <Model.h>
#include <Skybox.h>
#include <HUD.h>
#include <Map.h>


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
float specularPower = 100.0f;

//! Skybox 
std::string skyboxPaths[6] = {
"../models/skybox/right.bmp",
"../models/skybox/left.bmp",
"../models/skybox/top.bmp",
"../models/skybox/bottom.bmp",
"../models/skybox/front.bmp",
"../models/skybox/back.bmp"};

Skybox defaultSkybox(skyboxPaths);
GLuint skyboxShaderID;


//! Loaded Models
Model ground;

Model ringX;
Model ringY;
Model ringZ;

Player plane;

GLuint texture;
GLuint texture1;
GLuint texture2;

GLuint skybox_vertexPositionAttribute;
GLuint skybox_MVMatrixUniformLocation;
GLuint skybox_ProjectionUniformLocation;

Camera ThirdPerson;

HUD ThirdPersonHUD;

//! Bools for Toggles
bool PentaToggle = false;
bool WireFrame = false;


// Time
float t_new = 0.0;
float t_old = 0.0;
float t_delta = 0.0;

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

	//Get Time Uniform Location
	TimeUniformLocation = glGetUniformLocation(shaderProgramID,"t_uniform");

	//! Multiple Shader Testing	
	skybox_vertexPositionAttribute 	= glGetAttribLocation(skyboxShaderID, "aVertexPosition");
	skybox_MVMatrixUniformLocation 	= glGetUniformLocation(skyboxShaderID,"MVMatrix_uniform");
	skybox_ProjectionUniformLocation 	= glGetUniformLocation(skyboxShaderID,"ProjMatrix_uniform");

	defaultSkybox.Init();
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

	plane.loadOBJ("../models/plane2.obj", TextureMapUniformLocation, texture);

	ground.loadOBJ("../models/ground.obj", TextureMapUniformLocation, texture1);

	ringX.loadOBJ("../models/torus.obj", TextureMapUniformLocation, texture);
	ringY.loadOBJ("../models/torus.obj", TextureMapUniformLocation, texture1);
	ringZ.loadOBJ("../models/torus.obj", TextureMapUniformLocation, texture2);

	ringX.rotate(Vector3f(0.0f,0.0f,90.0f));
	ringY.rotate(Vector3f(0.0f,0.0f,90.0f));
	ringZ.rotate(Vector3f(0.0f,0.0f,90.0f));

	ringX.translate(Vector3f(0.0f,2.0f,0.0f));
	ringY.translate(Vector3f(1.0f,2.0f,0.0f));
	ringZ.translate(Vector3f(2.0f,2.0f,0.0f));

	plane.translate(Vector3f(0.0f,0.75f,0.0f));

	ground.translate(Vector3f(0.0f,0.0f,0.0f));
	ground.setScale(250.0f);
}	

//! Display Loop
void display(void)
{
	//! Temp Movements
	/*
	ringX.translate(Vector3f(	sin(t_global* 50) * 0.01f, 0.0f , 0.0f));
	ringY.translate(Vector3f(	0.0f, sin(t_global* 50) * 0.01f, 0.0f));
	ringZ.translate(Vector3f(	0.0f, 0.0f, sin(t_global* 50) * 0.01f));
	*/

    //Handle keys
    handleKeys();
	
	// Clear the screen
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//! Renders the Skybox
	glUseProgram(skyboxShaderID);
	defaultSkybox.Draw(ModelViewMatrix, skybox_MVMatrixUniformLocation, skybox_vertexPositionAttribute);

	//Use shader which were initialised in initShader()
	glUseProgram(shaderProgramID);
	
	//! Time
	t_old	 = t_new;	
	t_new = glutGet(GLUT_ELAPSED_TIME);
	t_delta = (t_new - t_old) / 1000;

	glUniform1f(TimeUniformLocation, t_new);

	//! Updates all Physics Items
	plane.update(t_delta);


	//! Calculates Third Person Camera Follow
	ThirdPerson.follow(plane.getMeshCentroid(),plane.facing(), Vector3f(-4.0f,0.5f,0.0f), Vector3f(0.0f,0.0f,0.0f));

	//! Camera and Projection
	ThirdPerson.setProjection(ProjectionUniformLocation);
	ThirdPerson.changeModelView(ModelViewMatrix);

	//! Lighting
	glUniform3f(LightPositionUniformLocation, lightPosition.x,lightPosition.y,lightPosition.z);
    glUniform4f(AmbientUniformLocation, ambient.x, ambient.y, ambient.z, 1.0);
    glUniform4f(SpecularUniformLocation, specular.x, specular.y, specular.z, 1.0);
    glUniform1f(SpecularPowerUniformLocation, specularPower);

	Vector3f diff;
	diff = plane.getMeshCentroid() - ThirdPerson.getPosition();
	VectorPrinter(plane.getMeshCentroid());

	plane.Draw(ModelViewMatrix,MVMatrixUniformLocation, vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);

	ground.Draw(ModelViewMatrix,MVMatrixUniformLocation, vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);

	ringX.setScale(10.0f);
	ringX.Draw(ModelViewMatrix, MVMatrixUniformLocation, vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);	

//	glUseProgram(skyboxShaderID);
	//ringY.Draw(ModelViewMatrix, new_MVMatrixUniformLocation, new_vertexPositionAttribute);

	/*
	
	ringY.Draw(ModelViewMatrix, MVMatrixUniformLocation, vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);	
	ringZ.Draw(ModelViewMatrix, MVMatrixUniformLocation, vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);	
	*/

	//Unuse Shader
	glUseProgram(0);

	//! HUD Elements
	ThirdPersonHUD.render2dText("Top?",1.0f,1.0f,1.0f,0.5f,-0.5f);
	ThirdPersonHUD.render2dText("Bottom?",1.0f,0.7f,1.0f,0.0f,0.0f);

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
		std::cout << "Plane is Facing" << std::endl;
		VectorPrinter(plane.facing());
		
		std::cout << "Plane is at" << std::endl;
		VectorPrinter(plane.getMeshCentroid());

		std::cout << "Cam is Facing" << std::endl;
		VectorPrinter(ThirdPerson.getDirection());
		std::cout << "Cam is at" << std::endl;
		VectorPrinter(ThirdPerson.getPosition());
		std::cout << "==========================" << std::endl;
	}
	else if (key == 'c')
	{
		std::cout <<"Cam: Location" << std::endl;
		//VectorPrinter(CamLocation);
		std::cout <<"Cam: LookAt" << std::endl;
		//VectorPrinter(CamLookAt);
	}
	else if (key == 'g')
	{
		PentaToggle = !PentaToggle;
		//ThirdPerson.follow(plane.getMeshCentroid(), Vector3f(0.0f,0.0f,0.0f));
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
		//Moving Forward
		plane.rotate(Vector3f(0.0f,0.0f,1.0f));
    }
	if (keyStates['s'])
	{
		plane.rotate(Vector3f(0.0f,0.0f,-1.0f));
	}
	if(keyStates['a'])
    {
		plane.rotate(Vector3f(0.0f, 1.0f,0.0f));
    }
	if (keyStates['d'])
	{
		plane.rotate(Vector3f(0.0f,-1.0f,0.0f));
	}
	if(keyStates['q'])
    {
		plane.rotate(Vector3f(1.0f,0.0f,0.0f));
    }
	if (keyStates['e'])
	{
		plane.rotate(Vector3f(-1.0f,0.0f,0.0f));
	}
	if (keyStates['i'])
	{
		ThirdPerson.move(Vector3f(0.25f,0.0f,0.0f));
	}
	if (keyStates['k'])
	{
		ThirdPerson.move(Vector3f(-0.25f,0.0f,0.0f));
	}
	if (keyStates['j'])
	{
		ThirdPerson.move(Vector3f(0.0f,0.0f,0.25f));
	}
	if (keyStates['l'])
	{
		ThirdPerson.move(Vector3f(0.0f,0.0f,-0.25f));
	}
	if (keyStates['o'])
	{
		ThirdPerson.move(Vector3f(0.0f,-0.25f,0.0f));
	}
	if (keyStates['u'])
	{
		ThirdPerson.move(Vector3f(0.0f,0.25f,0.0f));
	}
	if (keyStates[' '])
	{
		plane.addForce(plane.facing());
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
	std::cout << std::fixed << "X:" << vec.x << " Y:" << vec.y << " Z:" << vec.z <<"\n";
	//std::cout << std::fixed << "Mag:" << sqrt(vec.x*vec.x + vec.y*vec.y +vec.z*vec.z) << "\n";
}