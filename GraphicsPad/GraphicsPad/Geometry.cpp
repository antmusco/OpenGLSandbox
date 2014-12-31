/******************************************************************************
*                                                                             *
*                              Included Header Files                          *
*                                                                             *
******************************************************************************/
#include "Geometry.h"
#include <glm\glm.hpp>
#define ARRAY_SIZE(a) sizeof(a) / sizeof(*a)

/******************************************************************************
*                                                                             *
*                          Geometry::makeTriangle (static)                    *
*                                                                             *
*******************************************************************************
* PARAMETERS                                                                  *
*  void                                                                       *
*                                                                             *
*******************************************************************************
* RETURNS                                                                     *
*  A mesh object describing a simple triangle.                                *
*                                                                             *
*******************************************************************************
* DESCRIPTION                                                                 *
*  Static function which creates a new Mesh struct containing the data for a  *
*  simple triangle. The data for this triangle is stored on the heap, so the  *
*  caller must be sure to free the memory once the mesh is no longer needed.  *
*                                                                             *
*******************************************************************************/
Mesh Geometry::makeTriangle()
{
	/* Define return mesh. */
	Mesh mesh;

	/* Deine vertices. */
	Vertex triVerts[] =
	{
		glm::vec3(+0.0f, +1.0f, +0.0f),// 0
		glm::vec3(+1.0f, +0.0f, +0.0f),

		glm::vec3(-1.0f, -1.0f, +0.0f),// 1
		glm::vec3(+0.0f, +1.0f, +0.0f),

		glm::vec3(+1.0f, -1.0f, +0.0f),// 2
		glm::vec3(+0.0f, +0.0f, +1.0f),
	};

	/* Record the number of vertices. */
	mesh.numVertices = ARRAY_SIZE(triVerts);
	
	/* Allocate memory on the heap and copy data from stack. */
	mesh.vertices = new Vertex[mesh.numVertices];
	memcpy(mesh.vertices, triVerts, sizeof(triVerts));

	/* Define indices. */
	GLushort triIndices[] =
	{
		0, 1, 2
	};

	/* Record the number of indices. */
	mesh.numIndices = ARRAY_SIZE(triIndices);

	/* Allocate memory on the heap and copy data from stack. */
	mesh.indices = new GLushort[mesh.numIndices];
	memcpy(mesh.indices, triIndices, sizeof(triIndices));

	/* Return Mesh. */
	return mesh;
}

/******************************************************************************
*                                                                             *
*                          Geometry::makeCube (static)                        *
*                                                                             *
*******************************************************************************
* PARAMETERS                                                                  *
*  void                                                                       *
*                                                                             *
*******************************************************************************
* RETURNS                                                                     *
*  A mesh object describing a simple 3-D cube.                                *
*                                                                             *
*******************************************************************************
* DESCRIPTION                                                                 *
*  Static function which creates a new Mesh struct containing the data for a  *
*  simple 3-D cube. The data for this 3-D cube is stored on the heap, so the  *
*  caller must be sure to free the memory once the mesh is no longer needed.  *
*                                                                             *
*******************************************************************************/
Mesh Geometry::makeCube()
{
	/* Define return mesh. */
	Mesh cube;

	/* Deine vertices. */
	Vertex stackVerts[] = 
	{
		glm::vec3(-1.0f, +1.0f, +1.0f), // 0
		glm::vec3(+1.0f, +0.0f, +0.0f), // Colour
		glm::vec3(+1.0f, +1.0f, +1.0f), // 1
		glm::vec3(+0.0f, +1.0f, +0.0f), // Colour
		glm::vec3(+1.0f, +1.0f, -1.0f), // 2
		glm::vec3(+0.0f, +0.0f, +1.0f), // Colour
		glm::vec3(-1.0f, +1.0f, -1.0f), // 3
		glm::vec3(+1.0f, +1.0f, +1.0f), // Colour

		glm::vec3(-1.0f, +1.0f, -1.0f), // 4
		glm::vec3(+1.0f, +0.0f, +1.0f), // Colour
		glm::vec3(+1.0f, +1.0f, -1.0f), // 5
		glm::vec3(+0.0f, +0.5f, +0.2f), // Colour
		glm::vec3(+1.0f, -1.0f, -1.0f), // 6
		glm::vec3(+0.8f, +0.6f, +0.4f), // Colour
		glm::vec3(-1.0f, -1.0f, -1.0f), // 7
		glm::vec3(+0.3f, +1.0f, +0.5f), // Colour

		glm::vec3(+1.0f, +1.0f, -1.0f), // 8
		glm::vec3(+0.2f, +0.5f, +0.2f), // Colour
		glm::vec3(+1.0f, +1.0f, +1.0f), // 9
		glm::vec3(+0.9f, +0.3f, +0.7f), // Colour
		glm::vec3(+1.0f, -1.0f, +1.0f), // 10
		glm::vec3(+0.3f, +0.7f, +0.5f), // Colour
		glm::vec3(+1.0f, -1.0f, -1.0f), // 11
		glm::vec3(+0.5f, +0.7f, +0.5f), // Colour

		glm::vec3(-1.0f, +1.0f, +1.0f), // 12
		glm::vec3(+0.7f, +0.8f, +0.2f), // Colour
		glm::vec3(-1.0f, +1.0f, -1.0f), // 13
		glm::vec3(+0.5f, +0.7f, +0.3f), // Colour
		glm::vec3(-1.0f, -1.0f, -1.0f), // 14
		glm::vec3(+0.4f, +0.7f, +0.7f), // Colour
		glm::vec3(-1.0f, -1.0f, -1.0f), // 15
		glm::vec3(+0.2f, +0.5f, +1.0f), // Colour

		glm::vec3(+1.0f, +1.0f, +1.0f), // 16
		glm::vec3(+0.6f, +1.0f, +0.7f), // Colour
		glm::vec3(-1.0f, +1.0f, +1.0f), // 17
		glm::vec3(+0.6f, +0.4f, +0.8f), // Colour
		glm::vec3(-1.0f, -1.0f, +1.0f), // 18
		glm::vec3(+0.2f, +0.8f, +0.7f), // Colour
		glm::vec3(+1.0f, -1.0f, +1.0f), // 19
		glm::vec3(+0.2f, +0.7f, +1.0f), // Colour

		glm::vec3(+1.0f, -1.0f, -1.0f), // 20
		glm::vec3(+0.8f, +0.3f, +0.7f), // Colour
		glm::vec3(-1.0f, -1.0f, -1.0f), // 21
		glm::vec3(+0.8f, +0.9f, +0.5f), // Colour
		glm::vec3(-1.0f, -1.0f, +1.0f), // 22
		glm::vec3(+0.5f, +0.8f, +0.5f), // Colour
		glm::vec3(+1.0f, -1.0f, +1.0f), // 23
		glm::vec3(+0.9f, +1.0f, +0.2f), // Colour
	};

	/* Record the number of vertices. */
	cube.numVertices = ARRAY_SIZE(stackVerts);

	/* Allocate memory on the heap and copy data from stack. */
	cube.vertices = new Vertex[cube.numVertices];
	memcpy(cube.vertices, stackVerts, sizeof(stackVerts));

	/* Define indices. */
	unsigned short stackIndices[] = {
		0, 1, 2, 0, 2, 3, // Top
		4, 5, 6, 4, 6, 7, // Front
		8, 9, 10, 8, 10, 11, // Right
		12, 13, 14, 12, 14, 15, // Left
		16, 17, 18, 16, 18, 19, // Back
		20, 22, 21, 20, 23, 22, // Bottom
	};

	/* Record the number of indices. */
	cube.numIndices = ARRAY_SIZE(stackIndices);

	/* Allocate memory on the heap and copy data from stack. */
	cube.indices = new GLushort[cube.numIndices];
	memcpy(cube.indices, stackIndices, sizeof(stackIndices));

	/* Return mesh. */
	return cube;
}
