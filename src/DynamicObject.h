#pragma once
#include "WorldObject.h"

enum ForceType
{
	Impulse,
	Continuous
};


class DynamicObject : public WorldObject
{
public:
	DynamicObject(float x = 0.0f, float y = 0.0f, float width = 10.0f, float height = 10.0f, float linearDamping = 0, float angularDamping = 0, float gravity = 0);
	void physicsUpdate(double time);
	int addForce(glm::vec2 F, ForceType mode);
	bool removeForce(int id);
	glm::vec2 linearSpeed, acceleration;
	float linearDamping, angularDamping;
	std::map<int, glm::vec2> forces;
};

