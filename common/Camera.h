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
    Vector3f position = Vector3f(4.0,4.0,4.0);
    Vector3f lookAt = Vector3f(2.0,2.0,2.0);
    
public:
    //! Perspective Module
    Matrix4x4 ProjectionMatrix;
//! Functios
public:
    //! Transforms
    void setProjection(GLuint); //Sends projection to given address
    void changeModelView(Matrix4x4&); //Adjust given matrix to "look at" the correct place

    //! Moving Camera
    void follow(Vector3f, Vector3f);

    //! Constructors and Destructors 
    Camera();
    Camera(Vector3f, Vector3f);
    ~Camera();
private:

};


#endif

