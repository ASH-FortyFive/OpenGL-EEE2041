//!Includes
#include <GL/glew.h>
#include <GL/glut.h>
#include <Shader.h>
#include <iostream>
#include <Matrix.h>
#include <Mesh.h>

#include <Geometry.h>
#include <Camera.h>
#include <Model.h>

/*
#include <Vector.h>
#include <Texture.h>
#include <SphericalCameraManipulator.h>
#include <math.h>
#include <string>
*/

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

void initTemp();	


//! Screen size
int screenWidth   	        = 1920;
int screenHeight   	        = 1920;

//! Array of key states
bool keyStates[256];

//! Global Variables
	//Shaders
GLuint shaderProgramID;				// Shader Program ID
GLuint vertexPositionAttribute;		// Vertex Position Attribute Location
GLuint vertexColourAttribute;		// Vertex Colour Attribute Location
GLuint ColourUniformLocation;
GLuint TimeUniformLocation; 
	// Shaders - Textures
GLuint vertexTexcoordAttribute;
GLuint TextureMapUniformLocation;

//! For Transformations
Matrix4x4 ModelViewMatrix;		  	// ModelView Matrix (Not Used)
GLuint MVMatrixUniformLocation;		// ModelView Matrix Uniform

//! For Camera
Matrix4x4 ProjectionMatrix;		  	// Projection Matrix
GLuint ProjectionUniformLocation;	// Projection Matrix Uniform Location



Mesh Monkey;
Model newTriangle;

//! Bools for Toggles
bool PentaToggle = false;
bool WireFrame = false;



// Time
float t_global = 0.0;

//! Main Program Entry
int main(int argc, char** argv)
{	
    glutInit(&argc, argv); //Init GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH); //Set Display Mode
    glutInitWindowSize(screenWidth, screenHeight); //Set Window Size
    glutInitWindowPosition(0, 0); // Window Position
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
	 
	// Mesh 
	Monkey.loadOBJ("../models/torus.obj");
	newTriangle.loadOBJ("../models/triangle.obj", MVMatrixUniformLocation);


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
	glEnable(GL_TEXTURE_2D);
}

//! Loads and Sets Up the Shaders
void initShader()
{
	//Create/Load shader
    shaderProgramID = Shader::LoadFromFile("shader.vert","shader.frag");
    
    // Get a handle for our vertex position buffer
	vertexPositionAttribute = glGetAttribLocation(shaderProgramID, "aVertexPosition");

	//CGet a handle for our vertex colour buffer
	vertexColourAttribute = glGetAttribLocation(shaderProgramID, "aVertexColour");

	//Get Time Uniform Location
	TimeUniformLocation=glGetUniformLocation(shaderProgramID,"t_uniform");

	//Gets Matrix Uniform Location
	MVMatrixUniformLocation=glGetUniformLocation(shaderProgramID,"MVMatrix_uniform");

	//Gets Projection Matrix Uniform location
	ProjectionUniformLocation=glGetUniformLocation(shaderProgramID,"ProjMatrix_uniform");

    //Colour Uniform Location
	ColourUniformLocation = glGetUniformLocation(shaderProgramID, "Colour_uniform");

	//For Textures	
	//vertexTexcoordAttribute = glGetAttribLocation(shaderProgramID,"aVertexTexcoord");
	
	//TextureMapUniformLocation = glGetUniformLocation(shaderProgramID,"TextureMap_uniform");
}

void initTemp()
{

}	


//! Display Loop
void display(void)
{
    //Handle keys
    handleKeys();

	//Set Viewport
	glViewport(0,0,screenWidth, screenHeight);
	
	// Clear the screen
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//Use shader which were initialised in initShader()
	glUseProgram(shaderProgramID);

	//TIme
	t_global += 0.001;
	glUniform1f(TimeUniformLocation, t_global);

	ProjectionMatrix.perspective(90,1.0,0.01,10.0);
	glUniformMatrix4fv(
		ProjectionUniformLocation,	//Uniform location
		1,							//Number of Uniforms
		false,						//Transpose 
		ProjectionMatrix.getPtr());	//Pointer to ModelViewMatrixValues




	if(PentaToggle)
	{
		Monkey.Draw(vertexPositionAttribute);
	}

	newTriangle.Draw(vertexPositionAttribute, ColourUniformLocation);

	

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
		newTriangle.scale(-0.1);
		std::cout << "shrink" << std::endl;
    }
	else if (key == 'X')
	{
		std::cout << "grow" << std::endl;
		newTriangle.scale(0.1);
	}
	else if (key == 'p')
	{
		newTriangle.printMatrix();
	}
	else if (key == 'h')
	{
		//Penta.printMatrix();
	}
	else if (key == 'g')
	{
		PentaToggle = !PentaToggle;
	}
	else if(key == 'b')
	{
		glClearColor(1.0,1.0,1.0,1.0);
	}
	else if(key == 'B')
	{
		glClearColor(0.0,0.0,0.0,1.0);
	}
	else if(key == 'c')
	{
		newTriangle.setColour(Vector3f(1.0f,0.0f,0.0f));
	}
	else if(key == 'C')
	{
		newTriangle.setColour(Vector3f(0.0f,0.0f,1.0f));
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
		newTriangle.translate(Vector3f(0.0f,0.01f,0.0f));
    }
	if (keyStates['s'])
	{
		newTriangle.translate(Vector3f(0.0f,-0.01f,0.0f));
	}
	if(keyStates['a'])
    {
		newTriangle.translate(Vector3f(-0.01f,0.0f,0.0f));
    }
	if (keyStates['d'])
	{
		newTriangle.translate(Vector3f(0.01f,0.0f,0.0f));
	}
	if(keyStates['q'])
    {
		newTriangle.rotate(1.0f, Vector3f(0.0f,0.0f,1.0f));
    }
	if (keyStates['e'])
	{
		newTriangle.rotate(-1.0f, Vector3f(0.0f,0.0f,1.0f));
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



