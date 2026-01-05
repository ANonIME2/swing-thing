#include "Player.h"
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
	addImpulse(glm::vec2(0.0f, this->jumpForce));
}

void Player::walk(glm::vec2 direction)
{
	this->pos += direction * this->walkSpeed;
}

void Player::walkRight()
{
	if (!this->walkingRight) {
		this->walkRightForceIt = this->walkForces.add(glm::vec2(this->walkSpeed, 0.0f));
		this->walkingRight = true;
	}
}

void Player::walkLeft()
{
	if (!walkingLeft) {
		this->walkLeftForceIt = this->walkForces.add(glm::vec2(-this->walkSpeed, 0.0f));
		this->walkingLeft = true;
	}
}

void Player::stopRight()
{
	if (this->walkingRight) {
		this->walkForces.remove(this->walkRightForceIt);
		this->walkingRight = false;
	}
}
void Player::stopLeft()
{	
	if (this->walkingLeft) {
		this->walkForces.remove(this->walkLeftForceIt);
		this->walkingLeft = false;
	}
}

