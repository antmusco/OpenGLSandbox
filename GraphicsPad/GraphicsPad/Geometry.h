#pragma once

/******************************************************************************
*                                                                             *
*                              Included Header Files                          *
*                                                                             *
******************************************************************************/
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <vector>
#include <map>
#include "Shader.h"

#define NUM_TICKS 11.0f
#define NUM_BUFFERS 2

/******************************************************************************
*                                                                             *
*                           Geometry::Vertex (struct)                         *
*                                                                             *
*******************************************************************************
* MEMBERS                                                                     *
*  position                                                                   *
*          x, y, z coordinates for the position of this vertex.               *
*  color                                                                      *
*          r, g, b values for the color of this vertex.                       *
*                                                                             *
*******************************************************************************
* DESCRIPTION                                                                 *
*  Struct representing a simple vertex in 3-D space. The vertex consists of   *
*  6 sequential float values: the x, y, and z coordinates and the r, g, and b *
*  color values for the vertex.                                               *
*                                                                             *
*******************************************************************************/
struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
};

/******************************************************************************
*                                                                             *
*                          Geometry::Triangle (struct)                        *
*                                                                             *
*******************************************************************************
* MEMBERS                                                                     *
*  vertices                                                                   *
*          Index of the three corners of the triangle.                        *
*                                                                             *
*******************************************************************************
* DESCRIPTION                                                                 *
*  Struct representing a simple triangle in 3-D space. The triangle consists  *
*  of 3 sequential int values: the three indexes of the triangle.             *
*                                                                             *
*******************************************************************************/
struct Triangle
{
	GLushort v1;
	GLushort v2;
	GLushort v3;
};

/******************************************************************************
*                                                                             *
*                           Geometry::Mesh   (class)                          *
*                                                                             *
*******************************************************************************
* MEMBERS                                                                     *
*  vertices                                                                   *
*          Pointer to the collection of Vertex structs for this mesh.         *
*  numVertices                                                                *
*          Number of vertices in this Mesh struct.                            *
*  indices                                                                    *
*          Pointer to the written order in which the traingles are to be      *
*          drawn.                                                             *
*  numIndices                                                                 *
*          Number of indices used to draw the Mesh struct.                    *
*                                                                             *
*******************************************************************************
* DESCRIPTION                                                                 *
*  Class representing a collection of vertices in 3-D space representing an   *
*  object. All vertices are only recorded once, with the indexes indicating   *
*  the draw order for each vertex.                                            *
*                                                                             *
*******************************************************************************/
class Mesh
{
public:
	/* Constructor */
	Mesh() :
		vertices(0), numVertices(0),
		indices(0), numIndices(0) {}

	/* Calculate the number of bytes for the vertices. */
	GLsizeiptr	vertexBufferSize() const
	{
		int x = sizeof(Vertex);
		return numVertices * sizeof(Vertex);
	}

	/* Calculate the number of bytes for the indices. */
	GLsizeiptr	indexBufferSize() const
	{
		return numIndices * sizeof(GLuint);
	}

	void genBufferArrayID();
	void genVertexArrayID();

	/* Destructor */ 
	void cleanUp();

	Vertex*			vertices;
	GLuint			numVertices;
	GLushort*		indices;
	GLuint			numIndices;
	GLuint*			bufferIDs;
	GLuint			vertexArrayID;
	GLenum			drawMode;
};

/******************************************************************************
*                                                                             *
*                           Geometry::Geometry (class)                        *
*                                                                             *
*******************************************************************************
* MEMBERS                                                                     *
*  void                                                                       *
*                                                                             *
*******************************************************************************
* DESCRIPTION                                                                 *
*  Class consisitng of static functions to create a series of shapes.         *
*                                                                             *
*******************************************************************************/
class Geometry
{
public:
	static Shader*	shader;
	static Mesh makeTriangle();
	static Mesh makeCube();
	static Mesh makeSphere(GLuint tesselation);
	static Mesh makeIsocohedron();
	static Mesh makePlane(glm::vec3 x, glm::vec3 y);
	static Mesh makeCoordinatePlane();
	static GLushort getMiddlePoint(GLushort i1, GLushort i2, 
		std::vector<Vertex> *verts, std::map<GLuint, GLushort>* cache);
};