#include "Force.h"

Force::Force()
{
	this->force = glm::vec2(0.0f, 0.0f);
	this->type = Impulse;
}

Force::Force(glm::vec2 f, ForceType type)
{
	this->force = f;
	this->type = type;
}

Force::Force(float x, float y, ForceType type)
{
	this->force = glm::vec2(x, y);
	this->type = type;
}
