#pragma once
#include "WorldObject.h"
#include <vector>

class PhysicsObject : public WorldObject
{
public:
	PhysicsObject(float x = 0.0f, float y = 0.0f, float width = 10.0f, float height = 10.0f);
	std::vector<glm::vec2>* hitbox;

};

