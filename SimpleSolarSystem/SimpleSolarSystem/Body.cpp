#include "Body.h"
#include <glm\glm.hpp>
#include <GL\glew.h>
#include "OrbitalSystem.h"

Body::Body(const std::string name, int xPos, int yPos, int xVel, int yVel, float mass, float radius)
{
	this->name = name;
	position[0] = xPos;
	position[1] = yPos;
	velocity[0] = xVel;
	velocity[1] = yVel;
	force[0] = 0;
	force[1] = 0;
	this->mass = mass;
	this->radius = radius;
}

Body::~Body()
{
}

void Body::calculateForce(std::vector<Body> bodies)
{
	float delta_x = 0, delta_y = 0, distance = 0;
	glm::vec2 force_sum (0.0, 0.0);

	for (int i = 0; i < bodies.size(); i++)
	{
		glm::vec2 unit_vector(0.0, 0.0);
		if (&bodies[i] == this)
			continue;
		delta_x = bodies[i].getPosition()[0] - position[0];
		delta_y = bodies[i].getPosition()[1] - position[1];
		distance = sqrt(delta_x * delta_x + delta_y * delta_y);
		if (distance == 0) distance = 1;		
		unit_vector[0] += delta_x / distance;
		unit_vector[1] += delta_y / distance;

		double magnitude = (G * bodies[i].getMass()) / (distance * distance);

		force_sum[0] += unit_vector[0] * magnitude;
		force_sum[1] += unit_vector[1] * magnitude;
	}
	force = force_sum;
}

void Body::move(float delta_t)
{
	position[0] += velocity[0] * delta_t;
	position[1] += velocity[1] * delta_t;
	velocity[0] += force[0] * delta_t;
	velocity[1] += force[1] * delta_t;
}

void Body::draw()
{
	if (name == "Earth")
		glColor3f(0, 0.5, 0);
	else
		glColor3f(1, 0.9, 0);
	glBegin(GL_POLYGON);
	for (int j = 0; j < VERTEX_COUNT; j++)
	{
		float angle = ( (double)j / VERTEX_COUNT ) * ( 2 * 3.1415 );
		float x = position[0] + (cosf(angle) * radius);
		float y = position[1] + (sinf(angle) * radius);
		glVertex2f(x, y);

	}
	glEnd();
}
