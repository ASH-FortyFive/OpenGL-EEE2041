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
private:
    struct OBB
    {
        Vector3f centrePoint;
        Vector3f localAxis[3];
        Vector3f extents;
    }
    


    GLuint vertexPositionBuffer;
    GLuint elementbuffer;

    float width, height, debth; 

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
public:
    Vector3f corner[8];
    Vector3f centre;
    Matrix4x4 modelMatrix;

//! Functions
private:
void loadHitbox();
public:
    bool doCollsions(Hitbox&);

    void Draw(MasterShader, Matrix4x4);

    void Test();

    void changeHitbox(Vector3f, float, float, float);
    
    Vector3f trueCentre();

    Hitbox();
    Hitbox(Vector3f, float,float,float);
    ~Hitbox();
};



#endif