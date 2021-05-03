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
    Matrix4x4 ModelView;

    GLuint vertexPositionBuffer;
    GLuint elementbuffer;

    GLfloat vertexPositionData[24] = 
	{ 
         5.0f,  0.0f, -0.5f, //0 Bottom,front, left
		 5.0f,  0.0f,  0.5f, //1 Bottom,front, right
         5.0f,  1.0f, -0.5f, //2 Top,   front, left
         5.0f,  1.0f,  0.5f, //3 Top,   front, right
		 6.0f,  0.0f, -0.5f, //4 Bottom,back,  left
         6.0f,  0.0f,  0.5f, //5 Bottom,back,  right
         6.0f,  1.0f, -0.5f, //6 Top,   back,  left
         6.0f,  1.0f,  0.5f  //7 Top,   back,  right
	}; // 3D vertex position ‘f’ indicates floating point

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
        5,4,1,
        //Left
        0,2,6,
        6,4,0
        //Right
        1,3,7,
        7,5,1


        /*
        //Bottom
		0,1,5,
        5,4,0,
        //Top
        2,3,7,
        7,6,2,
        //Left
        0,4,6,
        6,2,0,
        //Right
        1,5,7,
        7,3,1,
        //Front
        0,1,3,
        3,4,0,
        //Back
        4,5,7,
        7,6,4*/
	};

//! Functions
public:
    bool doCollsions(Hitbox&);

    void Draw(MasterShader);

    void Test();

    void loadHitboxes(Vector3f, float, float, float);

    Hitbox();
    ~Hitbox();
};


#endif