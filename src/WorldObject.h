#pragma once
#include <utility>
#include <glm-master/glm/glm.hpp>
#include <map>



class WorldObject
{
public:
	WorldObject(float x = 0.0f, float y = 0.0f, float width = 10.0f, float height = 10.0f);
	float vertices[12] = {};
	int indices[6] = {
		0, 1, 3,
		1, 2, 3
	};
	glm::vec2 pos, size;

};
