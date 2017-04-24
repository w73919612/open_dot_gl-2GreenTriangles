
#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <fstream>
#include <chrono>
#include <GLFW/glfw3.h>
#include <string>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <glad/glad.h>
#include "Camera.hpp"
#include "MeGlWindow.hpp"
#include "Primitives\ShapeData.hpp"
#include "Primitives\ShapeGenerator.hpp"
#define d2rad(x) x*(3.141592f/180.0f)


//mathopenref.com/coordpolygonarea.html

using namespace std;
using glm::vec3;
using glm::mat4;
bool THREE_D = true;
GLint uniColor;
float t_start;
const GLfloat lineVertices[] =
{
	-10, -10, 0,
	-10, 10, 0,
	10, 10, 0,
	10, -10, 0
};
/* *************************************************************************************************/

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
	temp = readShaderCode("./Glitter/Sources/Shaders/VertexShaderCode.glsl");
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
		

		ShapeData shape = ShapeGenerator::makeArrow();
		GLuint vertexBufferID;
		glGenBuffers(1, &vertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, shape.vertexBufferSize(), shape.vertices, GL_STATIC_DRAW);

		// Create Vertex Array Object
		glGenVertexArrays(1, &vertexArrayObject_VAO);
		glBindVertexArray(vertexArrayObject_VAO);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (char*)(sizeof(float) * 3));
		GLuint indexArrayBufferID;
		glGenBuffers(1, &indexArrayBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexArrayBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.indexBufferSize(), shape.indices, GL_STATIC_DRAW);
		numIndices = shape.numIndices;
		//uniColor = glGetUniformLocation(programID, "triangleColor");
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


		t_start = (float)glfwGetTime();


		

	}
	else
	{

		ShapeData tri = ShapeGenerator::makeTriangle();
		numIndices = tri.numIndices;
		
		glGenBuffers(1, &vertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

		glBufferData(GL_ARRAY_BUFFER, tri.vertexBufferSize(), tri.vertices, GL_STATIC_DRAW);

		// Create Vertex Array Object
		glGenVertexArrays(1, &vertexArrayObject_VAO);
		glBindVertexArray(vertexArrayObject_VAO);

		//GLint posAttrib = glGetAttribLocation(programID, "position");  // This grabs the "location" from the shader.  We may have hardcoded it so we

		GLint posAttrib = 0;
		glEnableVertexAttribArray(posAttrib);							  // actually already know what it is. But this is how to get it if you don't.
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, 0);

		//GLint colAttrib = glGetAttribLocation(programID, "vertexColor");// This grabs the "location" from the shader.  We may have hardcoded it so we

		GLint colAttrib = 1;
		glEnableVertexAttribArray(colAttrib);							  // actually already know what it is. But this is how to get it if you don't.
		glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(GLfloat) * 3)); // last arg is how many to skip to get to this.
																												 // Here there were 3 position vertices to skip.

		glGenBuffers(1, &indexArrayBufferID);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexArrayBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, tri.indexBufferSize(), tri.indices, GL_STATIC_DRAW);

	}

}

void MeGlWindow::initializeGL()
{
	//setMouseTracking(true);
	glEnable(GL_DEPTH_TEST);
	sendDataToOpenGL();
	installShaders();
}

void MeGlWindow::paintGL()
{

	//glfwGetCursorPos(mWindow, &mouse_xpos, &mouse_ypos);
	//camera.mouseUpdate(glm::vec2(mouse_xpos, mouse_ypos));

	// GET TIME
	auto t_now = (float)glfwGetTime();
	float time = ((float)t_now - t_start);

	float sine = std::sin(time * 4.0f) + 1.0f / 2.0f;
	//vec3 dominatingColor = vec3(sine, 0.0f, 0.0f);

	
	//glUniform3fv(uniColor,1, &dominatingColor[0]);
	mat4 projectionMatrix = glm::perspective(45.0f, ((float)width) / height, 0.1f, 10.0f);

	mat4 fullTransforms[] =
	{
		//cube 1
		projectionMatrix * camera.getWorldToViewMatrix() * glm::translate(vec3(-1.0f, 0.0f, -3.00f)) * 
		glm::rotate(d2rad(36.0f), vec3(1.0f, .0f, 0.0f)),
		projectionMatrix * camera.getWorldToViewMatrix() * glm::translate(vec3(1.0f, 0.0f, -3.75f)) * 
		glm::rotate(d2rad(126.0f), vec3(0.0f, 1.0f, 0.0f))
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(fullTransforms), fullTransforms, GL_DYNAMIC_DRAW);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width, height);


	glDrawElementsInstanced
	(
	GL_TRIANGLES,
	numIndices,
	GL_UNSIGNED_SHORT,  // The type of the indices (see ShapeData.hpp)
	0,				    // offset into the element array buffer (usually zero or NULL) - its a very old function.
	2				// number of instances
	);

}

void MeGlWindow::runGL()
{
	initializeGL();
}

