#pragma once

#include  "Geometry.h"
#include  "glm\glm.hpp"

/******************************************************************************
 *																			  *
 *                             OrbitalBody Class                              *
 *																			  *
 ******************************************************************************
*  aspectRatio                                                                *
*          Ratio describing the width/height for the viewport.                *
*                                                                             *
*******************************************************************************
* DESCRIPTION                                                                 *
*  Class representing an OrbitalBody, which is any object having mass and     *
*  volume.                                                                    *
*                                                                             *
*******************************************************************************/
class OribialBody
{
/* Public Memebers. */
public:

	/* Constructor. */


private:

	/* Mesh describing the geometry of the body. */
	Mesh*          geometry;
	/* Mass of the body. */
	GLuint         mass;
	/* Velocity vector of the body. */
	glm::vec3      velocity;
	/* Force of gravity felt by the body. */
	glm::vec3      gravity;
	/* Thrust vector on the body. */
	glm::vec3      thrust;
	/* Axis of rotation of the body. */
	glm::vec3      rotationalAxis;

	/* Mesh transformation data. */
	glm::mat4      transformationMatrix;
	glm::vec3      rotationVector;
	glm::vec3      translationVector;
	glm::vec3      scaleVector;

};

