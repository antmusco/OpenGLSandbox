#include <iostream>
#include <GL/glew.h>
#include <SDL2\SDL.h>
#include <vector>
#include "Display.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera.h"
#include "Input.h"
#include "Body.h"
#include "OrbitalSystem.h"
#include <ctime>

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
	
	
	Vertex vertices[] = { Vertex(glm::vec3(-0.5, -0.5, 0), glm::vec2( -0.5, -0.5)),
						  Vertex(glm::vec3(   0,  0.5, 0), glm::vec2(    0,  1.0)),
						  Vertex(glm::vec3 (0.5, -0.5, 0), glm::vec2(  0.5, -0.5)) };
	

	unsigned int indices[] = { 0, 1, 2 };

	
	//Mesh mesh(vertices, (sizeof(vertices) / sizeof(vertices[0])), indices, sizeof(indices)/sizeof(indices[0]));
	//Mesh mesh2("./res/monkey.obj");
	Shader shader("./res/basicShader");
	//Texture texture("./res/bricks.jpg");
	Transform transform;
	//Camera camera(glm::vec3( 0, 0, -15), 70.0, (float)WIDTH / (float)HEIGHT, 0.01, 1000.0);
	Input input;

	bool closeWindow = false;

	OrbitalSystem system;
	Body sun("Sun", 0, 0, 0, 0, SUN_MASS * SCALE, SUN_RADIUS * SCALE * 12);
	Body earth("Earth", -( EARTH_TO_SUN * SCALE ), 0, 0, sqrt(G * ( SUN_MASS / EARTH_TO_SUN ) * SCALE), EARTH_MASS * SCALE, SUN_RADIUS * SCALE * 6);
	Body mars("Mars", ( MARS_TO_SUN * SCALE ), 0, 0, -sqrt(G * ( SUN_MASS / MARS_TO_SUN ) * SCALE), MARS_MASS * SCALE, SUN_RADIUS * SCALE * 6);
	
	system.addBody(mars);
	system.addBody(earth);
	system.addBody(sun);

	glScalef(0.0000025,0.00000375,0.00000375);

	while (!closeWindow)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
			closeWindow = input.handleEvent(event, transform);

		display.clear(0.0f, 0.15f, 0.3f, 1.0f);
		//shader.bind();
		//texture.bind(0);
		//shader.update(transform, camera);
		//mesh2.draw();
		//transform.getScale().x = 0.2;
		//transform.getScale().y = 0.2;
		system.update(1.0/CLOCKS_PER_SEC * 1000);
		display.update();
	}

	/* Quit SDL. */
	SDL_Quit();

	return 0;
}