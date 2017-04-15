
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

bool THREE_D = false;
const GLuint NUM_VERTICES_PER_TRI = 3;
const GLuint NUM_FLOATS_PER_VERTICE = 6;
const GLuint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(GLfloat);
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
	string temp;
	const GLchar* adapter[1];
	if (THREE_D)
		temp = readShaderCode("./Glitter/Sources/Shaders/VertexShaderCode.glsl");
	else
		temp = readShaderCode("./Glitter/Sources/Shaders/VertexShaderCode1D.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);

	if (THREE_D)
		temp = readShaderCode("./Glitter/Sources/Shaders/FragmentShaderCode.glsl");
	else
		temp = readShaderCode("./Glitter/Sources/Shaders/FragmentShaderCode1D.glsl");

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

	//glBindFragDataLocation(programID, 0, "daColor");
	glLinkProgram(programID);

	if (!checkProgramStatus(programID))
		return;

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	if (!checkProgramStatus(programID))
		return;
	glUseProgram(programID);
}

void MeGlWindow::sendDataToOpenGL()
{
	if (THREE_D)
	{
		ShapeData shape = ShapeGenerator::makeCube();
		GLuint vertexBufferID;
		glGenBuffers(1, &vertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, shape.vertexBufferSize(), shape.vertices, GL_STATIC_DRAW);

		// Create Vertex Array Object
		glGenVertexArrays(1, &vertexArrayObject_VAO);
		glBindVertexArray(vertexArrayObject_VAO);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (char*)(sizeof(float) * 3));

		GLuint indexArrayBufferID;
		glGenBuffers(1, &indexArrayBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexArrayBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.indexBufferSize(), shape.indices, GL_STATIC_DRAW);
		numIndices = shape.numIndices;
		shape.cleanup();

		GLuint transformationMatrixBufferID;
		glGenBuffers(1, &transformationMatrixBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, transformationMatrixBufferID);

		glBufferData(GL_ARRAY_BUFFER, sizeof(mat4) * 2, 0, GL_DYNAMIC_DRAW);


		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(float) * 0));
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(float) * 4));
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(float) * 8));
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(float) * 12));
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		glEnableVertexAttribArray(5);
		glVertexAttribDivisor(2, 1);
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
	}
	else
	{
		GLfloat vertices[] =
		{
		+0.0f, +0.0f,
		+1.0f, +0.0f, +0.0f,
		+1.0f, +1.0f,
		+0.0f, +1.0f, +0.0f,
		-1.0f, +1.0f,
		+0.0f, +0.0f, +1.0f,
		-1.0f, -1.0f,
		+0.0f, +1.0f, +0.0f,
		+1.0f, -1.0f,
		+0.0f, +0.0f, +1.0f,
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

}

void MeGlWindow::initializeGL()
{
	//setMouseTracking(true);
	if (THREE_D)
		glEnable(GL_DEPTH_TEST);
	sendDataToOpenGL();
	installShaders();
	paintGL();
}

void MeGlWindow::paintGL()
{
	if (THREE_D)
	{
		mat4 projectionMatrix = glm::perspective(60.0f, ((float)width) / height, 0.1f, 10.0f);
		mat4 fullTransforms[] =
		{
			projectionMatrix * camera.getWorldToViewMatrix() * glm::translate(vec3(-1.0f, 0.0f, -3.0f)) * glm::rotate(36.0f, vec3(1.0f, 0.0f, 0.0f)),
			projectionMatrix * camera.getWorldToViewMatrix() * glm::translate(vec3(1.0f, 0.0f, -3.75f)) * glm::rotate(126.0f, vec3(0.0f, 1.0f, 0.0f))
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof(fullTransforms), fullTransforms, GL_DYNAMIC_DRAW);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	}
}

void MeGlWindow::runGL()
{
	initializeGL();
}

