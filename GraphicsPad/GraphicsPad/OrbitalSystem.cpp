#include "OrbitalSystem.h"

void OrbitalSystem::addBody(OrbitalBody* body)
{
	/* Add the name of the body to the list of keys. */
	names.push_back(body->getName());
	meshes.push_back(body->getGeometry());
	transforms.push_back(body->getTransformation());
	/* Add the reference to the list of bodies. */
	bodies[body->getName()] = body;
}

void OrbitalSystem::removeBody(std::string name)
{
	/* Find the index of the name. */
	int i;
	for(i = 0; i < names.size(); i++)
		if(names.at(i) == name)
			break;

	/* If the index was found, erase it from the map list. */
	if( i < names.size()) 
	{
		bodies[name] = NULL;
		names.erase(names.begin() + i);
	}
}

/* Delta t is in real-time seconds. */
void OrbitalSystem::interpolate(GLfloat realSeconds)
{
	GLfloat gameSeconds = realSeconds * 
		                  SIM_HOURS_PER_REAL_SECOND * 
						  SECONDS_PER_HOUR;

	int steps     = (int) (gameSeconds / MAX_DELTA_T);
	int remainder = (int) gameSeconds - (steps * MAX_DELTA_T);

	for(int i = 0; i < steps; i++)
	{
		calculateForces();
		for(std::string n : names) 
			bodies[n]->increment(MAX_DELTA_T);
	}
	calculateForces();
	for(std::string n : names) 
	{
		bodies[n]->increment(remainder);
		bodies[n]->snapshotMatrix();	
	}
}

void OrbitalSystem::calculateForces()
{
	/* Declare local variables. */
	OrbitalBody* currentBody = NULL;
	OrbitalBody* testBody    = NULL;
	glm::vec3    gravitySum;
	
	for(int i = 0; i < names.size(); i++)
	{
		/* Get the next body and reset the gravity sum. */
		currentBody = bodies[names.at(i)];
		gravitySum = {0.0f, 0.0f, 0.0f};

		/* Calculate attraction to all bodies. */
		for(int j = 0; j < names.size(); j++)
		{
			/* Skip calculation for same body. */
			if( i == j) continue;

			/* Get the next body to calculate the force. */
			testBody = bodies[names.at(j)];

			/* Get the displacement vector. */
			glm::vec3 dis = testBody->getLinearPosition() - 
				            currentBody->getLinearPosition();

			/* Get the magnitude of the displacement vector. */
			GLfloat radius = glm::length(dis);
			dis /= radius;
		    /* F_g = G * m_1 * m_2 / r^2 */
			/* Instead of normalizing, divide by radius cubed. */
			gravitySum += dis * (G * currentBody->getMass() * testBody->getMass()) /
				(radius * radius);
		}

		/* Set new gravity vector; */
		currentBody->setGravityVector(gravitySum);
	}
}

void OrbitalSystem::cleanUp() 
{
	for(std::string n : names)
		bodies[n]->getGeometry()->cleanUp();
}
