#include <gl\glew.h>
#include <SDL\SDL.h>
#include <glm\glm.hpp>
#include <iostream>
#include <string>
#include "Display.h"
#include "Shader.h"
#include "Geometry.h"

int main(int argc, char* argv[])
{
	/* Initialize all subsystems. */
	SDL_Init(SDL_INIT_EVERYTHING);
	/* Create the display. */
	Display display("Display", 800, 600);
	Shader shader("shader.vs","shader.fs");

	Mesh cube = Geometry::makeCube();

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
	do 
	{
		display.repaint(0, 0, 0, 1, shader.getProgram(), cube.numIndices);
		SDL_PollEvent(&event);
	} 
	while (event.type != SDL_QUIT);

	cube.cleanUp();

	/* Quit using SDL. */
	SDL_Quit();
	return 0;
}