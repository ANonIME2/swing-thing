#pragma once
#include "WorldObject.h"
#include <vector>
#include <map>

class Force;
enum ForceType;

class Level;

enum PhysicsObjectType
{
	Static,
	Dynamic
};

class PhysicsObject : public WorldObject
{
public:
	PhysicsObject(Level* level, PhysicsObjectType physicsType, float x, float y, float width, float height, float mass = 1.0f, float linearDamping = 1.0f, float angularDamping = 1.0f, float gravity = 1.0f);
	//the following methods are (probably) only used by dynamic objects 
	void physicsUpdate(double time = 1.0);
	void physicsUpdate(std::vector<PhysicsObject*>& physicsObjects, double time = 1.0);
	int addForce(glm::vec2 F, ForceType type);
	int addForce(Force F);
	bool removeForce(int id);
	std::pair<int, int> colides(PhysicsObject* B);


	glm::vec2 linearSpeed, acceleration;
	std::map<int, Force> Forces;
	std::vector<glm::vec2>* hitbox;
	PhysicsObjectType physicsType;
	float linearDamping, angularDamping, gravity, mass;
	

private:
	bool intersects(glm::vec2 A, glm::vec2 B, glm::vec2 C, glm::vec2 D);
	bool intersects(std::pair<float, float> A, std::pair<float, float> B, std::pair<float, float> C, std::pair<float, float> D);
	bool intersects(std::pair<float*, float*> A, std::pair<float*, float*> B, std::pair<float*, float*> C, std::pair<float*, float*> D);
	bool intersects(std::pair<float, float>* A, std::pair<float, float>* B, std::pair<float, float>* C, std::pair<float, float>* D);


};

