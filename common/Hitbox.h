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

    Vector3f centre;

    float width, height, debth; 

    GLfloat vertexPositionData[24] = 
	{ 
         0.0f,  0.0f,  0.0f, //0 Bottom,front, left
		 0.0f,  0.0f,  1.0f, //1 Bottom,front, right
         0.0f,  1.0f,  0.0f, //2 Top,   front, left
         0.0f,  1.0f,  1.0f, //3 Top,   front, right
		 1.0f,  0.0f,  0.0f, //4 Bottom,back,  left
         1.0f,  0.0f,  1.0f, //5 Bottom,back,  right
         1.0f,  1.0f,  0.0f, //6 Top,   back,  left
         1.0f,  1.0f,  1.0f  //7 Top,   back,  right
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
        5,4,0,
        //Left
        0,2,6,
        6,4,0,
        //Right
        1,3,7,
        7,5,1
	};


//! Functions
private:
void loadHitbox();
public:
    bool doCollsions(Hitbox&);

    void Draw(MasterShader shader);

    void Test();

    void changeHitbox(Vector3f, float, float, float);
    

    Hitbox();
    Hitbox(Vector3f, float,float,float);
    ~Hitbox();
};



#endif