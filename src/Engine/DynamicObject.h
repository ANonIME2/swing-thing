#pragma once
#include "PhysicsObject.h"
#include <vector>
#include <map>
class Force;
enum ForceType;


class DynamicObject : public PhysicsObject
{
public:
	DynamicObject(float x = 0.0f, float y = 0.0f, float width = 10.0f, float height = 10.0f, float linearDamping = 0, float angularDamping = 0, float gravity = 0);
	void physicsUpdate(double time);
	int addForce(glm::vec2 F, ForceType type);
	int addForce(Force F);
	bool removeForce(int id);
	
	glm::vec2 linearSpeed, acceleration;
	float linearDamping, angularDamping;
	float gravity;
	std::map<int, Force> Forces;

};

