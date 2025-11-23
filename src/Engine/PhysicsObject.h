#pragma once
#include "WorldObject.h"

class PhysicsObject : public WorldObject
{
public:
	PhysicsObject(std::vector<glm::vec2> h = std::vector<glm::vec2>(0));
	bool colides(PhysicsObject* B);
	std::vector<std::pair<float*, float*>> hitbox;
private:
	bool intersects(glm::vec2 A, glm::vec2 B, glm::vec2 C, glm::vec2 D);
	bool intersects(std::pair<float, float> A, std::pair<float, float> B, std::pair<float, float> C, std::pair<float, float> D);
	bool intersects(std::pair<float*, float*> A, std::pair<float*, float*> B, std::pair<float*, float*> C, std::pair<float*, float*> D);
	bool intersects(std::pair<float, float>* A, std::pair<float, float>* B, std::pair<float, float>* C, std::pair<float, float>* D);
	
};

