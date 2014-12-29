#pragma once
#include <SDL\SDL.h>
#include <GL\GL.h>
#include <string>

///
///
///
class Display
{
public:
	Display(std::string title, GLushort width, GLushort height);
	~Display();
	void repaint(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
private:
	SDL_Window		*window;
	SDL_GLContext	context;
};

