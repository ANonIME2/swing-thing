#include "Player.h"
#include "Force.h"
#include "Level.h"
#include <iostream>
Player::Player(Level* level, float x, float y, float width, float height, float linearDamping, float angularDamping, float gravity, float jumpForce)
	: DynamicObject(level, x, y, width, height, linearDamping, angularDamping, gravity)
{
	this->jumpForce = jumpForce;
}
void Player::jump()
{
	addForce(glm::vec2(0.0f, this->jumpForce), Impulse);
}
