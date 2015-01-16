/******************************************************************************
*                                                                             *
*                              Included Header Files                          *
*                                                                             *
******************************************************************************/
#include "Geometry.h"
#include <string>
#include <iostream>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <SDL\SDL_image.h>
#include "tiny_obj_loader.h"

#pragma comment( lib, "SDL2.lib" )
#pragma comment( lib, "SDL2main.lib" )
#pragma comment( lib, "SDL2_image.lib" )

/******************************************************************************
*                                                                             *
*                                      Macros                                 *
*                                                                             *
******************************************************************************/
#define ARRAY_SIZE(a) sizeof(a) / sizeof(*a)

Shader* Geometry::shader = NULL;

/******************************************************************************
*                                                                             *
*                             Mesh::Mesh() (constructor)                      *
*                                                                             *
*******************************************************************************
* PARAMETERS                                                                  *
*  void                                                                       *
*                                                                             *
*******************************************************************************
* RETURNS                                                                     *
*  void                                                                       *
*                                                                             *
*******************************************************************************
* DESCRIPTION                                                                 *
*  Default constructor which initializes all members of Mesh to 0 (NULL)      *
*  except for members with default values (numBuffers and drawMode).          *
*                                                                             *
*******************************************************************************/
Mesh::Mesh() :
      vertices(0), numVertices(0),
      indices(0), numIndices(0),
      textureID(-1),
      numBuffers(DEFAULT_NUM_BUFFERS), bufferIDs(0), vertexArrayID(0),
      drawMode(DEFAULT_DRAW_MODE) {}
Mesh::Mesh(const Mesh& rhs) :
	  numVertices(rhs.getNumVertices()),
	  numIndices(rhs.getNumIndices()),
	  textureID(rhs.getTextureID()),
	  numBuffers(rhs.getNumBuffers()),
	  vertexArrayID(rhs.getVertexArrayID()),
	  drawMode(rhs.getDrawMode())
{
	vertices = new Vertex[rhs.getNumVertices()];
	indices  = new GLushort[rhs.getNumIndices()];
	memcpy(vertices, rhs.getVertices(), rhs.getNumVertices() * sizeof(Vertex));
	memcpy(indices, rhs.getIndices(), rhs.getNumIndices() * sizeof(GLushort));
	bufferIDs = new GLuint[rhs.getNumBuffers()];
	memcpy(bufferIDs, rhs.getBufferIDs(), rhs.getNumBuffers() * sizeof(GLuint));
}

/******************************************************************************
*                                                                             *
*                          Mesh::vertexBufferSize()  (const)                  *
*                                                                             *
*******************************************************************************
* PARAMETERS                                                                  *
*  void                                                                       *
*                                                                             *
*******************************************************************************
* RETURNS                                                                     *
*  The number of bytes required for this Mesh's vertex buffer.                *
*                                                                             *
*******************************************************************************
* DESCRIPTION                                                                 *
*  Calculates the number of bytes required for this Mesh's vertex buffer.     *
*                                                                             *
*******************************************************************************/
GLsizeiptr Mesh::vertexBufferSize() const
{
	return numVertices * sizeof(Vertex);
}

/******************************************************************************
*                                                                             *
*                          Mesh::indexBufferSize()  (const)                   *
*                                                                             *
*******************************************************************************
* PARAMETERS                                                                  *
*  void                                                                       *
*                                                                             *
*******************************************************************************
* RETURNS                                                                     *
*  The number of bytes required for this Mesh's index buffer.                 *
*                                                                             *
*******************************************************************************
* DESCRIPTION                                                                 *
*  Calculates the number of bytes required for this Mesh's index buffer.      *
*                                                                             *
*******************************************************************************/
GLsizeiptr Mesh::indexBufferSize() const
{
	return numIndices * sizeof(GLushort);
}

/******************************************************************************
*                                                                             *
*                         Mesh::setVertices()  (overloaded)                   *
*                                                                             *
*******************************************************************************
* PARAMETERS (1)                                                              *
*  n                                                                          *
*           The number of elements in the array to copy.                      *
*  a                                                                          *
*           A pointer to the array of values to be set as the vertices.       *
*                                                                             *
* PARAMETERS (2)                                                              *
*  v                                                                          *
*           A pointer to a vector containing the values to be set as the      *
*           vertices.                                                         *
*                                                                             *
*******************************************************************************
* RETURNS                                                                     *
*  void                                                                       *
*                                                                             *
*******************************************************************************
* DESCRIPTION                                                                 *
*  Sets the number of vertices, as well as their values, for this Mesh.       *
*                                                                             *
*******************************************************************************/
void Mesh::setVertices(GLuint n, Vertex* a)
{
	/* Set number of vertices. */
	numVertices = n;
	/* Allocate space on the heap. */
	vertices = new Vertex[n];
	/* Copy the data over to the allocated space. */
	memcpy(vertices, a, sizeof(Vertex) * n);
}
void Mesh::setVertices(std::vector<Vertex>* v)
{
	/* Set number of vertices. */
	numVertices = v->size();
	/* Allocate space on the heap. */
	vertices = new Vertex[v->size()];
	/* Copy the data over to the allocated space. */
	memcpy(vertices, v->data(), sizeof(Vertex) * v->size());
}

/******************************************************************************
*                                                                             *
*                         Mesh::setIndices()   (overloaded)                   *
*                                                                             *
*******************************************************************************
* PARAMETERS (1)                                                              *
*  n                                                                          *
*           The number of elements in the array to copy.                      *
*  a                                                                          *
*           A pointer to the array of values to be set as the indices.        *
*                                                                             *
* PARAMETERS (2)                                                              *
*  v                                                                          *
*           A pointer to a vector containing the values to be set as the      *
*           indices.                                                          *
*                                                                             *
*******************************************************************************
* RETURNS                                                                     *
*  void                                                                       *
*                                                                             *
*******************************************************************************
* DESCRIPTION                                                                 *
*  Sets the number of indices, as well as their values, for this Mesh.        *
*                                                                             *
*******************************************************************************/
void Mesh::setIndices(GLuint n, GLushort* a)
{
	/* Set number of indices. */
	numIndices = n;
	/* Allocate space on the heap. */
	indices = new GLushort[n];
	/* Copy the data over to the allocated space. */
	memcpy(indices, a, sizeof(GLushort) * n);
}
void Mesh::setIndices(std::vector<GLushort>* i)
{
	/* Set number of vertices. */
	numIndices = i->size();
	/* Allocate space on the heap. */
	indices = new GLushort[i->size()];
	/* Copy the data over to the allocated space. */
	memcpy(indices, i->data(), sizeof(GLushort) * i->size());
}

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

	/* Deine vertices. */
	Vertex localVertices[] =
	{
		/* Vertex 0. */
		glm::vec3(+0.0f, +1.0f, +0.0f), // Position.
		glm::vec3(+1.0f, +0.0f, +0.0f), // Color.
		glm::vec2(+0.0f, +0.0f),        // Texture Coordinate.

		glm::vec3(-1.0f, -1.0f, +0.0f), // Position.
		glm::vec3(+0.0f, +1.0f, +0.0f), // Color.
		glm::vec2(+0.0f, +0.0f), // Texture Coordinate.

		glm::vec3(+1.0f, -1.0f, +0.0f), // Position.
		glm::vec3(+0.0f, +0.0f, +1.0f), // Color.
		glm::vec2(+0.0f, +0.0f), // Texture Coordinate.
	};

	/* Define indices. */
	GLushort localIndices[] =
	{
		0, 1, 2
	};

	/* Set the vertices and the indices for this triangle. */
	triangle.setVertices(ARRAY_SIZE(localVertices), localVertices);
	triangle.setIndices(ARRAY_SIZE(localIndices), localIndices);

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

	/* Deine vertices. */
	Vertex localVertices[] =
	{
		/* Vertex 0. */
		glm::vec3(-1.0f, +1.0f, +1.0f), // Position.
		glm::vec3(+1.0f, +0.0f, +0.0f), // Color.
		glm::vec2(+0.0f, +0.0f),        // Texture Coordinate.
		/* Vertex 1. */
		glm::vec3(+1.0f, +1.0f, +1.0f), // Position.
		glm::vec3(+0.0f, +1.0f, +0.0f), // Color
		glm::vec2(+0.0f, +0.0f),        // Texture Coordinate.
		/* Vertex 2. */
		glm::vec3(+1.0f, +1.0f, -1.0f), // Position.
		glm::vec3(+0.0f, +0.0f, +1.0f), // Color
		glm::vec2(+0.0f, +0.0f),        // Texture Coordinate.
		/* Vertex 3. */
		glm::vec3(-1.0f, +1.0f, -1.0f), // Position.
		glm::vec3(+1.0f, +1.0f, +1.0f), // Color
		glm::vec2(+0.0f, +0.0f),        // Texture Coordinate.
		/* Vertex 4. */
		glm::vec3(-1.0f, +1.0f, -1.0f), // Position.
		glm::vec3(+1.0f, +0.0f, +1.0f), // Color
		glm::vec2(+0.0f, +0.0f),        // Texture Coordinate.
		/* Vertex 5. */
		glm::vec3(+1.0f, +1.0f, -1.0f), // Position.
		glm::vec3(+0.0f, +0.5f, +0.2f), // Color
		glm::vec2(+0.0f, +0.0f),        // Texture Coordinate.
		/* Vertex 6. */
		glm::vec3(+1.0f, -1.0f, -1.0f), // Position.
		glm::vec3(+0.8f, +0.6f, +0.4f), // Color
		glm::vec2(+0.0f, +0.0f),        // Texture Coordinate.
		/* Vertex 7. */
		glm::vec3(-1.0f, -1.0f, -1.0f), // Position.
		glm::vec3(+0.3f, +1.0f, +0.5f), // Color
		glm::vec2(+0.0f, +0.0f),        // Texture Coordinate.
		/* Vertex 8. */
		glm::vec3(+1.0f, +1.0f, -1.0f), // Position.
		glm::vec3(+0.2f, +0.5f, +0.2f), // Color
		glm::vec2(+0.0f, +0.0f),        // Texture Coordinate.
		/* Vertex 9. */
		glm::vec3(+1.0f, +1.0f, +1.0f), // Position.
		glm::vec3(+0.9f, +0.3f, +0.7f), // Color
		glm::vec2(+0.0f, +0.0f),        // Texture Coordinate.
		/* Vertex 10. */
		glm::vec3(+1.0f, -1.0f, +1.0f), // Position.
		glm::vec3(+0.3f, +0.7f, +0.5f), // Color
		glm::vec2(+0.0f, +0.0f),        // Texture Coordinate.
		/* Vertex 11. */
		glm::vec3(+1.0f, -1.0f, -1.0f), // Position.
		glm::vec3(+0.5f, +0.7f, +0.5f), // Color
		glm::vec2(+0.0f, +0.0f),        // Texture Coordinate.
		/* Vertex 12. */
		glm::vec3(-1.0f, +1.0f, +1.0f), // Position.
		glm::vec3(+0.7f, +0.8f, +0.2f), // Color
		glm::vec2(+0.0f, +0.0f),        // Texture Coordinate.
		/* Vertex 13. */
		glm::vec3(-1.0f, +1.0f, -1.0f), // Position.
		glm::vec3(+0.5f, +0.7f, +0.3f), // Color
		glm::vec2(+0.0f, +0.0f),        // Texture Coordinate.
		/* Vertex 14. */
		glm::vec3(-1.0f, -1.0f, -1.0f), // Position.
		glm::vec3(+0.4f, +0.7f, +0.7f), // Color
		glm::vec2(+0.0f, +0.0f),        // Texture Coordinate.
		/* Vertex 15. */
		glm::vec3(-1.0f, -1.0f, -1.0f), // Position.
		glm::vec3(+0.2f, +0.5f, +1.0f), // Color
		glm::vec2(+0.0f, +0.0f),        // Texture Coordinate.
		/* Vertex 16. */
		glm::vec3(+1.0f, +1.0f, +1.0f), // Position.
		glm::vec3(+0.6f, +1.0f, +0.7f), // Color
		glm::vec2(+0.0f, +0.0f),        // Texture Coordinate.
		/* Vertex 17. */
		glm::vec3(-1.0f, +1.0f, +1.0f), // Position.
		glm::vec3(+0.6f, +0.4f, +0.8f), // Color
		glm::vec2(+0.0f, +0.0f),        // Texture Coordinate.
		/* Vertex 18. */
		glm::vec3(-1.0f, -1.0f, +1.0f), // Position.
		glm::vec3(+0.2f, +0.8f, +0.7f), // Color
		glm::vec2(+0.0f, +0.0f),        // Texture Coordinate.
		/* Vertex 19. */
		glm::vec3(+1.0f, -1.0f, +1.0f), // Position.
		glm::vec3(+0.2f, +0.7f, +1.0f), // Color
		glm::vec2(+0.0f, +0.0f),        // Texture Coordinate.
		/* Vertex 21. */
		glm::vec3(+1.0f, -1.0f, -1.0f), // Position.
		glm::vec3(+0.8f, +0.3f, +0.7f), // Color
		glm::vec2(+0.0f, +0.0f),        // Texture Coordinate.
		/* Vertex 22. */
		glm::vec3(-1.0f, -1.0f, -1.0f), // Position.
		glm::vec3(+0.8f, +0.9f, +0.5f), // Color
		glm::vec2(+0.0f, +0.0f),        // Texture Coordinate.
		/* Vertex 23. */
		glm::vec3(-1.0f, -1.0f, +1.0f), // Position.
		glm::vec3(+0.5f, +0.8f, +0.5f), // Color
		glm::vec2(+0.0f, +0.0f),        // Texture Coordinate.
		/* Vertex 24. */
		glm::vec3(+1.0f, -1.0f, +1.0f), // Position.
		glm::vec3(+0.9f, +1.0f, +0.2f), // Color
		glm::vec2(+0.0f, +0.0f),        // Texture Coordinate.
	};

	/* Define indices. */
	GLushort localIndices[] = {
		  0,  1,  2,  0,  2,  3, // Top
		  4,  5,  6,  4,  6,  7, // Front
		  8,  9, 10,  8, 10, 11, // Right
		 12, 14, 13, 12, 14, 15, // Left
		 16, 17, 18, 16, 18, 19, // Back
		 20, 22, 21, 20, 23, 22, // Bottom
	};

	/* Set the vertices and the indices for the mesh.. */
	cube.setVertices(ARRAY_SIZE(localVertices), localVertices);
	cube.setIndices(ARRAY_SIZE(localIndices), localIndices);
	
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

	const GLint NUM_TICKS = 10;
	GLfloat lowBound = (NUM_TICKS - 1.0f) / 2; 
	glm::vec3 color = { 1.0f, 1.0f, 1.0f };
	glm::vec2 texture  = { 0.0f, 0.0f };

	/* Create vectors to store the vertices and the indices.  */
	std::vector<Vertex> localVertices;
	std::vector<GLushort> localIndices;

	/* Define vertices. */
	for (GLint i = 0; i < NUM_TICKS; i++)
	{
		for (GLint j = 0; j < NUM_TICKS; j++)
		{
			Vertex v = {
				( i - lowBound ) * x + ( j - lowBound ) * y,
				color,
				texture
			};
			localVertices.push_back(v);
		}
	}
	
	/* Define indices. */
	for (GLint i = 0; i < NUM_TICKS - 1; i++)
	{
		for (GLint j = 0; j < NUM_TICKS - 1; j++)
		{
			/* Triangle 1. */
			localIndices.push_back(
				(GLushort)( ( i * NUM_TICKS ) + j )); // Top-left
			localIndices.push_back(
				(GLushort)( ( i * NUM_TICKS ) + j + 1 )); // Top-right
			localIndices.push_back(
				(GLushort)( ( ( i + 1 ) * NUM_TICKS ) + j + 1 )); // Bottom-right
									  
			/* Triangle 2. */		  
			localIndices.push_back(
				(GLushort)( ( i * NUM_TICKS ) + j )); // Top-left
			localIndices.push_back(
				(GLushort)( ( ( i + 1 ) * NUM_TICKS ) + j + 1 )); // Bottom-right
			localIndices.push_back(
				(GLushort)( ( ( i + 1 ) * NUM_TICKS ) + j )); // Bottom-left

		}
	}

	/* Set the vertices and the indices for the mesh.. */
	plane.setVertices(&localVertices);
	plane.setIndices(&localIndices);

	/* Generate buffer and vertex arrays. */
	plane.genBufferArrayID();
	plane.genVertexArrayID();

	/* Return the mesh. */
	return plane;
}

Mesh Geometry::makeCoordinatePlane(GLint xWidth, GLint yWidth, GLint zWidth)
{
	/* Define mesh. */
	Mesh  coordPlane;
	GLint xTicks = (xWidth == 0) ? 0 : (xWidth * 2) + 1;
	GLint yTicks = (yWidth == 0) ? 0 : (yWidth * 2) + 1;
	GLint zTicks = (zWidth == 0) ? 0 : (zWidth * 2) + 1;
	GLint numAxes = (xWidth != 0 && yWidth != 0 && zWidth != 0) ? 4 : 2;
	GLint numVertices = numAxes * ( xTicks + yTicks + zTicks );

	/* Set the custom draw mode. */
	coordPlane.setDrawMode(GL_LINES);

	std::vector<Vertex> localVertices;
	std::vector<GLushort> localIndices;

	glm::vec3 xyColor = { 1.0f, 0.0f, 0.0f };
	glm::vec3 xzColor = { 0.0f, 1.0f, 0.0f };
	glm::vec3 yzColor = { 0.0f, 0.0f, 1.0f };
	glm::vec2 texture = { 0.0f, 0.0f };

	for (GLint i = 0; i < xTicks; i++)
	{
		if (yTicks != 0) /* XY Lines */
		{
			localVertices.push_back({
				glm::vec3((float)( i - xWidth ), -((float)yWidth), 0.0f),
				xyColor,
			    texture
			});
			localVertices.push_back({
				glm::vec3((float)( i - xWidth ), (float)yWidth, 0.0f),
				xyColor,
			    texture
			});
		}
		if (zTicks != 0) /* XZ Lines */
		{
			localVertices.push_back({
				glm::vec3((float)( i - xWidth ), 0.0f, -((float)zWidth)), 
				xzColor,
			    texture
			});
			localVertices.push_back({
				glm::vec3((float)( i - xWidth ), 0.0f, (float) zWidth), 
				xzColor,
			    texture
			});
		}
	}

	for (GLint i = 0; i < yTicks; i++)
	{
		if (xTicks != 0) /* YX Lines */
		{
			localVertices.push_back({
				glm::vec3((float)xWidth, (float)( i - yWidth ), 0.0f),
				xyColor,
			    texture
			});
			localVertices.push_back({
				glm::vec3(-( (float)xWidth ), (float)( i - yWidth ), 0.0f),
				xyColor,
			    texture
			});
		}
		if (zTicks != 0) /* YZ Lines */
		{
			localVertices.push_back({
				glm::vec3( 0.0f, (float)( i - yWidth ), -((float)zWidth)),
				yzColor,
			    texture
			});
			localVertices.push_back({
				glm::vec3(0.0f, (float)( i - yWidth ), (float)zWidth),
				yzColor,
			    texture
			});
		}
	}

	for (GLint i = 0; i < zTicks; i++)
	{ 
		if (xTicks != 0) /* ZX Lines */
		{
			localVertices.push_back({
				glm::vec3((float)xWidth, 0.0f, (float)( i - zWidth )),
				xzColor,
			    texture
			});
			localVertices.push_back({
				glm::vec3(-( (float)xWidth ), 0.0f, (float)( i - zWidth )),
				xzColor,
			    texture
			});
		}
		if (yTicks != 0) /* ZY Lines */
		{
			localVertices.push_back({
				glm::vec3(0.0f, (float)yWidth, (float)( i - zWidth )),
				yzColor,
			    texture
			});
			localVertices.push_back({
				glm::vec3(0.0f, -( (float)yWidth ), (float)( i - zWidth )),
				yzColor,
			    texture
			});
		}
	}

	/* Define the indices locally. */
	for (GLushort i = 0; i < localVertices.size(); i++)
		localIndices.push_back(i);

	/* Set the vertice and the indices for this mesh. */
	coordPlane.setVertices(&localVertices);
	coordPlane.setIndices(&localIndices);

	/* Generate buffer and vertex arrays. */
	coordPlane.genBufferArrayID();
	coordPlane.genVertexArrayID();

	return coordPlane;
}

Mesh Geometry::makeIsocohedron()
{
	/* Define return mesh. */
	Mesh ico;

	/* Constant offset. */
	GLfloat t = (GLfloat)( 1.0 + std::sqrtf(5.0) ) / 2;
	glm::vec2 texture = { 0.0f, 0.0f };

	/* Define the vertices locally. */
	Vertex localVertices[] = {

		glm::vec3(-1.0, +t, +0.0), // 0 - Top
		glm::vec3(+1.0, +t, +0.0), // Color
		glm::vec2(+0.0f, 0.0f),    // Texture Coordinate     

		glm::vec3(+1.0, +t, +0.0), // 1
		glm::vec3(+1.0, +t, +0.0), // Color
		glm::vec2(+0.0f, 0.0f),    // Texture Coordinate 

		glm::vec3(-1.0, -t, +0.0), // 2
		glm::vec3(+0.0, +t, +0.0), // Color
		glm::vec2(+0.0f, 0.0f),    // Texture Coordinate 

		glm::vec3(+1.0, -t, +0.0), // 3
		glm::vec3(+1.0, +t, +0.0), // Color
		glm::vec2(+0.0f, 0.0f),    // Texture Coordinate 

		glm::vec3(+0.0, -1.0, +t), // 4
		glm::vec3(+0.0, +1.0, +t), // Color
		glm::vec2(+0.0f, 0.0f),    // Texture Coordinate 

		glm::vec3(+0.0, +1.0, +t), // 5
		glm::vec3(+0.0, +1.0, +t), // Color
		glm::vec2(+0.0f, 0.0f),    // Texture Coordinate 

		glm::vec3(+0.0, -1.0, -t), // 6
		glm::vec3(+0.0, +1.0, +t), // Color
		glm::vec2(+0.0f, 0.0f),    // Texture Coordinate 

		glm::vec3(+0.0, +1.0, -t), // 7
		glm::vec3(+0.0, +1.0, +t), // Color
		glm::vec2(+0.0f, 0.0f),    // Texture Coordinate 

		glm::vec3(+t, +0.0, -1.0), // 8
		glm::vec3(+t, +0.0, -1.0), // Color
		glm::vec2(+0.0f, 0.0f),    // Texture Coordinate 

		glm::vec3(+t, +0.0, +1.0), // 9
		glm::vec3(+t, +0.0, +1.0), // Color
		glm::vec2(+0.0f, 0.0f),    // Texture Coordinate 

		glm::vec3(-t, +0.0, -1.0), // 10
		glm::vec3(+t, +0.0, +1.0), // Color
		glm::vec2(+0.0f, 0.0f),    // Texture Coordinate 

		glm::vec3(-t, +0.0, +1.0), // 11
		glm::vec3(+t, +0.0, +1.0), // Color
		glm::vec2(+0.0f, 0.0f),    // Texture Coordinate 

	};

	/* Define the indices locally. */
	GLushort localIndices[] =
	{
		/* 5 indices around point 0 */
		 0, 11,  5,
		 0,  5,  1,
		 0,  1,  7,
		 0,  7, 10,
		 0, 10, 11,
		/* 5 adjacent indices */
		 1,  5,  9,
		 5, 11,  4,
		11, 10,  2,
		10,  7,  6,
		 7,  1,  8,
		/* 5 indices around point 3 */
		 3,  9,  4,
		 3,  4,  2,
		 3,  2,  6,
		 3,  6,  8,
		 3,  8,  9,
		/* 5 adjacent indices. */
		 4,  9,  5,
		 2,  4, 11,
		 6,  2, 10,
		 8,  6,  7,
		 9,  8,  1,
	};

	/* Set the vertices and indices of this mesh. */
	ico.setVertices(ARRAY_SIZE(localVertices), localVertices);
	ico.setIndices(ARRAY_SIZE(localIndices), localIndices);

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

	/* Constant offset. */
	GLfloat t = (GLfloat)( 1.0 + std::sqrtf(5.0) ) / 2;

	/* Get the vertices from icosohedron. */
	std::vector<Vertex> localVertices;
	for (GLuint i = 0; i < ico.getNumVertices(); i++)
		localVertices.push_back(ico.getVertex(i));

	/* Get the indices from the icosohedron. */
	std::vector<GLushort> localIndices;
	for (GLuint i = 0; i < ico.getNumIndices(); i++)
		localIndices.push_back(ico.getIndex(i));

	/* Normalize the position of the vertices. */
	for (GLuint i = 0; i < localVertices.size(); i++)
		localVertices.at(i).position /= glm::length(localVertices.at(i).position);

	/* Temp variables for looping. */
	GLushort a, b, c;

	/* Declare the buffer of new indices. */
	std::vector<GLushort> newIndices;

	/* Tesselate and normalize. */
	for (GLuint j = 0; j < tesselation; j++)
	{
		/* Clear out the buffer of new indices. */
		newIndices.clear();

		/* Split each triangle into 4 new triangles. */
		for (GLuint i = 0; i < localIndices.size(); i += 3)
		{
			/* Get the middle points of each side of the triangle. */
			a = getMiddlePoint(localIndices.at(i + 0), localIndices.at(i + 1), 
					&localVertices, &middlePointIndexCache);
			b = getMiddlePoint(localIndices.at(i + 1), localIndices.at(i + 2),
					&localVertices, &middlePointIndexCache);
			c = getMiddlePoint(localIndices.at(i + 2), localIndices.at(i + 0),
					&localVertices, &middlePointIndexCache);

			/* Add 4 new triangles in place of 'tri' in the new vector. */

			/* Top  */
			newIndices.push_back(localIndices.at(i + 0));
			newIndices.push_back(a);
			newIndices.push_back(c);
			/* Bottom Right */
			newIndices.push_back(localIndices.at(i + 1));
			newIndices.push_back(b);
			newIndices.push_back(a);
			/* Bottom Left */
			newIndices.push_back(localIndices.at(i + 2));
			newIndices.push_back(c);
			newIndices.push_back(b);
			/* Middle */
			newIndices.push_back(a);
			newIndices.push_back(b);
			newIndices.push_back(c);

		}

		/* Update the vector of local indices. */
		localIndices = newIndices;
	}

	/* Set the vertices and indices of this mesh. */
	sphere.setVertices(&localVertices);
	sphere.setIndices(&localIndices);
	
	/* Generate buffer and vertex arrays. */
	sphere.genBufferArrayID();
	sphere.genVertexArrayID();

	/* Return the mesh. */
	return sphere;
}

Mesh* Geometry::loadObj(const char* objFile, const char* textFile)
{
	Mesh* obj = new Mesh();

	glm::vec3                        color = {+1.0f, +1.0f, +1.0f};
	std::vector<tinyobj::shape_t>    shapes;
	std::vector<tinyobj::material_t> materials;

	std::string errMsg = tinyobj::LoadObj(shapes, materials, objFile);

	if(!errMsg.empty())
		std::cerr << "Error loading obj: " << errMsg << std::endl;

	tinyobj::shape_t  s = shapes.at(0);

	std::vector<Vertex> localVertices;
	for(GLuint i = 0; i < (s.mesh.positions.size() / 3); i++) 
	{
		localVertices.push_back({
			{s.mesh.positions.at((3 * i) + 0), s.mesh.positions.at((3 * i) + 1), s.mesh.positions.at((3 * i) + 2)},
			color,
			{1 - s.mesh.texcoords.at((2 * i) + 0), s.mesh.texcoords.at((2 * i) + 1)}
		});
	}

	std::vector<GLushort> localIndices;
	for(GLuint i = 0; i < (s.mesh.indices.size() / 3); i++) 
	{
		localIndices.push_back((GLushort) s.mesh.indices.at((3 * i) + 0));
		localIndices.push_back((GLushort) s.mesh.indices.at((3 * i) + 1));
		localIndices.push_back((GLushort) s.mesh.indices.at((3 * i) + 2));
	}

		/* Set the vertices and indices of this mesh. */
	obj->setVertices(&localVertices);
	obj->setIndices(&localIndices);
	
	/* Generate buffer and vertex arrays. */
	obj->genBufferArrayID();
	obj->genVertexArrayID();
	obj->genTextureID(textFile);

	/* Return the mesh. */
	return obj;
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

void Mesh::genTextureID(const char* filename)
{

	/* Enable Texture 2D. */
	glEnable(GL_TEXTURE_2D);

	/* If the filename is null, do nothing. */
	if(filename != NULL) 
	{
		/* Load the SDL_Surface from the file. */
		SDL_Surface* textureSurface = IMG_Load(filename);
		/* The default color scheme is RGB. */
		GLenum colorScheme = GL_RGB;

		/* If the file is a bitmap, change the color scheme to BGR. */
		std::string file(filename);
		std::string ext = file.substr(file.find('.'), file.length() - 1);
		if(ext == ".bmp") 
			colorScheme = GL_BGR;

		/* If the texture surface is null, do nothing. */
		if(textureSurface != NULL) 
		{
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureSurface->w, 
				textureSurface->h, 0, colorScheme, GL_UNSIGNED_BYTE, 
				textureSurface->pixels);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
			glBindVertexArray(vertexArrayID);

			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
				(void*) ATTRIBUTE_2_OFFSET);
		}
	}
}
/******************************************************************************
*                                                                             *
*                          Mesh::genBufferArrayID()                           *
*                                                                             *
*******************************************************************************
* PARAMETERS                                                                  *
*  void                                                                       *
*                                                                             *
*******************************************************************************
* RETURNS                                                                     *
*  void                                                                       *
*                                                                             *
*******************************************************************************
* DESCRIPTION                                                                 *
*  Generates the graphics hardware buffers for data regarding this Mesh. The  *
*  two specific buffers for this class are the vertex buffer and the index    *
*  buffer. The IDs of these buffers are stored in the bufferIDs array.        *
*                                                                             *
*******************************************************************************/
void Mesh::genBufferArrayID()
{
	/* Generate the buffer space. */
	bufferIDs = new GLuint[numBuffers];
	glGenBuffers(numBuffers, bufferIDs);

	/* Create vertex buffer. */
	glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, vertexBufferSize(), vertices,
		GL_STATIC_DRAW);

	/* Create index buffer. */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIDs[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize(), indices, 
		GL_STATIC_DRAW);
}

/******************************************************************************
*                                                                             *
*                          Mesh::genVertexArrayID()                           *
*                                                                             *
*******************************************************************************
* PARAMETERS                                                                  *
*  void                                                                       *
*                                                                             *
*******************************************************************************
* RETURNS                                                                     *
*  void                                                                       *
*                                                                             *
*******************************************************************************
* DESCRIPTION                                                                 *
*  Generates the vertex array object buffer for this mesh. The vertex array   *
*  object keeps track of the vertex attribute locations for this specific     *
*  mesh. To draw this mesh, the vertex array object must be bound before      *
*  telling OpenGL to draw its elements. The vertex array ID is stored in the  *
*  vertexArrayID value.                                                       *
*                                                                             *
*******************************************************************************/
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
	glBindAttribLocation(Geometry::shader->getProgram(), 0, "modelPostion");
	glBindAttribLocation(Geometry::shader->getProgram(), 1, "modelColor");

	/* Vertex position attribute. */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
		(void*) ATTRIBUTE_0_OFFSET);

	/* Vertex color attribute. */
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,	sizeof(Vertex), 
		(void*) ATTRIBUTE_1_OFFSET);
}

/******************************************************************************
*                                                                             *
*                            Mesh::cleanUp() (Destructor)                     *
*                                                                             *
*******************************************************************************
* PARAMETERS                                                                  *
*  void                                                                       *
*                                                                             *
*******************************************************************************
* RETURNS                                                                     *
*  void                                                                       *
*                                                                             *
*******************************************************************************
* DESCRIPTION                                                                 *
*  Ensures that any allocated space is freed before the Mesh is discarded.    *
*  This call can be made explicitly or implicitly by the descructor.          *
*                                                                             *
*******************************************************************************/
void Mesh::cleanUp()
{
	/* Delete the buffers on the graphics hardware. */
	glDeleteBuffers(numBuffers, bufferIDs);
	glDeleteBuffers(1, &vertexArrayID);

	/* Free the space allocated on the heap for vertex/index data. */
	delete[] vertices;
	delete[] indices;
	delete[] bufferIDs;

	/* Remove any dangling pointers. */
	vertices = NULL;
	indices = NULL;
	bufferIDs = NULL;

	/* Set the number of vertices/indices to 0. */
	numVertices = numIndices = 0;
}
