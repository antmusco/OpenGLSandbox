#include <iostream>
#include <GL/glew.h>
#include <SDL2\SDL.h>
#include "Display.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera.h"
#include "Input.h"

/**
 * Width of the display.
 */
#define WIDTH 800

/**
 * Height of the display.
 */
#define HEIGHT 600

/**
 * Main program.
 *
 * @param argc
 *		Count of the string arguments.
 * @param argv
 *		Array of string arguments.
 */
int main(int argc, char** argv)
{
	/* Initialize the SDL. */
	SDL_Init(SDL_INIT_EVERYTHING);

	/* Create Display */
	Display display(800, 600, "New Window");
	
	/*
	Vertex vertices[] = { Vertex(glm::vec3(-0.5, -0.5, 0), glm::vec2( -0.5, -0.5)),
						  Vertex(glm::vec3(   0,  0.5, 0), glm::vec2(    0,  1.0)),
						  Vertex(glm::vec3 (0.5, -0.5, 0), glm::vec2(  0.5, -0.5)) };
	

	unsigned int indices[] = { 0, 1, 2 };

	Mesh mesh(vertices, (sizeof(vertices) / sizeof(vertices[0])), indices, sizeof(indices)/sizeof(indices[0]));
	*/
	Mesh mesh2("./res/monkey.obj");
	Shader shader("./res/basicShader");
	Texture texture("./res/bricks.jpg");
	Transform transform;
	Camera camera(glm::vec3( 0, 0, -5), 70.0, (float)WIDTH / (float)HEIGHT, 0.01, 1000.0);
	Input input;

	float mouseOriginX = 0.0;
	float mouseOriginY = 0.0;
	float mouseOffsetX = 0.0;
	float mouseOffsetY = 0.0;
	int counter = 0;
	bool closeWindow = false;
	bool dragging = false;

	while (!closeWindow)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
			closeWindow = input.handleEvent(event, transform);

		//std::cout << "Transform X: " << transform.getPos().x << " Y: " << transform.getPos().y  << std::endl;

		display.clear(0.0f, 0.15f, 0.3f, 1.0f);
		shader.bind();
		texture.bind(0);
		shader.update(transform, camera);
		mesh2.draw();

		display.update();
	}

	/* Quit SDL. */
	SDL_Quit();

	return 0;
}