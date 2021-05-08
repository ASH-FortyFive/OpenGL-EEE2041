/* Model Class (and Helper Functions)
 * Created by ASH for EEE2041
 * Expands the provided Mesh Class to allow for Transforms and Textures
 *
 * Also handels the loading of textures (as the Texture.h does not do this)
 */
#ifndef hitbox_H_
#define hitbox_H_

#include <GL/glew.h>
#include <GL/glut.h>

#include <math.h>
#include <iostream>
#include <Vector.h>
#include <Matrix.h>
#include <Mesh.h>
#include <list>
#include <vector>
#include <iterator>
#include <ShaderClass.h>


class Hitbox{
//! Member Variables
public:
     Matrix4x4 ModelMatrix;


    enum hbType
    {
        Obstacle,
        Target,
        Main,
        NONE
    };

    hbType Type;

    struct OBB
    {
        Vector3f centrePoint;
        Vector3f localAxis[3] = {
            Vector3f(-1,0,0),
            Vector3f(0,-1,0),
            Vector3f(0,0,-1)
        };
        float extents[3];
    };
private:
    GLuint vertexPositionBuffer;
    GLuint elementbuffer;

    GLfloat transformedVertexPositionData[24];

    GLuint vertexIndexData[36] = 
	{
        //Front
        0,1,3,
        3,2,0,
        //Back
        4,5,7,
        7,6,4,
        //Top
        2,3,7,
        7,6,2,
        //Bottom
        0,1,5,
        5,4,0,
        //Left
        0,2,6,
        6,4,0,
        //Right
        1,3,7,
        7,5,1
	};

    Vector3f corner[8];
    OBB obb; 
//! Functions
private:
    void loadHitbox();
    void drawLine(Vector3f, Vector3f);
    bool againstAxis(Vector3f axis, Vector3f distance, Vector3f pE[2][3]);
public:
    bool doCollsions(Hitbox&);
    void Draw(MasterShader);    
    Hitbox();
    Hitbox(Vector3f, float,float,float, hbType type = Obstacle);
    Hitbox(OBB newOBB, hbType type = Obstacle);
    ~Hitbox();

    Hitbox(Hitbox const&); // copy constructor
    Hitbox& operator=(Hitbox const&); // copy assignment

};



#endif