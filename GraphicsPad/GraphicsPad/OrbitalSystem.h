#pragma once
/******************************************************************************
*                                                                             *
*                              Included Header Files                          *
*                                                                             *
******************************************************************************/

#include  <string>
#include  <map>

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
 *  angularPosition                                                           *
 *          RADIANS                                                           *
 *          Number of radians (0 - 2*pi) the body has rotated.                *
 *  angularVelocity                                                           *
 *          RADIANS / SECOND                                                  *
 *          Float value representing the change in angular position with      *
 *          respect to time.                                                  *
 *  angularAccel  y                                                           *
 *          RADIANS / SECOND&2                                                *
 *          Float value representing the change in angular velocity with      *
 *          respect to time.                                                  *
 *  angularThrust                                                             *
 *          RADIANS / SECOND^2                                                *
 *          Float value representing the change in angular acceleration with  *
 *          respect to time.                                                  *
 *  transformationMatrix                                                      *
 *          Matrix describing the body's current transformation, which is     *
 *          based on the current linear and angular positions of the body.    *
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
	OrbitalSystem();
	~OrbitalSystem();
};

