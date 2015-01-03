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
#include <vector>
#include "Camera.h"
#include "Geometry.h"

#define  FOV   30.0f
#define  NEAR   0.1f
#define  FAR   10.0f

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
	void updateViewport();
	/* repaint */
	void repaint(GLuint programID, std::vector<Mesh*> meshes,
                 std::vector<glm::mat4*> modelToWorldMatrices);
	void setClearColor(GLclampf red, GLclampf blue, GLclampf green, 
		GLclampf alpha)
	{
		glClearColor(red, blue, green, alpha);
	}
	Camera* getCamera() { return &camera; }
/* Private Members.*/
private:
	/* Current aspect ratio of the window. */
	GLfloat         aspectRatio;
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
	/* View to Projection matrix. */
	glm::mat4       viewToProjectionMatrix;
};
