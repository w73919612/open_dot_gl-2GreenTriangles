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

static MeGlWindow *meglw;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void cursorPostionCallback(GLFWwindow *window, double xPos, double yPos);
//void cursorEnterCallback(GLFWwindow *window, int entered);

int main(int argc, char * argv[]) {

    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);
	//glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return EXIT_FAILURE;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

	meglw = new MeGlWindow(mWidth, mHeight, mWindow);
	glfwSetKeyCallback(mWindow, key_callback);


	//glfwSetCursorEnterCallback(mWindow, cursorEnterCallback);
	glfwSetCursorPosCallback(mWindow, cursorPostionCallback);
	//glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetInputMode(mWindow, GLFW_STICKY_KEYS, 1);

    // Rendering Loop
		while (glfwWindowShouldClose(mWindow) == false) {
			if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(mWindow, true);
			
			// Flip Buffers and Draw
			glfwSwapBuffers(mWindow);
			glfwPollEvents();
			
			meglw->paintGL();
			
    }   

	glDeleteProgram(meglw->programID);
	glDeleteBuffers(1, &meglw->vertexBufferID);
	glDeleteBuffers(1, &meglw->indexArrayBufferID);
	glfwTerminate();

    return EXIT_SUCCESS;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_REPEAT)
	{
		switch (key)
		{
		case GLFW_KEY_W:
			meglw->camera.moveForward();
			break;
		case GLFW_KEY_E:
			meglw->camera.moveBackward();
			break;

		case GLFW_KEY_S:
			meglw->camera.strafeLeft();
			break;
		case GLFW_KEY_D:
			meglw->camera.strafeRight();
			break;

		case GLFW_KEY_A:
			meglw->camera.moveUp();
			break;
		case GLFW_KEY_F:
			meglw->camera.moveDown();
			break;
		}
	}
	/*
	if (key == GLFW_KEY_W)
	{
		switch (action)
		{
			case GLFW_REPEAT:
			meglw->camera.moveForward();
			break;
		}
	}
	*/
}
/*
void cursorEnterCallback(GLFWwindow *window, int entered)
{
	cout << "Entered Callback";
}
*/
static void cursorPostionCallback(GLFWwindow *window, double xPos, double yPos)
{

	meglw->camera.mouseUpdate(glm::vec2(xPos, yPos));
}