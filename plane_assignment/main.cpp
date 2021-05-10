//!Includes
#include <GL/glew.h>
#include <GL/glut.h>

#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <iomanip>

#include <Texture.h>
#include <Vector.h>
#include <Matrix.h>
#include <Mesh.h>
#include <Shader.h>

#include <Player.h>
#include <Camera.h>
#include <Model.h>
#include <Skybox.h>
#include <HUD.h>
#include <Map.h>
#include <ShaderClass.h>
#include <Hitbox.h>


//!Function Prototypes
void initGLUTFunctions();
void display(void);
void keyboard(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void handleKeys();
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void Reset();
void LoadMap(std::string mapPath);

//! Screen size
int screenWidth   	        = 1920;
int screenHeight   	        = 1080;

//! Array of key states
bool keyStates[256];

//! Global Variables
//! Light Settings
//Material Properties
Vector3f ambient    = Vector3f(0.1,0.1,0.1);
Vector3f specular   = Vector3f(1.0,1.0,1.0);
float specularPower = 100.0f;

//! Shaders
MasterShader defaultShader,skyboxShader, hitboxShader;

//! Player Model
Player plane;

//! Ring Template
Model ring;
Model box;

//! All Textures
GLuint texture;
GLuint texture1;
GLuint texture2;

Camera ThirdPerson(Vector3f(-5.0f,2,0.0f), Vector3f(10.0f,0,0));
//Camera ThirdPerson(Vector3f(1.0f,0.25f,0.0f), Vector3f(10.0f,0,0));

HUD ThirdPersonHUD;

//! Bools for Toggles
bool PentaToggle = false;
bool WireFrame = false;

// Time
double t_new = 0.0;
double t_old = 0.0;
double t_delta = 0.0;

double t_sinceSecond = 0.0;
double t_timer;
double t_score;

Map map;
std::vector<std::string> mapPaths;
auto mapCounter = mapPaths.begin();

//! HUD Details
int frames;
std::string fps_count;
int outOfBoundsTimer;

int collectedRings(0);
int score(0);
std::vector<int> scores;

//! States
enum State{
    paused,
    start,
	end,
    playing,
	died
};

bool win(false);

State currentState = start;


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

    // Init Key States to false;    
    for(int i = 0 ; i < 256; i++)
        keyStates[i] = false;
    
    // Sets up shader program
    defaultShader.loadShader("../shaders/shader.vert","../shaders/shader.frag");
	skyboxShader.loadShader("../shaders/skybox_shader.vert","../shaders/skybox_shader.frag");
	hitboxShader.loadShader("../shaders/hitbox_shader.vert","../shaders/hitbox_shader.frag");

	// Loads All Textures
	GLuint texture, texture1, texture2;

	ModelHelper::initTexture("../models/plane1.bmp", texture);
	ModelHelper::initTexture("../models/coin.bmp", texture1);
	ModelHelper::initTexture("../models/Crate.bmp", texture2);

	//Creates Ring template to be used in Map
	ring.loadOBJ("../models/torus.obj", defaultShader.TextureMapUniformLocation, texture1);
	ring.loadHitbox("../models/hitboxes/torus.hitbox");

	//Creates Box Obstacle 
	box.loadOBJ("../models/cube.obj", defaultShader.TextureMapUniformLocation, texture2);
	box.loadHitbox("../models/hitboxes/box.hitbox");

	//! If specific map is given, that is loaded other wise the default array occurs
	if(argc != 1)
	{
		std::cout << argv[1] << std::endl;
		mapPaths.push_back(argv[1]);
	}
	else
	{
		mapPaths.push_back("../maps/map3.map");
		mapPaths.push_back("../maps/map2.map");
		mapPaths.push_back("../maps/map1.map");
	}
	
	LoadMap(mapPaths.back());
	

	//! Main Player Plane
	plane.loadOBJ("../models/plane2.obj", defaultShader.TextureMapUniformLocation, texture);
	plane.loadHitbox("../models/hitboxes/plane2.hitbox");
	plane.setScale(0.75f);

	//! Possible Some Inital Camera Work
	//ThirdPerson.followUpdate(plane.relativeAxis, plane.getRotation(), plane.getMeshCentroid());		

	// Init Light
	Vector3f lightPosition= Vector3f(1000.0,1000.0,-1414.0f);
	glUseProgram(defaultShader.ID);
	glUniform3f(defaultShader.LightPositionUniformLocation, lightPosition.x,lightPosition.y,lightPosition.z);
    glUniform4f(defaultShader.AmbientUniformLocation, ambient.x, ambient.y, ambient.z, 1.0);
    glUniform4f(defaultShader.SpecularUniformLocation, specular.x, specular.y, specular.z, 1.0);
    glUniform1f(defaultShader.SpecularPowerUniformLocation, specularPower);

	//! Sets background colour (which should never be seen)
	glClearColor(0.1,0.1,0.1,1.0);

    // Enter main loop
    glutMainLoop();

	// Clean-Up
    glDeleteProgram(defaultShader.ID);
	glDeleteProgram(skyboxShader.ID);
	glDeleteProgram(hitboxShader.ID);

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

    //glutTimerFunc(100,Timer, 0); //Start start timer function after 100 milliseconds

	glEnable(GL_TEXTURE_2D); //Enables 2D textures in OpenGl
	glEnable(GL_DEPTH_TEST); //Enables Depth Test to ensure things render in the correct order
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

	//! Game Logic
	if(win)
	{
		ThirdPersonHUD.render2dText(("Level Complete!"),0,0,0,-0.15f,0);
		if(t_new - t_timer > 1500)
		{
			
			if(!mapPaths.empty())
			{
				mapPaths.pop_back();
			}

			
			scores.push_back(score);
			win = false;

			if(mapPaths.empty())
			{
				Reset();
				currentState = end;
			}
			else
			{
				currentState = start;
				LoadMap(mapPaths.back());
			}
		}
	}
	else if(collectedRings >= map.ringCount)
	{
		win = true;
		t_timer = t_new;
	}

	//! Handels Consquences of Reset
	if(currentState == died)
	{
		if(t_new - t_timer > 500)
		{	
			Reset();
			currentState = playing;
		}
	}

	if(currentState == playing && !win)
	{
	//! Removes score slowly as time passes
	if(t_new - t_score >= 1000.0)
	{
			score -= 10;
			t_score += 1000.0;
	}

	//! Does Collisions
	Hitbox::hbType collison = map.checkCollisions(plane.getMeshCentroid(), plane.hitboxes);
	if(collison == Hitbox::Obstacle)
	{
		if(currentState != win)
		{
		plane.colour = Vector3f(0.25f,0,0);
		currentState = died;
		t_timer = t_new;

		collectedRings = 0;
		}
	} 
	else if (collison == Hitbox::Target)
	{
		collectedRings++;
		score += 150;
	}

	//! Stops Player From Leaving Map Bounds
	if(!map.inBounds(plane.getMeshCentroid()))
	{
		ThirdPersonHUD.render2dText(("RETURN TO MAP BOUNDS IN " + std::to_string(5 - outOfBoundsTimer) + " SECONDS"),1,0,0,-0.22f,0);
		if(outOfBoundsTimer >= 5)
		{
			plane.colour = Vector3f(0.25f,0,0);
			currentState = died;
			t_timer = t_new;

			collectedRings = 0;
		}	
	}
	else
	{
		outOfBoundsTimer = 0;
	}

	//! Does Plane Physics
	plane.update(t_delta);

	
	}

	//! Calculates Third Person Camera Follow
	ThirdPerson.followUpdate(plane.ModelMatrix, plane.relativeAxis, plane.getMeshCentroid());			

	if(currentState != end)
	{
	//! Renders the Skybox
	ThirdPerson.updateShader(skyboxShader);
	map.DrawSkybox(ThirdPerson.getPosition(), skyboxShader);
	
	//! Draws Main Models
	ThirdPerson.updateShader(defaultShader);
	plane.Draw(defaultShader);
	map.Draw(defaultShader); // Includes Rings and Ground

	//! Hitboxes
	if(WireFrame)
	{
		ThirdPerson.updateShader(hitboxShader);
		plane.DrawHitboxes(hitboxShader);
		map.DrawHitboxes(hitboxShader);
	}
	}
	//Unuse Shader
	glUseProgram(0);

	if(currentState == start)
	{
		ThirdPersonHUD.render2dText(( "Press Space to Begin"),1,1,1,-0.12f,0.0f);
		ThirdPerson.followUpdate(plane.ModelMatrix, plane.relativeAxis, plane.getMeshCentroid());
	}
	else if(currentState == end)
	{
		
		ThirdPersonHUD.render2dText(("Your Scores: "),1,1,1,-0.1f,0.85f);
		int i(1);
		for(auto mapScore : scores)
		{
			ThirdPersonHUD.render2dText(("Level " +  std::to_string(i) + ": " + std::to_string(mapScore)),1,1,1,-0.2f,0.85f - (0.1f * i));
			i++;
		}

		ThirdPersonHUD.render2dText(("Press any key to Quit"),1,1,1,-0.2f,-0.85f);
	}
	else
	{
		//! HUD Elements
		ThirdPersonHUD.render2dText(fps_count,0,0,0,-1,0.95f);
		ThirdPersonHUD.render2dText(( "Speed: " + std::to_string(plane.getSpeed())),0,0,0,-1.0f,0.90f);
		ThirdPersonHUD.render2dText(("Rings: " + std::to_string(collectedRings) + "/" + std::to_string(map.ringCount)),0,0,0,0.85f,0.95f);
		ThirdPersonHUD.render2dText(("Score: " + std::to_string(score)),0,0,0,-0.1f,0.95f);

		//! Seconds per frame counter;
		frames++;
		if(t_new - t_sinceSecond >= 1000.0)
		{
			fps_count = "ms/frame: " + std::to_string(1000/frames) + " | FPS: " + std::to_string(frames);
			frames = 0;
			outOfBoundsTimer++;
			t_sinceSecond += 1000.0;
		}
	}

	if(currentState == paused)
	{
	ThirdPersonHUD.render2dText(("PAUSED"),1,1,1,-0.05f,0);
	}


    //Swap Buffers and post redisplay
	glutSwapBuffers();
	glutPostRedisplay();
}

//! Keyboard Interaction
void keyboard(unsigned char key, int x, int y)
{
	if(currentState == end)
	{
		exit(0);
	}
	key = tolower(key);
	//Quits program when esc is pressed
	if (key == 27)	//esc key code
	{
		exit(0);
	}
	else if (key == 'r')
	{
		Reset();
	}
	else if(key == 'z')
	{
		WireFrame = !WireFrame;
	}
	else if(key == ' ')
	{
		if(currentState == paused || currentState == start)
		{
			currentState = playing;
		}
		else if(currentState == playing)
		{
			currentState = paused;
		}
	}
	else if(key == 'f')
	{
		ThirdPerson.togglePOV();
	}
	else if(key == '/')
	{
		win = true;
	}

    //Set key status

    keyStates[key] = true;
}

//! Handle key up situation
void keyUp(unsigned char key, int x, int y)
{
	key = tolower(key);
    keyStates[key] = false;
}

//! Handle Keys
void handleKeys()
{
	if(currentState == playing){
    //Keys are handled here
	if(keyStates['w'])
    {
		plane.addBoost(1);
    }
	if (keyStates['s'])
	{
		plane.addBoost(-1);
	}
	if(keyStates['a'])
    {
		plane.addSpin(Vector3f( 0, 1, 0));//plane.relativeAxis[0]);
    }
	if (keyStates['d'])
	{
		plane.addSpin(Vector3f( 0,-1, 0));//plane.relativeAxis[0]);
	}
	if (keyStates['i'])
	{
		plane.addSpin(Vector3f( 0, 0, 1));
	}
	if (keyStates['k'])
	{
		plane.addSpin(Vector3f( 0, 0,-1));
	}
	if (keyStates['j'])
	{
		plane.addSpin(Vector3f(-1, 0, 0));//plane.relativeAxis[0]);
	}
	if (keyStates['l'])
	{
		plane.addSpin(Vector3f( 1, 0, 0));//plane.relativeAxis[0]);
	}
	}
}

//! Mouse Interaction
void mouse(int button, int state, int x, int y)
{
	//std::cout << "Button: " << button << " State: " << state << " X: " << x << " Y: " << y << std::endl;
}

//! Motion
void motion(int x, int y)
{
    //std::cout << " X: " << x << " Y: " << y << std::endl;
}

//! Reset Function
void Reset()
{
	score = 0;
	collectedRings = 0;
	plane.colour = Vector3f(0,0,0);
	plane.Reset();
	map.Reset();
	plane.setPosition(map.planePos);
}

void LoadMap(std::string mapPath)
{
	score = 0;
	collectedRings = 0;
	if(map.Init(mapPath, skyboxShader, defaultShader,ring,box))
	{
		std::cout << "Map file loaded" << std::endl;
	}
	else
	{
		std::cout << "Could not load Map file" << std::endl;
		exit(0);
	}
	plane.Reset();
	plane.setPosition(map.planePos);
}