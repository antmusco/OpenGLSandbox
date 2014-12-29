#include <gl\glew.h>
#include <iostream>
#include "Display.h"

Display::Display(std::string title, GLushort width, GLushort height)
{
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	context = SDL_GL_CreateContext(window);
	GLenum status = glewInit();
	if (status != GLEW_OK)
	{
		std::cerr << "Glew failed to initialize!" << std::endl;
		std::cerr << "Status: " << glewGetErrorString(status) << std::endl;
	}
	fprintf(stdout, "Stats: Using GLEW %s\n", glewGetString(GLEW_VERSION));
}

void Display::repaint(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
	glClearColor(red, green, blue, alpha);
	glClear(GL_COLOR_BUFFER_BIT);
	GLint w, h;
	SDL_GetWindowSize(window, &w, &h);
	glViewport(0, 0, w, h);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	SDL_GL_SwapWindow(window);
}

Display::~Display()
{
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
}
