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
		linearSpeed += i.force * time_v;
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
		std::pair<int, int> colidingEdges = this->colides(i);
		if (colidingEdges.first != -1) {
			if (i->physicsType == Static) {
				//find which vertex is inside the other object
				std::vector<glm::vec2> hitboxA = *(this->hitbox);
				std::vector<glm::vec2> hitboxB = *(i->hitbox);
				glm::vec2 A = hitboxA[colidingEdges.first];
				glm::vec2 B = hitboxA[(colidingEdges.first+1)%hitboxA.size()];
				glm::vec2 C = hitboxB[colidingEdges.second];
				glm::vec2 D = hitboxB[(colidingEdges.second+1)%hitboxB.size()];
				glm::vec2 CD = D - C;
				//delta is the vector from the vertex inside this and the vertex inside the other object
				glm::vec2 delta = this->pos + B - i->pos - C;
				float cross = CD.x * delta.y - CD.y * delta.x;
				if (cross > 0) {
					std::swap(A, B);
				}

				//snap this object back to where they don't collide
				glm::vec2 n = -glm::normalize(this->linearSpeed);
				float scalar = std::min(abs(delta.x / n.x), abs(delta.y / n.y));
				this->pos += n * scalar;
			}
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
// else, return std::make_pair(-1, -1)
std::pair<int, int>  PhysicsObject::colides(PhysicsObject* B)
{
	// check if it's not the same object
	if (B == this) {
		return std::make_pair(-1, -1);
	}

	std::vector<glm::vec2>
		hitboxVectorsA = *(this->hitbox),
		hitboxVectorsB = *(B->hitbox),
		hitboxA(this->hitbox->size()+1),
		hitboxB(B->hitbox->size()+1);

	int aSize = hitboxA.size();
	int bSize = hitboxB.size();
	if (hitboxVectorsA.size() == 0 || hitboxVectorsB.size() == 0) {
		return std::make_pair(-1, -1);
	}

	for (int i = 0; i < hitboxVectorsA.size(); i++) {
		hitboxA[i] = glm::vec2(this->pos.x + hitboxVectorsA[i].x, this->pos.y + hitboxVectorsA[i].y);
	}
	hitboxA[hitboxA.size() - 1] = hitboxA[0];

	for (int i = 0; i < hitboxVectorsB.size(); i++) {
		hitboxB[i] = glm::vec2(B->pos.x + hitboxVectorsB[i].x, B->pos.y + hitboxVectorsB[i].y);
	}
	hitboxB[hitboxB.size() - 1] = hitboxB[0];




	// check if any of the edges intersect
	for (int i = 0; i < aSize-1; i++) {
		for (int j = 0; j < bSize-1; j++) {
			if (intersects(hitboxA[i], hitboxA[i + 1], hitboxB[j], hitboxB[j + 1])) {
				return std::make_pair(i, j);
			}
		}
	}


	return std::make_pair(-1, -1);
}

bool PhysicsObject::intersects(glm::vec2 A, glm::vec2 B, glm::vec2 C, glm::vec2 D) {
	//checks if the line segment AB intersects the line segment CD
	/*std::cout
		<< "A = ("
		<< A.x
		<< ", "
		<< A.y
		<< ")"
		<< "B = ("
		<< B.x
		<< ", "
		<< B.y
		<< ")"
		<< "C = ("
		<< C.x
		<< ", "
		<< C.y
		<< ")"
		<< "D = ("
		<< D.x
		<< ", "
		<< D.y
		<< ")"
		<< std::endl;*/

	//assertions
	glm::vec2* arr[4] = { &A, &B, &C, &D };
	for (int i = 0; i < sizeof(arr) / sizeof(glm::vec2); i++) {
		assert(isfinite(arr[i]->x) && isfinite(arr[i]->y) && "All the points' coordinates must not be nan, or inf)");
	}


	float x1 = A.x, x2 = B.x, x3 = C.x, x4 = D.x, y1 = A.y, y2 = B.y, y3 = C.y, y4 = D.y;
	float uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
	float uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
	//std::cout << A.x << " " << A.y << " " << B.x << " " << B.y << " " << C.x << " " << C.y << " " << D.x << " " << D.y << " " << std::endl;
	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {
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
