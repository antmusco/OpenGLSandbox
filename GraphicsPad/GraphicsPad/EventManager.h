#pragma once
#include  "Camera.h"
#include  "SDL\SDL.h"
#include  <GL\glew.h>

/******************************************************************************
 *																			  *
 *                              EventManager Class                            *
 *																			  *
 ******************************************************************************
*  aspectRatio                                                                *
*          Ratio describing the width/height for the viewport.                *
*                                                                             *
*******************************************************************************
* DESCRIPTION                                                                 *
*  Class which acts as an event manager for the application. All input and    *
*  output will be handled by an event manager object.                         *
*                                                                             *
*******************************************************************************/
class EventManager
{
/* Public Members. */
public:

	/* Constructor. */
	EventManager(Camera* camera, GLfloat* speed);

	/* Handle an SDL Event. */
	void           handleSDLEvent(SDL_Event* event);
	/* Handle a key press event. */
	void           handleKeyPress(SDL_Scancode key);

	/* Getters. */
	Camera*        getCamera()                   {  return camera;        }

	/* Setters. */
	void           setCamera(Camera* c)          {  camera = c;           }

	/* Destructor. */
	~EventManager()                              {                        }

/* Private Members. */
private:
	/* Camera for the application. */
	Camera*        camera;
	GLfloat*        speed;
};

