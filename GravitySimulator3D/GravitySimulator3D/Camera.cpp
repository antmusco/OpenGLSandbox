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
*  void                                                                       *
*                                                                             *
*******************************************************************************
* DESCRIPTION                                                                 *
*  Constructor which sets the position, view direction, and up direction to   *
*  the camera defaults.                                                       *
*                                                                             *
*******************************************************************************/
Camera::Camera() :
	position(DEFAILT_POSITION),
	viewDirection(DEFAULT_VIEW_DRIECTION),
	upDirection(DEFAULT_UP_DIRECTION),
	rotateSpeed(DEFAULT_ROTATE_SPEED),
	maxMovement(DEFAULT_MAX_MOVEMENT)
{
}

/******************************************************************************
*                                                                             *
*                       Camera::getWorldToViewMatrix()                        *
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
	return glm::lookAt(position, position + viewDirection, upDirection);
}

void Camera::updateLookAt(const glm::vec2 &newMousePosition)
{
	/* Calculate the change in mouse position. */
	glm::vec2 mouseDelta = newMousePosition - oldMousePosition;

	/* Get the horizontal rotation axis and normalize. */
	glm::vec3 sideDirection = glm::cross(viewDirection, upDirection);	
	sideDirection /= glm::length(sideDirection);

	/* If the mouse moved greater than MAX_MOVEMENT pixels, don't move */
	if (glm::length(mouseDelta) <= maxMovement) 
	{
			/* Generate the transformation = sideways * vertical rotation. */
			glm::mat4 rotator =
				glm::rotate(mouseDelta.x * rotateSpeed,	-upDirection) *
				glm::rotate(mouseDelta.y * rotateSpeed,	-sideDirection);

			/* Set the new view direction. */
			viewDirection = glm::mat3(rotator) * viewDirection;
	}

	/* Update the mouse position. */
	oldMousePosition = newMousePosition;
}
