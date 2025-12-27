#include "Level.h"
#include "WorldObject.h"
#include "PhysicsObject.h"
#include <chrono>
#include <thread>
Level::Level(unsigned int framerate_goal): FRAMERATE_GOAL(framerate_goal)
{

}

void Level::addObject(PhysicsObject* object)
{
	this->physicsObjects.push_back(object);
}

void Level::physicsUpdate(double time)
{
	for (auto i : this->physicsObjects) {
		if (i->physicsType == Dynamic) {
			i->physicsUpdate(this->physicsObjects, time);
		}
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