/******************************************************************************
*                                                                             *
*                              Included Header Files                          *
*                                                                             *
******************************************************************************/
#include "Geometry.h"
#include <glm\glm.hpp>
#include <map>

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

Mesh Geometry::makePlane(glm::vec3 x, glm::vec3 y)
{
	/* Decleare return mesh and color of indices. */
	Mesh plane;
	glm::vec3 color = { 1.0f, 1.0f, 1.0f };

	/* Calculate number of vertices and indices.  */
	plane.numVertices = (GLint) NUM_TICKS * NUM_TICKS;
	plane.numIndices = 4 * (NUM_TICKS - 1) * (NUM_TICKS - 1);

	/* Allocate heap space. */
	plane.vertices = new Vertex[plane.numVertices]();
	plane.indices = new GLushort[plane.numIndices]();

	GLushort index = 0;

	/* Define vertices. */
	for (GLint i = 0; i < NUM_TICKS; i++)
	{
		for (GLint j = 0; j < NUM_TICKS; j++)
		{
			index = (GLushort)( i * NUM_TICKS ) + j;
			plane.vertices[index] =
			{
				( ( i - ( ( NUM_TICKS - 1 ) / 2 ) ) * x ) + ( ( j - ( ( NUM_TICKS - 1 ) / 2 ) ) * y ),
				color
			};
			plane.indices[index] = { index };
		}
	}
	
	/* Define indices. */
	index = 0;
	for (GLint i = 0; i < NUM_TICKS - 1; i++)
	{
		for (GLint j = 0; j < NUM_TICKS - 1; j++)
		{
			plane.indices[index + 0] = ( i * NUM_TICKS ) + j; // Top-left
			plane.indices[index + 3] = ( ( i + 1 ) * NUM_TICKS ) + j; // Bottom-left
			plane.indices[index + 2] = ( ( i + 1 ) * NUM_TICKS ) + j + 1; // Bottom-right
			plane.indices[index + 1] = ( i * NUM_TICKS ) + j + 1; // Top-right
			index += 4;
		}
	}

	/* Return the mesh. */
	return plane;
}

Mesh Geometry::makeIsocohedron()
{
	/* Define return mesh. */
	Mesh ico;

	/* Constant offset. */
	GLfloat t = (GLfloat)( 1.0 + std::sqrtf(5.0) ) / 2;

	/* Define the vertices locally. */
	Vertex localVerts[] = {

		glm::vec3(-1.0, +t, +0.0), // 0
		glm::vec3(+1.0, +t, +0.0), // Color

		glm::vec3(+1.0, +t, +0.0), // 1
		glm::vec3(+1.0, +t, +0.0), // Color

		glm::vec3(-1.0, -t, +0.0), // 2
		glm::vec3(+0.0, +t, +0.0), // Color

		glm::vec3(+1.0, -t, +0.0), // 3
		glm::vec3(+1.0, +t, +0.0), // Color

		glm::vec3(+0.0, -1.0, +t), // 4
		glm::vec3(+0.0, +1.0, +t), // Color

		glm::vec3(+0.0, +1.0, +t), // 5
		glm::vec3(+0.0, +1.0, +t), // Color

		glm::vec3(+0.0, -1.0, -t), // 6
		glm::vec3(+0.0, +1.0, +t), // Color

		glm::vec3(+0.0, +1.0, -t), // 7
		glm::vec3(+0.0, +1.0, +t), // Color

		glm::vec3(+t, +0.0, -1.0), // 8
		glm::vec3(+t, +0.0, -1.0), // Color

		glm::vec3(+t, +0.0, +1.0), // 9
		glm::vec3(+t, +0.0, +1.0), // Color

		glm::vec3(-t, +0.0, -1.0), // 10
		glm::vec3(+t, +0.0, +1.0), // Color

		glm::vec3(-t, +0.0, +1.0), // 11
		glm::vec3(+t, +0.0, +1.0), // Color

	};

	/* Define the faces locally. */
	GLushort localIndices[] =
	{
		/* 5 faces around point 0 */
		0, 11, 5,
		0, 5, 1,
		0, 1, 7,
		0, 7, 10,
		0, 10, 11,
		/* 5 adjacent faces */
		1, 5, 9,
		5, 11, 4,
		11, 10, 2,
		10, 7, 6,
		7, 1, 8,
		/* 5 faces around point 3 */
		3, 9, 4,
		3, 4, 2,
		3, 2, 6,
		3, 6, 8,
		3, 8, 9,
		/* 5 adjacent faces. */
		4, 9, 5,
		2, 4, 11,
		6, 2, 10,
		8, 6, 7,
		9, 8, 1,
	};
	
	/* Record the number of vertices. */
	ico.numVertices = ARRAY_SIZE(localVerts);

	/* Allocate memory on the heap and copy data from stack. */
	ico.vertices = new Vertex[ico.numVertices];
	std::copy(localVerts, (localVerts + ico.numVertices), ico.vertices);

	/* Record the number of indices. */
	ico.numIndices = ARRAY_SIZE(localIndices);

	/* Allocate memory on the heap and copy data from stack. */
	ico.indices = new GLushort[ico.numIndices];
	std::copy(localIndices, localIndices + ico.numIndices, ico.indices);

	/* Return mesh. */
	return ico;
}

/******************************************************************************
*                                                                             *
*                        Geometry::makeSphere (static)                        *
*                                                                             *
*******************************************************************************
* PARAMETERS                                                                  *
*  void                                                                       *
*                                                                             *
*******************************************************************************
* RETURNS                                                                     *
*  A mesh object describing a simple 3-D sphere.                              *
*                                                                             *
*******************************************************************************
* DESCRIPTION                                                                 *
*  Static function which creates a new Mesh struct containing the data for a  *
*  simple 3-D sphere. The data for this 3-D sphere is stored on the heap, so  *
*  caller must be sure to free the memory once the mesh is no longer needed.  *
*                                                                             *
*******************************************************************************/
//Mesh Geometry::makeSphere()
//{
//	/* Define return mesh. */
//	Mesh sphere;
//
//	std::map<GLuint, GLushort> middlePointIndexCache;
//
//	/* Generate vertices and faces. */
//	std::vector<Vertex> verts;
//	std::vector<Triangle> faces;
//	genIcosohedron(&verts, &faces);
//
//	/* Loop and normalize. */
//	for (int i = 0; i < 5; i++)
//	{
//		std::vector<Triangle> newFaces;
//		for (Triangle tri : faces)
//		{
//			GLushort a = getMiddlePoint(tri.v1, tri.v2, &verts, &middlePointIndexCache);
//			GLushort b = getMiddlePoint(tri.v2, tri.v3, &verts, &middlePointIndexCache);
//			GLushort c = getMiddlePoint(tri.v3, tri.v1, &verts, &middlePointIndexCache);
//
//			newFaces.push_back({ tri.v1, a, c });
//			newFaces.push_back({ tri.v2, b, a });
//			newFaces.push_back({ tri.v3, c, b });
//			newFaces.push_back({      a, b, c });
//		}
//		faces = newFaces;
//	}
//
//
//
//
//	/* Record the number of vertices. */
//	shpere.numVertices = ARRAY_SIZE(stackVerts);
//
//	/* Allocate memory on the heap and copy data from stack. */
//	cube.vertices = new Vertex[cube.numVertices];
//	memcpy(cube.vertices, stackVerts, sizeof(stackVerts));
//
//	/* Define indices. */
//	unsigned short stackIndices[] = {
//		0, 1, 2, 0, 2, 3, // Top
//		4, 5, 6, 4, 6, 7, // Front
//		8, 9, 10, 8, 10, 11, // Right
//		12, 13, 14, 12, 14, 15, // Left
//		16, 17, 18, 16, 18, 19, // Back
//		20, 22, 21, 20, 23, 22, // Bottom
//	};
//
//	/* Record the number of indices. */
//	cube.numIndices = ARRAY_SIZE(stackIndices);
//
//	/* Allocate memory on the heap and copy data from stack. */
//	cube.indices = new GLushort[cube.numIndices];
//	memcpy(cube.indices, stackIndices, sizeof(stackIndices));
//
//	/* Return mesh. */
//	return cube;
//}
//
//GLushort getMiddlePoint(GLushort i1, GLushort i2, std::vector<Vertex> *verts, std::map<GLuint, GLushort>* cache)
//{
//	bool firstSmaller = i1 < i2;
//	GLushort smaller = ( firstSmaller ) ? i1 : i2;
//	GLushort larger = ( firstSmaller ) ? i2 : i1;
//	GLuint key = ( smaller << 16 ) + larger;
//
//	/* If the middle value exists, return the index value. */
//	if (cache->count(key))
//		return cache->at(key);
//
//	/* Otherwise, the value is not in cache and must be calculated. */
//	Vertex v1 = verts->at(i1);
//	Vertex v2 = verts->at(i2);
//	Vertex middle = { (v1.position + v2.position), (v1.color + v2.color)};
//	middle.color /= 2;
//	middle.position /= 2;
//
//
//
//}
