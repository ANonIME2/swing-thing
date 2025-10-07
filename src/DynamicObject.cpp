#include "DynamicObject.h"



DynamicObject::DynamicObject(float x, float y, float width, float height, float linearDamping, float angularDamping, float gravity) {
	pos = { x, y };
	size = glm::vec2(width, height);
	linearSpeed = glm::vec2(0.0f, 0.0f);
	acceleration = glm::vec2(0.0f, 0.0f);
	this->linearDamping = linearDamping;
	this->angularDamping = angularDamping;
	forces[0] = glm::vec2(0.0f, -gravity);
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


//updates all physics related things. speed, acceleration, position. handles Continuous forces
void DynamicObject::physicsUpdate(double time)
{
	glm::vec2 time_v = { time, time };
	for (int i = 0; i < forces.size(); i++) {
		linearSpeed += forces[i]*time_v;
	}

	linearSpeed += acceleration*time_v;
	linearSpeed -= linearDamping*time_v;
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

// adds a force.
// if mode = IMPULSE: applies it once. returns -1;
// if mode = Continuous: pushes it to a list of forces that gets applied every physicsUpdate(). returns the id of the force;
int DynamicObject::addForce(glm::vec2 F, ForceType mode)
{
	if (mode == Impulse) {
		linearSpeed += F;
	}
	else if (mode == Continuous) {
		//pick an index
		//if something is fucked in this whole project, this is prolly it. sorry, i had a funny idea, okay?
		std::map<int, glm::vec2>::iterator iterator = forces.begin();
		int i = 0;
		while (iterator != forces.end() && i == iterator->first) {
			i++;
			iterator++;
		}
		if (iterator == forces.end()) {
			i++;
		}

		forces[i] = F;
		return i;
	}
	return -1;
}


//removes a Continuous force specified by the id
bool DynamicObject::removeForce(int id)
{
	if (forces.find(id) != forces.end()) {
		forces.erase(id);
		return true;
	}
	else {
		return false;
	}
}
