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


#define  FULLSCREEN_ENABLED   true
#define  DEFAULT_HEIGHT		  600
#define  DEFAULT_WIDTH        800
#define  DIMENSION_HEIGHT     0x0
#define  DIMESNION_WIDTH      0x1


static void handleEvent(SDL_Event* event, Camera* camera)
{
	glm::vec3 move;
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
			*( camera->getPosition() ) += move;
			//*( camera->getViewDirection() ) += move;
			break;
		case SDL_SCANCODE_A:
		case SDL_SCANCODE_LEFT:
			move = glm::cross(*( camera->getViewDirection() ), *( camera->getUpDirection() ));
			move = move / glm::length(move);
			*( camera->getPosition() ) += move;
			//*( camera->getViewDirection() ) += move;
			break;
		case SDL_SCANCODE_S:
		case SDL_SCANCODE_DOWN:
			move = *(camera->getUpDirection());
			move /= glm::length(move);
			*( camera->getPosition() ) += move;
			*( camera->getViewDirection() ) += move;
			break;
		case SDL_SCANCODE_W:
		case SDL_SCANCODE_UP:
			move = *( camera->getUpDirection() );
			move /= -glm::length(move);
			*( camera->getPosition() ) += move;
			*( camera->getViewDirection() ) += move;
			break;
		case SDL_SCANCODE_Z:
			move = *(camera->getViewDirection());
			move = -1.0f * ( move / glm::length(move) );
			*( camera->getPosition() ) += move;
			*( camera->getViewDirection() ) += move;
			break;
		case SDL_SCANCODE_X:
			move = *( camera->getViewDirection() );
			move /= glm::length(move);
			*( camera->getPosition() ) += move;
			*( camera->getViewDirection() ) += move;
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
	
	/* Create the geometries. */
	Mesh sun = Geometry::makeSphere(3);
	Mesh earth = Geometry::makeSphere(3);

	/* Add geometries to the list of meshes. */
	std::vector<Mesh*> meshes;
	meshes.push_back(&sun);
	meshes.push_back(&earth);

	/* Create transformation matrices. */
	std::vector<glm::mat4*> modelToWorldMatrices;

	GLfloat    rot                 = + 0.0f;
	GLfloat    radius              = +10.0f;
	GLfloat    sun_scale           = + 3.0f;
	glm::vec3  initialPositions[]  = { 
		                                { +0.0f, +0.0f, +0.0f }, 
		                                { +0.0f, +0.0f, +0.0f },
	                                 };	 
	glm::vec3  bases[]             = {
                                	    {+1.0f, +0.0f, +0.0f},
                                        {+0.0f, +1.0f, +0.0f},
                                        {+0.0f, +0.0f, +1.0f}
                                	 };


	/* Main loop. */	
	SDL_Event event;
	GLuint t = clock();
	SDL_PollEvent(&event);

	while (event.type != SDL_QUIT)
	{
		/* Handle the event. */
		handleEvent(&event, camera);

		if (( clock() - t ) % 10)
		{
			/* Sun */
			modelToWorldMatrices.push_back( 
				&(glm::translate(initialPositions[1]) *
				  glm::scale(glm::vec3(sun_scale, sun_scale, sun_scale)) * 
				  glm::rotate(rot, glm::vec3(+0.0f, +1.0f, +0.0f)))
			);

			modelToWorldMatrices.push_back(
				&(glm::translate(initialPositions[0] + (bases[0] * radius * cosf(rot)) + (bases[2] * radius * sinf(rot))) *
				  glm::rotate(-rot, glm::vec3(0.0f, 0.5f, 1.0f)))
			);

			display.repaint(meshes, modelToWorldMatrices);
			rot += 0.001f;
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
