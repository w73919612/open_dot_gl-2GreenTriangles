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
	GLint programID;

	void initializeGL();
	void paintGL();
	void runGL();
	//void mouseMoveEvent(QMouseEvent*);

public:
	const int width = 1280;
	const int height = 800;

	MeGlWindow()
	{
		this->runGL();
	}
		
	~MeGlWindow() = default;
};
