#pragma once

#include <glm\glm.hpp>

class Camera
{
public:
	Camera();
	glm::mat4 getWorldToViewMatrix() const;
	glm::vec3* getPosition() { return &position; }
	glm::vec3* getViewDirection() { return &viewDirection; }
	glm::vec3* getUpDirection() { return &upDirection; }

	~Camera();
private:
	glm::vec3 position;
	glm::vec3 viewDirection;
	glm::vec3 upDirection;
};

