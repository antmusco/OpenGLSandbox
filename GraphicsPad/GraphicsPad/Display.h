#pragma once

/******************************************************************************
*                                                                             *
*                              Included Header Files                          *
*                                                                             *
******************************************************************************/
#include <SDL\SDL.h>
#include <GL\GL.h>
#include <glm\glm.hpp>
#include <string>
#include "Camera.h"

/******************************************************************************
 *																			  *
 *								Display Class								  *
 *																			  *
 *****************************************************************************/
class Display
{
/* Public Members. */
public:
	/* Display */
	Display(std::string title, GLushort width, GLushort height);
	/* ~Display */
	~Display();
	/* repaint */
	void repaint(GLuint programID, GLuint numIndices, glm::mat4 *transformation);
	void setClearColor(GLclampf red, GLclampf blue, GLclampf green, 
		GLclampf alpha)
	{
		glClearColor(red, blue, green, alpha);
	}
	Camera* getCamera() { return &camera; }
/* Private Members.*/
private:
	/* Pointer to the SDL Window. */
	SDL_Window*		window;
	/* Pointer to the GL Context. */
	SDL_GLContext	context;
	/* Pointer to the window surface. */
	SDL_Surface*	screenSurface;
	/* Pointer to the background surface. */
	SDL_Surface*	backgroundSurface;
	/* Camera for looking at the world. */
	Camera			camera;
};
