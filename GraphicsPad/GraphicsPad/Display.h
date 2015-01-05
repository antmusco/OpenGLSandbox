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
#include "Shader.h"

/* Field of View parameter. */
#define  FOV   30.0f
/* Near clipping plane parameter. */
#define  NEAR   0.1f
/* Far clipping plane parameter. */
#define  FAR   50.0f

/* Default vertex and fragment shader source files. */
#define DEFAULT_VERTEX_SHADER "shader.vs"
#define DEFAULT_FRAGMENT_SHADER "shader.fs"

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
	void repaint(std::vector<Mesh*> meshes,
                 std::vector<glm::mat4*> modelToWorldMatrices);
	void setClearColor(GLclampf red, GLclampf blue, GLclampf green, 
		GLclampf alpha)
	{
		glClearColor(red, blue, green, alpha);
	}
	void setShader(Shader shader);
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
	/* Uniform location for the full transformation. */
	GLuint			fullTransformUniformLocation;
	/* View to Projection matrix. */
	glm::mat4       viewToProjectionMatrix;
};
