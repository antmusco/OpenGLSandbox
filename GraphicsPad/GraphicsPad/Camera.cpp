/******************************************************************************
*                                                                             *
*                              Included Header Files                          *
*                                                                             *
******************************************************************************/
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

/******************************************************************************
*                                                                             *
*                          Camera::Camera (Constructor)                       *
*                                                                             *
*******************************************************************************
* PARAMETERS                                                                  *
*  void                                                                       *
*                                                                             *
*******************************************************************************
* RETURNS                                                                     *
*  A new Camera object with initialized variables.                            *
*                                                                             *
*******************************************************************************
* DESCRIPTION                                                                 *
*  Constructor which sets the position, view direction, and up direction to   *
*  the camera defaults.                                                       *
*                                                                             *
*******************************************************************************/
Camera::Camera() :
	/* Center at origin. */
	position(0.0f, 2.0f, 0.0f),
	/* Look along -z axis. */
	viewDirection(0.0f, 2.0f, -1.0f),
	/* Top of camera is along +y axis. */
	upDirection(0.0f, -1.0f, 0.0f)
{
}

/******************************************************************************
*                                                                             *
*                       Camera::getWorldToViewMatrix                          *
*                                                                             *
*******************************************************************************
* PARAMETERS                                                                  *
*  void                                                                       *
*                                                                             *
*******************************************************************************
* RETURNS                                                                     *
*  The mat4 representing the World-To-View projection matrix.                 *
*                                                                             *
*******************************************************************************
* DESCRIPTION                                                                 *
*  Function which returns a matrix representing the projection of the 3-D     *
*  world onto the lens of the camera.                                         *
*                                                                             *
*******************************************************************************/
glm::mat4 Camera::getWorldToViewMatrix() const
{
	/* Look in the current direction. */
	return glm::lookAt(position, viewDirection, upDirection);
}

void Camera::updateLookAt(const glm::vec2 &newMousePosition)
{
	/* Calculate the change in mouse position. */
	glm::vec2 mouseDelta = newMousePosition - oldMousePosition;

	/* If the mouse moved greater than MAX_MOVEMENT pixels, don't move */
	if (glm::length(mouseDelta) <= MAX_MOVEMENT)
	{
		/* Get the horizontal rotation axis. */
		glm::vec3 sideDirection = glm::cross(viewDirection, upDirection);

		/* Generate the transformation = sideways * vertical rotation. */
		glm::mat4 rotator = glm::rotate(mouseDelta.x * ROTATE_SPEED, 
			upDirection) * glm::rotate(mouseDelta.y * ROTATE_SPEED,
			sideDirection);

		/* Set the new view direction. */
		viewDirection = glm::mat3(rotator) * viewDirection;
	}
	/* Update the mouse position. */
	oldMousePosition = newMousePosition;
}