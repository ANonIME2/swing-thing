#include "PhysicsObject.h"
#include <iostream>
#include "Level.h"
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
	baseSpeed = glm::vec2(0.0f, 0.0f);
	acceleration = glm::vec2(0.0f, 0.0f);
	this->physicsType = physicsType;
	for (int i = 0; i < sizeof(this->verticesVectors) / sizeof(float); i += 3) {
		this->hitbox = &(this->verticesVectors);
	}

}


glm::vec2 PhysicsObject::speed()
{
	return this->baseSpeed + this->walkForces.sum;
}

//updates all physics related things. speed, acceleration, position. handles Continuous forces. time means how many frames long was the lasts frame.
// if the game runs in 60 fps and the frame lasted 2/60 sec, time=2
void PhysicsObject::physicsUpdate(double time)
{
	assert(isfinite(this->pos.x) && isfinite(this->pos.y) && "position must be finite");
	if (this->physicsType == Static) {
		return;
	}

	glm::vec2 timeVector = { time, time };
	this->baseSpeed.y -= this->gravity * time;
	
	//TODO start using kinetic energy instead of adding just raw speed
	this->baseSpeed += this->continuousForces.sum;
	this->baseSpeed += this->acceleration * timeVector;


	//linear damping
	this->baseSpeed += -this->baseSpeed * glm::length(this->baseSpeed) * this->linearDamping * timeVector;
	this->pos += this->walkForces.sum;
	this->pos += this->baseSpeed;
	//std::cout << "pos = (" << this->pos.x << ", " << this->pos.y << ")" << std::endl;
	assert(isfinite(this->pos.x) && isfinite(this->pos.y) && "position must be finite");

}

void PhysicsObject::physicsUpdate(std::vector<PhysicsObject*>& physicsObjects, double time)
{
	// regular position updates
	this->physicsUpdate(time);

	// collision detection
	glm::vec2 primitiveSnap = -glm::normalize(this->baseSpeed);
	for (auto other : physicsObjects) {
		std::vector<std::pair<int, int>> collidingEdges = this->collides(other);
		float snapMagnitude = 1;
		for (int j = 0; j < collidingEdges.size(); j++) {
			std::vector<glm::vec2> hitboxA = *(this->hitbox);
			std::vector<glm::vec2> hitboxB = *(other->hitbox);
			glm::vec2 A = hitboxA[collidingEdges[j].first] + this->pos;
			glm::vec2 B = hitboxA[(collidingEdges[j].first + 1) % hitboxA.size()] + this->pos;
			glm::vec2 C = hitboxB[collidingEdges[j].second] + other->pos;
			glm::vec2 D = hitboxB[(collidingEdges[j].second + 1) % hitboxB.size()] + other->pos;
			glm::vec2 BC = B - C;
			glm::vec2 CD = D - C;

			// find which vertex is inside the other object
			// if cross is smaller than 0, that means that B is inside the other object
			// (i mean it could be outside, if it was poking out the other end
			// of the other object, but that doesn't change anything)
			float cross = CD.x * BC.y - CD.y * BC.x;
			if (cross > 0) {
				std::swap(A, B);
			}

			// line segment from B to primitiveStep has length = 1 and the same angle as this->speed()
			glm::vec2 primitiveEndB = B + primitiveSnap;
			for (int otherEdge = 0; otherEdge < hitboxB.size(); otherEdge++) {
				glm::vec2 E = hitboxB[otherEdge] + other->pos;
				glm::vec2 F = hitboxB[(otherEdge + 1) % hitboxB.size()] + other->pos;
				float ua = ((F.x - E.x) * (primitiveEndB.y - E.y) - (F.y - E.y) * (primitiveEndB.x - E.x)) / ((F.y - E.y) * (B.x - primitiveEndB.x) - (F.x - E.x) * (B.y - primitiveEndB.y));
				float ub = ((B.x - primitiveEndB.x) * (primitiveEndB.y - C.y) - (B.y - primitiveEndB.y) * (primitiveEndB.x - C.x)) / ((D.y - C.y) * (B.x - primitiveEndB.x) - (D.x - C.x) * (B.y - primitiveEndB.y));
				if (0 < ub && ub < 1 && ua < snapMagnitude && isfinite(ua) && isfinite(ub)) {
					snapMagnitude = ua;
				}
			}

			if (j == collidingEdges.size() - 1) {
				this->pos -= primitiveSnap * (snapMagnitude - 1);
			}
		}
	}
}


//add a force with a Force object
void PhysicsObject::addImpulse(glm::vec2 F)
{
	// TODO use kinetic energy instead oof raw dogging it
	this->baseSpeed += F;
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
	
	//theorem of sines
 	float rLength = glm::length(AC) * sin(gamma);
	float CDAngle = atan(AB.y / AB.x);
	// rAngle = CDAngle + 90deg, so CD and r are perpendicular
	// we will later check if this actually should have been - 90deg
	float rAngle = PI / 2 + CDAngle;
	glm::vec2 r(rLength * cos(rAngle), rLength * sin(rAngle));
	// check if we have to flip r
	// AR and ARprime are vectors from A to: A+r and A-r respectively
	glm::vec2 AR = C + r - A;
	glm::vec2 ARprime = C - r - A;
	// if the distance from A to C+r is smaller than from A to C-r
	if (AR.x * AR.x + AR.y * AR.y < ARprime.x * ARprime.x + ARprime.y * ARprime.y) {
		return r;
	}
	else {
		return -r;
	}
}

std::list<glm::vec2>::iterator PhysicsObject::ForcesRegister::add(glm::vec2 F)
{
	this->forces.push_front(F);
	this->sum += F;
	return this->forces.begin();
}

void PhysicsObject::ForcesRegister::remove(std::list<glm::vec2>::iterator id)
{
	this->sum -= *id;
	this->forces.erase(id);
}
