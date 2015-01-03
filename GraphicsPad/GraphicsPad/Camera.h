#pragma once

/******************************************************************************
*                                                                             *
*                              Included Header Files                          *
*                                                                             *
******************************************************************************/
#include <glm\glm.hpp>
#include <SDL\SDL.h>

/******************************************************************************
*                                                                             *
*                           Geometry::Geometry (class)                        *
*                                                                             *
*******************************************************************************
* MEMBERS                                                                     *
*  position                                                                   *
*           The x, y, z position of the camera in 3-D space.                  *
*  viewDirection                                                              *
*           The x, y, z direction that the camera is pointed.                 *
*  upDirection                                                                *
*           The x, y, z direction indicating the top of the camera.           *
*  oldMousePosition                                                           *
*           The x, y position the mouse was last recorded.                    *
*                                                                             *
*******************************************************************************
* DESCRIPTION                                                                 *
*  Class representing the camera observing the 3-D space.                     *
*                                                                             *
*******************************************************************************/
class Camera
{
/* Public members. */
public:
	/* Constructor. */
	Camera();
	/* Getters. */
	glm::vec3* getPosition() { return &position; }
	glm::vec3* getViewDirection() { return &viewDirection; }
	glm::vec3* getUpDirection() { return &upDirection; }
	/* Generate World-To-View Matrix. */
	glm::mat4 getWorldToViewMatrix() const;
	/* Respond to moouse input. */
	void updateLookAt(const glm::vec2 &newMousePosition);
	/* Destructor. */
	~Camera() {}
/* Private members. */
private:
	/* Scaling factor for camera rotation. */
	const float ROTATE_SPEED = 0.005f;
	/* Maximum amount of movement for the camera in one update. */
	const float MAX_MOVEMENT = 50.0f;
	/* Position vector of the camera. */
	glm::vec3 position;
	/* Direction vector the camera is pointed. */
	glm::vec3 viewDirection;
	/* Direction vector indicating the top of the camera. */
	glm::vec3 upDirection;
	/* Last recorded mouse position */
	glm::vec2 oldMousePosition;
};

