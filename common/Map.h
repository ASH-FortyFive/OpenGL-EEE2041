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
    Skybox sky;
    Vector3f mapDimensions;
    Model ground;
    std::vector<Model*> rings;  

    float mapDimensionBuffer = 10.0f; // extra space around the ground plane
//! Member Functions
private:

public:
    //! Constructors and Destructors 
    Map();
    ~Map();

    //! Sets up Map based on file
    bool Init(
        std::string mapFile, 
        MasterShader skyboxShader,
        MasterShader defaultShader, 
        Model ring);

    //! Drawing Function
    void Draw(MasterShader);
    void DrawSkybox(Vector3f,Matrix4x4, MasterShader);
    void DrawHitboxes(MasterShader);

    //! Created new .OBJ for the ground
    void createGround(Vector3f);
    
    //! For checking bounds
    bool inBounds(Vector3f);
    Hitbox::hbType checkCollisions(Vector3f, std::vector<Hitbox*>);
};

#endif

