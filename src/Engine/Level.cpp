#include "Level.h"
#include "WorldObject.h"
#include "DynamicObject.h"
#include <chrono>
#include <thread>
Level::Level(unsigned int framerate_goal): FRAMERATE_GOAL(framerate_goal)
{

}

void Level::addObject(DynamicObject* object)
{
	this->physicsObjects.push_back(object);
}

void Level::physicsUpdate(double time)
{
	for (auto i : this->physicsObjects) {
		std::vector<DynamicObject*> noIPhysicsObjects = this->physicsObjects;//this->physicsObjects, but without the i elemetn
		for (int j = 0; j < noIPhysicsObjects.size(); j++) {
			if (noIPhysicsObjects[j] == i) {
				noIPhysicsObjects.erase(noIPhysicsObjects.begin() + j);
			}
		}

		i->physicsUpdate(noIPhysicsObjects, time);
	}
}

void Level::render() {
	auto now = std::chrono::high_resolution_clock::now();
	double deltaTime = std::chrono::duration<double>(now - last_frame).count();
	last_frame = now;
	if (1.0 / FRAMERATE_GOAL > deltaTime) {
		// the 1000000000 is just the conversion from seconds to nanoseconds
		std::this_thread::sleep_for(std::chrono::nanoseconds((int)(1000000000 / FRAMERATE_GOAL - deltaTime * 1000000000)));
	}

	for (auto i : this->physicsObjects) {
		i->render();
	}
}