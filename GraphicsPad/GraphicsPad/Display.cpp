/******************************************************************************
*                                                                             *
*                              Included Header Files                          *
*                                                                             *
******************************************************************************/
#include <gl\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <iostream>
#include "Display.h"

/******************************************************************************
*                                                                             *
*                          Display::Display (Constructor)                     *
*                                                                             *
*******************************************************************************
* PARAMETERS                                                                  *
*  @param title                                                               *
*           The title of the window to be displayed in the toolbar.           *
*  @param width                                                               *
*           The width of the window in pixels.                                *
*  @param height                                                              *
*           The height of the window in pixels.                               *
*                                                                             *
*******************************************************************************
* RETURNS                                                                     *
*  void                                                                       *
*                                                                             *
*******************************************************************************
* DESCRIPTION                                                                 *
*  Constructor for the Display class. This constructor creates the window and *
*  the GL context using SDL (Simple Directmedia Layer) and initializes GLEW.  *
*  GLEW (GL Extension Wrangler Library) "binds" all of OpenGL's functions     *
*  to the hardware-specific implementation (OpenGL acts as an Adapter Class)  *
*                                                                             *
*******************************************************************************/
Display::Display(std::string title, GLushort width, GLushort height)
{
	/* Create the SDL window. */
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, width, height, 
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	/* Create the SDL GL context. */
	context = SDL_GL_CreateContext(window);

	/* Set the clear color to default. */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	/* Initialize GLEW (binds all of OpenGL's functions to the hardware). */
	GLenum status = glewInit();

	/* Validate GLEW initialized correctly. */
	if (status != GLEW_OK)
	{
		/* If the functions were not bound, output error. */
		std::cerr << "Glew failed to initialize!" << std::endl;
		std::cerr << "Status: " << glewGetErrorString(status) << std::endl;
	}

	/* Enable depth buffering. */
	glEnable(GL_DEPTH_TEST);

	/* Blit the background image onto the screen. */
	screenSurface = SDL_GetWindowSurface(window);
	backgroundSurface = SDL_LoadBMP("res/background.bmp");
	SDL_BlitSurface(backgroundSurface, NULL, screenSurface, NULL);
	SDL_UpdateWindowSurface(window);

	/* Show the version of GLEW currently being used. */
	fprintf(stdout, "Stats: Using GLEW %s\n", glewGetString(GLEW_VERSION));
}

/******************************************************************************
*                                                                             *
*                             Display::repaint                                *
*                                                                             *
*******************************************************************************
* PARAMETERS                                                                  *
*  @param red                                                                 *
*           The amount of red to be painted on the color buffer (0.0 - 1.0)   *
*  @param green                                                               *
*           The amount of green to be painted on the color buffer (0.0 - 1.0) *
*  @param blue                                                                *
*           The amount of blue to be painted on the color buffer (0.0 - 1.0)  *
*  @param alpha                                                               *
*           The opaqueness of the color buffer.                               *
*                                                                             *
*******************************************************************************
* RETURNS                                                                     *
*  void                                                                       *
*                                                                             *
*******************************************************************************
* DESCRIPTION                                                                 *
*  Function which clears the window by changing all of the pixels to the      *
*  specified color and opacity.                                               *
*                                                                             *
*******************************************************************************/
void Display::repaint(GLuint programID, GLuint numIndicies, 
	glm::mat4 *transformation)
{
	
	/* Tell OpenGL to clear the color buffer. */
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);	

	SDL_BlitSurface(backgroundSurface, NULL, screenSurface, NULL);
	SDL_UpdateWindowSurface(window);

	/* Get the window dimensions and update the viewport. */
	GLint w, h;
	SDL_GetWindowSize(window, &w, &h);
	glViewport(w*0.2, h*0.2, w*0.6, h*0.6);

	/* Create the transformation matrix and projection matrix. */
	glm::mat4 fullTransformMatrix = glm::perspective(30.0f, ( (float)w / h ),
		0.1f, 1000.0f) * (*transformation);

	fullTransformMatrix *= camera.getWorldToViewMatrix();

	/* Get the location of the fullTransformMatrix uniform variable. */
	GLint fullTransformMatrixUniformLocation = glGetUniformLocation(programID, 
		"fullTransformMatrix");

	/* Sphere 1: */
	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1, GL_FALSE,
		&fullTransformMatrix[0][0]);

	/* Draw the elements to the window. */
	glDrawElements(GL_TRIANGLES, numIndicies, GL_UNSIGNED_SHORT, 0);

	fullTransformMatrix = glm::translate(fullTransformMatrix, glm::vec3(-5.0f, 0.0f, 0.0f));

	/* Sphere 2: */
	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1, GL_FALSE,
		&fullTransformMatrix[0][0]);

	/* Draw the elements to the window. */
	glDrawElements(GL_TRIANGLES, numIndicies, GL_UNSIGNED_SHORT, 0);

	/* Swap the double buffer. */
	SDL_GL_SwapWindow(window);
}

/******************************************************************************
*                                                                             *
*                           Display::~Display (Destructor)                    *
*                                                                             *
*******************************************************************************
* PARAMETERS                                                                  *
*  void                                                                       *
*                                                                             *
*******************************************************************************
* RETURNS                                                                     *
*  void                                                                       *
*                                                                             *
*******************************************************************************
* DESCRIPTION                                                                 *
*  Constructor for the Display class. This constructor creates the window and *
*  the GL context using SDL (Simple Directmedia Layer) and initializes GLEW.  *
*  GLEW (GL Extension Wrangler Library) "binds" all of OpenGL's functions     *
*  to the hardware-specific implementation (OpenGL acts as an Adapter Class)  *
*                                                                             *
*******************************************************************************/
Display::~Display()
{
	/* Deallocate space for the surface. */
	SDL_FreeSurface(backgroundSurface);

	/* Delete the GL context. */
	SDL_GL_DeleteContext(context);

	/* Destroy the window. */
	SDL_DestroyWindow(window);

}
