#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
Camera::Camera() :
position(0.0f, 0.0f, 0.0f),
viewDirection(0.0f, 0.0f, -1.0f),
upDirection(0.0f, 1.0f, 0.0f)
{
}


Camera::~Camera()
{
}


glm::mat4 Camera::getWorldToViewMatrix() const
{
	return glm::lookAt(position, viewDirection, upDirection);
}