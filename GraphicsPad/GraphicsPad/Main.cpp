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
	SDL_PollEvent(&event);
	GLfloat rot = 0.0, xPos = 0.0, yPos = 0.0, zPos = 0.0;

	glm::mat4 transMatrix;
	glm::mat4 transfomration;
	std::clock_t t = clock();
	while (event.type != SDL_QUIT)
	{
		if (event.type == SDL_MOUSEMOTION)
		{
			camera->updateLookAt({event.motion.x, event.motion.y});
		}
		if (( clock() - t ) % 10)
		{
			float xPos = 2 * std::sinf(rot);
			float zPos = 2 * std::cosf(rot) - 5;
			transMatrix = glm::translate(glm::vec3(xPos, 0.0f, zPos));
			transfomration = glm::rotate(transMatrix, rot, glm::vec3(1.0f, 0.5f, 0.0f));
			display.repaint(shader.getProgram(), cube.numIndices, &transfomration);
			rot += 0.001;
		}
		SDL_PollEvent(&event);
	}

	cube.cleanUp();

	/* Quit using SDL. */
	SDL_Quit();
	return 0;
}