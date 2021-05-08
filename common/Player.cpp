#include <Player.h>
#include <type_traits>

#define minSpeed 5.0f

//! Constructors and Destructors 
Player::Player()
{}
Player::~Player()
{}

void Player::update(float frac)
{


    //! Go down faster then it goes up
    speed -= Model::relativeAxis[0].y * frac * 20.0f;
    if(speed < minSpeed)
    {
        speed = minSpeed;
    }
    std::cout << speed << std::endl; 

    //! Decays Forces
    velocity = Model::relativeAxis[0] * speed;
    spin = spin * (0.95f);



    //addForce( gravity * frac );
    Model::translate(velocity * frac);
    Model::rotate(spin * frac);

    
    
}

void Player::addForce(Vector3f newForce)
{
    velocity = velocity + newForce;
}

void Player::stop()
{
    velocity = Vector3f();
}

void Player::addSpin(Vector3f newSpin)
{
    spin = spin + newSpin;
}

void Player::rotateAround(float angle, Vector3f axis)
{
    relativeRotations.rotate(angle, axis.x, axis.y, axis.z);
}
 
void Player::Draw(MasterShader shader)
{

    Vector3f up, right, forward;
    forward =Model::position + Model::relativeAxis[0] * 10;
    up = Model::position + Model::relativeAxis[1] * 10;
    right = Model::position + Model::relativeAxis[2] * 10;

    glBegin(GL_LINES);
  	glVertex3f(forward.x, forward.y, forward.z);
  	glVertex3f(Model::position.x, Model::position.y, Model::position.z);
	glEnd();

    glBegin(GL_LINES);
  	glVertex3f(up.x, up.y, up.z);
  	glVertex3f(Model::position.x, Model::position.y, Model::position.z);
	glEnd();

    glBegin(GL_LINES);
  	glVertex3f(right.x, right.y, right.z);
  	glVertex3f(Model::position.x, Model::position.y, Model::position.z);
	glEnd();

    //relativeRotations.toIdentity();
    Model::ModelMatrix = Model::getMatrix();

    Model::ModelMatrix = Model::ModelMatrix * relativeRotations;

    Model::Draw(shader);
}

//! Debug
void Player::Reset()
{
    velocity = Vector3f();
    spin = Vector3f();
    relativeRotations.toIdentity();
    speed = 30.0f;

    Model::Reset();
}