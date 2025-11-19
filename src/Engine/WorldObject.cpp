#include "WorldObject.h"

WorldObject::WorldObject(float x, float y, float width, float height) {
	pos = { x, y };
	size = { width, height };
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
	VBO = 0;
	VAO = 0;
	EBO = 0;
	//indices[0] = 0;
	//indices[1] = 1;
	//indices[2] = 3;
	//indices[3] = 1;
	//indices[4] = 2;
	//indices[5] = 3;
}

void WorldObject::render()
{
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//if we didn't have an EBO, this would be glDrawarrays()
	glBindVertexArray(0);
	GLint vbo = 0;
	//glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &vbo);
	//std::cout << "VBO bound during render: " << vbo << "   the VBO we want: "<< this->VBO << std::endl;
	//// Allocate space to receive the data
	//std::vector<int> data;
	//data.resize(12);
	//// Read data from the buffer into CPU memory
	//glGetBufferSubData(GL_ARRAY_BUFFER, 0, data.size() * sizeof(float), data.data());
	//for (int i = 0; i < 12; i++) {
	//	std::cout <<data[i] << " ";
	//}
	//std::cout << std::endl;

	
}

void WorldObject::setUpAVO() {

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	//binding VAO, VBO, EBO
	glBindVertexArray(this->VAO);

	// Allocate space to receive the data
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->indices), this->indices, GL_STATIC_DRAW);
	//set our vertex attributes pointers. essentially, telling opengl how our vertex data is formated
	//https://learnopengl.com/img/getting-started/vertex_attribute_pointer.png
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}