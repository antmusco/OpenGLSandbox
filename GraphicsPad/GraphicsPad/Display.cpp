/******************************************************************************
*                                                                             *
*                              Included Header Files                          *
*                                                                             *
******************************************************************************/
#include <gl\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <SDL\SDL_video.h>
#include <iostream>
#include "Display.h"
#include "Geometry.h"

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
	/* Get the Screen info. */
	


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

	/* Show the version of GLEW currently being used. */
	fprintf(stdout, "Stats: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	/* Update the viewport. */
	updateViewport();
}

/******************************************************************************
*                                                                             *
*                           Display::updateViweport                           *
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
*  Function which gets the width and height of the window and updates the     *
*  aspect ratio, the glViewwport, and the viewToPorojectionMatrix for the     *
*  display.                                                                   *
*                                                                             *
*******************************************************************************/
void Display::updateViewport()
{
	/* Get the width and height of the window and calculate aspect ratio. */
	GLint width, height;
	SDL_GetWindowSize(window, &width, &height);
	aspectRatio = (GLfloat)width / height;

	/* Update the GLviewport. */
	glViewport(0, 0, width, height);

	/* Calculate the View-To-Projection matrix. */
	viewToProjectionMatrix = glm::perspective(FOV, aspectRatio, NEAR, FAR);
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
void Display::repaint(GLuint programID, std::vector<Mesh*> meshes,
                      std::vector<glm::mat4*> modelToWorldMatrices)
{
	/* Tell OpenGL to clear the color buffer and depth buffer. */
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);	

	/* Get the window dimensions and update the viewport. */
	updateViewport();

	/* Get the location of the fullTransformMatrix uniform variable. */
	GLint fullTransformMatrixUniformLocation = glGetUniformLocation(programID,
		"fullTransformMatrix");

	/* Declare the fullTransformMatrix. */
	glm::mat4 fullTransformMatrix;

	/* Display Meshes. */
	GLuint byteOffset = 0;
	for (GLuint i = 0; i < meshes.size(); i++)
	{
		/* Generate the full transformation. */
		fullTransformMatrix = viewToProjectionMatrix *
				camera.getWorldToViewMatrix() *	(*modelToWorldMatrices.at(i));

		/* Send the transformation data down to the buffer. */
		glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1, GL_FALSE,
			&fullTransformMatrix[0][0]);

		/* Draw the elements to the window. */
		glDrawElements(meshes.at(i)->drawMode, meshes.at(i)->numIndices, 
			GL_UNSIGNED_SHORT, (char*)byteOffset);

		/* Update the byte offset. */
		byteOffset += meshes.at(i)->indexBufferSize();
	}

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
