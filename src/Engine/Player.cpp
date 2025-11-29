#include "Player.h"
#include "Force.h"
#include "Level.h"
#include <iostream>
Player::Player(Level* level, float x, float y, float width, float height, float linearDamping, float angularDamping, float gravity, float jumpForce)
	: jumpForce(jumpForce)
{
	pos = { x, y };
	size = glm::vec2(width, height);
	linearSpeed = glm::vec2(0.0f, 0.0f);
	acceleration = glm::vec2(0.0f, 0.0f);
	this->angularDamping = angularDamping;
	this->linearDamping = linearDamping;
	this->gravity = gravity;
	for (int i = 2; i < 12; i += 3) {
		vertices[i] = 0.0f;
	}
	level->addObject(this);
	vertices[0] = pos.x;
	vertices[1] = pos.y;
	vertices[3] = pos.x;
	vertices[4] = pos.y + height;
	vertices[6] = pos.x + width;
	vertices[7] = pos.y + height;
	vertices[9] = pos.x + width;
	vertices[10] = pos.y;
}
void Player::jump()
{
	addForce(glm::vec2(0.0f, this->jumpForce), Impulse);
}
