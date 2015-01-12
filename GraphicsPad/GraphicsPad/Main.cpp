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


#define  FULLSCREEN_ENABLED   false
#define  DEFAULT_HEIGHT		  600
#define  DEFAULT_WIDTH        800
#define  DIMENSION_HEIGHT     0x0
#define  DIMESNION_WIDTH      0x1
#define  DEGREES_PER_DAY      9.863014e-1f
#define  DEGREES_PER_HOUR     4.109589e-2f
#define  DEGREES_PER_MINUTE   6.849315e-4f

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
	EventManager eventManager(camera);


	glm::vec3  bases[] =
	{
        {+1.0f, +0.0f, +0.0f},
        {+0.0f, +1.0f, +0.0f},
        {+0.0f, +0.0f, +1.0f}
    };

	char*    sphereObj    =        "res/body.obj";
	char*    earthTexture =       "res/earth.jpg";
	char*    starsTexture =    "res/milkyway.jpg";
	char*    sunTexture   = "res/preview_sun.jpg";
	char*    sunName      =                 "Sun";
	char*    earthName    =               "Earth";
	GLfloat  timer        =           +      0.0f;
	GLfloat  dif          =           +      0.0f;
	GLfloat  rot          =           +      0.0f;
	GLfloat  sunToEarth   =           +    500.0f;
	GLfloat  stars_scale  =           +  10000.0f;
	GLfloat  sun_scale    =           +     50.0f;
	GLfloat  speed        =           +    0.050f;
	GLfloat  fps          =           +    100.0f;
	GLfloat  sunMass      =           +   1000.0f;
	GLfloat  sunRadius    =           +     50.0f;
	GLfloat  earthMass    =           +      1.0f;
	GLfloat  earthRadius  =           +      1.0f;

	glm::vec3  initialPositions[]  = 
	{ 
		{ +     0.0f, +0.0f, +0.0f }, // Sun 
		{ sunToEarth, +0.0f, +0.0f }, // Earth
	};	

	/* Create the geometries. */
	OrbitalSystem system;
	Planet sun(sunName, sunMass, sunRadius, sphereObj, sunTexture, initialPositions[0]);
	Planet earth(earthName, earthMass, earthRadius, sphereObj, earthTexture, initialPositions[1]);
	earth.setRotationalAxis({-1.0f / sinf(23.5), +1.0f, +0.0f});
	earth.setAngularVelocity(0.004166);
	system.addBody(&sun);
	system.addBody(&earth);

	camera->setPosition({sunToEarth + 5.0f, 1.0f, +0.0f});
	camera->setViewDirection({sunToEarth + 4.0f, 0.0f, +0.0f});

	std::vector<Mesh*>     meshes;
	std::vector<glm::mat4> transformations;

	for(std::string n : *(system.getNames()))
		meshes.push_back(system.getBody(n)->getGeometry());

	/* Main loop. */	
	SDL_Event event;
	SDL_PollEvent(&event);

	GLuint start, t1, t2;
	t1 = start = SDL_GetTicks();
	while (event.type != SDL_QUIT)
	{
		t2 = SDL_GetTicks();
		system.interpolate((t2 - t1) / 1000);

		/* Handle the event. */
		eventManager.handleSDLEvent(&event);

		if ((t2 - t1) >= ((1 / fps) * 1000))
		{
			t1 = t2;

			for(std::string n : *(system.getNames()))
				transformations.push_back(system.getBody(n)->snapshotMatrix());

			display.repaint(meshes, transformations);
			timer += DEGREES_PER_MINUTE * speed;
		}
		SDL_PollEvent(&event);
	}

	/* Free the shapes. */
	int size = meshes.size();
	for(int i = 0; i < size; i ++)
		meshes.at(i)->cleanUp();

	/* Quit using SDL. */
	SDL_Quit();
	return 0;
}
