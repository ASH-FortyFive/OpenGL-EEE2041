/* Player Class
 * Created by ASH for EEE2041
 * Handels the input and actions for the Player (an airplane)
 */
#ifndef Player_H_
#define Player_H_

#include "Vector.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <Model.h>

class Player : public Model
{
//! Member Variables
private:
    Vector3f direction;
    GLfloat speed;

    float prevT_global; // The time when UpdatePosition was last called, avoids speed being frame rate based.
    //GLfloat acceleration;
public:

//! Member Functions
private:


public:
    void takeInput(); // Takes the input and turns it into motion
    void update(float);

	//! Constructors and Destructors     
    Player();
    ~Player();
};

#endif

