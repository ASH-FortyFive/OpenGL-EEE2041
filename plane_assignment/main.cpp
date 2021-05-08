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

//! Loaded Models
Model ground;

Model ring;

Player plane;

GLuint texture;
GLuint texture1;
GLuint texture2;

Camera ThirdPerson(Vector3f(-4.0f,0.25,0.0f));

HUD ThirdPersonHUD;

//! Bools for Toggles
bool PentaToggle = false;
bool WireFrame = false;

// Time
float t_new = 0.0;
float t_old = 0.0;
float t_delta = 0.0;

float t_sinceSecond = 0.0;


Hitbox hitbox(Vector3f(0,0.0,0)	,2,0.5f,0.5f);

Hitbox boxhit(Vector3f(0,0.0,0)	,2,0.1f,2);

Map map;

//! HUD Details
int frames;
std::string fps_count;

int score(0);

//! Debug Performance Checking
float action_time;

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
	//! Init Light
	//Set colour variable and light position
	lightPosition= Vector3f(1000.0,1000.0,-1414.0f);

	ModelHelper::initTexture("../models/plane1.bmp", texture);
	ModelHelper::initTexture("../models/Crate.bmp", texture2);

	plane.loadOBJ("../models/plane2.obj", defaultShader.TextureMapUniformLocation, texture);
	plane.loadHitbox("../models/hitboxes/plane2.hitbox");
	plane.translate(Vector3f(10.0f,10.0f,25.0f));

	ring.loadOBJ("../models/torus.obj", defaultShader.TextureMapUniformLocation, texture2);
	ring.loadHitbox("../models/hitboxes/torus.hitbox");


	if(map.Init("../maps/default.map", skyboxShader, defaultShader,ring))
	{
		std::cout << "Map file loaded" << std::endl;
	}
	else
	{
		std::cout << "Could not load Map file" << std::endl;
		//exit(0);
	}

	ThirdPerson.followUpdate(plane.facing(), plane.getRotiation(), plane.getMeshCentroid());		

	glUseProgram(defaultShader.ID);
	glUniform3f(defaultShader.LightPositionUniformLocation, lightPosition.x,lightPosition.y,lightPosition.z);
    glUniform4f(defaultShader.AmbientUniformLocation, ambient.x, ambient.y, ambient.z, 1.0);
    glUniform4f(defaultShader.SpecularUniformLocation, specular.x, specular.y, specular.z, 1.0);
    glUniform1f(defaultShader.SpecularPowerUniformLocation, specularPower);

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

	action_time = glutGet(GLUT_ELAPSED_TIME);
	//! Updates all Physics Items
	//=============================================================//
	Hitbox::hbType collison = map.checkCollisions(plane.getMeshCentroid(), plane.hitboxes);
	if(collison == Hitbox::Obstacle)
	{
		plane.stop();
	} 
	else if (collison == Hitbox::Target)
	{
		score++;
	}

	if(!map.inBounds(plane.getMeshCentroid()))
	{
		std::cout << "Outside" << std::endl;
	}

	plane.update(t_delta);
	//=============================================================//

	//! Calculates Third Person Camera Follow
	
	ThirdPerson.followUpdate(plane.facing(), plane.getRotiation(), plane.getMeshCentroid());	
	action_time = glutGet(GLUT_ELAPSED_TIME) - action_time;

	//! Renders the Skybox
	ThirdPerson.updateShader(skyboxShader);
	map.DrawSkybox(ThirdPerson.getPosition(),ViewMatrix, skyboxShader);
	
	//! Draws Main Models
	ThirdPerson.updateShader(defaultShader);

	//! Lighting
	glUseProgram(defaultShader.ID);
	
	//! Probaly Needs to be Changed
	plane.Draw(defaultShader);
	map.Draw(defaultShader);
	
	//! Hitboxes
	if(WireFrame)
	{
		ThirdPerson.updateShader(hitboxShader);
		plane.DrawHitboxes(hitboxShader);
		map.DrawHitboxes(hitboxShader);
	}
	
	//Unuse Shader
	glUseProgram(0);


	//! Seconds per frame counter;
	frames++;
	if(t_new - t_sinceSecond >= 1000.0)
	{
		fps_count = "ms/frame: " + std::to_string(1000/frames) + " | FPS: " + std::to_string(frames);
		frames = 0;
		t_sinceSecond += 1000.0;
	}
	
	//!Performance check
	//std::cout << "Time taken: "<< action_time << std::endl;

	//! HUD Elements
	ThirdPersonHUD.render2dText(fps_count,0,0,0,-1,0.95f);
	ThirdPersonHUD.render2dText(std::to_string(score),0,0,0,-1.0f,0.85f);

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
		plane.translate(1,0,0);
		std::cout << "Forward" <<std::endl;
    }
	else if (key == 'X')
	{
		plane.translate(-1,0,0);
		std::cout << "backward" <<std::endl;
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
		plane.test();
	}
	else if (key == 'g')
	{
		PentaToggle = !PentaToggle;
		plane.setRotation(Vector3f());
		plane.setPosition(Vector3f(10.0f,10.0f,25.0f));
	}
	else if(key == 'z' || key == 'Z')
	{
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
		plane.addForce(plane.facing() * 1.0f);
	}
	if (keyStates['b'])
	{
		plane.addForce(plane.facing() * -1.0f);
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