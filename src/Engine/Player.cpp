#include "Player.h"
#include "Force.h"
#include "Level.h"
#include <iostream>
Player::Player(Level* level, PhysicsObjectType physicsType, float x, float y, float width, float height, float mass, float linearDamping, float angularDamping, float gravity, float jumpForce, float walkSpeed)
	: PhysicsObject(level, physicsType, x, y, width, height, mass, linearDamping, angularDamping, gravity)
{
	this->jumpForce = jumpForce;
	this->walkSpeed = walkSpeed;
}
void Player::jump()
{
	addForce(glm::vec2(0.0f, this->jumpForce), Impulse);
}

void Player::walk(glm::vec2 direction)
{
	this->pos += direction * this->walkSpeed;
}

void Player::walkRight()
{
	walk(glm::vec2(1, 0));
}

void Player::walkLeft()
{
	walk(glm::vec2(-1, 0));
}

