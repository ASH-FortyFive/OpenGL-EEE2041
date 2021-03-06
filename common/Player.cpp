#include <Player.h>
#include <type_traits>

#define minSpeed 2.5f
#define maxSpeed 40.0f
#define bias 0.96f

//! Constructors and Destructors 
Player::Player()
{
    
}
Player::Player(Vector3f scale, Vector3f pos, Vector3f rot)
{
   Model(scale, pos, rot);
}
Player::~Player()
{}

void Player::update(float frac)
{
    //! Velocity
    speed -= Model::relativeAxis[0].y * frac * 2.5f;
    speed += boost * frac;
    if(speed < minSpeed)
    {
        speed = minSpeed;
    }
    if(speed > maxSpeed)
    {
        speed = maxSpeed;
        boost = 0;
    }

    //! Rotations
    relativeRotations.toIdentity();

    spin = spin + spinAcceleration * frac * 2.0f;

    relativeRotations.rotate(spin.x, 1,0,0);
    relativeRotations.rotate(spin.y, 0,1,0);
    relativeRotations.rotate(spin.z, 0,0,1); 

    //! Decays Forces
    speed = speed * (0.99999f);
    boost = boost * (0.95f);
    spinAcceleration = spinAcceleration * (0.95f);


    velocity = Model::relativeAxis[0] * (speed);
    //addForce( gravity * frac );
    Model::translate(velocity * frac);
    //Model::rotate(spin * frac);
}

void Player::addBoost(float newBoost)
{
    boost = boost + newBoost * 0.2f;
}

void Player::stop()
{
    velocity = Vector3f();
}

void Player::addSpin(Vector3f newSpin)
{
    spinAcceleration = spinAcceleration + newSpin * 2.0f;
}
 
void Player::Draw(MasterShader shader)
{   
    Model::ModelMatrix = getMatrix();

    Model::Draw(shader);
}

//! Accesor
Matrix4x4 Player::getMatrix()
{
    Matrix4x4 model;
    model = Model::getMatrix();
    model = model * relativeRotations;

    return model;
}

float Player::getSpeed()
{
    return velocity.length();
}

//! Debug
void Player::Reset()
{
    boost = 0;
    spinAcceleration = Vector3f();

    velocity = Vector3f();
    spin = Vector3f();
    relativeRotations.toIdentity();
    speed = 30.0f;

    Model::Reset();
}