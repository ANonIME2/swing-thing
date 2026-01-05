#pragma once
#include "WorldObject.h"

#include <vector>
#include <map>
#include <algorithm>
#include <list>

class Force;
enum ForceType;

class Level;

enum PhysicsObjectType
{
	Static,
	Dynamic
};
enum ForceType
{
	Impulse,
	Continuous,
	Walk
};


class PhysicsObject : public WorldObject
{
public:
	class ForcesRegister {
	public:
		ForcesRegister(ForceType type=Continuous) :type(type) {}
		std::list<glm::vec2>::iterator add(glm::vec2 F);
		void remove(std::list<glm::vec2>::iterator id);
		ForceType type;
		std::list<glm::vec2> forces;
		glm::vec2 sum;
	};
	PhysicsObject(Level* level, PhysicsObjectType physicsType, float x, float y, float width, float height, float mass = 1.0f, float linearDamping = 1.0f, float angularDamping = 1.0f, float gravity = 1.0f);
	//the following methods are (probably (idk go crazy)) only used by objects with physicsType=Dynamic
	
	glm::vec2 speed();
	void physicsUpdate(double time = 1.0);
	void physicsUpdate(std::vector<PhysicsObject*>& physicsObjects, double time = 1.0);
	void addImpulse(glm::vec2 F);

	std::vector<std::pair<int, int>> collides(PhysicsObject* B);


	glm::vec2 acceleration;
	ForcesRegister continuousForces;
	ForcesRegister walkForces;
	std::vector<glm::vec2>* hitbox;
	PhysicsObjectType physicsType;
	float linearDamping, angularDamping, gravity, mass;
	

private:
	bool intersects(glm::vec2 A, glm::vec2 B, glm::vec2 C, glm::vec2 D);
	bool intersects(std::pair<float, float> A, std::pair<float, float> B, std::pair<float, float> C, std::pair<float, float> D);
	bool intersects(std::pair<float*, float*> A, std::pair<float*, float*> B, std::pair<float*, float*> C, std::pair<float*, float*> D);
	bool intersects(std::pair<float, float>* A, std::pair<float, float>* B, std::pair<float, float>* C, std::pair<float, float>* D);
	glm::vec2 distVecPoint(glm::vec2 A, glm::vec2 B, glm::vec2 C);
	glm::vec2  baseSpeed;

};

