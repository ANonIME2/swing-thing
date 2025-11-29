#include <glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include "Engine.h"
#include <thread>
#include <iostream>



unsigned int WINDOW_WIDTH = 1200, WINDOW_HEIGHT = 800;
unsigned int FRAMERATE_GOAL = 120;
Level level(FRAMERATE_GOAL);
Player player(&level, -30.2f, 0.0f, 40.0f, 60.0f, 0.01f, 0.0f, 0.3f);
DynamicObject otherObject(&level, 0.0f, -30.0f, 80.0f, 60.0f, 0.01f, 0.0f, 0.0f);
//unsigned int VBO;
//unsigned int VAO;
//unsigned int EBO;

//activated when the window changes size
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	WINDOW_WIDTH = width; WINDOW_HEIGHT = height;
	glViewport(0, 0, width, height);
}


//input processing
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		if (!player.jumpedLastFrame) {
			player.jump();
			player.jumpedLastFrame = true;
		}
	}
	else {
		player.jumpedLastFrame = false;
	}
	
}


//ACTUALL OPENGL STUFF
auto setUpShaderProgram() {
	//shader setup
	//vertex shader
	const char* vertexShaderSource =
		"#version 330 core																												\n"
		"layout(location = 0) in vec3 aPos;																								\n"
		"																																\n"
		"uniform vec3 cameraPos; //where is the center of camera view expressed in the coordinate system used by the vertices			\n"
		"uniform vec2 cameraSize; //how many points wide and tall is camera view. expressed in the coordinate system used by vertices	\n"
		"uniform vec2 windowSize;																										\n"
		"void main() {																													\n"
		"	gl_Position = vec4((aPos.x - cameraPos.x) / (cameraSize.x / 2) / windowSize.x, (aPos.y - cameraPos.y) / (cameraSize.y / 2) / windowSize.y, aPos.z, 1.0);		\n"
		"}																																\n";
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);


	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//fragment shader
	const char* fragmentShaderSource =
		"#version 330 core							\n"
		"out vec4 FragColor;						\n"
		"											\n"
		"void main() {								\n"
		"	FragColor = vec4(0.8f, 0.1f, 0.3, 0.7f);\n"
		"}											\n";

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//creating a shader program and linking the shaders to it
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
	}
	else {
		//telling opengl to use this program from now on
		glUseProgram(shaderProgram);
		//setting up the shader program
		int cameraPosLoc = glGetUniformLocation(shaderProgram, "cameraPos");
		glUniform3f(cameraPosLoc, 0.0f, 0.0f, 1.0f);
		int cameraSizeLoc = glGetUniformLocation(shaderProgram, "cameraSize");
		glUniform2f(cameraSizeLoc, 1.5f, 1.5f);
		int windowSizeLoc = glGetUniformLocation(shaderProgram, "windowSize");
		glUniform2f(windowSizeLoc, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT);
	}

	//deleting shader objects, since we don't need them anymore
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return shaderProgram;
}


//void drawPlayer() {
//	glBindVertexArray(VAO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(player.vertices), player.vertices, GL_STREAM_DRAW);
//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//if we didn't have an EBO, this would be glDrawarrays()
//	glBindVertexArray(0);
//
//}
//
//void setUpAVO() {
//	
//
//	glGenBuffers(1, &VBO);
//	glGenBuffers(1, &EBO);
//	glGenVertexArrays(1, &VAO);
//
//	//binding VAO, VBO, EBO
//	glBindVertexArray(VAO);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(player.vertices), player.vertices, GL_STREAM_DRAW);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(player.indices), player.indices, GL_STREAM_DRAW);
//
//
//	//set our vertex attributes pointers. essentially, telling opengl how our vertex data is formated
//	//https://learnopengl.com/img/getting-started/vertex_attribute_pointer.png
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//
//	glClearColor(40.0f / 255, 40.0f / 255, 40.0f / 255, 1.0f);
//}

int main() {

	//initialising everything
	//WINDOW INITIALISATION STUFF
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//creating the window
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Learn OpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//initialising GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	/*set the coordinate system.
	This line tells open gl that the bottom-left corner has the coordinates(0, 0)
	and that the window is WINDOW_WIDTH px wide and WINDOW_HEIGHT px tall
	*/
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	/*it's like addEventListener for the window changing size.
	it calls the function framebuffer_size_callback*/
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	framebuffer_size_callback(window, WINDOW_WIDTH, WINDOW_HEIGHT);

	//vsync
	glfwSwapInterval(1);




	setUpShaderProgram();
	player.setUpAVO();
	otherObject.setUpAVO();

	glClearColor(40.0f / 255, 40.0f / 255, 40.0f / 255, 1.0f);
	//render loop
	while (!glfwWindowShouldClose(window)) {
		//input
		processInput(window);

		//physycs updates
		level.physicsUpdate();


		//rendering commands here
		glClear(GL_COLOR_BUFFER_BIT);
		level.render();
		
		
		//check and call events and swap the buffers
		glfwPollEvents();
		glfwSwapBuffers(window);

	}

	//deletes all resources that we have allocated in the program,
	// so we don't leave any trash(we are such clean tik tok girls)
	glfwTerminate();
	return 0;
}