/* Model Class (and Helper Functions)
 * Created by ASH for EEE2041
 * The Third Person Camera used in the Flight Sim
 */
#ifndef Camera_H_
#define Camera_H_



#include "Model.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <Matrix.h>
#include <Vector.h>
#include <Player.h>
#include <math.h>
#include <ShaderClass.h>

class Camera
{
//! Member Variables
private:
    //! View Model Member Varibles
    Vector3f position = Vector3f(1.0,1.0,0.0);

    //! Internal Camera Vectors
    Vector3f direction;
    Vector3f target;

    Vector3f GlobalUp = Vector3f(0.0f,1.0f,0.0f);

    Vector3f targetOffset,positionOffset;

    bool flipped = false;

    //! Perspective Module
    Matrix4x4 ProjectionMatrix;
    Matrix4x4 ViewMatrix;
//! Functios
public:
    //! Moving Camera
    void move(Vector3f);

    //! Accessors
    Vector3f getPosition();
    Vector3f getDirection();

    
    void updateShader(MasterShader); //Sends projection to given address

    void followUpdate(Matrix4x4&, Vector3f);

    //! Constructors and Destructors 
    Camera();
    Camera(Vector3f posOffset, Vector3f tarOffset = Vector3f(0,0,0), Vector3f pos = Vector3f(0,0,0));
    ~Camera();
private:
    //! Transforms
    void follow(Matrix4x4&, Vector3f, Vector3f, Vector3f);

};


#endif

