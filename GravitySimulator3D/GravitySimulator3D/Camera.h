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
*                                 Default Values                              *
*                                                                             *
******************************************************************************/
#define  DEFAULT_ROTATE_SPEED                    0.005f
#define  DEFAULT_MAX_MOVEMENT                     50.0f
#define  DEFAILT_POSITION        +1500.0f, +0.0f, +0.0f
#define  DEFAULT_VIEW_DRIECTION  -   1.0f, +0.0f, +0.0f
#define  DEFAULT_UP_DIRECTION    +   0.0f, +1.0f, +0.0f

/******************************************************************************
*                                                                             *
*                              Camera::Camera  (class)                        *
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

	/* Generate World-To-View Matrix. */
	glm::mat4      getWorldToViewMatrix()   const;
	/* Respond to moouse input. */
	void           updateLookAt(const glm::vec2 &newMousePosition);

	/* Getters. */
	glm::vec3*     getPosition()            {  return &position;       }
	glm::vec3*     getViewDirection()       {  return &viewDirection;  }
	glm::vec3*     getUpDirection()         {  return &upDirection;    }

    void           setPosition(glm::vec3 p) {  position = p;           }
	void           setViewDirection(glm::vec3 d)
	                                        {  viewDirection = d;      }

/* Private members. */
private:
	/* Scaling factor for camera rotation. */
	float          rotateSpeed;
	/* Maximum amount of movement for the camera in one update. */
	float          maxMovement;
	/* Position vector of the camera. */
	glm::vec3      position;
	/* Direction vector the camera is pointed. */
	glm::vec3      viewDirection;
	/* Direction vector indicating the top of the camera. */
	glm::vec3      upDirection;
	/* Last recorded mouse position */
	glm::vec2      oldMousePosition;
};
