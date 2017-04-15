
#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <fstream>
#include <string>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <glad/glad.h>
#include "Camera.hpp"
#include "MeGlWindow.hpp"
#include "Primitives\ShapeData.hpp"
#include "Primitives\ShapeGenerator.hpp"

using namespace std;
using glm::vec3;
using glm::mat4;

GLint numIndices;
Camera camera;

bool checkStatus(
	GLuint objectID,
	PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	GLenum statusType)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);

		fprintf(stderr, "CHECK SHADER STATUS %s\n\n", buffer);
		delete[] buffer;
		return false;
	}
	return true;
}

bool MeGlWindow::checkShaderStatus(GLint shaderID)
{
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool MeGlWindow::checkProgramStatus(GLint programID)
{
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

string MeGlWindow::readShaderCode(const char* fileName)
{
	ifstream meInput(fileName);
	if (!meInput.good())
	{
		cout << "File failed to load..." << fileName;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>());
}



void MeGlWindow::installShaders()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	string temp = readShaderCode("./Glitter/Sources/Shaders/VertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = readShaderCode("./Glitter/Sources/Shaders/FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);
	
	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		return;

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	glBindFragDataLocation(programID, 0, "daColor");
	glLinkProgram(programID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	if (!checkProgramStatus(programID))
		return;
	glUseProgram(programID);
}

void MeGlWindow::sendDataToOpenGL()
{
	ShapeData shape = ShapeGenerator::makeCube();

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
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Create Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObject_VAO);
	glBindVertexArray(vertexArrayObject_VAO);


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)(sizeof(GLfloat) * 2));

	
	GLuint indices[] = { 0,1,2, 0,3,4 };
	glGenBuffers(1, &indexBufferID);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
}

void MeGlWindow::initializeGL()
{
	//setMouseTracking(true);
	//glEnable(GL_DEPTH_TEST);
	sendDataToOpenGL();
	installShaders();
	paintGL();
}

void MeGlWindow::paintGL()
{
	/*
	mat4 projectionMatrix = glm::perspective(60.0f, ((float)width) / height, 0.1f, 10.0f);
	mat4 fullTransforms[] =
	{
		projectionMatrix * camera.getWorldToViewMatrix() * glm::translate(vec3(-1.0f, 0.0f, -3.0f)) * glm::rotate(36.0f, vec3(1.0f, 0.0f, 0.0f)),
		projectionMatrix * camera.getWorldToViewMatrix() * glm::translate(vec3(1.0f, 0.0f, -3.75f)) * glm::rotate(126.0f, vec3(0.0f, 1.0f, 0.0f))
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(fullTransforms), fullTransforms, GL_DYNAMIC_DRAW);
	*/
}

void MeGlWindow::runGL()
{
	initializeGL();
}

