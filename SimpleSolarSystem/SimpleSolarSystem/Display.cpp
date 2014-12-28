#include "Display.h"
#include <iostream>
#include <GL/glew.h>

/**
 * Constructor for a display using SDL and Glew.
 *
 */
Display::Display(int width, int height, const std::string& title)
{
	/* Set the red, green, blue, alpha, and buffer bit size. */
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	/* Create the window. */
	m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	/* Create the GL context. */
	m_glContext = SDL_GL_CreateContext(m_window);

	GLenum status = glewInit();
	if (status != GLEW_OK)
	{
		std::cerr << "Glew failed to initialize!" << std::endl;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}


Display::~Display()
{
	/* Delete the context. */
	SDL_GL_DeleteContext(m_glContext);
	/* Destroy the window. */
	SDL_DestroyWindow(m_window);
}

void Display::clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Display::update()
{
	SDL_GL_SwapWindow(m_window);
}
