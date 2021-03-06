#pragma once
#include <glm\glm.hpp>

class Camera
{
	glm::vec3 position;
	glm::vec3 viewDirection;
	const glm::vec3 UP;
	glm::vec2 oldMousePosition;
	glm::vec3 strafeDirection;
public:
	Camera();
	void mouseUpdate(const glm::vec2& newMousePosition);
	glm::mat4 getWorldToViewMatrix() const;
	void moveForward();
	void moveBackward();
	void moveUp();
	void moveDown();
	void strafeLeft();
	void strafeRight();
	const float MOVEMENT_SPEED = 0.1f;
};

