#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
};

struct Mesh
{
	Mesh() :
		vertices(0), numVertices(0),
		indices(0), numIndices(0) {}
	Vertex*		vertices;
	GLuint		numVertices;
	GLushort*	indices;
	GLuint		numIndices;
	GLsizeiptr	vertexBufferSize() const
	{
		return numVertices * sizeof(Vertex);
	}
	GLsizeiptr	indexBufferSize() const
	{
		return numIndices * sizeof(GLushort);
	}
	void cleanUp()
	{
		delete[] vertices;
		delete[] indices;
		numVertices = numIndices = 0;
	}

};

class Geometry
{
public:
	static Mesh makeTriangle();
	static Mesh makeCube();
	GLuint vertexByteSize();
};