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
#include <ShaderClass.h>
#include <Hitbox.h>


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

//Lights

//! Screen size
int screenWidth   	        = 1920;
int screenHeight   	        = 1080;

//! Array of key states
bool keyStates[256];

//! Global Variables
Matrix4x4 ViewMatrix;	

//! Light Settings
Vector3f lightPosition;				                // Light Position 
//Material Properties
Vector3f ambient    = Vector3f(0.1,0.1,0.1);
Vector3f specular   = Vector3f(1.0,1.0,1.0);
float specularPower = 100.0f;

MasterShader defaultShader,skyboxShader, hitboxShader;

//! Skybox 
std::string skyboxPaths[6] = {
"../models/skybox/right.bmp",
"../models/skybox/left.bmp",
"../models/skybox/top.bmp",
"../models/skybox/bottom.bmp",
"../models/skybox/front.bmp",
"../models/skybox/back.bmp"};

Skybox skybox;

//! Loaded Models
Model ground;

Model ringX;
Model ringY;
Model ringZ;

Player plane;

GLuint texture;
GLuint texture1;
GLuint texture2;

Camera ThirdPerson(Vector3f(-4.0f,0.65f,0.0f));

HUD ThirdPersonHUD;

//! Bools for Toggles
bool PentaToggle = false;
bool WireFrame = false;

// Time
float t_new = 0.0;
float t_old = 0.0;
float t_delta = 0.0;

float t_sinceSecond = 0.0;
int frames;
std::string fps_count;


GLuint vertexPositionBuffer;

Hitbox hitbox(Vector3f(0,0.0,0)	,2,0.5f,0.5f);

Hitbox boxhit(Vector3f(0,0.0,0)	,2,0.1f,2);

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
    glDeleteProgram(defaultShader.ID);
	glDeleteProgram(skyboxShader.ID);

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
	defaultShader.loadShader("../shaders/shader.vert","../shaders/shader.frag");
	skyboxShader.loadShader("../shaders/skybox_shader.vert","../shaders/skybox_shader.frag");
	hitboxShader.loadShader("../shaders/hitbox_shader.vert","../shaders/hitbox_shader.frag");
}

void initTemp()
{

	skybox.Init(skyboxShader.TextureMapUniformLocation, skyboxPaths);

	//! Init Light
	//Set colour variable and light position
	//colour = Vector3f(1.0,0.0,0.0);
	lightPosition= Vector3f(1000.0,1000.0,-1414.0f);

	ModelHelper::initTexture("../models/plane1.bmp", texture);
	ModelHelper::initTexture("../models/grass.bmp", texture1);
	ModelHelper::initTexture("../models/Crate.bmp", texture2);

	plane.loadOBJ("../models/plane2.obj", defaultShader.TextureMapUniformLocation, texture);

	ground.loadOBJ("../models/ground.obj", defaultShader.TextureMapUniformLocation, texture1);

	ringX.loadOBJ("../models/torus.obj", defaultShader.TextureMapUniformLocation, texture);
	ringY.loadOBJ("../models/torus.obj", defaultShader.TextureMapUniformLocation, texture2);
	ringZ.loadOBJ("../models/torus.obj", defaultShader.TextureMapUniformLocation, texture2);

	ringX.rotate(Vector3f(0.0f,0.0f,90.0f));
	ringY.rotate(Vector3f(0.0f,0.0f,90.0f));
	ringZ.rotate(Vector3f(0.0f,0.0f,90.0f));

	ringX.translate(Vector3f(0.0f,2.0f,0.0f));
	ringY.translate(Vector3f(10.0f,1.0f,0.0f));
	ringY.setScale(2.0f);
	ringZ.translate(Vector3f(2.0f,2.0f,0.0f));

	plane.translate(Vector3f(0.0f,0.75f,0.0f));

	ground.translate(Vector3f(0.0f,0.0f,0.0f));
	ground.setScale(10.0f);
}	

//! Display Loop
void display(void)
{
    //Handle keys
    handleKeys();
	
	// Clear the screen
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//! Time
	t_old	 = t_new;	
	t_new = glutGet(GLUT_ELAPSED_TIME);
	t_delta = (t_new - t_old) / 1000;



	//! Updates all Physics Items
	//=============================================================//
	plane.update(t_delta);
	//=============================================================//

	//! Calculates Third Person Camera Follow
	ThirdPerson.followUpdate(plane);
	

	//! Renders the Skybox
	glUseProgram(skyboxShader.ID);

	glDepthMask(GL_FALSE);
	ThirdPerson.updateShader(skyboxShader);
	skybox.Draw(ThirdPerson.getPosition(),ViewMatrix, skyboxShader);
	glDepthMask(GL_TRUE);
	
	//! Hitboxes
	Matrix4x4 mod = plane.getMatrix();

	glUseProgram(hitboxShader.ID);
	ThirdPerson.updateShader(hitboxShader);
	boxhit.Draw(hitboxShader, ringY.getMatrix());
	hitbox.Draw(hitboxShader, plane.getMatrix());

	std::cout << hitbox.doCollsions(boxhit) << std::endl;
	//boxhit.doCollsions(boxhit);

	//std::cout << hitbox.centre << " to " << boxhit.centre << std::endl;

	

	//! Draws Main Models
	glUseProgram(defaultShader.ID);
	ThirdPerson.updateShader(defaultShader);


	//! Lighting
	glUniform3f(defaultShader.LightPositionUniformLocation, lightPosition.x,lightPosition.y,lightPosition.z);
    glUniform4f(defaultShader.AmbientUniformLocation, ambient.x, ambient.y, ambient.z, 1.0);
    glUniform4f(defaultShader.SpecularUniformLocation, specular.x, specular.y, specular.z, 1.0);
    glUniform1f(defaultShader.SpecularPowerUniformLocation, specularPower);

	//! Probaly Needs to be Changed
	plane.Draw(defaultShader);
	//ringX.Draw(defaultShader);
	//ringY.rotate(Vector3f(0.0f,1.0f,0.0f));	
	ringY.Draw(defaultShader);	

	ground.Draw(defaultShader);
	

	//Unuse Shader
	glUseProgram(0);


	//! Seconds per frame counter;
	frames++;
	if(t_new - t_sinceSecond >= 1000.0)
	{
		fps_count = "ms/frame: " + std::to_string(1000/frames);
		frames = 0;
		t_sinceSecond += 1000.0;
	}
	
	
	//! HUD Elements
	ThirdPersonHUD.render2dText(fps_count,0,0,0,-1,0.95f);
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
		std::cout << "Plane is Facing" << std::endl << plane.facing() << std::endl;
		
		std::cout << "Plane is at" << std::endl << plane.getMeshCentroid() << std::endl;
		std::cout << "Plane is rotated" << std::endl << plane.getRotiation() << std::endl;
		std::cout << std::endl << "Cam is Facing" << std::endl << ThirdPerson.getDirection() << std::endl;
		std::cout << "Cam is at" << std::endl << ThirdPerson.getPosition() << std::endl;
		std::cout << "==========================" << std::endl ;
	}
	else if (key == 'c')
	{
		hitbox.Test();
	}
	else if (key == 'g')
	{
		PentaToggle = !PentaToggle;
		plane.setRotation(Vector3f());
		plane.setPosition(Vector3f(0.0f,0.75f,0.0f));
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
		plane.addSpin(Vector3f(0.0f,0.0f,3.0f));
    }
	if (keyStates['s'])
	{
		plane.addSpin(Vector3f(0.0f,0.0f,-3.0f));
	}
	if(keyStates['a'])
    {
		plane.addSpin(Vector3f(0.0f, 3.0f,0.0f));
    }
	if (keyStates['d'])
	{
		plane.addSpin(Vector3f(0.0f,-3.0f,0.0f));
	}
	if(keyStates['q'])
    {
		plane.addSpin(Vector3f(3.0f,0.0f,0.0f));
    }
	if (keyStates['e'])
	{
		plane.addSpin(Vector3f(-3.0f,0.0f,0.0f));
	}
	if (keyStates['i'])
	{
		lightPosition = lightPosition + Vector3f(1.0,0.0,0.0);
	}
	if (keyStates['k'])
	{
		lightPosition = lightPosition + Vector3f(-1.0,0.0,0.0);
	}
	if (keyStates['j'])
	{
		lightPosition = lightPosition + Vector3f(0.0,0.0,1.0);
	}
	if (keyStates['l'])
	{
		lightPosition = lightPosition + Vector3f(0.0,0.0,-1.0);
	}
	if (keyStates['o'])
	{
		lightPosition = lightPosition + Vector3f(0.0,1.0,0.0);
	}
	if (keyStates['u'])
	{
		lightPosition = lightPosition + Vector3f(0.0,-1.0,0.0);
	}
	if (keyStates[' '])
	{
		plane.addForce(plane.facing() * 0.2f);
	}
	if (keyStates['b'])
	{
		plane.addForce(plane.facing() * -0.2f);
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