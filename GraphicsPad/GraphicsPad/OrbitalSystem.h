#pragma once
/******************************************************************************
*                                                                             *
*                              Included Header Files                          *
*                                                                             *
******************************************************************************/

#include  <string>
#include  <map>
#include  <vector>
#include  "OrbitalBody.h"
#include  "GL\glew.h"
#include  "Geometry.h"

#define   SIM_HOURS_PER_REAL_SECOND          0.005f
#define   SECONDS_PER_HOUR                  3600.0f
#define   MIN_SECONDS                        100.0f                
#define   G                            6.67384e-11f
#define   STARS_OBJ                  "res/body.obj"
#define   STARS_TEXTURE             "res/stars.bmp"

/******************************************************************************
 *																			  *
 *                            OrbitalSystem Class                             *
 *																			  *
 ******************************************************************************
 * MEMBERS                                                                    *
 *  geometry                                                                  *
 *          Mesh describing the position, color, and texture of the vertices  *
 *          to be displayed.                                                  *
 *  radius                                                                    *
 *          METERS                                                            *
 *          Bounding distance from the center of the object to its surface.   *
 *                                                                            *
 ******************************************************************************
 * DESCRIPTION                                                                *
 *  Class representing an Orbital System, which is a collection of orbital    *
 *  bodies. This system manages the interactions between bodies via gravity   *
 *  and collisions. This class deines several orbital system constants used   *
 *  by the orbital bodies to simulate physics.                                *
 *                                                                            *
 ******************************************************************************/
class OrbitalSystem
{
/* Public Members. */
public:

	/* Default constructor. */
	OrbitalSystem() 
	{
		stars = Geometry::loadObj(STARS_OBJ, STARS_TEXTURE);
	}

	void           addBody(OrbitalBody* body);
	void           removeBody(std::string name);
	void           interpolate(GLfloat seconds);
	void           calculateForces();
	std::vector<std::string>*  getNames()   { return &names;    }
	OrbitalBody*   getBody(std::string name) { return bodies[name]; }
	GLuint         starsRadius = 1000;

private:
	
	/* Collection of orbital bodies in this system. */
	std::map<std::string,OrbitalBody*> bodies;
	/* Collection of names for the orbital bodies in this system.*/
	std::vector<std::string>           names;
	Mesh*                              stars;

};

