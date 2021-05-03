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
		 6.0f,  0.0f, -0.5f, //2 Bottom,back,  left
         6.0f,  0.0f,  0.5f, //3 Bottom,back,  right
         5.0f,  1.0f, -0.5f, //4 Top,   front, left
         5.0f,  1.0f,  0.5f, //5 Top,   front, right
         6.0f,  1.0f, -0.5f, //6 Top,   back,  left
         6.0f,  1.0f,  0.5f  //7 Top,   back,  right
	}; // 3D vertex position ‘f’ indicates floating point

    GLuint vertexIndexData[36] = 
	{
        //Bottom
		0,1,3,
        3,2,0,
        //Top
        4,5,7,
        7,6,4,
        //Left
        0,2,6,
        6,4,0,
        //Right
        1,3,7,
        7,5,1,
        //Front
        0,1,5,
        5,4,0,
        //Back
        2,3,7,
        7,6,2
	};

//! Functions
public:
    bool doCollsions(Hitbox&);

    void Draw(MasterShader);

    void loadHitboxes(Vector3f, float, float, float);

    Hitbox();
    ~Hitbox();
};


#endif