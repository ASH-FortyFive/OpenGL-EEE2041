#include <Player.h>
#include <type_traits>

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
    velocity = velocity * (0.95f);
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