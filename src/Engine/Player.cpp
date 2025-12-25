#include "Player.h"
#include "Force.h"
#include "Level.h"
#include <iostream>
Player::Player(Level* level, PhysicsObjectType physicsType, float x, float y, float width, float height, float mass, float linearDamping, float angularDamping, float gravity, float jumpForce)
	: PhysicsObject(level, physicsType, x, y, width, height, mass, linearDamping, angularDamping, gravity)
{
	this->jumpForce = jumpForce;
}
void Player::jump()
{
	addForce(glm::vec2(0.0f, this->jumpForce), Impulse);
}
