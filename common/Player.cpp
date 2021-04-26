#include <Player.h>
#include <type_traits>

//! Constructors and Destructors 
Player::Player()
{}
Player::~Player()
{}

void Player::update(float time_global)
{
    //! Changes speed to be consistent based on frame rate
    GLfloat adjustedSpeed(5.0f * (time_global - prevT_global));
    
    //Model::translate(Model::facing() * adjustedSpeed);

    prevT_global = time_global;
}
