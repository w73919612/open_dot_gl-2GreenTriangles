#pragma once

//#include <QtOpenGL\qglwidget>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.hpp"


class MeGlWindow
{
	void sendDataToOpenGL();
	bool checkShaderStatus(GLint shaderID);
	bool checkProgramStatus(GLint programID);
	std::string readShaderCode(const char* fileName);
	void installShaders();

protected:

	const GLuint NUM_VERTICES_PER_TRI = 3;
	const GLuint NUM_FLOATS_PER_VERTICE = 6;
	const GLuint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(GLfloat);

	void initializeGL();
	void runGL();
public:
	GLint programID;
	GLuint vertexBufferID;
	GLuint indexArrayBufferID;
	GLFWwindow *mWindow;
	GLint numIndices;
	Camera camera;
	GLuint planeNormalsVertexArrayObjectID;

	int width;
	int height;
	double mouse_xpos=0, mouse_ypos=0;
	GLuint fullTransformationUniformLocation;

	void paintGL();

	MeGlWindow() = default;

	MeGlWindow(int w, int h, GLFWwindow *mWin)
	{
		this->runGL();
		this->width = w;
		this->height = h;
		mWindow = mWin;
	}
		
	~MeGlWindow() = default;
};
