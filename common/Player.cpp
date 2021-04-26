#include <Player.h>
#include <type_traits>

//! Constructors and Destructors 
Player::Player()
{}
Player::~Player()
{}

void Player::update(float frac)
{
    //addForce(gravity * frac);
    Model::translate((velocity + gravity * frac) * frac);
    velocity = velocity * (0.99f);
}

void Player::addForce(Vector3f newForce)
{
    velocity = velocity + newForce;
}