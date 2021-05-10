/* Map Class
 * Created by ASH for EEE2041
 * Handels the terrain and rings
 */
#ifndef Map_H_
#define Map_H_

#include <GL/glew.h>
#include <GL/glut.h>

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include <Vector.h>
#include <Model.h>
#include <ShaderClass.h>
#include <Skybox.h>



class Map
{
//! Member Variables
private:
    Skybox * sky;
    Vector3f mapDimensions;
    Model * ground;
    std::vector<Model*> rings; 
    std::vector<Model*> boxes;  

    float mapDimensionBuffer = 10.0f; // extra space around the ground plane
public:
    Vector3f planePos;
    int ringCount;
//! Member Functions
private:
    //! Created new .OBJ for the ground
    void createGround(Vector3f);
public:
    //! Constructors and Destructors 
    Map();
    ~Map();

    //! Sets up Map based on file
    bool Init(
        std::string mapPath, 
        MasterShader skyboxShader,
        MasterShader defaultShader, 
        Model ring,
        Model box);
    //! Resets based on previous Map File
    void Reset(); 

    //! Drawing Function
    void Draw(MasterShader);
    void DrawSkybox(Vector3f, MasterShader);
    void DrawHitboxes(MasterShader);

    
    //! For checking bounds
    bool inBounds(Vector3f);
    Hitbox::hbType checkCollisions(Vector3f, std::vector<Hitbox*>);
};

#endif

