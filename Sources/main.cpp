// Local Headers

#include <glm\glm.hpp>
#include "glitter.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;
using glm::vec3;
using glm::mat4;

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
};

Vertex myTri[] =
{
	glm::vec3(+0.0f, +1.0f, +0.0f),
	glm::vec3(+1.0f, +0.0f, +0.0f),

	glm::vec3(-1.0f, -1.0f, +0.0f),
	glm::vec3(+0.0f, +1.0f, +0.0f),

	glm::vec3(+1.0f, -1.0f, +0.0f),
	glm::vec3(+0.0f, +0.0f, +1.0f),
};

GLushort indices[] = { 0, 1, 2 };

int main(int argc, char * argv[]) {

    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return EXIT_FAILURE;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));
	
	const char* vertexSource =
		"#version 430\r\n"
		"in layout(location=0) vec2 position;"
		"void main()"
		"{"
		"gl_Position = vec4(position, 0.0, 1.0);"
		"}"
		"";

	const char* fragmentSource =
		"#version 430\r\n"
		"out vec4 daColor;"
		"void main()"
		"{"
		"daColor = vec4(0.0, 1.0, 0.0, 1.0);"
		"}";
	GLfloat vertices[] =
	{
		+0.0f, +0.0f,
		+0.0f, +0.0f, +0.0f,
		+1.0f, +1.0f,
		+0.0f, +0.0f, +0.0f,
		-1.0f, +1.0f,
		+0.0f, +0.0f, +0.0f,
		-1.0f, -1.0f,
		+0.0f, +0.0f, +0.0f,
		+1.0f, -1.0f,
		+0.0f, +0.0f, +0.0f,
	};
	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);



	// Create Vertex Array Object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)(sizeof(GLfloat) * 2));



	GLuint indices[] = { 0,1,2, 0,3,4 };
	GLuint indexBufferID;
	glGenBuffers(1, &indexBufferID);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, &vertexSource, 0);
	glCompileShader(vertexShaderID);


	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, &fragmentSource, 0);
	glCompileShader(fragmentShaderID);


	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);


	glBindFragDataLocation(programID, 0, "daColor");
	glLinkProgram(programID);
	glUseProgram(programID);

    // Rendering Loop
		while (glfwWindowShouldClose(mWindow) == false) {
			if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(mWindow, true);
			
			// Flip Buffers and Draw
			glfwSwapBuffers(mWindow);
			glfwPollEvents();

			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			//glDrawArrays(GL_TRIANGLES, 0, 3);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }   
		
/*		
		glDeleteProgram(shaderProgram);
		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);

	//	glDeleteBuffers(1, &indexBufferID);
		glDeleteBuffers(1, &vertexBufferID);

		glDeleteVertexArrays(1, &vao);
		glfwTerminate();

*/
    return EXIT_SUCCESS;
}
