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
		}
	}
}

int main(int argc, char* argv[])
{
	/* Initialize all subsystems. */
	SDL_Init(SDL_INIT_EVERYTHING);

	/* Create the display, shader, and camera. */
	Display display("Display", 800, 600);
	Shader shader(DEFAULT_VERTEX_SHADER, DEFAULT_FRAGMENT_SHADER);
	display.setShader(shader);
	Camera* camera = display.getCamera();
	Geometry::shader = &shader;

	/* Create the geometries. */
	Mesh shape1 = Geometry::makeSphere(6);
	Mesh shape2 = Geometry::makeCube();
	Mesh shape3 = Geometry::makeCoordinatePlane(30.0f, 30.0f, 30.f);
	Mesh shape4 = Geometry::makeCube();
	Mesh shape5 = Geometry::makePlane({1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});

	/* Add geometries to the list of meshes. */
	std::vector<Mesh*> meshes;
	meshes.push_back(&shape1);
	meshes.push_back(&shape2);
	meshes.push_back(&shape3);
	meshes.push_back(&shape4);
	meshes.push_back(&shape5);

	/* Create transformation matrices. */
	std::vector<glm::mat4*> modelToWorldMatrices;

	GLfloat rot = 0.0f;
	glm::vec3 initialPositions[] = { 
		{ -4.0f, +0.0f, -3.0f }, 
		{ -2.0f, +0.0f, -3.0f }, 
		{ +0.0f, +0.0f, +0.0f }, 
		{ +2.0f, +0.0f, -3.0f },
		{ +0.0f, +0.0f, +0.0f }
	};	 

	int i = 1;
	for (Mesh* m : meshes)
		std::cout << "Shape " << i++ 
			<< ": vaoID = " << m->getVertexArrayID() 
			<< " | buffIDs = " << m->getBufferIDs()[0] << " " << m->getBufferIDs()[1]
			<< std::endl;

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
			modelToWorldMatrices.push_back( 
				&(glm::translate(initialPositions[0] + 
					glm::vec3(+14.0f, +0.0f, -2.0f) * cosf(rot) +
					glm::vec3(+0.0f, +1.0f, +2.0f) * sinf(rot)) *
				glm::rotate(rot, glm::vec3(1.0f, 0.5f, 0.0f)))
			);

			modelToWorldMatrices.push_back(
				&(glm::translate(initialPositions[1]) *
				glm::rotate(-rot, glm::vec3(0.0f, 0.5f, 1.0f)))
			);

			modelToWorldMatrices.push_back(
				&(glm::translate(initialPositions[2]) *
				glm::rotate(0.0f, glm::vec3(0.0f, 1.0f, 5.0f)))
			);

			modelToWorldMatrices.push_back( 
				&(glm::translate(initialPositions[3]) *
				glm::rotate(-rot, glm::vec3(1.0f, 0.0f, 5.0f)))
			);

			modelToWorldMatrices.push_back(
				&(glm::translate(initialPositions[4]) *
				glm::rotate(-rot, glm::vec3(0.0f, 1.0f, 0.0f)))
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
