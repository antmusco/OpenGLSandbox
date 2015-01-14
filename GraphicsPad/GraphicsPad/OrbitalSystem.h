#pragma once
/******************************************************************************
*                                                                             *
*                              Included Header Files                          *
*                                                                             *
******************************************************************************/

#include  <string>
#include  <map>
#include  <vector>
#include  <glm\glm.hpp>
#include  <GL\glew.h>
#include  "OrbitalBody.h"
#include  "Geometry.h"

#define   SIM_SECONDS_PER_REAL_SECOND             60.0f
#define   SECONDS_PER_HOUR                      3600.0f
#define   MAX_DELTA_T                            100.0f                
#define   G                                6.67384e-20f
#define   STARS_OBJ                      "res/body.obj"
#define   STARS_TEXTURE              "res/milkyway.jpg"
#define   STARS_SCALE                          10000.0f

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
	OrbitalSystem() : clock(0)
	{
		/* Initialize the stars. */
		stars = Geometry::loadObj(STARS_OBJ, STARS_TEXTURE);
		meshes.push_back(stars);
		starsMatrix = glm::scale(glm::mat4(), glm::vec3(STARS_SCALE));
		transforms.push_back(&starsMatrix);
	}

	/* Add a body to the system. */
	void                      addBody          (      OrbitalBody* body       );

	
	/* Remove a body from the system given its name. */
	void                      removeBody       (const GLuint       i          );
	
	/* Update the system by incrementing the time until seconds have passed. */
	void                      interpolate      (const GLfloat      seconds    );
	
	/* Calculate the gravitational forces felt by each body. */
	glm::vec3                 gravityVector    (      OrbitalBody* subject,      
	                                                  glm::vec3    position  );


	glm::vec3                 A                (      OrbitalBody* subject, 
	                                            const glm::vec3    position, 
	                                            const GLfloat      dt         );
	
	/* Approximation of the change in variables using Runge-Katta method. */
	void                      rungeKattaApprx  (      OrbitalBody* subject, 
	                                            const GLfloat      t          );

	/* Remove all of the allocated space. */
	void                      cleanUp();

	/* Getters. */
	GLuint                    t()               const  {  return clock;        }
	OrbitalBody*              getBody(GLuint i)        {  return bodies.at(i); }
	std::vector<Mesh*>        getMeshes()       const  {  return meshes;       }
	std::vector<glm::mat4*>   getTransforms()   const  {  return transforms;   }

private:
	
	/* Collection of orbital bodies in this system. */
	GLuint                    clock;
	std::vector<OrbitalBody*> bodies;
	Mesh*                     stars;
	glm::mat4                 starsMatrix;
	std::vector<Mesh*>        meshes;
	std::vector<glm::mat4*>   transforms;
};

