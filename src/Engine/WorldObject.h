#pragma once
#include <glm.hpp>
#include <vector>


class WorldObject
{
public:
	WorldObject(float x = 0.0f, float y = 0.0f, float width = 10.0f, float height = 10.0f);
	void render();
	void setUpAVO();
	std::vector<glm::vec2> verticesVectors;  //holds vectors that point from (pos.x, pos.y) to each vertex
	int indices[6] = {
		0, 1, 3,
		1, 2, 3
	};
	glm::vec2 pos, size;
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
};
