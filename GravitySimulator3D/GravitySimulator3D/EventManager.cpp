#include "EventManager.h"
#include "glm\glm.hpp"
#include "GL\glew.h"

/******************************************************************************
*                                                                             *
*                     EventManager::EventManager (Constructor)                *
*                                                                             *
*******************************************************************************
* PARAMETERS                                                                  *
*  @param camera                                                              *
*           The camera which this event manager controls.                     *
*                                                                             *
*******************************************************************************
* RETURNS                                                                     *
*  void                                                                       *
*                                                                             *
*******************************************************************************
* DESCRIPTION                                                                 *
*  Constructor for the EventManager class. This constructor initializes the   *
*  camera used by this event manager.                                         *
*                                                                             *
*******************************************************************************/
EventManager::EventManager(Camera* camera, GLfloat* speed)
{
	this->camera = camera;
	this->speed = speed;
}

/******************************************************************************
*                                                                             *
*                         EventManager::handleSDLEvent()                      *
*                                                                             *
*******************************************************************************
* PARAMETERS                                                                  *
*  @param event                                                               *
*           The SDL event to be handled.                                      *
*                                                                             *
*******************************************************************************
* RETURNS                                                                     *
*  void                                                                       *
*                                                                             *
*******************************************************************************
* DESCRIPTION                                                                 *
*  Primary event handling function for the application. All SDL events are    *
*  sent here, which then routes the events to specific subroutines to handle  *
*  the specific action required.                                              *
*                                                                             *
*******************************************************************************/
void EventManager::handleSDLEvent(SDL_Event* event) 
{
	if (event->type == SDL_MOUSEMOTION)
	{
		camera->updateLookAt({ event->motion.x, event->motion.y });
	}
	else if (event->type == SDL_KEYDOWN)
	{
		handleKeyPress(event->key.keysym.scancode);
	}
}

/******************************************************************************
*                                                                             *
*                         EventManager::handleKeyPress()                      *
*                                                                             *
*******************************************************************************
* PARAMETERS                                                                  *
*  @param key                                                                 *
*           The SDL_Scancode indicating the key which was pressed.            *
*                                                                             *
*******************************************************************************
* RETURNS                                                                     *
*  void                                                                       *
*                                                                             *
*******************************************************************************
* DESCRIPTION                                                                 *
*  Handles a key press event, typically indicating camera motion.             *
*                                                                             *
*******************************************************************************/
void EventManager::handleKeyPress(SDL_Scancode key) 
{	
	glm::vec3 move;
	float scale = 20.0f;

	switch (key) 
	{
	/* Speed Up. */
	case SDL_SCANCODE_T:
		if(*speed < 4.0f)
			*speed += 0.5f;
		break;
	
	/* Slow Down. */
	case SDL_SCANCODE_R:
		if(*speed < 0.0f)
			*speed -= 0.5f;
		break;

	/* Strafe Right. */
	case SDL_SCANCODE_D:
	case SDL_SCANCODE_RIGHT:
		move = glm::cross(*(camera->getViewDirection()), *(camera->getUpDirection()));
		move = move / glm::length(move);
		*( camera->getPosition() ) += move * scale;
		break;

	/* Strafe Left. */
	case SDL_SCANCODE_A:
	case SDL_SCANCODE_LEFT:
		move = glm::cross(*( camera->getUpDirection() ), *( camera->getViewDirection() ));
		move = move / glm::length(move);
		*( camera->getPosition() ) += move * scale;
		break;

	/* Step Forward. */
	case SDL_SCANCODE_W:
	case SDL_SCANCODE_UP:
		move = *( camera->getViewDirection() );
		move /= glm::length(move);
		*( camera->getPosition() ) += move * scale;
		break;

	/* Step Backward. */
	case SDL_SCANCODE_S:
	case SDL_SCANCODE_DOWN:
		move = *(camera->getViewDirection());
		move = -1.0f * ( move / glm::length(move) );
		*( camera->getPosition() ) += move * scale;
		break;

	/* Step Down. */
	case SDL_SCANCODE_Z:
		move = *(camera->getUpDirection());
		move /= glm::length(move);
		*( camera->getPosition() ) -= move * scale;
		break;

	/* Step Up. */
	case SDL_SCANCODE_X:
		move = *( camera->getUpDirection() );
		move /= glm::length(move);
		*( camera->getPosition() ) += move * scale;
		break;
	case  SDL_SCANCODE_ESCAPE:
		exit(0);
	}
}