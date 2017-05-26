
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
float rotatingAngle = 0.0;
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

	ShapeData plane = ShapeGenerator::makePyramid();
	//ShapeData plane = ShapeGenerator::makePlane(20);
	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, plane.vertexBufferSize(), plane.vertices, GL_DYNAMIC_DRAW);

	// Create Vertex Array Object
	glGenVertexArrays(1, &planeNormalsVertexArrayObjectID);
	glBindVertexArray(planeNormalsVertexArrayObjectID);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (char*)(sizeof(float) * 3));
	GLuint indexArrayBufferID;
	glGenBuffers(1, &indexArrayBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexArrayBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, plane.indexBufferSize(), plane.indices, GL_DYNAMIC_DRAW);
	numIndices = plane.numIndices;
	plane.cleanup();






/*
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
*/

	t_start = (float)glfwGetTime();



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

	//float sine =  std::sin(time * 4.0f) + 1.0f / 2.0f;
	//vec3 dominatingColor = vec3(sine, 0.0f, 0.0f);
	if (rotatingAngle == 360.0)
		rotatingAngle = 0.0;
	rotatingAngle = rotatingAngle + .05f;
	//glUniform3fv(uniColor,1, &dominatingColor[0]);	//model to world
	//world to view
	//view to projection
	//world to projection
	//model to world to view to projection (i.e. full)



	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width, height);

	mat4 fullTransformMatrix;
	mat4 planeModelToWorldMatrix = glm::translate(vec3(0.0f, 0.0f, -15.0f));
	mat4 worldToViewMatrix = camera.getWorldToViewMatrix();
	mat4 viewToProjectionMatrix = glm::perspective(d2rad(60.0f), ((float)width) / height, 0.1f, 100.0f);
	mat4 worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;

	fullTransformMatrix = worldToProjectionMatrix * planeModelToWorldMatrix;

	glBindVertexArray(planeNormalsVertexArrayObjectID);
	fullTransformationUniformLocation = glGetUniformLocation(programID, "fullTransformMatrix");
	glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);

	/*
	glDrawElementsInstanced
	(
	GL_TRIANGLES,
	numIndices,
	GL_UNSIGNED_SHORT,  // The type of the indices (see ShapeData.hpp)
	0,				    // offset into the element array buffer (usually zero or NULL) - its a very old function.
	2				// number of instances
	);
	*/
}

void MeGlWindow::runGL()
{
	initializeGL();
}

