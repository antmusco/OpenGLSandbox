#include "OrbitalSystem.h"
#include "tinyxml2.h"
#include <iostream>
#include "Planet.h"


OrbitalSystem::OrbitalSystem(const OrbitalSystem& rhs) :
	  G(rhs.getG()), clock(rhs.t()), starsMatrix(rhs.getStarsMatrix())
{
	stars = new Mesh(*rhs.stars);
	for(OrbitalBody* b : rhs.bodies)
		bodies.push_back(new OrbitalBody(*b));
	
	meshes.push_back(stars);
	for(int i = 0; i < bodies.size(); i++)
		meshes.push_back(bodies.at(i)->getGeometry());

	transforms.push_back(&starsMatrix);
	for(int i = 0; i < bodies.size(); i++)
		transforms.push_back(bodies.at(i)->getTransformation());
}

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
	}
	
}

OrbitalSystem OrbitalSystem::loadFile(const char* xmlFile)
{
	//OrbitalSystem newSystem("res/meshes/body.obj", "res/textures/milkyway.jpg", 1.000e5f);
	OrbitalSystem newSystem;

	if(true) {
		tinyxml2::XMLDocument doc;
		if(!doc.LoadFile(xmlFile)) 
		{
			tinyxml2::XMLElement* root   = doc.RootElement();

			const char* g     = root->FirstChildElement("g")->GetText();
			const char* scale = root->FirstChildElement("scale")->GetText();
			newSystem.scale   = (GLfloat) atof(scale);
			newSystem.G       = (GLfloat) atof(g) / newSystem.scale;

			tinyxml2::XMLElement* background = root->FirstChildElement("background");
			const char* backMeshFile = background->FirstChildElement("meshFile")->GetText();
			const char* backTextFile = background->FirstChildElement("textureFile")->GetText();
			const char* backRadius   = background->FirstChildElement("radius")->GetText();
			
			newSystem.stars = Geometry::loadObj(backMeshFile, backTextFile);
			newSystem.meshes.push_back(newSystem.stars);
			newSystem.starsMatrix = glm::scale(glm::mat4(), glm::vec3(atof(backRadius)));
			newSystem.transforms.push_back(&newSystem.starsMatrix);
			
			tinyxml2::XMLElement* center = root->FirstChildElement("center")->FirstChildElement("body");
			const char* centerName     = center->FirstChildElement("name")->GetText();
			const char* centerMass     = center->FirstChildElement("mass")->GetText();
			const char* centerRadius   = center->FirstChildElement("radius")->GetText();
			const char* centerMeshFile = center->FirstChildElement("meshFile")->GetText();
			const char* centerTextFile = center->FirstChildElement("textureFile")->GetText();
			const char* centerPosX     = center->FirstChildElement("position")->FirstChildElement("x")->GetText();
			const char* centerPosY     = center->FirstChildElement("position")->FirstChildElement("y")->GetText();
			const char* centerPosZ     = center->FirstChildElement("position")->FirstChildElement("z")->GetText();
			const char* centerVelX     = center->FirstChildElement("velocity")->FirstChildElement("x")->GetText();
			const char* centerVelY     = center->FirstChildElement("velocity")->FirstChildElement("y")->GetText();
			const char* centerVelZ     = center->FirstChildElement("velocity")->FirstChildElement("z")->GetText();
			const char* centerTilt     = center->FirstChildElement("tilt")->GetText();
			const char* centerRotSpeed = center->FirstChildElement("rotationalSpeed")->GetText();

			GLfloat   cm = (GLfloat) atof(centerMass) / newSystem.scale;
			GLfloat   cr = (GLfloat) atof(centerRadius) / newSystem.scale;
			glm::vec3 cp = glm::vec3(atof(centerPosX), atof(centerPosY), atof(centerPosZ)) / newSystem.scale;
			glm::vec3 cv = glm::vec3(atof(centerVelX), atof(centerVelY), atof(centerVelZ)) / sqrt(newSystem.scale);

			newSystem.addBody(new Planet(centerName, cm, cr, centerMeshFile, centerTextFile, cp, cv));
		
			tinyxml2::XMLElement* bodies = root->FirstChildElement("bodies");
			for(tinyxml2::XMLElement* body = bodies->FirstChildElement("body"); body != NULL; body = body->NextSiblingElement("body"))
			{
				const char* bodyName     = body->FirstChildElement("name")->GetText();
				const char* bodyMass     = body->FirstChildElement("mass")->GetText();
				const char* bodyRadius   = body->FirstChildElement("radius")->GetText();
				const char* bodyMeshFile = body->FirstChildElement("meshFile")->GetText();
				const char* bodyTextFile = body->FirstChildElement("textureFile")->GetText();
				const char* bodyPosX     = body->FirstChildElement("position")->FirstChildElement("x")->GetText();
				const char* bodyPosY     = body->FirstChildElement("position")->FirstChildElement("y")->GetText();
				const char* bodyPosZ     = body->FirstChildElement("position")->FirstChildElement("z")->GetText();
				const char* bodyVelX     = body->FirstChildElement("velocity")->FirstChildElement("x")->GetText();
				const char* bodyVelY     = body->FirstChildElement("velocity")->FirstChildElement("y")->GetText();
				const char* bodyVelZ     = body->FirstChildElement("velocity")->FirstChildElement("z")->GetText();
				const char* bodyTilt     = body->FirstChildElement("tilt")->GetText();
				const char* bodyRotSpeed = body->FirstChildElement("rotationalSpeed")->GetText();
				
				GLfloat   bm = (GLfloat) atof(bodyMass) / newSystem.scale;
				GLfloat   br = (GLfloat) atof(bodyRadius) / newSystem.scale;
				std::cout << bodyPosX << std::endl;
				glm::vec3 bp = glm::vec3(atof(bodyPosX), atof(bodyPosY), atof(bodyPosZ)) / newSystem.scale;
			    glm::vec3 bv = glm::vec3(atof(bodyVelX), atof(bodyVelY), atof(bodyVelZ)) / sqrt(newSystem.scale);

				newSystem.addBody(new Planet(bodyName, bm, br, bodyMeshFile, bodyTextFile, bp, bv));
			}

		}
	}
	return newSystem;
}


void OrbitalSystem::cleanUp() 
{
	stars->cleanUp();
	for(OrbitalBody* body : bodies)
		body->getGeometry()->cleanUp();
}
