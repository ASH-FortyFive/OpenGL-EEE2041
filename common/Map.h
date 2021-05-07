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
    std::vector<Model*> grounds;
    std::vector<Model*> rings;  
//! Member Functions
private:
    //! For taking in the Map file
    friend std::istream& operator>>(std::istream& in, Map& map);
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
    void DrawHitbox(MasterShader);


    
};

#endif

