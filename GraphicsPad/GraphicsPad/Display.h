#pragma once

/******************************************************************************
*                                                                             *
*                              Included Header Files                          *
*                                                                             *
******************************************************************************/
#include <SDL\SDL.h>
#include <GL\GL.h>
#include <string>

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
	void repaint(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha,
		GLuint programID, GLuint numIndices);
/* Private Members.*/
private:
	/* Pointer to the SDL Window. */
	SDL_Window		*window;
	/* Pointer to the GL Context. */
	SDL_GLContext	context;
};
