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

	MeGlWindow meglw = MeGlWindow(mWidth, mHeight);

    // Rendering Loop
		while (glfwWindowShouldClose(mWindow) == false) {
			if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(mWindow, true);
			
			// Flip Buffers and Draw
			glfwSwapBuffers(mWindow);
			glfwPollEvents();
			
			meglw.paintGL();
			
    }   

	glDeleteProgram(meglw.programID);
	glDeleteBuffers(1, &meglw.vertexBufferID);
	glDeleteBuffers(1, &meglw.indexArrayBufferID);
	glDeleteVertexArrays(1, &meglw.vertexArrayObject_VAO);
	glfwTerminate();

    return EXIT_SUCCESS;
}
