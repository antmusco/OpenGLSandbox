#include <gl\glew.h>
#include <SDL\SDL.h>
#include <glm\glm.hpp>
#include <iostream>
#include <string>
#include "Display.h"
#include "Shader.h"

int main(int argc, char* argv[])
{
	/* Initialize all subsystems. */
	SDL_Init(SDL_INIT_EVERYTHING);
	/* Create the display. */
	Display display("Display", 800, 600);
	Shader shader("shader.vs","shader.fs");

	/* Deine vertices. */
	GLfloat verts[] =
	{
		+0.0f, +0.0f, // 0
		+1.0f, +0.0f, +0.0f,

		+1.0f, +1.0f, // 1
		+0.0f, +1.0f, +0.0f,

		-1.0f, +1.0f, // 2
		+0.0f, +0.0f, +1.0f,

		-1.0f, -1.0f, // 3
		+0.0f, +1.0f, +0.0f,

		+1.0f, -1.0f, // 4
		+0.0f, +0.0f, +1.0f,
	};

	/* Generate vertex buffer. */
	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	/* Vertex position attribute. */
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, 0);
	/* Vertex color attribute. */
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (char*)(sizeof(GLfloat) * 2));

	/* Define indices. */
	GLushort indices[] =
	{
		0, 1, 2,
		0, 3, 4,
	};

	/* Generate index buffer. */
	GLuint indexBufferID;
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/* Main loop. */
	SDL_Event event;
	do 
	{
		display.repaint(0, 0, 0, 1);
		SDL_PollEvent(&event);
	} 
	while (event.type != SDL_QUIT);
	/* Quit using SDL. */
	SDL_Quit();
	return 0;
}