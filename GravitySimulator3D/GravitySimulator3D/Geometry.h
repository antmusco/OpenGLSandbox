#pragma once

/******************************************************************************
*                                                                             *
*                              Included Header Files                          *
*                                                                             *
******************************************************************************/
#include <GL\glew.h>
#include <SDL\SDL.h>
#include <glm\glm.hpp>
#include <vector>
#include <map>
#include "Shader.h"

/******************************************************************************
*                                                                             *
*                           Defined Constants / Macros                        *
*                                                                             *
******************************************************************************/
#define DEFAULT_NUM_BUFFERS	    2
#define DEFAULT_DRAW_MODE       GL_TRIANGLES
#define DEFAULT_VERTEX_COLOR    glm::vec3(+1.0f, +1.0f, +1.0f)
#define ATTRIBUTE_0_OFFSET      (sizeof(GLfloat) * 0)
#define ATTRIBUTE_1_OFFSET      (sizeof(GLfloat) * 3)
#define ATTRIBUTE_2_OFFSET      (sizeof(GLfloat) * 6)
#define ATTRIBUTE_3_OFFSET      (sizeof(GLfloat) * 9)

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
*  textureCoordinate                                                          *
*          s, t values for the texture coordinate of this vertex.             *
*                                                                             *
*******************************************************************************
* DESCRIPTION                                                                 *
*  Struct representing a simple vertex in 3-D space. The vertex consists of   *
*  8 sequential float values: the x, y, and z coordinates of the position,    *
*  the r, g, and b values of the color, and the s, t values for the texture   *
*  coordinate.                                                                *
*                                                                             *
*******************************************************************************/
struct Vertex
{
	glm::vec3      position;
	glm::vec3      color;
	glm::vec3      normal;
	glm::vec2      textureCoordinate;
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
	GLushort       v1;
	GLushort       v2;
	GLushort       v3;
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
*          Number of vertices in this Mesh object.                            *
*  indices                                                                    *
*          Pointer to the written order in which the traingles are to be      *
*          drawn.                                                             *
*  numIndices                                                                 *
*          Number of indices used to draw the Mesh object.                    *
*  texutreSurface                                                             *
*          Pointer to the SDL_Surface conatining the texture data.            *
*  textureID                                                                  *
*          ID of the texture buffer in which the texture is located.          *
*  numBuffers                                                                 *
*          Number of buffers to be generated for the Mesh object.             *
*  bufferIDs                                                                  *
*          IDs of the buffers which has been generated for the Mesh.          *
*  vertexArrayID                                                              *
*          ID of the buffer in which the vertex array object for this Mesh    *
*          is located.                                                        *
*  drawMode                                                                   *
*          GLenum for the draw mode of this Mesh. Can be GL_TRIANGLES,        *
*          GL_LINES, GL_QUADS, etc.                                           *
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
	               Mesh();
				   Mesh(const Mesh& rhs);

	/* Calculate the number of bytes for the vertices. */
	GLsizeiptr	   vertexBufferSize()    const;
	/* Calculate the number of bytes for the indices. */
	GLsizeiptr	   indexBufferSize()     const;
	/* Generate the graphics buffers and IDs for the mesh.  */
	void           genBufferArrayID();
	/* Generate the texture buffer and ID for the mesh.  */
	void           genTextureID(const char* filename);
	/* Generate the vertex array object and ID for the mesh. */
	void           genVertexArrayID();

	/* Getters*/
	Vertex*        getVertices()         const   {  return vertices;       }
	Vertex         getVertex(GLuint i)   const   {  return vertices[i];    }
	GLuint         getNumVertices()      const   {  return numVertices;    }
	GLushort*      getIndices()          const   {  return indices;        }
	GLushort       getIndex(GLuint i)    const   {  return indices[i];     }
	GLuint         getNumIndices()       const   {  return numIndices;     }
	GLuint         getTextureID()        const   {  return textureID;      }
	GLuint         getNumBuffers()       const   {  return numBuffers;     }
	GLuint*        getBufferIDs()        const   {  return bufferIDs;      }
	GLuint         getBufferID(GLuint i) const   {  return bufferIDs[i];   } 
	GLuint         getVertexArrayID()    const   {  return vertexArrayID;  }
	GLenum         getDrawMode()         const   {  return drawMode;       }
											    						 
	/* Setters */							    						 
	void           setVertices(GLuint n, 
                               Vertex* a);
	void           setVertices(std::vector<Vertex>* v);
	void           setIndices(GLuint n, 
                              GLushort* a);
	void           setIndices(std::vector<GLushort>* v);
	void           setTextureID(GLuint t)        {  textureID        = t;  }
	void           setNumBuffers(GLuint n)       {  numBuffers       = n;  }
	void           setBufferIDs(GLuint* b)       {  bufferIDs        = b;  }
	void           setVertexArrayID(GLuint v)    {  vertexArrayID    = v;  }
	void           setDrawMode(GLenum d)         {  drawMode         = d;  }

	/* Destructor */ 
	void           cleanUp();

protected:
	/* Vertex Data */
	Vertex*        vertices;
	GLuint         numVertices;
	/* Index Data */
	GLushort*      indices;
	GLuint         numIndices;
	/* Texture Data */
	GLuint         textureID;
	/* Buffer Data */
	GLuint         numBuffers;
	GLuint*        bufferIDs;
	GLuint         vertexArrayID;
	/* Draw Data */
	GLenum         drawMode;
};

/******************************************************************************
*                                                                             *
*                           Geometry::Geometry (class)                        *
*                                                                             *
*******************************************************************************
* MEMBERS                                                                     *
*  shader (static)                                                            *
*          Shader program associated with all Geometries.                     *
*                                                                             *
*******************************************************************************
* DESCRIPTION                                                                 *
*  Class consisitng of static functions to create a series of shapes.         *
*                                                                             *
*******************************************************************************/
class Geometry
{
public:
	/* Shader program. */
	static Shader*   shader;
	/* Load from .obj file. */
	static Mesh*     loadObj(const char* objFile, 
                             const char* textFile = NULL);
};
