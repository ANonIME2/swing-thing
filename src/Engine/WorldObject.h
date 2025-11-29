#pragma once
#include <glm.hpp>
#include <glad/glad.h>



class WorldObject
{
public:
	WorldObject(float x = 0.0f, float y = 0.0f, float width = 10.0f, float height = 10.0f);
	void render();
	void setUpAVO();
	float vertices[12] = {};
	int indices[6] = {
		0, 1, 3,
		1, 2, 3
	};
	glm::vec2 pos, size;
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
};
