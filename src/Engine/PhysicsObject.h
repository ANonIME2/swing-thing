#pragma once
#include "WorldObject.h"

class PhysicsObject : public WorldObject
{
public:
	PhysicsObject(std::vector<glm::vec2> h = std::vector<glm::vec2>(0));
	bool colides(PhysicsObject* B);
	std::vector<glm::vec2> hitbox;
private:
	bool intersects(glm::vec2 A, glm::vec2 B, glm::vec2 C, glm::vec2 D);
};

