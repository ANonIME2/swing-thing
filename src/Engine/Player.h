#pragma once
#include "PhysicsObject.h"
class Level;
class Player : public PhysicsObject
{
public:

	Player(
		Level* level,
		PhysicsObjectType physicsType,
		float x = 0.0f,
		float y = 0.0f,
		float width = 10.0f,
		float height = 10.0f,
		float mass = 1.0f,
		float linearDamping = 0.0f,
		float angularDamping = 0.0f,
		float gravity = 0.0f,
		float jumpForce = 50.0f);

	void jump();
	bool jumpedLastFrame = false;
	float jumpForce, walkSpeed;
	void walk(glm::vec2 direction);
	void walkRight();
	void walkLeft();

};
