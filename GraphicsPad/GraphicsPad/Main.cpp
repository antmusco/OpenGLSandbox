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
	Display display("Display", 800, 600);
	Shader shader("shader.vs","shader.fs");
	Camera* camera = display.getCamera();

	Mesh cube = Geometry::makeIsocohedron();

	/* Generate vertex buffer. */
	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, cube.vertexBufferSize(), cube.vertices, GL_STATIC_DRAW);

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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube.indexBufferSize(), cube.indices, GL_STATIC_DRAW);

	/************** Playing with uniform values... *************/
	//glm::vec3 dominatingColor(1.0f, 0.0f, 0.0f);
	//GLfloat yFlip = 1.0;
	//GLint dominatingColorUniformLocation = glGetUniformLocation(shader.getProgram(), "dominatingColor");
	//GLint yFlipUniformLocation = glGetUniformLocation(shader.getProgram(), "yFlip");
	//glUniform3fv(dominatingColorUniformLocation, 1, &dominatingColor[0]);
	//glUniform1f(yFlipUniformLocation, yFlip);
	/***********************************************************/

	/* Main loop. */
	SDL_Event event;
	GLfloat rot = 0.0, xPos = 0.0, yPos = 0.0, zPos = 0.0;
	std::clock_t t = clock();
	do 
	{
		if (!(( clock() - t ) % 10))
		{
			xPos = 2 * std::sinf(rot);
			zPos = 2 * std::cosf(rot);
			glm::mat4 transMatrix = glm::translate(glm::vec3(0.0f, 0.0f, -3.0f));
			glm::mat4 transfomration = glm::rotate(transMatrix, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
			display.repaint(shader.getProgram(), cube.numIndices, &transfomration);
			//camera->getPosition()->x = xPos;
			//camera->getPosition()->y = zPos;
			rot += 0.01;
		}
		SDL_PollEvent(&event);
	} 
	while (event.type != SDL_QUIT);

	cube.cleanUp();

	/* Quit using SDL. */
	SDL_Quit();
	return 0;
}