#pragma once
#include <glm.hpp>

enum ForceType
{
	Impulse,
	Continuous,
	Walk
};


class Force
{
public:
	Force();
	Force(glm::vec2 f, ForceType type);
	Force(float x, float y, ForceType type);
	
	glm::vec2 force;
	ForceType type;
};

