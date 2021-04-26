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

class Camera
{
//! Member Variables
private:
    //! View Model Member Varibles
    Vector3f position = Vector3f(1.0,1.0,0.0);
    Vector3f target = Vector3f(2.0,2.0,2.0);

    //! Internal Camera Vectors
    Vector3f up;
    Vector3f right;
    Vector3f direction;

    Vector3f GlobalUp = Vector3f(0.0f,1.0f,0.0f);
public:
    //! Perspective Module
    Matrix4x4 ProjectionMatrix;

    

//! Functios
public:
    //! Transforms
    void setProjection(GLuint); //Sends projection to given address
    void changeModelView(Matrix4x4&); //Adjust given matrix to "look at" the correct place

    //! Moving Camera
    void move(Vector3f);
    void follow(Vector3f, Vector3f, Vector3f, Vector3f);

    //! Accessors
    Vector3f getPosition();
    Vector3f getDirection();

    void update();

    void debugPrint();

    //! Constructors and Destructors 
    Camera();
    Camera(Vector3f, Vector3f);
    ~Camera();
private:

};


#endif

