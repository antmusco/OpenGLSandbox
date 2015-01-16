#pragma once

#include "OrbitalBody.h"
#include <GL\glew.h>
#include "Geometry.h"

class Planet : public OrbitalBody
{
public:
	Planet(const char* name, 
           const GLfloat     mass, 
           const GLfloat     radius, 
		   const char* objFile,
		   const char* textFile,
           const glm::vec3   initialPosition,
		   const glm::vec3   initialVelocity) 
	{
		this->geometry       = Geometry::loadObj(objFile, textFile);
		this->name           = std::string(name);
		this->mass           = mass;
		this->radius         = radius;
		this->scale          = glm::vec3(1.0f) * radius;
		this->linearPosition = initialPosition;
		this->linearVelocity = initialVelocity;

	}

	~Planet() {}
};

