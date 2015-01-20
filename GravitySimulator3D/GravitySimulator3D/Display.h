#pragma once

/******************************************************************************
 *                                                                            *
 *                              Included Header Files                         *
 *                                                                            *
 ******************************************************************************/
#include <SDL\SDL.h>
#include <GL\GL.h>
#include <glm\glm.hpp>
#include <string>
#include <vector>
#include "Camera.h"
#include "Geometry.h"
#include "Shader.h"

/******************************************************************************
 *                                                                            *
 *                        Defined Constants and Macros                        *
 *                                                                            *
 ******************************************************************************/

/* Field of View parameter. */
#define  DEFAULT_FOV              (60.0 * M_PI / 180.0)
/* Near clipping plane parameter. */
#define  DEFAULT_NEAR_PLANE       1.0f
/* Far clipping plane parameter. */
#define  DEFAULT_FAR_PLANE        1500000.0f
/* Default vertex and fragment shader source files. */
#define  DEFAULT_VERTEX_SHADER    "res/shaders/shader.vs"
#define  DEFAULT_FRAGMENT_SHADER  "res/shaders/shader.fs"

/******************************************************************************
 *																			  *
 *	                           Dimension Enum                                 *
 *																			  *
 ******************************************************************************
 *  WIDTH                                                                     *
 *       Represents the width of an object.                                   *
 *  HEIGHT                                                                    *
 *       Represents the height of an object.                                  *
 *  DEPTH                                                                     *
 *       Represents the depht of an object.                                   *
 *                                                                            *
 ******************************************************************************
 * DESCRIPTION                                                                *
 *  Enumeration specifying valid dimensions for function calls.               *
 *                                                                            *
 ******************************************************************************/
enum class Dimension
{
	WIDTH,
	HEIGHT,
	DEPTH,
};

/******************************************************************************
 *																			  *
 *	                             Display Class                                *
 *																			  *
 ******************************************************************************
 *  aspectRatio                                                               *
 *          Ratio describing the width/height for the viewport.               *
 *  window                                                                    *
 *          Pointer to the SDL_Window instance for this display.              *
 *  context                                                                   *
 *          Pointer to the OpenGL context created for this window.            *
 *          drawn.                                                            *
 *  camera                                                                    *
 *          Camera instance whose perspective this display shows.             *
 *  modelToProjectionMatrix                                                   *
 *          4-D matrix representing the total transformation to the display.  *
 *  viewToProjectionMatrix                                                    *
 *          4-D matrix representing the transformation from the view to the   *
 *          projection (camera view).                                         *
 *  modelToProjectionUniformLocation                                          *
 *          ID  of the location for the modelToProjectionMatrix in the shader *
 *          program.                                                          *
 *  textureUniformLocation                                                    *
 *          ID  of the location for the texture sampler in the shader program *
 *                                                                            *
 ******************************************************************************
 * DESCRIPTION                                                                *
 *  Class representing the window in which the OpenGL context may render.     *
 *                                                                            *
 ******************************************************************************/
class Display
{
/* Public Members. */
public:

	/* Constructor. */
	         Display(std::string title, 
	                 GLushort    width, 
	                 GLushort    height);

	/* Calculate the width and height of the screen dimensions. */
	GLushort getScreenDimension(Dimension d);

	/* Update the size of the viewport on window resize. */
	void     updateViewport();

	/* Maximnize the display on the current screen. */
	void     maximize();

	/* Repaint the graphics. */
	void     repaint(std::vector<Mesh*>      meshes,
                     std::vector<glm::mat4*> modelToWorldMatrices);
	
	/* Getters. */
	Camera*  getCamera()               {  return &camera;            }

	/* Setters. */     
	void    setShader(Shader shader);
	void    setClearColor(GLclampf r, 
                          GLclampf b,
                          GLclampf g, 
                          GLclampf a) {  glClearColor(r, b, g, a);  } 

	/* Destructor. */
	               ~Display();

/* Private Members.*/
private:

	/* Current aspect ratio of the window. */
	GLfloat        aspectRatio;
	/* Pointer to the SDL Window. */
	SDL_Window*    window;
	/* Pointer to the GL Context. */
	SDL_GLContext  context;
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
	/* Uniform location for the light source. */
	GLuint         lightSourceUniformLocation;
	/* Uniform location for the ambient light. */
	GLuint         ambientLightUniformLocation;
	/* Uniform location for the model to world transformation.*/
	GLuint         modelToWorldUniformLocation;

};
