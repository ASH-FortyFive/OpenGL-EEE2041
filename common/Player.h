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
    Vector3f velocity;
    Vector3f spin;
    Vector3f gravity = Vector3f(0.0f, -9.8f, 0.0f);
    //Vector3f gravity = Vector3f(0.0f, 0.0f, 0.0f);
    Matrix4x4 relativeRotations;
public:

//! Member Functions
private:


public:
    void addForce(Vector3f); // Takes the input and turns it into motion
    void addSpin(Vector3f);
    void rotateAround(float, Vector3f);
    void update(float);

    void Draw(MasterShader shader); // Overloads the draw function to allow for relative rotations, this is immensly cursed

    void stop();

    void Reset();


	//! Constructors and Destructors     
    Player();
    ~Player();
};

#endif

