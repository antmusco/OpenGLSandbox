#ifndef INPUT_H
#define INPUT_H
#define DAMPENING 100;

#include "Transform.h"
#include <SDL2\SDL.h>

class Input
{
public:
	Input();
	~Input();
	bool handleEvent(SDL_Event event, Transform& transform);
protected:
private:
	double mouseCurrentX = 0.0, mouseCurrentY = 0.0;
	double clickOriginX = 0.0, clickOriginY = 0.0;
	enum TransformType
	{
		NONE, PAN, SCALE, ROTATE
	};
	TransformType transformType = NONE;
	bool panning = false, scaling = false, rotating = false;
};

#endif // INPUT_H