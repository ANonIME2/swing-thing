#pragma once
#include "WorldObject.h"
#include <vector>
#include <utility>

class PhysicsObject : public WorldObject
{
public:
	PhysicsObject(std::vector<glm::vec2> h = std::vector<glm::vec2>(0));
	std::vector<std::pair<float*, float*>> hitbox;

};

