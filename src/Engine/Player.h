#pragma once
#include "DynamicObject.h"

class Player : public DynamicObject
{
public:
	Player(
		float x = 0.0f,
		float y = 0.0f,
		float width = 10.0f,
		float height = 10.0f,
		float linearDamping = 0.0f,
		float angularDamping = 0.0f,
		float gravity = 0.0f,
		float jumpForce = 50.0f);

	void jump();
	bool jumpedLastFrame = false;
	float jumpForce;
};
