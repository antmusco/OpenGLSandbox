#include "Input.h"
#include <iostream>
#include <glm\glm.hpp>

Input::Input()
{
}


Input::~Input()
{
}

bool Input::handleEvent(SDL_Event event, Transform& transform)
{
	double scale = 0.0;
	glm::vec3 *scaleVector = NULL;

	switch (event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		clickOriginX = event.button.x;
		clickOriginY = event.button.y;
		mouseCurrentX = clickOriginX;
		mouseCurrentY = clickOriginY;
		switch (event.button.button) 
		{
		case SDL_BUTTON_LEFT: /* Left button click. */
			transformType = PAN;
			break;
		case SDL_BUTTON_RIGHT: /* Right button click. */
			transformType = ROTATE;
			break;
		}
		break;
	case SDL_MOUSEBUTTONUP:
		switch (transformType)
		{
		case PAN:
			transform.getPos().x -= ( clickOriginX - mouseCurrentX ) / DAMPENING;
			transform.getPos().y -= ( clickOriginY - mouseCurrentY ) / DAMPENING;
			break;
		case SCALE:
			break;
		case ROTATE:
			transform.getRot().x += ( clickOriginY - mouseCurrentY ) / DAMPENING;
			transform.getRot().y += ( clickOriginX - mouseCurrentX ) / DAMPENING;
			break;
		}
		transformType = NONE;
		break;
	case SDL_MOUSEMOTION:
		mouseCurrentX = event.motion.x;
		mouseCurrentY = event.motion.y;
		switch (transformType)
		{
		case PAN:
			transform.getPos().x = ( clickOriginX - mouseCurrentX ) / DAMPENING;
			transform.getPos().y = ( clickOriginY - mouseCurrentY ) / DAMPENING;
			break;
		case SCALE:
			break;
		case ROTATE:
			transform.getRot().x = -( clickOriginY - mouseCurrentY ) / DAMPENING;
			transform.getRot().y = -( clickOriginX - mouseCurrentX ) / DAMPENING;
			break;
		}
		break;
	case SDL_MOUSEWHEEL:
		scale = (double) event.wheel.y;
		scaleVector = &transform.getScale();
		if (scale < 0)
		{
			scaleVector->x *= -(scale / 1.1);
			scaleVector->y *= -(scale / 1.1);
			scaleVector->z *= -(scale / 1.1);
		}
		else
		{
			scaleVector->x *= 1.1;
			scaleVector->y *= 1.1;
			scaleVector->z *= 1.1;
		}
		break;
	case SDL_WINDOWEVENT:
		switch (event.window.type)
		{
		case SDL_WINDOWEVENT_LEAVE:
			transformType = NONE;
		}
		break;
	case SDL_QUIT:
		return true;
	}
	return false;
}
