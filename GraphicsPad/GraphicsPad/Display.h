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
#define  DEFAULT_FOV              30.0f
/* Near clipping plane parameter. */
#define  DEFAULT_NEAR_PLANE       0.1f
/* Far clipping plane parameter. */
#define  DEFAULT_FAR_PLANE        150000.0f

/* Default vertex and fragment shader source files. */
#define  DEFAULT_VERTEX_SHADER    "res/shader.vs"
#define  DEFAULT_FRAGMENT_SHADER  "res/shader.fs"

/******************************************************************************
 *																			  *
 *	                             Display Class                                *
 *																			  *
 ******************************************************************************
*  aspectRatio                                                                *
*          Ratio describing the width/height for the viewport.                *
*  window                                                                     *
*          Pointer to the SDL_Window instance for this display.               *
*  context                                                                    *
*          Pointer to the OpenGL context created for this window.             *
*          drawn.                                                             *
*  camera                                                                     *
*          Camera instance whose perspective this display shows.              *
*  modelToProjectionMatrix                                                    *
*          4-D matrix representing the total transformation to the display.   *
*  viewToProjectionMatrix                                                     *
*          4-D matrix representing the transformation from the view to the    *
*          projection (camera view).                                          *
*  modelToProjectionUniformLocation                                           *
*          ID  of the location for the modelToProjectionMatrix in the shader  *
*          program.                                                           *
*                                                                             *
*******************************************************************************
* DESCRIPTION                                                                 *
*  Class representing the window in which the OpenGL context may render.      *
*                                                                             *
*******************************************************************************/
class Display
{
/* Public Members. */
public:

	/* Constructor. */
	Display(std::string title, GLushort width, GLushort height);

	/* Update the size of the viewport on window resize. */
	void updateViewport();
	/* Repaint the graphics. */
	void repaint(std::vector<Mesh*> meshes,
                 std::vector<glm::mat4*> modelToWorldMatrices);
	
	/* Getters. */
	Camera*        getCamera()               {  return &camera;            }

	/* Setters. */     
	void           setShader(Shader shader);
	void           setClearColor(GLclampf r, 
                                 GLclampf b,
                                 GLclampf g, 
                                 GLclampf a) {  glClearColor(r, b, g, a);  } 

	/* ~Display */
	~Display();

/* Private Members.*/
private:
	/* Current aspect ratio of the window. */
	GLfloat        aspectRatio;
	/* Pointer to the SDL Window. */
	SDL_Window*    window;
	/* Pointer to the GL Context. */
	SDL_GLContext  context;
	/* Pointer to the window surface. */
	SDL_Surface*   screenSurface;
	/* Pointer to the background surface. */
	SDL_Surface*   backgroundSurface;
	/* Camera for looking at the world. */
	Camera         camera;
	/* Model to Projection (complete) matrix. */
	glm::mat4      modelToProjectionMatrix;
	/* View to Projection matrix. */
	glm::mat4      viewToProjectionMatrix;
	/* Uniform location for the full transformation. */
	GLuint         modelToProjectionUniformLocation;
	/* Uniform location for the texture. */
	GLuint         textureUniformLocation;

};
