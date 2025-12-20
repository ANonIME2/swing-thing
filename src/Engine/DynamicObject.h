#pragma once
#include "PhysicsObject.h"
#include <vector>
#include <map>
class Force;
class Level;
enum ForceType;


class DynamicObject : public PhysicsObject
{
public:
	DynamicObject(float x = 0.0f, float y = 0.0f, float width = 10.0f, float height = 10.0f, float linearDamping = 0, float angularDamping = 0, float gravity = 0);
	DynamicObject(Level* level, float x = 0.0f, float y = 0.0f, float width = 10.0f, float height = 10.0f, float linearDamping = 0, float angularDamping = 0, float gravity = 0);
	void physicsUpdate(double time = 1.0);
	void physicsUpdate(std::vector<DynamicObject*> &physicsObjects, double time = 1.0);
	int addForce(glm::vec2 F, ForceType type);
	int addForce(Force F);
	bool removeForce(int id);
	bool colides(PhysicsObject* B);

	glm::vec2 linearSpeed, acceleration;
	float linearDamping, angularDamping;
	float gravity;
	std::map<int, Force> Forces;

private:
	bool intersects(glm::vec2 A, glm::vec2 B, glm::vec2 C, glm::vec2 D);
	bool intersects(std::pair<float, float> A, std::pair<float, float> B, std::pair<float, float> C, std::pair<float, float> D);
	bool intersects(std::pair<float*, float*> A, std::pair<float*, float*> B, std::pair<float*, float*> C, std::pair<float*, float*> D);
	bool intersects(std::pair<float, float>* A, std::pair<float, float>* B, std::pair<float, float>* C, std::pair<float, float>* D);
};

