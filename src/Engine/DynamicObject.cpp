#include <iostream>
#include "DynamicObject.h"
#include "Force.h"
#include "Level.h"
#include <vector>


DynamicObject::DynamicObject(float x, float y, float width, float height, float linearDamping, float angularDamping, float gravity)
	: linearDamping(linearDamping), angularDamping(angularDamping), gravity(gravity)
{
	pos = { x, y };
	size = glm::vec2(width, height);
	linearSpeed = glm::vec2(0.0f, 0.0f);
	acceleration = glm::vec2(0.0f, 0.0f);
	for (int i = 2; i < 12; i += 3) {
		vertices[i] = 0.0f;
	}
	vertices[0] = pos.x;
	vertices[1] = pos.y;
	vertices[3] = pos.x;
	vertices[4] = pos.y + height;
	vertices[6] = pos.x + width;
	vertices[7] = pos.y + height;
	vertices[9] = pos.x + width;
	vertices[10] = pos.y;

}

DynamicObject::DynamicObject(Level* level, float x, float y, float width, float height, float linearDamping, float angularDamping, float gravity)
	: linearDamping(linearDamping), angularDamping(angularDamping), gravity(gravity)
{
	pos = { x, y };
	size = glm::vec2(width, height);
	linearSpeed = glm::vec2(0.0f, 0.0f);
	acceleration = glm::vec2(0.0f, 0.0f);
	level->addObject(this);
	for (int i = 2; i < 12; i += 3) {
		vertices[i] = 0.0f;
	}
	vertices[0] = pos.x;
	vertices[1] = pos.y;
	vertices[3] = pos.x;
	vertices[4] = pos.y + height;
	vertices[6] = pos.x + width;
	vertices[7] = pos.y + height;
	vertices[9] = pos.x + width;
	vertices[10] = pos.y;
}



//updates all physics related things. speed, acceleration, position. handles Continuous forces. time means how many frames long was the lasts frame.
// if the game runs in 60 fps and the frame lasted 2/60 sec, time=2
void DynamicObject::physicsUpdate(double time)
{
	glm::vec2 time_v = { time, time };
	linearSpeed.y -= gravity * time;
	for (int i = 0; i < Forces.size(); i++) {
		linearSpeed += Forces[i].force * time_v;
	}

	linearSpeed += acceleration * time_v;

	//linear damping
	linearSpeed += -linearSpeed * glm::length(linearSpeed) * linearDamping * time_v;
	pos += linearSpeed;
	vertices[0] = pos.x;
	vertices[1] = pos.y;
	vertices[3] = pos.x;
	vertices[4] = pos.y + size.y;
	vertices[6] = pos.x + size.x;
	vertices[7] = pos.y + size.y;
	vertices[9] = pos.x + size.x;
	vertices[10] = pos.y;

}

void DynamicObject::physicsUpdate(std::vector<DynamicObject*> &physicsObjects, double time)
{
	this->physicsUpdate(time);
	for (auto i : physicsObjects) {
		if (this->colides(i)) {
		
			std::cout << "tak" << std::endl;
		}
	}
}



// adds a force.
// if mode = IMPULSE: applies it once. returns -1;
// if mode = Continuous: pushes it to a list of forces that gets applied every physicsUpdate(). returns the id of the force;
int DynamicObject::addForce(glm::vec2 F, ForceType type)
{
	if (type == Impulse) {
		linearSpeed.x += F.x;linearSpeed.y += F.y;
	}
	else if (type == Continuous || type == Walk) {
		//pick an index
		//if something is fucked in this whole project, this is prolly it. sorry, i had a funny idea, okay?
		auto iterator = Forces.begin();
		int i = 0;
		while (iterator != Forces.end() && i == iterator->first) {
			i++;
			iterator++;
		}
		if (iterator == Forces.end()) {
			i++;
		}

		Forces[i] = Force(F, type);
		return i;
	}
	
	return -1;
}

int DynamicObject::addForce(Force F)
{
	return addForce(F.force, F.type);
}


//removes a force specified by the id
bool DynamicObject::removeForce(int id)
{
	if (Forces.find(id) != Forces.end()) {
		Forces.erase(id);
		return true;
	}
	else {
		return false;
	}
}

bool DynamicObject::colides(PhysicsObject* B)
{
	//checks if two PhysicsObjects' hitboxes are coliding
	auto A = this;
	int aSize = A->hitbox.size();
	int bSize = B->hitbox.size();
	if (aSize == 0 || bSize == 0) {
		return false;
	}


	for (int i = 0; i < aSize - 1; i++) {
		for (int j = 0; j < bSize - 1; j++) {
			if (intersects(A->hitbox[i], A->hitbox[i + 1], B->hitbox[j], B->hitbox[j + 1])) {
				return true;
			}
		}
		if (intersects(A->hitbox[i], A->hitbox[i + 1], B->hitbox[0], B->hitbox[bSize - 1])) {
			return true;
		}
	}

	for (int i = 0; i < bSize - 1; i++) {
		if (intersects(A->hitbox[aSize - 1], A->hitbox[0], B->hitbox[i], B->hitbox[i + 1])) {
			return true;
		}
	}
	return intersects(A->hitbox[aSize - 1], A->hitbox[0], B->hitbox[bSize - 1], B->hitbox[0]);
}

bool DynamicObject::intersects(glm::vec2 A, glm::vec2 B, glm::vec2 C, glm::vec2 D) {
	//checks if the line segment AB intersects the line segment CD

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

bool DynamicObject::intersects(std::pair<float, float> A, std::pair<float, float> B, std::pair<float, float> C, std::pair<float, float> D)
{
	return this->intersects(glm::vec2(A.first, A.second), glm::vec2(B.first, B.second), glm::vec2(C.first, C.second), glm::vec2(D.first, D.second));
}

bool DynamicObject::intersects(std::pair<float, float>* A, std::pair<float, float>* B, std::pair<float, float>* C, std::pair<float, float>* D)
{
	return this->intersects(glm::vec2(A->first, A->second), glm::vec2(B->first, B->second), glm::vec2(C->first, C->second), glm::vec2(D->first, D->second));
}

bool DynamicObject::intersects(std::pair<float*, float*> A, std::pair<float*, float*> B, std::pair<float*, float*> C, std::pair<float*, float*> D)
{
	return this->intersects(glm::vec2(*A.first, *A.second), glm::vec2(*B.first, *B.second), glm::vec2(*C.first, *C.second), glm::vec2(*D.first, *D.second));
}
