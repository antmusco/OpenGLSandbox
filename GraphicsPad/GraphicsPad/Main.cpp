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


#define  FULLSCREEN_ENABLED   false
#define  DEFAULT_HEIGHT		  600
#define  DEFAULT_WIDTH        800
#define  DIMENSION_HEIGHT     0x0
#define  DIMESNION_WIDTH      0x1


static void handleEvent(SDL_Event* event, Camera* camera)
{
	glm::vec3 move;
	float scale = 2.0f;
	if (event->type == SDL_MOUSEMOTION)
	{
		camera->updateLookAt({ event->motion.x, event->motion.y });
	}
	else if (event->type == SDL_KEYDOWN)
	{
		switch (event->key.keysym.scancode)
		{
		case SDL_SCANCODE_D:
		case SDL_SCANCODE_RIGHT:
			move = glm::cross(*(camera->getUpDirection()), *(camera->getViewDirection()));
			move = move / glm::length(move);
			*( camera->getPosition() ) += move * scale;
			//*( camera->getViewDirection() ) += move;
			break;
		case SDL_SCANCODE_A:
		case SDL_SCANCODE_LEFT:
			move = glm::cross(*( camera->getViewDirection() ), *( camera->getUpDirection() ));
			move = move / glm::length(move);
			*( camera->getPosition() ) += move * scale;
			//*( camera->getViewDirection() ) += move;
			break;
		case SDL_SCANCODE_S:
		case SDL_SCANCODE_DOWN:
			move = *(camera->getUpDirection());
			move /= glm::length(move);
			*( camera->getPosition() ) += move * scale;
			*( camera->getViewDirection() ) += move * scale;
			break;
		case SDL_SCANCODE_W:
		case SDL_SCANCODE_UP:
			move = *( camera->getUpDirection() );
			move /= -glm::length(move);
			*( camera->getPosition() ) += move * scale;
			*( camera->getViewDirection() ) += move * scale;
			break;
		case SDL_SCANCODE_Z:
			move = *(camera->getViewDirection());
			move = -1.0f * ( move / glm::length(move) );
			*( camera->getPosition() ) += move * scale;
			*( camera->getViewDirection() ) += move * scale;
			break;
		case SDL_SCANCODE_X:
			move = *( camera->getViewDirection() );
			move /= glm::length(move);
			*( camera->getPosition() ) += move * scale;
			*( camera->getViewDirection() ) += move * scale;
			break;
		case  SDL_SCANCODE_ESCAPE:
			exit(0);
		}
	}
}

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

	char* sphereObj = "res/earth.obj";
	char* earthTexture = "res/SnowIceCover_Daily.bmp";
	char* starsTexture = "res/stars.bmp";
	char* sunTexture = "res/preview_sun.jpg";


	glm::vec3  bases[]             = {
                                	    {+1.0f, +0.0f, +0.0f},
                                        {+0.0f, +1.0f, +0.0f},
                                        {+0.0f, +0.0f, +1.0f}
                                	 };

	/* Create the geometries. */
	Mesh stars = Geometry::loadObj(sphereObj, starsTexture);
	Mesh earth = Geometry::loadObj(sphereObj, earthTexture);
	Mesh sun   = Geometry::loadObj(sphereObj, sunTexture);

	/* Add geometries to the list of meshes. */
	std::vector<Mesh*> meshes;
	meshes.push_back(&stars);
	meshes.push_back(&earth);
	meshes.push_back(&sun);

	/* Create transformation matrices. */
	std::vector<glm::mat4*> modelToWorldMatrices;

	GLfloat    timer               = +    0.0f;
	GLfloat    dif                 = +    0.0f;
	GLfloat    rot                 = +    0.0f;
	GLfloat    radius              = +  250.0f;
	GLfloat    stars_scale         = + 1000.0f;
	GLfloat    sun_scale           = +   20.0f;
	GLfloat    speed               = +   10.0f;
	GLfloat    fps                 = +  100.0f;
	glm::vec3  initialPositions[]  = { 
		                                { +0.0f, +0.0f, +0.0f }, 
		                                { +0.0f, +0.0f, +0.0f },
	                                 };	 

	/* Main loop. */	
	SDL_Event event;
	SDL_PollEvent(&event);

	GLuint start, t1, t2;
	t1 = start = SDL_GetTicks();
	while (event.type != SDL_QUIT)
	{
		t2 = SDL_GetTicks();
		/* Handle the event. */
		handleEvent(&event, camera);

		if ((t2 - t1) >= ((1 / fps) * 1000))
		{
			t1 = t2;
			dif = 0.25f * timer * speed;
			rot = 10.0f * timer * speed;

			/* Stars */
			modelToWorldMatrices.push_back( 
				  &glm::scale(glm::vec3(stars_scale, stars_scale, stars_scale))
			);

			/* Earth */
			modelToWorldMatrices.push_back(
				&(glm::translate(initialPositions[0] + (bases[0] * radius * cosf(0.25 * dif)) + (bases[2] * radius * sinf(0.25 * dif))) *
				  glm::rotate(rot, glm::vec3(0.0f, 1.0f, 0.0f)))
			);

			/* Sun */
			modelToWorldMatrices.push_back(
				&(glm::scale(glm::vec3(sun_scale, sun_scale, sun_scale)))
			);

			display.repaint(meshes, modelToWorldMatrices);
			timer += 0.0001f;
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
