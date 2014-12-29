#ifndef ORBITALSYSTEM_H
#define ORBITALSYSTEM_H

#include <glm\glm.hpp>
#include <vector>
#include "Body.h"

/* Gravitational Constant */
#define G					6.67384E-11	/*m^3 / (kg * s^2)*/

/* Celestial Masses */
#define SUN_MASS			1.989E30	/*kg*/
#define EARTH_MASS			5.972E24	/*kg*/
#define MARS_MASS			6.39E23		/*kg*/

/* Celestial Distances */
#define EARTH_TO_SUN		1.496E11	/*m*/
#define MARS_TO_SUN			2.279E11	/*m*/

/* Celestial Radii */
#define SUN_RADIUS			6.958E8		/*m*/
#define EARTH_RADIUS		6.371E6		/*m*/
#define MARS_RADIUS			3.390E6		/*m*/


#define SCALE				1.0E-6
#define VERTEX_COUNT		50

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
