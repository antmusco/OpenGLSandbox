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
	/* Create the display. */
	Display display("Display", 800, 600);
	Shader shader("shader.vs", "shader.fs");
	Camera* camera = display.getCamera();

	/* Create a list of two cubes (instancing) */
	std::vector<Mesh*> meshes;
	Mesh cube = Geometry::makeIsocohedron();
	Mesh ico = Geometry::makeCube();
	Mesh plane = Geometry::makePlane({ 1.0f, 0.0f, 0.0f }, {0.0f, 1.0f, 0.0f});
	
	/*
	for (int i = 0; i < plane.numVertices; i++)
	{
		std::cout << "Vertex  "<< i << "\t| Position {"
			<< " x: " << ( plane.vertices + i )->position.x 
			<< " y: " << ( plane.vertices + i )->position.y
			<< " z: " << ( plane.vertices + i )->position.z
			<< "} \t| Color {" 
			<< " r: " << ( plane.vertices + i )->color.r
			<< " g: " << ( plane.vertices + i )->color.g
			<< " b: " << ( plane.vertices + i )->color.b
			<< "}"
			<< std::endl;
	}

	for (int i = 0; i < plane.numIndices / 3; i++)
	{
		std::cout << "Tri " << i << "\t| {";
		for (int j = 0; j < 2; j++)
		{
			std::cout << *( plane.indices + ( i * 3 ) + j ) << ", ";
		}
		std::cout << *( plane.indices + ( i * 3 ) + 2 ) << "}" << std::endl;
	}
	*/

	meshes.push_back(&cube);
	meshes.push_back(&ico);
	meshes.push_back(&plane);

	for (int i = 0; i < ico.numIndices; i++)
		*( ico.indices + i ) += cube.numVertices;

	for (int i = 0; i < plane.numIndices; i++)
		*( plane.indices + i ) += (cube.numVertices + ico.numVertices);

	/* Generate vertex buffer. */
	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	GLuint bufferOffset = cube.vertexBufferSize() + ico.vertexBufferSize() + plane.vertexBufferSize();
	glBufferData(GL_ARRAY_BUFFER, bufferOffset, 0, GL_STATIC_DRAW);
	bufferOffset = 0;
	glBufferSubData(GL_ARRAY_BUFFER, bufferOffset, cube.vertexBufferSize(), cube.vertices);
	bufferOffset += cube.vertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, bufferOffset, ico.vertexBufferSize(), ico.vertices);
	bufferOffset += ico.vertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, bufferOffset, plane.vertexBufferSize(), plane.vertices);

	/* Vertex position attribute. */
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, 0);

	/* Vertex color attribute. */
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (char*)(sizeof(GLfloat) * 3));

	/* Generate index buffer. */
	GLuint indexBufferID;
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	bufferOffset = cube.indexBufferSize() + ico.indexBufferSize() + plane.indexBufferSize();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferOffset, 0, GL_STATIC_DRAW);
	bufferOffset = 0;
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, bufferOffset, cube.indexBufferSize(), cube.indices);
	bufferOffset += cube.indexBufferSize();
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, bufferOffset, ico.indexBufferSize(), ico.indices);
	bufferOffset += ico.indexBufferSize();
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, bufferOffset, plane.indexBufferSize(), plane.indices);

	/************** Playing with uniform values... *************/
	//glm::vec3 dominatingColor(1.0f, 0.0f, 0.0f);
	//GLfloat yFlip = 1.0;
	//GLint dominatingColorUniformLocation = glGetUniformLocation(shader.getProgram(), "dominatingColor");
	//GLint yFlipUniformLocation = glGetUniformLocation(shader.getProgram(), "yFlip");
	//glUniform3fv(dominatingColorUniformLocation, 1, &dominatingColor[0]);
	//glUniform1f(yFlipUniformLocation, yFlip);
	/***********************************************************/

	SDL_Event event;
	SDL_PollEvent(&event);
	GLfloat rot  = 0.0f,
		    xPos1 = 0.0f, xPos2 = 0.0f,
            yPos1 = 0.0f, yPos2 = 0.0f,
            zPos1 = 0.0f, zPos2 = 0.0f;

	/* Create the vector of matrix transformations. */
	std::vector<glm::mat4*> modelToWorldMatrices;
	glm::mat4 cubeTrans, icoTrans, identity;
	modelToWorldMatrices.push_back(&cubeTrans);
	modelToWorldMatrices.push_back(&icoTrans);
	modelToWorldMatrices.push_back(&identity);

	std::clock_t t = clock();

	/* Main loop. */
	while (event.type != SDL_QUIT)
	{

		handleEvent(&event, camera);

		if (( clock() - t ) % 10)
		{
			float xPos1 =  0; //2 * std::sinf(rot);
			float yPos1 =  2; //2 * std::cosf(rot) - 5;
			float zPos1 = -7; //std::sinf(rot) - 5;
			float xPos2 =  3; //2 * std::cosf(2*rot) - 5;
			float yPos2 =  0; //std::cosf(3 * rot);
			float zPos2 = -2;
			cubeTrans = glm::translate(glm::vec3(xPos1, yPos1, zPos1)) *
				         glm::rotate(rot, glm::vec3(1.0f, 0.5f, 0.0f));
			icoTrans = glm::translate(glm::vec3(xPos2, yPos2, zPos2)) *
				         glm::rotate(-rot, glm::vec3(1.0f, 0.5f, 0.0f));
			display.repaint(shader.getProgram(), meshes, modelToWorldMatrices);
			rot += 0.001f;
		}
		SDL_PollEvent(&event);
	}

	cube.cleanUp();

	/* Quit using SDL. */
	SDL_Quit();
	return 0;
}
