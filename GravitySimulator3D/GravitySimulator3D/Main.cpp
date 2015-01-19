/*******************************************************************************
 *                                                                             *
 *                              Included Header Files                          *
 *                                                                             *
 ******************************************************************************/
#include <gl\glew.h>
#include <SDL\SDL.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <iostream>
#include <string>
#include <ctime>
#include "Display.h"
#include "Shader.h"
#include "Geometry.h"
#include "Camera.h"
#include "EventManager.h"
#include "OrbitalBody.h"
#include "OrbitalSystem.h"
#include "Planet.h"

/*******************************************************************************
 *                                                                             *
 *                        Defined Constants and Macros                         *
 *                                                                             *
 ******************************************************************************/
#define  FULLSCREEN_ENABLED   true
#define  DEFAULT_HEIGHT		  600
#define  DEFAULT_WIDTH        800
#define  MILLIS_PER_SECOND    1000
#define  MESHES_PATH          "res/meshes/";
#define  TEXUTRES_PATH        "res/textures/";
#define  SHADERS_PATH         "res/shaders/";
#define  FRAMES_PER_SECOND    100
#define  PROJECT_TITLE        "GravitySimulator3D"
#define  PRINT(a)             std::cout << a << std::endl;

/*******************************************************************************
 *                                                                             *
 *                                Global Variables                             *
 *                                                                             *
 ******************************************************************************/
/* Speed of the simulation. */
GLfloat speed = + 8.000e2f;

/* Unit vectors for the 3-D space. */
glm::vec3  bases[] =
	{
        {+1.0f, +0.0f, +0.0f},
        {+0.0f, +1.0f, +0.0f},
        {+0.0f, +0.0f, +1.0f}
    };

/*******************************************************************************
 *                                                                             *
 *                                     main                                    *
 *                                                                             *
 *******************************************************************************
 * PARAMETERS                                                                  *
 *  argc                                                                       *
 *        The number of command line strings.                                  *
 *  argv                                                                       *
 *        The array of command line stirngs.                                   *
 *                                                                             *
 *******************************************************************************
 * RETURNS                                                                     *
 *  0 on success, any non-zero value on failure.                               *
 *                                                                             *
 *******************************************************************************
 * DESCRIPTION                                                                 *
 *  Beginning point of the program.                                            *
 *                                                                             *
 *******************************************************************************/
int main(int argc, char* argv[])
{
	/* Initialize SDL with all subsystems. */
	SDL_Init(SDL_INIT_EVERYTHING);

	/* Create the display, shader, camera, and event manager. */
	Display      display(PROJECT_TITLE, DEFAULT_WIDTH, DEFAULT_HEIGHT);
	Shader       shader(DEFAULT_VERTEX_SHADER, DEFAULT_FRAGMENT_SHADER);
	Camera*      camera = display.getCamera();
	EventManager eventManager(camera, &speed);

	/* Apply the shaders and maximize the display. */
	Geometry::shader = &shader;
	display.setShader(shader);
	display.maximize();

	/* Create the orbital system. */
	OrbitalSystem system = OrbitalSystem::loadFile("res/data/system.xml");

	/* Instantiate the event reference. */
	SDL_Event event;
	SDL_PollEvent(&event);	

	/* Begin the milliseconds counter. */
	GLuint startMillis = 0, tempMillis = 0, currentMillis = 0, millisPerFrame = 0;
	startMillis = tempMillis = currentMillis = SDL_GetTicks();	
	millisPerFrame = (GLuint) ((1.0 / FRAMES_PER_SECOND) * MILLIS_PER_SECOND);
	PRINT(millisPerFrame)

	/* Main loop. */
	while (event.type != SDL_QUIT)
	{
		/* Handle the new event. */
		eventManager.handleSDLEvent(&event);

		/* Get the new number of milliseconds. */
		currentMillis = SDL_GetTicks();

		/* Interpolate the system over the interval. */
		system.interpolate((speed * (currentMillis - tempMillis)) / 1000);

		//PRINT(glm::distance(system.getBody(0)->getLinearPosition(), system.getBody(1)->getLinearPosition()));

		/* If a new frame is to be drawn, update the display. */
		if ((currentMillis - startMillis) >= millisPerFrame)
		{
			startMillis = currentMillis;
			display.repaint(system.getMeshes(), system.getTransforms());
		}

		/* Update the temporary millisecond counter. */
		tempMillis = currentMillis;
		
		/* Get the next event. */
		SDL_PollEvent(&event);
	}

	/* Free the shapes. */
	system.cleanUp();

	/* Quit using SDL. */
	SDL_Quit();

	/* Exit Success. */
	return 0;
}
