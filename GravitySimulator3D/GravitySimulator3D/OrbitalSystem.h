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

#define   SIM_SECONDS_PER_REAL_SECOND                            1.0f
#define   SECONDS_PER_HOUR                                    3600.0f
#define   MAX_DELTA_T                                          100.0f                
#define   DEFAULT_G                                      6.67384e-20f
#define   DEFAULT_TILT_AXIS            glm::vec3{+1.0f, +0.0f, +0.0f}

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

	/* Custom constructor. */
	OrbitalSystem(const char* objFile,
		          const char* textureFile,
				  const GLfloat starsScale) : G(DEFAULT_G), clock(0), scale(1)
	{
		/* Initialize the stars. */
		stars = Geometry::loadObj(objFile, textureFile);
		Vertex* vertices = stars->getVertices();
		for(unsigned int i = 0; i < stars->getNumVertices(); i++)
		{
			std::cout << "Old normal: {" 
					  << vertices[i].normal.x << ", "
					  << vertices[i].normal.y << ", "
					  << vertices[i].normal.z << "} ";
			vertices[i].normal = -glm::normalize(vertices[i].position);
			std::cout << "New normal: {" 
					  << vertices[i].normal.x << ", "
					  << vertices[i].normal.y << ", "
					  << vertices[i].normal.z << "} "
					  << std::endl;
		}

		meshes.push_back(stars);
		starsMatrix = glm::scale(glm::mat4(), glm::vec3(starsScale));
		transforms.push_back(&starsMatrix);
	}

	OrbitalSystem(const OrbitalSystem& rhs);

	/* Load an orbital system from a file. */
	static OrbitalSystem      loadFile         (const char*        xmlFile    );

	/* Add a body to the system. */
	void                      addBody          (      OrbitalBody* body       );
	
	/* Remove a body from the system given its name. */
	void                      removeBody       (const GLuint       i          );
	
	/* Adjust the gravity vector for each body in the system. */
	void                      compute          (                              );
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
	GLfloat                   getG()            const  {  return G;            }
	GLfloat                   t()               const  {  return clock;        }
	OrbitalBody*              getBody(GLuint i)        {  return bodies.at(i); }
	std::vector<Mesh*>        getMeshes()       const  {  return meshes;       }
	std::vector<glm::mat4*>   getTransforms()   const  {  return transforms;   }
	glm::mat4                 getStarsMatrix()  const  {  return starsMatrix;  }
	Mesh*                     getStars()        const  {  return stars;        }

protected:
	
	/* Private default constructor (used for loading xml file).*/
	OrbitalSystem() :
	G(0.0f), clock(0), stars(nullptr) {}

	/* Collection of orbital bodies in this system. */
	GLfloat                   G;
	GLfloat                   clock;
	GLfloat                   scale;
	std::vector<OrbitalBody*> bodies;
	Mesh*                     stars;
	glm::mat4                 starsMatrix;
	std::vector<Mesh*>        meshes;
	std::vector<glm::mat4*>   transforms;
};

