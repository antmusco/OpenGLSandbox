/******************************************************************************
*                                                                             *
*                              Included Header Files                          *
*                                                                             *
******************************************************************************/
#include <gl\glew.h>
#include <glm\glm.hpp>
#include<glm\gtc\matrix_transform.hpp>
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
void Display::repaint(GLclampf red, GLclampf green, GLclampf blue,
                      GLclampf alpha, GLuint programID, GLuint numIndicies)
{
	/* Get the window dimensions and update the viewport. */
	GLint w, h;
	SDL_GetWindowSize(window, &w, &h);
	glViewport(0, 0, w, h);

	/* Create the transformation matrix and projection matrix. */

	glm::mat4 projectionMatrix = glm::perspective(30.0f, ((float)w / h), 
		0.1f, 10.0f);	
	glm::mat4 translationMatrix = glm::translate(projectionMatrix,
		glm::vec3(0.0f, 0.0f, -3.0f));
	glm::mat4 fullTransformMatrix = glm::rotate(translationMatrix, 54.0f,
		glm::vec3(1.0f, 0.0f, 0.0f));

	/* Apply the matricies to the shader uniform matrix variable. */
	GLint fullTransformMatrixUniformLocation = glGetUniformLocation(programID, 
		"fullTransformMatrix");
	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1, GL_FALSE,
		&fullTransformMatrix[0][0]);

	/* Set the clear color to the specified RGBA. */
	glClearColor(red, green, blue, alpha);

	/* Tell OpenGL to clear the color buffer. */
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	/* Draw the elements to the window. */
	//glDrawArrays(GL_TRIANGLES, 0, 6);
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
	/* Delete the GL context. */
	SDL_GL_DeleteContext(context);

	/* Destroy the window. */
	SDL_DestroyWindow(window);
}
