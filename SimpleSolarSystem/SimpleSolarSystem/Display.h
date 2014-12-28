#pragma once
#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2\SDL.h>
#include <string>

class Display
{
public:
	Display(int width, int height, const std::string& title);
	~Display();
	void update();
	void clear(float r, float g, float b, float a);
protected:

private:
	SDL_Window *m_window;
	SDL_GLContext m_glContext;
};

#endif // DISPLAY_H
