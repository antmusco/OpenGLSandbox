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

GLuint* genVertexArrayIDs(GLuint numObjects, GLuint* bufferIDs)
{
	/* Create array of VAO IDs and generate arrays. */
	GLuint* vertexArrayIDs = new GLuint[numObjects];
	glGenVertexArrays(numObjects, vertexArrayIDs);

	for (int i = 0; i < numObjects; i++)
	{
		/* Bind this vertex array ID.*/
		glBindVertexArray(vertexArrayIDs[i]);
		
		/* Bind the vertex buffer. */
		glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[( i * 2 ) + 0]);

		/* Vertex position attribute. */
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);

		/* Vertex color attribute. */
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)( sizeof(GLfloat) * 3 ));
		
		/* Bind the index buffer. */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIDs[( i * 2 ) + 1]);
	}

	/* Return the vertex array IDs. */
	return vertexArrayIDs;
}

GLuint* genBufferArrayIDs(std::vector<Mesh*>* meshes)
{
	const GLuint NUM_BUFFERS = 2;
	GLuint* bufferIDs = new GLuint[NUM_BUFFERS * meshes->size()];

	glGenBuffers(NUM_BUFFERS * meshes->size(), bufferIDs);
	for (int i = 0; i < meshes->size(); i++)
	{
		/* Create vertex buffer. */
		glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[( i * NUM_BUFFERS ) + 0]);
		glBufferData(GL_ARRAY_BUFFER, meshes->at(i)->vertexBufferSize(),
			meshes->at(i)->vertices, GL_STATIC_DRAW);

		/* Create index buffer. */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIDs[( i * NUM_BUFFERS ) + 1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshes->at(i)->indexBufferSize(),
			meshes->at(i)->faces, GL_STATIC_DRAW);
	
	}

	/* Return buffer IDs */
	return bufferIDs;
}

int main(int argc, char* argv[])
{
	/* Initialize all subsystems. */
	SDL_Init(SDL_INIT_EVERYTHING);

	/* Create the display. */
	Display display("Display", 800, 600);
	Shader shader("shader.vs", "shader.fs");
	Camera* camera = display.getCamera();

	/* Create the geometries. */
	Mesh shape1 = Geometry::makeSphere(0);
	Mesh shape2 = Geometry::makeSphere(1);
	Mesh shape3 = Geometry::makeSphere(2);
	Mesh shape4 = Geometry::makeSphere(3);

	/* Add geometries to the list of meshes. */
	std::vector<Mesh*> meshes;
	meshes.push_back(&shape1);
	meshes.push_back(&shape2);
	meshes.push_back(&shape3);
	meshes.push_back(&shape4);

	/* Generate Vertex/Index Buffers */
	GLuint* bufferIDs = genBufferArrayIDs(&meshes);

	/* Generate the vertex array IDs */
	GLuint* vertexArrayIDs = genVertexArrayIDs(meshes.size(), bufferIDs);

	/* Create the vector of matrix transformations. */
	glm::mat4 shape1Trans;
	glm::mat4 shape2Trans; 
	glm::mat4 shape3Trans; 
	glm::mat4 shape4Trans;

	std::vector<glm::mat4*> modelToWorldMatrices;
	modelToWorldMatrices.push_back(&shape1Trans);
	modelToWorldMatrices.push_back(&shape2Trans);
	modelToWorldMatrices.push_back(&shape3Trans);
	modelToWorldMatrices.push_back(&shape4Trans);
	std::clock_t t = clock();

	SDL_Event event;
	SDL_PollEvent(&event);
	GLfloat rot  = 0.0f,
		xPos1 = -4.0f, xPos2 = -2.0f, xPos3 = +0.0f, xPos4 = +2.0f,
		yPos1 = +0.0f, yPos2 = +0.0f, yPos3 = +0.0f, yPos4 = +0.0f,
		zPos1 = -3.0f, zPos2 = -3.0f, zPos3 = -3.0f, zPos4 = -3.0f;

	/* Main loop. */
	while (event.type != SDL_QUIT)
	{
		/* Handle the event. */
		handleEvent(&event, camera);

		if (( clock() - t ) % 10)
		{
			shape1Trans = glm::translate(glm::vec3(xPos1, yPos1, zPos1)) *
				          glm::rotate(rot, glm::vec3(1.0f, 0.5f, 0.0f));
			shape2Trans = glm::translate(glm::vec3(xPos2, yPos2, zPos2)) *
				          glm::rotate(-rot, glm::vec3(0.0f, 0.5f, 1.0f));
			shape3Trans = glm::translate(glm::vec3(xPos3, yPos3, zPos3)) *
						  glm::rotate(rot, glm::vec3(0.0f, 1.0f, 5.0f));
			shape4Trans = glm::translate(glm::vec3(xPos4, yPos4, zPos4)) *
						  glm::rotate(-rot, glm::vec3(1.0f, 0.0f, 5.0f));
			display.repaint(shader.getProgram(), meshes, modelToWorldMatrices, vertexArrayIDs);
			rot += 0.001f;
		}
		SDL_PollEvent(&event);
	}

	/* Free the shapes. */
	shape1.cleanUp(); 
	shape2.cleanUp(); 
	shape3.cleanUp(); 
	shape4.cleanUp();

	/* Free the buffers. */
	glDeleteBuffers(meshes.size(), vertexArrayIDs);
	glDeleteBuffers(meshes.size() * 2, bufferIDs);
	delete[] vertexArrayIDs;
	delete[] bufferIDs;

	/* Quit using SDL. */
	SDL_Quit();
	return 0;
}
