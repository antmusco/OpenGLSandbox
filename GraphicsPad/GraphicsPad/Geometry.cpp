/******************************************************************************
*                                                                             *
*                              Included Header Files                          *
*                                                                             *
******************************************************************************/
#include "Geometry.h"
#include <GL\glew.h>
#include <glm\glm.hpp>

#define ARRAY_SIZE(a) sizeof(a) / sizeof(*a)

Shader* Geometry::shader = NULL;

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
	Mesh triangle;

	triangle.drawMode = GL_TRIANGLES;

	/* Deine vertices. */
	Vertex localVerts[] =
	{
		glm::vec3(+0.0f, +1.0f, +0.0f),// 0
		glm::vec3(+1.0f, +0.0f, +0.0f),

		glm::vec3(-1.0f, -1.0f, +0.0f),// 1
		glm::vec3(+0.0f, +1.0f, +0.0f),

		glm::vec3(+1.0f, -1.0f, +0.0f),// 2
		glm::vec3(+0.0f, +0.0f, +1.0f),
	};

	/* Record the number of vertices. */
	triangle.numVertices = ARRAY_SIZE(localVerts);
	
	/* Allocate memory on the heap and copy data from stack. */
	triangle.vertices = new Vertex[triangle.numVertices];
	memcpy(triangle.vertices, localVerts, sizeof(localVerts));

	/* Define indices. */
	Triangle localFaces[] =
	{
		{ 0, 1, 2 }
	};

	/* Record the number of indices. */
	triangle.numFaces = ARRAY_SIZE(localFaces);

	/* Allocate memory on the heap and copy data from stack. */
	triangle.faces = new Triangle[triangle.numFaces];
	memcpy(triangle.faces, localFaces, sizeof(localFaces));

	/* Generate buffer and vertex arrays. */
	triangle.genBufferArrayID();
	triangle.genVertexArrayID();

	/* Return Mesh. */
	return triangle;
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

	cube.drawMode = GL_TRIANGLES;

	/* Deine vertices. */
	Vertex localVerts[] =
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
	cube.numVertices = ARRAY_SIZE(localVerts);

	/* Allocate memory on the heap and copy data from stack. */
	cube.vertices = new Vertex[cube.numVertices];
	memcpy(cube.vertices, localVerts, sizeof(localVerts));

	/* Define indices. */
	Triangle localFaces[] = {
		{  0,  1,  2 }, {  0,  2,  3 }, // Top
		{  4,  5,  6 }, {  4,  6,  7 }, // Front
		{  8,  9, 10 }, {  8, 10, 11 }, // Right
		{ 12, 14, 13 }, { 12, 14, 15 }, // Left
		{ 16, 17, 18 }, { 16, 18, 19 }, // Back
		{ 20, 22, 21 }, { 20, 23, 22 }, // Bottom
	};

	/* Record the number of indices. */
	cube.numFaces = ARRAY_SIZE(localFaces);

	/* Allocate memory on the heap and copy data from stack. */
	cube.faces = new Triangle[cube.numFaces];
	memcpy(cube.faces, localFaces, sizeof(localFaces));

	/* Generate buffer and vertex arrays. */
	cube.genBufferArrayID();
	cube.genVertexArrayID();

	/* Return mesh. */
	return cube;
}

Mesh Geometry::makePlane(glm::vec3 x, glm::vec3 y)
{
	/* Decleare return mesh and color of indices. */
	Mesh plane;

	plane.drawMode = GL_TRIANGLES;

	glm::vec3 color = { 1.0f, 1.0f, 1.0f };

	/* Calculate number of vertices and indices.  */
	plane.numVertices = (GLint) NUM_TICKS * NUM_TICKS;
	plane.numFaces = 6 * (NUM_TICKS - 1) * (NUM_TICKS - 1);

	/* Allocate heap space. */
	plane.vertices = new Vertex[plane.numVertices];
	plane.faces = new Triangle[plane.numFaces];

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
		}
	}
	
	/* Define indices. */
	index = 0;
	for (GLint i = 0; i < NUM_TICKS - 1; i++)
	{
		for (GLint j = 0; j < NUM_TICKS - 1; j++)
		{
			/* Triangle 1. */
			plane.faces[index++] = {
				( i * NUM_TICKS ) + j, // Top-left
				( i * NUM_TICKS ) + j + 1, // Top-right
				( ( i + 1 ) * NUM_TICKS ) + j + 1 // Bottom-right
			};

			/* Triangle 2. */
			plane.faces[index++] = {
				( i * NUM_TICKS ) + j, // Top-left
				( ( i + 1 ) * NUM_TICKS ) + j + 1, // Bottom-right
				( ( i + 1 ) * NUM_TICKS ) + j, // Bottom-left
			};
		}
	}

	/* Generate buffer and vertex arrays. */
	plane.genBufferArrayID();
	plane.genVertexArrayID();

	/* Return the mesh. */
	return plane;
}

//Mesh Geometry::makeCoordinatePlane()
//{
//	/* Define mesh. */
//	Mesh coordPlane;
//	coordPlane.drawMode = GL_LINES;
//	coordPlane.vertices = new Vertex[4 * 2 * 30];
//	coordPlane.faces = new 
//	for (int i = 0; i < 60; i++)
//	{
//		coordPlane.vertices[i] = glm::vec3(i - 30f, -30, 0);
//		coordPlane.vertices[i] = glm::vec3(i - 30f, +30, 0);
//		coordPlane.vertices[i] = glm::vec3(-30f, i - 30, 0);
//		coordPlane.vertices[i] = glm::vec3(+30f, i - 30, 0);
//	}
//
//}

Mesh Geometry::makeIsocohedron()
{
	/* Define return mesh. */
	Mesh ico;

	ico.drawMode = GL_TRIANGLES;

	/* Constant offset. */
	GLfloat t = (GLfloat)( 1.0 + std::sqrtf(5.0) ) / 2;

	/* Define the vertices locally. */
	Vertex localVerts[] = {

		glm::vec3(-1.0, +t, +0.0), // 0 - Top
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
	Triangle localFaces[] =
	{
		/* 5 faces around point 0 */
		{  0, 11,  5 },
		{  0,  5,  1 },
		{  0,  1,  7 },
		{  0,  7, 10 },
		{  0, 10, 11 },
		/* 5 adjacent faces */
		{  1,  5,  9 },
		{  5, 11,  4 },
		{ 11, 10,  2 },
		{ 10,  7,  6 },
		{  7,  1,  8 },
		/* 5 faces around point 3 */
		{  3,  9,  4 },
		{  3,  4,  2 },
		{  3,  2,  6 },
		{  3,  6,  8 },
		{  3,  8,  9 },
		/* 5 adjacent faces. */
		{  4,  9,  5 },
		{  2,  4, 11 },
		{  6,  2, 10 },
		{  8,  6,  7 },
		{  9,  8,  1 },
	};

	/* Record the number of vertices. */
	ico.numVertices = ARRAY_SIZE(localVerts);

	/* Allocate memory on the heap and copy data from stack. */
	ico.vertices = new Vertex[ico.numVertices];
	memcpy(ico.vertices, localVerts, sizeof(localVerts));

	/* Record the number of indices. */
	ico.numFaces = ARRAY_SIZE(localFaces);

	/* Allocate memory on the heap and copy data from stack. */
	ico.faces = new Triangle[ico.numFaces];
	memcpy(ico.faces, localFaces, sizeof(localFaces));

	/* Generate buffer and vertex arrays. */
	ico.genBufferArrayID();
	ico.genVertexArrayID();

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
Mesh Geometry::makeSphere(GLuint tesselation)
{
	/* Define return mesh and cahce for tessellating the sphere. */
	Mesh sphere;
	Mesh ico = Geometry::makeIsocohedron();
	std::map<GLuint, GLushort> middlePointIndexCache;

	/* Define the draw mode for the geometry. */
	sphere.drawMode = GL_TRIANGLES;

	/* Constant offset. */
	GLfloat t = (GLfloat)( 1.0 + std::sqrtf(5.0) ) / 2;

	sphere.numFaces = ico.numFaces;
	sphere.numVertices = ico.numVertices;

	/* Get the vertices from icosohedron. */
	std::vector<Vertex> localVerts;
	for (int i = 0; i < ico.numVertices; i++)
		localVerts.push_back(ico.vertices[i]);

	/* Get the faces from the icosohedron. */
	std::vector<Triangle> localFaces;
	for (int i = 0; i < ico.numFaces; i++)
		localFaces.push_back(ico.faces[i]);

	/* Normalize the position of the vertices. */
	for (int i = 0; i < localVerts.size(); i++)
		localVerts.at(i).position /= glm::length(localVerts.at(i).position);

	/* Temp variables for looping. */
	GLushort a, b, c;

	/* Tesselate and normalize. */
	for (int i = 0; i < tesselation; i++)
	{
		std::vector<Triangle> newFaces;

		/* Split each triangle into 4 new triangles. */
		for (Triangle tri : localFaces)
		{
			/* Get the middle points of each side of the triangle. */
			a = getMiddlePoint(tri.v1, tri.v2, &localVerts, 
					&middlePointIndexCache);
			b = getMiddlePoint(tri.v2, tri.v3, &localVerts,
					&middlePointIndexCache);
			c = getMiddlePoint(tri.v3, tri.v1, &localVerts, 
					&middlePointIndexCache);

			/* Add 4 new triangles in place of 'tri' in the new vector. */
			newFaces.push_back({ tri.v1, a, c });
			newFaces.push_back({ tri.v2, b, a });
			newFaces.push_back({ tri.v3, c, b });
			newFaces.push_back({ a, b, c });

		}

		/* Update the vector of local faces. */
		localFaces = newFaces;
	}

	/* Copy over the local vertex data. */
	sphere.numVertices = localVerts.size();
	sphere.vertices = new Vertex[sphere.numVertices];
	memcpy(sphere.vertices, localVerts.data(), localVerts.size() * sizeof(Vertex));

	/* Copy over the local face data. */
	sphere.numFaces = localFaces.size();
	sphere.faces = new Triangle[sphere.numFaces];
	memcpy(sphere.faces, localFaces.data(), localFaces.size() * sizeof(Triangle));
	
	/* Generate buffer and vertex arrays. */
	sphere.genBufferArrayID();
	sphere.genVertexArrayID();

	/* Return the mesh. */
	return sphere;
}

/******************************************************************************
*                                                                             *
*                       Geometry::getMiddlePoint (static)                     *
*                                                                             *
*******************************************************************************
* PARAMETERS                                                                  *
*  i1                                                                         *
*           Index of the first point.                                         *
*  i2                                                                         *
*           Index of the second point.                                        *
*  verts                                                                      *
*           Pointer to the vector containing all of the vertices.             *
*  cache                                                                      *
*           Pointer to the cache of already-generated index points.           *
*                                                                             *
*******************************************************************************
* RETURNS                                                                     *
*  The index of the point between the indicated indices.                      *
*                                                                             *
*******************************************************************************
* DESCRIPTION                                                                 *
*  Static function which takes in two indices and returns the index of the    *
*  point between them, located on the unit shpere. The points are stored in   *
*  cache as they are generated, so that future calls to this function will    *
*  remember the previous values.                                              *
*                                                                             *
*******************************************************************************/
GLushort Geometry::getMiddlePoint(GLushort i1, GLushort i2, 
	std::vector<Vertex> *verts, std::map<GLuint, GLushort>* cache)
{
	/* Generate the key for mapping the index.*/ 
	bool firstSmaller = i1 < i2;
	GLushort smaller = ( firstSmaller ) ? i1 : i2;
	GLushort larger = ( firstSmaller ) ? i2 : i1;

	/*key = smaller * 2^10 + larger */
	GLuint key = ( smaller << 10 ) + larger;

	/* If the middle value exists, return the index value. */
	if (cache->count(key) != 0)
		return cache->at(key);

	/* Otherwise, the value is not in cache and must be calculated. */
	Vertex v1 = verts->at(i1);
	Vertex v2 = verts->at(i2);
	Vertex middle = { (v1.position + v2.position) * 0.5f, 
					  (v1.color + v2.color) * 0.5f };
	middle.position /= glm::length(middle.position);

	/* Add the new vertex to verts and map the index to the cache. */
	GLushort index = verts->size();
	verts->push_back(middle);
	( *cache )[key] = index;

	/* Return the index of the vertex. */
	return index;
}

void Mesh::genBufferArrayID()
{
	/* Generate the buffer space. */
	bufferIDs = new GLuint[NUM_BUFFERS];
	glGenBuffers(NUM_BUFFERS, bufferIDs);

	/* Create vertex buffer. */
	glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, vertexBufferSize(), vertices,
		GL_STATIC_DRAW);

	/* Create index buffer. */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIDs[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize(), faces, 
		GL_STATIC_DRAW);

}

void Mesh::genVertexArrayID()
{
	/* Generate Vertex Array Object. */
	glGenVertexArrays(1, &vertexArrayID);

	/* Bind this vertex array ID.*/
	glBindVertexArray(vertexArrayID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
		
	/* Bind the vertex buffer. */
	glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[0]);

	/* Vertex position attribute. */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, 0);
	glBindAttribLocation(Geometry::shader->getProgram(), 0, "modelPostion");
	glBindAttribLocation(Geometry::shader->getProgram(), 1, "modelColor");

	/* Vertex color attribute. */
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6,
		(char*)( sizeof(GLfloat) * 3 ));
		
	/* Bind the index buffer. */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIDs[1]);

}

void Mesh::cleanUp()
{
	glDeleteBuffers(NUM_BUFFERS, bufferIDs);
	glDeleteBuffers(1, &vertexArrayID);
	delete[] vertices;
	delete[] faces;
	delete[] bufferIDs;
	numVertices = numFaces = 0;
}
