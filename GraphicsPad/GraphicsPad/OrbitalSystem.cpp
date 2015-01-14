#include "OrbitalSystem.h"

void OrbitalSystem::addBody(OrbitalBody* body)
{
	/* Add the pointer, mesh, and transformation. */
	bodies.push_back(body);
	meshes.push_back(body->getGeometry());
	transforms.push_back(body->getTransformation());
}

void OrbitalSystem::removeBody(const GLuint i)
{
	bodies.erase(bodies.begin() + i);
}


glm::vec3 OrbitalSystem::gravityVector(OrbitalBody* subject, glm::vec3 position)
{
	glm::vec3 netGravity(0);
	glm::vec3 direction(0);

	/* Calculate attraction to all bodies. */
	for(OrbitalBody* body : bodies)
	{
		/* Do not compare subject with itself. */
		if(body == subject)
			continue;

		/* Get the displacement vector. */
		direction = body->getLinearPosition() - position;

		/* Get the magnitude of the displacement vector. */
		GLfloat radius = glm::length(direction);
		direction /= radius;
		
		/* Get the magnitude of the force of gravity. *
		 *  -> magnitude = G * m / r^2                */
		GLfloat magnitude = (G * body->getMass()) / (radius * radius);
		
		/* Calculate gravity and apply to body. */
		netGravity += magnitude * direction;
	}
	/* Return gravity vector */
	return netGravity;
}


glm::vec3 OrbitalSystem::A(OrbitalBody* subject, const glm::vec3 position, float dt)
{
	/* Calculate the force of gravity the the body's new position. */
	glm::vec3 netAcceleration = gravityVector(subject, position);
	/* Account for thrust and return net Acceleration. */
	return netAcceleration;
	//return netAcceleration += dt * subject->getLinearThrust();
}

void OrbitalSystem::rungeKattaApprx(OrbitalBody* subject, const GLfloat dt)
{
	const GLuint  order      = 4;
	const GLfloat c          = 1.0f / 6.0f;
	glm::vec3     k[order];
	glm::vec3     l[order];
	glm::vec3     r          = subject->getLinearPosition();
	glm::vec3     v          = subject->getLinearVelocity();
	glm::vec3     a          = subject->getLinearAccel();
			     	  
	k[0]  = dt * v;
	l[0]  = dt * a;
	k[1]  = dt * (v + (l[0] / 2.0f));
	l[1]  = dt * A(subject, r + (k[0] / 2.0f), (dt / 2.0f));
	k[2]  = dt * (v + (l[1] / 2.0f));
	l[2]  = dt * A(subject, r + (k[1] / 2.0f), (dt / 2.0f));
	k[3]  = dt * (v + l[2]);
	l[3]  = dt * A(subject, r + k[2], dt);

	r += c * (k[0] + k[1] + k[2] + k[3]);
	v += c * (l[0] + l[1] + l[2] + l[3]);

	subject->setLinearPosition(r);
	subject->setLinearVelocity(v);
	subject->setGravityVector(gravityVector(subject, r));
	subject->snapshotMatrix();
}

/* Delta t is in real-time seconds. */
void OrbitalSystem::interpolate(GLfloat realSeconds)
{
	/* Convert from real time to game time. */
	GLuint dt = (GLuint) (realSeconds * SIM_SECONDS_PER_REAL_SECOND);

	/* Add the time to the global clock. */
	clock += dt;

	/* Use Runge-Katta approximation to update the state vectors. */
	for(OrbitalBody* subject : bodies) 
	{
		rungeKattaApprx(subject, dt);	
		glm::vec3 g = subject->getGravityVector();
		std::cout << subject->getName() <<" {" << g.x << ", " << g.y << ", " << g.z << "}" << std::endl; 
	}
	
}

void OrbitalSystem::cleanUp() 
{
	stars->cleanUp();
	for(OrbitalBody* body : bodies)
		body->getGeometry()->cleanUp();
}
