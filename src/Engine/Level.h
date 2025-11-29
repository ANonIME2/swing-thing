#pragma once
#include <vector>
#include <chrono>
class DynamicObject;


class Level
{
public:
	Level(unsigned int framerate_goal);
	void addObject(DynamicObject* object);
	void physicsUpdate();
	void render();
	std::chrono::steady_clock::time_point last_frame;
	const unsigned int FRAMERATE_GOAL;
	std::vector<DynamicObject*> physicsObjects;
};

