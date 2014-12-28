#include "OrbitalSystem.h"
#include <iostream>

OrbitalSystem::OrbitalSystem()
{
}


OrbitalSystem::~OrbitalSystem()
{
}

void OrbitalSystem::addBody(Body body)
{
	bodies.push_back(body);
}

void OrbitalSystem::update(double delta_t)
{
	for (int i = 0; i < bodies.size(); i++)
	{
		bodies[i].calculateForce(bodies);
		bodies[i].draw();
		bodies[i].move(delta_t);
	}
}