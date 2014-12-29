#ifndef ORBITALSYSTEM_H
#define ORBITALSYSTEM_H

#include <glm\glm.hpp>
#include <vector>
#include "Body.h"

#define G 1 //6.67384E-11
#define VERTEX_COUNT 50

class OrbitalSystem
{
public:
	OrbitalSystem();
	~OrbitalSystem();
	void addBody(Body body);
	void update(double delta_t);
private:
	std::vector<Body> bodies;
};

#endif
