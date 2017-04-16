#pragma once

//#include <QtOpenGL\qglwidget>
#include <string>
#include <glad/glad.h>

class MeGlWindow
{
	void sendDataToOpenGL();
	bool checkShaderStatus(GLint shaderID);
	bool checkProgramStatus(GLint programID);
	std::string readShaderCode(const char* fileName);
	void installShaders();

protected:

	void initializeGL();
	void paintGL();
	void runGL();
	//void mouseMoveEvent(QMouseEvent*);

public:
	GLint programID;
	GLuint vertexBufferID;
	GLuint vertexArrayObject_VAO;
	GLuint indexArrayBufferID;

	GLint numIndices;

	int width;
	int height;

	MeGlWindow(int w, int h)
	{
		this->runGL();
		this->width = w;
		this->height = h;
	}
		
	~MeGlWindow() = default;
};
