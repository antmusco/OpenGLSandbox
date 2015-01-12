#pragma once

#include "OrbitalBody.h"
#include <GL\glew.h>
#include "Geometry.h"

class Planet : public OrbitalBody
{
public:
	Planet(char* name, 
           GLfloat     mass, 
           GLfloat     radius, 
		   char* objFile,
		   char* textFile,
           glm::vec3   initialPosition) 
	{
		this->geometry = Geometry::loadObj(objFile, textFile);
		this->name     = std::string(name);
		this->mass     = mass;
		this->radius   = radius;
		this->scale    = glm::vec3() * radius;
		this->linearPosition = initialPosition;
	}

	~Planet() {}
};

