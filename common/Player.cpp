#include <Player.h>
#include <type_traits>

#define wingArea 0.0174532925199f
#define wingLength 0.0174532925199f
 
#define liftSlope 6.2f
#define skinFriction 0.02f
#define zeroLiftAoA 0.0f
#define stallAngle 15.0f
#define chord 1.0f
#define flapFraction 0.2f
#define aspectRatio 2.0f

//! Constructors and Destructors 
Player::Player()
{}
Player::~Player()
{}

void Player::update(float frac)
{

    //addForce( gravity * frac );
    Model::translate(velocity * frac);
    Model::rotate(spin * frac);

    //! Decays Forces
    velocity = Model::relativeAxis[0] * 10;
    spin = spin * (0.95f);
    
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

    Model::Reset();
}