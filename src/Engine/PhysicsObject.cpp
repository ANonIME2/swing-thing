#include "PhysicsObject.h"
#include <iostream>
#include "Force.h"
#include "Level.h"
#include "Force.h"
#include <vector>
#include <algorithm>


PhysicsObject::PhysicsObject(Level* level, PhysicsObjectType physicsType, float x, float y, float width, float height, float mass, float linearDamping, float angularDamping, float gravity)
	: WorldObject(x, y, width, height)
{
	if (physicsType == Static) {
		this->mass = INFINITY;
		this->linearDamping = INFINITY;
		this->angularDamping = INFINITY;
		this->gravity = 0;

	}
	else {
		this->mass = mass;
		this->linearDamping = linearDamping;
		this->angularDamping = angularDamping;
		this->gravity = gravity;
	}


	level->addObject(this);
	linearSpeed = glm::vec2(0.0f, 0.0f);
	acceleration = glm::vec2(0.0f, 0.0f);
	this->physicsType = physicsType;
	for (int i = 0; i < sizeof(this->verticesVectors) / sizeof(float); i += 3) {
		this->hitbox = &(this->verticesVectors);
	}

}


//updates all physics related things. speed, acceleration, position. handles Continuous forces. time means how many frames long was the lasts frame.
// if the game runs in 60 fps and the frame lasted 2/60 sec, time=2
void PhysicsObject::physicsUpdate(double time)
{
	assert(isfinite(this->pos.x) && isfinite(this->pos.y) && "position must be finite");
	if (this->physicsType == Static) {
		return;
	}
	//std::cout << "pos = (" << this->pos.x << ", " << this->pos.y << ")" << std::endl;
	glm::vec2 time_v = { time, time };
	linearSpeed.y -= gravity * time;
	for (auto i: this->Forces) {
		//TODO start using kinetic energy instead of adding just raw speed
		if (i.type == Continuous) {
			linearSpeed += i.force * time_v;
		}
		else {
			this->pos += i.force;
		}
	}

	linearSpeed += acceleration * time_v;

	//linear damping
	linearSpeed += -linearSpeed * glm::length(linearSpeed) * linearDamping * time_v;
	pos += linearSpeed;
	//std::cout << "pos = (" << this->pos.x << ", " << this->pos.y << ")" << std::endl;
	assert(isfinite(this->pos.x) && isfinite(this->pos.y) && "position must be finite");

}

void PhysicsObject::physicsUpdate(std::vector<PhysicsObject*>& physicsObjects, double time)
{
	// regular position updates
	this->physicsUpdate(time);

	// collision detection
	for (auto i : physicsObjects) {
		std::vector<std::pair<int, int>> collidingEdges = this->collides(i);
		while (collidingEdges.size() != 0) {
			if (i->physicsType == Static) {
				//this only works if one vertex is inside the other object
				//find which vertex is inside the other object
				std::vector<glm::vec2> hitboxA = *(this->hitbox);
				std::vector<glm::vec2> hitboxB = *(i->hitbox);
				glm::vec2 A = hitboxA[collidingEdges[0].first] + this->pos;
				glm::vec2 B = hitboxA[(collidingEdges[0].first + 1) % hitboxA.size()] + this->pos;
				glm::vec2 C = hitboxB[collidingEdges[0].second] + i->pos;
				glm::vec2 D = hitboxB[(collidingEdges[0].second + 1) % hitboxB.size()] + i->pos;
				glm::vec2 CD = D - C;

				//delta is the vector from the vertex inside this object and the vertex inside the other object
				glm::vec2 delta = this->pos + B - i->pos - C;
				float cross = CD.x * delta.y - CD.y * delta.x;
				if (cross > 0) {
					std::swap(A, B);
				}

				//snap this object back to where they don't collide and update the speed accordingly
				// get the cos of the angle between AB and CD
				/*glm::vec2 oldSpeed = this->linearSpeed;
				float lengthOldSpeed = glm::length(oldSpeed);
				float dot = glm::dot(oldSpeed, CD);
				float cos = glm::cos(dot / (lengthOldSpeed * glm::length(CD)));
				float cosTimesLengthOldSpeed = cos * lengthOldSpeed;
				this->linearSpeed = cosTimesLengthOldSpeed *glm::normalize(CD);*/
				// how much this object needs to snap back
 				glm::vec2 snap = distVecPoint(C, D, B);
				// find out if we have to move this by snap or -snap
				// IMPORTANT this part relies on the format returned by collides().
				// And that is vector<pair<int, int>> with the first int in the pair
				// belonging to THIS OBJECT AND NOT THE OTHER ONE
				if (collidingEdges[0].second == collidingEdges[1].second) {
					this->pos += snap;
				}
				else {
					this->pos -= snap;
				}
				this->pos = glm::vec2(std::round(this->pos.x * 100) / 100, std::round(this->pos.y * 100) / 100);
			}
			collidingEdges = this->collides(i);
		}
	}
}



// adds a force.
// if mode = IMPULSE: applies it once. returns std::list<Force>::iterator();
// if mode = Continuous or mode == Walk: pushes it to a list of forces that gets applied every physicsUpdate(). returns the iterator of the force;
std::list<Force>::iterator PhysicsObject::addForce(glm::vec2 F, ForceType type)
{
	if (type == Impulse) {
		//TODO start using kinetic energy instead of adding just raw speed
		linearSpeed.x += F.x;linearSpeed.y += F.y;
		return std::list<Force>::iterator();
	}
	else if (type == Continuous || type == Walk) {
		Forces.push_front(Force(F, type));	
		return Forces.begin();
	}

	
}

//add a force with a Force object
std::list<Force>::iterator PhysicsObject::addForce(Force F)
{
	return addForce(F.force, F.type);
}


//removes a force specified by the iterator
void PhysicsObject::removeForce(std::list<Force>::iterator id)
{
	this->Forces.erase(id);
}

// checks if two PhysicsObjects' hitboxes are coliding.
// if they are, returns the indexes of the edges that are intersecting.
// FORMAT: vector<pair<int, int>> with the first int in the pair
// belonging to THIS OBJECT and the second one TO OBJECT B
std::vector<std::pair<int, int>>  PhysicsObject::collides(PhysicsObject* B)
{
	// check if it's not the same object
	if (B == this) {
		return std::vector<std::pair<int, int>>();
	}

	std::vector<glm::vec2>
		hitboxVectorsA = *(this->hitbox),
		hitboxVectorsB = *(B->hitbox),
		hitboxA(this->hitbox->size()+1),
		hitboxB(B->hitbox->size()+1);

	int aSize = hitboxA.size();
	int bSize = hitboxB.size();
	if (hitboxVectorsA.size() == 0 || hitboxVectorsB.size() == 0) {
		return std::vector<std::pair<int, int>>();
	}

	for (int i = 0; i < hitboxVectorsA.size(); i++) {
		hitboxA[i] = glm::vec2(this->pos.x + hitboxVectorsA[i].x, this->pos.y + hitboxVectorsA[i].y);
	}
	hitboxA[hitboxA.size() - 1] = hitboxA[0];

	for (int i = 0; i < hitboxVectorsB.size(); i++) {
		hitboxB[i] = glm::vec2(B->pos.x + hitboxVectorsB[i].x, B->pos.y + hitboxVectorsB[i].y);
	}
	hitboxB[hitboxB.size() - 1] = hitboxB[0];



	std::vector<std::pair<int, int>> result;
	// check if any of the edges intersect
	for (int i = 0; i < aSize-1; i++) {
		for (int j = 0; j < bSize-1; j++) {
			if (intersects(hitboxA[i], hitboxA[i + 1], hitboxB[j], hitboxB[j + 1])) {
				result.push_back(std::make_pair(i, j));
			}
		}
	}


	return result;
}

//checks if the line segment AB intersects the line segment CD
// returns false if, for example, point A is ON the line segment CD. 
bool PhysicsObject::intersects(glm::vec2 A, glm::vec2 B, glm::vec2 C, glm::vec2 D) {
	
	//assertions
	glm::vec2* arr[4] = { &A, &B, &C, &D };
	for (int i = 0; i < sizeof(arr) / sizeof(glm::vec2); i++) {
		assert(isfinite(arr[i]->x) && isfinite(arr[i]->y) && "All the points' coordinates must not be nan, or inf)");
	}


	float x1 = A.x, x2 = B.x, x3 = C.x, x4 = D.x, y1 = A.y, y2 = B.y, y3 = C.y, y4 = D.y;
	float uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
	float uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
	//std::cout << A.x << " " << A.y << " " << B.x << " " << B.y << " " << C.x << " " << C.y << " " << D.x << " " << D.y << " " << std::endl;
	if (uA > 0 && uA < 1 && uB > 0 && uB < 1) {
		return true;
	}
	return false;
}

bool PhysicsObject::intersects(std::pair<float, float> A, std::pair<float, float> B, std::pair<float, float> C, std::pair<float, float> D)
{
	return this->intersects(glm::vec2(A.first, A.second), glm::vec2(B.first, B.second), glm::vec2(C.first, C.second), glm::vec2(D.first, D.second));
}
bool PhysicsObject::intersects(std::pair<float, float>* A, std::pair<float, float>* B, std::pair<float, float>* C, std::pair<float, float>* D)
{
	return this->intersects(glm::vec2(A->first, A->second), glm::vec2(B->first, B->second), glm::vec2(C->first, C->second), glm::vec2(D->first, D->second));
}
bool PhysicsObject::intersects(std::pair<float*, float*> A, std::pair<float*, float*> B, std::pair<float*, float*> C, std::pair<float*, float*> D)
{
	return this->intersects(glm::vec2(*A.first, *A.second), glm::vec2(*B.first, *B.second), glm::vec2(*C.first, *C.second), glm::vec2(*D.first, *D.second));
}

// let A, B and C be poins in 2D space
// returns a vector from C to the closest point on the vector from A to B
glm::vec2 PhysicsObject::distVecPoint(glm::vec2 A, glm::vec2 B, glm::vec2 C)
{	
	float PI = 2 * acos(0.0);
	glm::vec2 AB = B - A;
	glm::vec2 AC = C - A;
	//gamma = angle BAC 
	float gamma = acos(glm::dot(AB, AC) / (glm::length(AB) * glm::length(AC)));
	//beta = angle ACB (sum of angles in a triangle)
	float beta = PI / 2 - gamma;
	//theorem of sines
 	float rLength = glm::length(AC) * sin(gamma);
	float CDAngle = atan(AB.y / AB.x);
	//sum of angles in a triangle. two of the triangles vertices are on the OX axis and the third is the D point
	float rAngle = PI / 2 + CDAngle;

	return glm::vec2(rLength * cos(rAngle), rLength * sin(rAngle));
}

