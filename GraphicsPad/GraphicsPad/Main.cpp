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

int main(int argc, char* argv[])
{
	/* Initialize all subsystems. */
	SDL_Init(SDL_INIT_EVERYTHING);
	/* Create the display. */
	Display display("Display", 1500, 1000);
	Shader shader("shader.vs", "shader.fs");
	Camera* camera = display.getCamera();

	/* Create a list of two cubes (instancing) */
	std::vector<Mesh*> meshes;
	Mesh cube = Geometry::makeIsocohedron();
	Mesh ico = Geometry::makeCube();

	for (int i = 0; i < ico.numIndices; i++)
		*( ico.indices + i ) += cube.numVertices;

	//Mesh plane = Geometry::makePlane({ 1.0f, 0.0f, 0.0f }, {0.0f, 1.0f, 0.0f});
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

	for (int i = 0; i < plane.numIndices / 4; i++)
	{
		std::cout << "Quad " << i << "\t| {";
		for (int j = 0; j < 3; j++)
		{
			std::cout << *( plane.indices + ( i * 4 ) + j ) << ", ";
		}
		std::cout << *( plane.indices + ( i * 4 ) + 3 ) << "}" << std::endl;
	}
	*/

	//meshes.push_back(&plane);
	meshes.push_back(&cube);
	meshes.push_back(&ico);

	/* Generate vertex buffer. */
	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, cube.vertexBufferSize() + ico.vertexBufferSize(), 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, cube.vertexBufferSize(), cube.vertices);
	glBufferSubData(GL_ARRAY_BUFFER, cube.vertexBufferSize(), ico.vertexBufferSize(), ico.vertices);

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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube.indexBufferSize() + ico.indexBufferSize(), 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, cube.indexBufferSize(), cube.indices);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, cube.indexBufferSize(), ico.indexBufferSize(), ico.indices);

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
	glm::mat4 cubeTrans, icoTrans;
	modelToWorldMatrices.push_back(&cubeTrans);
	modelToWorldMatrices.push_back(&icoTrans);

	std::clock_t t = clock();

	/* Main loop. */
	while (event.type != SDL_QUIT)
	{
		if (event.type == SDL_MOUSEMOTION)
		{
			camera->updateLookAt({event.motion.x, event.motion.y});
		}
		if (( clock() - t ) % 10)
		{
			float xPos1 = 2 * std::sinf(rot);
			float yPos1 = 2 * std::cosf(rot) - 5;
			float zPos1 = std::sinf(rot) - 5;
			float xPos2 = 2 * std::cosf(2*rot) - 5;
			float yPos2 = std::cosf(3*rot);
			float zPos2 = -6;
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