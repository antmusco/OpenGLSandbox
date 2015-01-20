#include "OrbitalSystem.h"
#include "tinyxml2.h"
#include <glm\gtx\rotate_vector.hpp>
#include <iostream>
#include "Planet.h"


OrbitalSystem::OrbitalSystem(const OrbitalSystem& rhs) :
	  G(rhs.getG()), clock(rhs.t()), starsMatrix(rhs.getStarsMatrix())
{
	stars = new Mesh(*rhs.stars);
	for(OrbitalBody* b : rhs.bodies)
		bodies.push_back(new OrbitalBody(*b));
	
	meshes.push_back(stars);
	for(unsigned int i = 0; i < bodies.size(); i++)
		meshes.push_back(bodies.at(i)->getGeometry());

	transforms.push_back(&starsMatrix);
	for(unsigned int i = 0; i < bodies.size(); i++)
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
			     	  
	k[0]  = dt * v;
	l[0]  = dt * A(subject, r, 0);
	k[1]  = dt * (v + (0.5f * l[0]));
	l[1]  = dt * A(subject, r + (0.5f * k[0]), (0.5f * dt));
	k[2]  = dt * (v + (0.5f * l[1]));
	l[2]  = dt * A(subject, r + (0.5f * k[1]), (0.5f * dt));
	k[3]  = dt * (v + l[2]);
	l[3]  = dt * A(subject, r + k[2], dt);

	r += c * (k[0] + k[1] + k[2] + k[3]);
	v += c * (l[0] + l[1] + l[2] + l[3]);

	subject->setLinearPosition(r);
	subject->setLinearVelocity(v);
	subject->setGravityVector(gravityVector(subject, r));
	subject->setAngularPosition(subject->getAngularPosition() + subject->getAngularVelocity() * dt);
	subject->snapshotMatrix();
}

/* Delta t is in real-time seconds. */
void OrbitalSystem::interpolate(GLfloat realSeconds)
{
	/* Convert from real time to game time. */
	GLfloat dt = (GLfloat) (realSeconds * SIM_SECONDS_PER_REAL_SECOND);
	//std::cout << realSeconds << " -> " << dt << std::endl;

	/* Add the time to the global clock. */
	clock += dt;

	/* Use Runge-Katta approximation to update the state vectors. */
	for(OrbitalBody* subject : bodies) 
		rungeKattaApprx(subject, dt);
	
}

OrbitalSystem OrbitalSystem::loadFile(const char* xmlFile)
{
	//OrbitalSystem newSystem("res/meshes/body.obj", "res/textures/milkyway.jpg", 1.000e5f);
	OrbitalSystem newSystem;

	if(true) {
		tinyxml2::XMLDocument doc;
		if(!doc.LoadFile(xmlFile)) 
		{
			/* Get the root, background, and bodies elements of the document. */
			tinyxml2::XMLElement* root   = doc.RootElement();
			tinyxml2::XMLElement* background = root->FirstChildElement("background");
			tinyxml2::XMLElement* bodies = root->FirstChildElement("bodies");

			/* Parse the root parameters of the system. */
			const char* g_str          = root->FirstChildElement("g")->GetText();
			GLfloat     g_float        = (GLfloat) atof(g_str);
			const char* scale_str      = root->FirstChildElement("scale")->GetText();
			GLfloat     scale_float    = (GLfloat) atof(scale_str);

			/* Set the root parameters of the system. */
			newSystem.scale            = scale_float;
			newSystem.G                = g_float / scale_float;

			/* Parse the background parameters of the system. */
			const char* bgMeshFile_str = background->FirstChildElement("meshFile")->GetText();
			const char* bgTextFile_str = background->FirstChildElement("textureFile")->GetText();
			const char* bgRadius_str   = background->FirstChildElement("radius")->GetText();
			GLfloat     bgRadius_float = (GLfloat) atof(bgRadius_str);
			const char* bgTilt_str     = background->FirstChildElement("tilt")->GetText();
			GLfloat     bgTilt_float   = (GLfloat) atof(bgTilt_str);

			/* Set the background parameters of the system. */
			newSystem.stars = Geometry::loadObj(bgMeshFile_str, bgTextFile_str);
			Vertex* vertices = newSystem.stars->getVertices();
			for(unsigned int i = 0; i < newSystem.stars->getNumVertices(); i++)
				vertices[i].normal = glm::vec3(0.0f, -1.0f, 0.0f);//glm::normalize(vertices[i].position);
			newSystem.meshes.push_back(newSystem.stars);
			glm::mat4 starsMatrix = glm::scale(glm::mat4(), glm::vec3(bgRadius_float));
			newSystem.starsMatrix = glm::rotate(starsMatrix, bgTilt_float, DEFAULT_TILT_AXIS);
			newSystem.transforms.push_back(&newSystem.starsMatrix);
		
			/* Parse each body of the system. */
			for(tinyxml2::XMLElement* body = bodies->FirstChildElement("body"); body != NULL; body = body->NextSiblingElement("body"))
			{
				/* Parse the body parameters for each body. */
				const char* bodyName_str       = body->FirstChildElement("name")->GetText();
				const char* bodyMass_str       = body->FirstChildElement("mass")->GetText();
				GLfloat     bodyMass_float     = (GLfloat) atof(bodyMass_str);
				const char* bodyRadius_str     = body->FirstChildElement("radius")->GetText();
				GLfloat     bodyRadius_float   = (GLfloat) atof(bodyRadius_str);
				const char* bodyMeshFile_str   = body->FirstChildElement("meshFile")->GetText();
				const char* bodyTextFile_str   = body->FirstChildElement("textureFile")->GetText();
				const char* bodyPosX_str       = body->FirstChildElement("position")->FirstChildElement("x")->GetText();
				GLfloat     bodyPosX_float     = (GLfloat) atof(bodyPosX_str);  
				const char* bodyPosY_str       = body->FirstChildElement("position")->FirstChildElement("y")->GetText();
				GLfloat     bodyPosY_float     = (GLfloat) atof(bodyPosY_str);  
				const char* bodyPosZ_str       = body->FirstChildElement("position")->FirstChildElement("z")->GetText();
				GLfloat     bodyPosZ_float     = (GLfloat) atof(bodyPosZ_str);  
				const char* bodyVelX_str       = body->FirstChildElement("velocity")->FirstChildElement("x")->GetText();
				GLfloat     bodyVelX_float     = (GLfloat) atof(bodyVelX_str);  
				const char* bodyVelY_str       = body->FirstChildElement("velocity")->FirstChildElement("y")->GetText();
				GLfloat     bodyVelY_float     = (GLfloat) atof(bodyVelY_str);  
				const char* bodyVelZ_str       = body->FirstChildElement("velocity")->FirstChildElement("z")->GetText();
				GLfloat     bodyVelZ_float     = (GLfloat) atof(bodyVelZ_str);  
				const char* bodyTilt_str       = body->FirstChildElement("tilt")->GetText();
				GLfloat     bodyTilt_float     = (GLfloat) atof(bodyTilt_str);  
				const char* bodyRotSpeed_str   = body->FirstChildElement("rotationalSpeed")->GetText();
				GLfloat     bodyRotSpeed_float = (GLfloat) atof(bodyRotSpeed_str);

				/* Set the body parameters for each body, and add the body to the system. */
				glm::vec3   bodyPos_vec{bodyPosX_float, bodyPosY_float, bodyPosZ_float};
				glm::vec3   bodyVel_vec{bodyVelX_float, bodyVelY_float, bodyVelZ_float};
				Planet*     newBody = new Planet(bodyName_str,
				                                 bodyMass_float/ scale_float, 
				                                 bodyRadius_float/ scale_float,
				                                 bodyMeshFile_str, 
				                                 bodyTextFile_str, 
				                                 bodyPos_vec/ scale_float, 
				                                 bodyVel_vec/ sqrt(scale_float));

				newBody->setRotationalAxis(bodyTilt_float);
				newBody->setAngularVelocity(bodyRotSpeed_float);

				newSystem.addBody(newBody);
			}

		}
	}
	/* Return the system. */
	return newSystem;
}


void OrbitalSystem::cleanUp() 
{
	stars->cleanUp();
	for(OrbitalBody* body : bodies)
		body->getGeometry()->cleanUp();
}
