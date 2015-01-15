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

#define  FULLSCREEN_ENABLED   true
#define  DEFAULT_HEIGHT		  600
#define  DEFAULT_WIDTH        800
#define  DIMENSION_HEIGHT     0x0
#define  DIMESNION_WIDTH      0x1
#define  DEGREES_PER_DAY      9.863014e-1f
#define  DEGREES_PER_HOUR     4.109589e-2f
#define  DEGREES_PER_MINUTE   6.849315e-4f
#define  MESHES_PATH          "res/meshes/";
#define  TEXUTRES_PATH        "res/textures/";
#define  SHADERS_PATH         "res/shaders/";


GLfloat speed = + 1.000f;

GLushort getScreenDimension(int dim) 
{
	SDL_Rect display;
	SDL_GetDisplayBounds(0, &display);
	
	if(dim == DIMENSION_HEIGHT)
		return display.h;
	else if(dim == DIMESNION_WIDTH)
		return display.w;
	else
		return -1;
}

int main(int argc, char* argv[])
{
	/* Initialize all subsystems. */
	SDL_Init(SDL_INIT_EVERYTHING);

	/* Create the display, shader, and camera. */
	int height = (FULLSCREEN_ENABLED) ? getScreenDimension(DIMENSION_HEIGHT) : DEFAULT_HEIGHT;
	int width = (FULLSCREEN_ENABLED) ? getScreenDimension(DIMESNION_WIDTH) : DEFAULT_WIDTH;

	Display display("Display", width, height);
	Shader shader(DEFAULT_VERTEX_SHADER, DEFAULT_FRAGMENT_SHADER);
	display.setShader(shader);
	Camera* camera = display.getCamera();
	Geometry::shader = &shader;
	EventManager eventManager(camera, &speed);

	glm::vec3  bases[] =
	{
        {+1.0f, +0.0f, +0.0f},
        {+0.0f, +1.0f, +0.0f},
        {+0.0f, +0.0f, +1.0f}
    };

	std::string  sphereObj    = MESHES_PATH;
	sphereObj.append("body.obj");
	std::string  earthTexture = TEXUTRES_PATH;
	earthTexture.append("earth.jpg");
	std::string  starsTexture = TEXUTRES_PATH;
	starsTexture.append("milkyway.jpg");
	std::string  sunTexture   = TEXUTRES_PATH;
	sunTexture.append("sun.jpg");
	std::string  sunName      =                          "Sun";
	std::string  earthName    =                        "Earth";
	GLfloat      sunToEarth   =                   + 1.000e+02f;
	GLfloat      stars_scale  =                   + 1.000e+05f;
	GLfloat      sun_scale    =                   + 6.958e+01f;
	GLfloat      fps          =                   +      50.0f;
	GLfloat      sunMass      =                   + 1.989e+21f;
	GLfloat      sunRadius    =                   +      50.0f;
	GLfloat      earthMass    =                   + 5.972e+15f;
	GLfloat      earthRadius  =                   + 1.000e+01f;

	glm::vec3  initialPositions[]  = 
	{ 
		{ +     0.0f, +0.0f, +0.0f }, // Sun 
		{ sunToEarth, +0.0f, +0.0f }, // Earth
	};	

	/* Create the geometries. */
	//OrbitalSystem system(sphereObj.c_str(), starsTexture.c_str(), stars_scale);
	OrbitalSystem system = OrbitalSystem::loadFile("res/data/system.xml");

	/* Main loop. */	
	SDL_Event event;
	SDL_PollEvent(&event);

	GLuint start, t1, t2;
	t1 = start = SDL_GetTicks();

	while (event.type != SDL_QUIT)
	{
		/* Handle the event. */
		eventManager.handleSDLEvent(&event);

		t2 = SDL_GetTicks();

		if ((t2 - t1) >= ((1 / fps) * 1000))
		{
			system.interpolate((speed * (t2 - t1)) / 1000);
			
			t1 = t2;

			display.repaint(system.getMeshes(), system.getTransforms());
		}
		SDL_PollEvent(&event);
	}

	/* Free the shapes. */
	system.cleanUp();

	/* Quit using SDL. */
	SDL_Quit();
	return 0;
}
