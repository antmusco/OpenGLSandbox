#pragma once

/******************************************************************************
*                                                                             *
*                              Included Header Files                          *
*                                                                             *
******************************************************************************/
#include  <math.h>
#include  <string>
#include  "Geometry.h"
#include  "glm\glm.hpp"
#include  "glm\gtc\matrix_transform.hpp"
#include  "glm\gtx\vector_angle.hpp"
#include  <glm\gtx\rotate_vector.hpp>
#include  <iostream>

/******************************************************************************
*                                                                             *
*                           Defined Constants / Macros                        *
*                                                                             *
******************************************************************************/
#define   DEGREES_PER_REV    360
#define   DEFAULT_ROT_AXIS   glm::vec3(+0.0f, +1.0f, +0.0f)
#define   RAD_TO_DEG         (180 / M_PI)

/******************************************************************************
 *																			  *
 *                             OrbitalBody Class                              *
 *																			  *
 ******************************************************************************
 * MEMBERS                                                                    *
 *  name                                                                      *
 *          Name of the orbital body (used for hash mapping).                 *
 *  geometry                                                                  *
 *          Mesh describing the position, color, and texture of the vertices  *
 *          to be displayed.                                                  *
 *  radius                                                                    *
 *          METERS                                                            *
 *          Bounding distance from the center of the object to its surface.   *
 *  scale                                                                     *
 *          Multiplier for the volume of this body.                           *
 *  mass                                                                      *
 *          KILOGRAMS                                                         *
 *          Amount of mass this object contains in kilograms.                 *
 *  gravityVector                                                             *
 *          (KILOGRAMS * METERS) / SECOND^2                                   *
 *          Vector which indicates the force of gravity felt by the body.     *
 *  linearPosition                                                            *
 *          METERS:                                                           *
 *          Position vector representing the location in world space.         *
 *  linearVelocity                                                            *
 *          METERS / SECOND:                                                  *
 *          Velocity vector representing the direction and magnitude of       *
 *          motion in world space.                                            *
 *  linearAccel                                                               *
 *          METERS / SECOND^2                                                 *
 *          Acceleration vector representing the direction and magnitude of   *
 *          the change in motion in world space.                              *
 *  linearThrust                                                              *
 *          (KILOGRAMS * METERS) / SECOND^2                                   *
 *          Force vector representing the direction and magnitude of the      *
 *          change in acceleration in world space.                            *
 *  rotationalAxis                                                            *
 *          Vector representing the axis of rotation for the body.            *
 *  rotationalAngle                                                           *
 *          Angle of inclination from the default rotational angle.           *
 *  angularPosition                                                           *
 *          DEGREES                                                           *
 *          Number of degrees (0 - 360) the body has rotated.                 *
 *  angularVelocity                                                           *
 *          DEGREES / SECOND                                                  *
 *          Float value representing the change in angular position with      *
 *          respect to time.                                                  *
 *  angularAccel  y                                                           *
 *          DEGREES / SECOND&2                                                *
 *          Float value representing the change in angular velocity with      *
 *          respect to time.                                                  *
 *  angularThrust                                                             *
 *          DEGREES / SECOND^2                                                *
 *          Float value representing the change in angular acceleration with  *
 *          respect to time.                                                  *
 *  transformationMatrix                                                      *
 *          Matrix describing the body's current transformation, which is     *
 *          based on the current linear and angular positions of the body.    *
 *                                                                            *
 ******************************************************************************
 * DESCRIPTION                                                                *
 *  Class representing an OrbitalBody, which is any object having mass and    *
 *  volume. Orbital bodies are part of an orbital system, which is a collect- *
 *  ion of bodies that influence each other via gravity. Orbital bodies can   *
 *  have linear and rotational postitions, velocities, accelerations, and     *
 *  thrusts, which may be altered by outside forces.                          *
 *                                                                            *
 ******************************************************************************/
class OrbitalBody
{
/* Public Memebers. */
public:

	/* Default Constructor. */
	OrbitalBody() :
		radius(0),
		scale(1),
		mass(0),  
		gravityVector(0), 
		linearPosition(0), 
		linearVelocity(0),
		linearAccel(0),
		linearThrust(0),  
		rotationalAxis(DEFAULT_ROT_AXIS),
		rotationalAngle(0),
		angularPosition(0),
		angularVelocity(0),
		angularAccel(0),
		angularThrust(0), 
		transMatrix(0)                            {}

	/************************************************************************** 
	 *  Calculate the current transformation matrix based upon the object's   *
	 *  linear and angular position.                                          *
	 *************************************************************************/
	void snapshotMatrix()           
	{
		/* Scale the body. */
		glm::mat4 scaleM          = glm::scale(glm::mat4(), 
                                               scale);
		/* Rotate the body. */
		glm::mat4 rotM;
		
		glm::vec3 cross = glm::cross(DEFAULT_ROT_AXIS, rotationalAxis);

		/* Rotate to angle of inclination. */
		if(rotationalAxis != DEFAULT_ROT_AXIS)
			rotM                  = glm::rotate(rotationalAngle, cross);
		
		rotM                      = glm::rotate(rotM, 
                                                angularPosition,
                                                DEFAULT_ROT_AXIS);	       
		/* Translate the body. */
		glm::mat4 tranM           = glm::translate(glm::mat4(), 
                                                   linearPosition);
		transMatrix   = tranM * rotM * scaleM ;
	}

	/************************************************************************** 
	 *  Step forward in time by dt seconds and calculate the updated values   *
	 *  for the position, velocity, and acceleration of the body.             *
	 *************************************************************************/
	void increment(GLfloat dt)
	{
		/* If the mass of the body is 0, do nothing. */
		if (mass == 0) return;

		/* Translational parameters. */
		linearAccel     += dt * (linearThrust / mass);
		linearVelocity  += dt * linearAccel + (gravityVector / mass);
		linearPosition  += dt * linearVelocity;

		/* Rotational parameters. */
		angularAccel    += dt * (angularThrust / mass);
		angularVelocity += dt * angularAccel;
		angularPosition += dt * angularVelocity;

		/* Account for full revolution. */
		if(angularPosition > DEGREES_PER_REV) 
			angularPosition -= DEGREES_PER_REV;
	}
												  
	/* Getters. */			
	std::string    getName()            const     {  return name;            }
	Mesh*          getGeometry()        const     {  return geometry;        }
	GLfloat        getRadius()          const     {  return radius;          }
	glm::vec3      getScale()           const     {  return scale;           }
	GLfloat        getMass()            const     {  return mass;            }
	glm::vec3      getGravityVector()   const     {  return gravityVector;   }
	glm::vec3      getLinearPosition()  const     {  return linearPosition;  }
	glm::vec3      getLinearVelocity()  const     {  return linearVelocity;  }
	glm::vec3      getLinearAccel()     const     {  return linearAccel;     }
	glm::vec3      getLinearThrust()    const     {  return linearThrust;    }
	glm::vec3      getRotationalAxis()  const     {  return rotationalAxis;  }
	GLfloat        getAngularPosition() const     {  return angularPosition; }
	GLfloat        getAngularVelocity() const     {  return angularVelocity; }
	GLfloat        getAngularAccel()    const     {  return angularAccel;    }
	GLfloat        getAngularThrust()   const     {  return angularThrust;   }
	glm::mat4*     getTransformation()            {  return &transMatrix;    }
												  
	/* Setters. */			
	void           setName(std::string n)         {  name              = n;  }
	void           setGeometry(Mesh* g)           {  geometry          = g;  }
	void           setRadius(GLfloat r)           {  radius            = r;  }
	void           setScale(glm::vec3 s)          {  scale             = s;  }
	void           setMass(GLfloat m)             {  mass              = m;  }
	void           setGravityVector(glm::vec3 g)  {  gravityVector     = g;  }
	void           setLinearPosition(glm::vec3 p) {  linearPosition    = p;  }
	void           setLinearVelocity(glm::vec3 v) {  linearVelocity    = v;  }
	void           setLinearAccel(glm::vec3 a)    {  linearAccel       = a;  }
	void           setLinearThrust(glm::vec3 t)   {  linearThrust      = t;  }
	void           setRotationalAxis(GLfloat tilt) 
	{  
		rotationalAngle = tilt * 3.14f / 180.0f;
		rotationalAxis  = glm::rotateX(DEFAULT_ROT_AXIS, rotationalAngle);
	}
	void           setAngularPosition(GLfloat p)  
	{  
		if(p < 2 * M_PI)
			angularPosition = p; 
		else
			angularPosition = p - ((GLfloat) (2 * M_PI));
	}
	void           setAngularVelocity(GLfloat v)  {  angularVelocity   = v;  }
	void           setAngularAccel(GLfloat a)     {  angularAccel      = a;  }
	void           setAngularThrust(GLfloat t)    {  angularThrust     = t;  }

	/* Destructor. */
	~OrbitalBody()                                {                          }

/* Protected Members. */
protected:
	/* Name of the body. */
	std::string    name;
	/* Mesh describing the geometry of the body. */
	Mesh*          geometry;
	/* Bounding radius of the orbital body. */
	GLfloat        radius;
	/* Scale of x, y, and z dimensions of the body. */
	glm::vec3      scale;
	/* Mass of the body. */
	GLfloat        mass;
	/* Force of gravity felt by the body. */
	glm::vec3      gravityVector;
	/* Position of the body in 3-D space. */
	glm::vec3      linearPosition;
	/* Velocity vector of the body in METERS PER SECOND. */
	glm::vec3      linearVelocity;
	/* Linear thrust vector on the body. */
	glm::vec3      linearThrust;
	/* Linear acceleration vector on the body. */
	glm::vec3      linearAccel;
	/* Axis of rotation of the body. */
	glm::vec3      rotationalAxis; 
	/* Angle offset from the default axis of rotation. */
	GLfloat        rotationalAngle;
	/* Angular offset of the body. */
	GLfloat        angularPosition;
	/* Angular velocity of the body in RADIANS PER SECOND. */
	GLfloat        angularVelocity;
	/* Angular acceleration vector on the body. */
	GLfloat        angularAccel; 
	/* Angular thrust vector on the body. */
	GLfloat        angularThrust;

	/* Mesh transformation data. */
	glm::mat4      transMatrix;
	Mesh*          trail;

};
