#pragma comment( lib, "SDL2.lib" )
#pragma comment( lib, "SDL2main.lib" )
#pragma comment( lib, "SDL2_image.lib" )

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

/******************************************************************************
*                                                                             *
*                                      Macros                                 *
*                                                                             *
******************************************************************************/
#define ARRAY_SIZE(a) sizeof(a) / sizeof(*a)

Shader* Geometry::shader = NULL;

/******************************************************************************
*                                                                             *
*                       Mesh::Mesh  (constructor - overloaded)                *
*                                                                             *
*******************************************************************************
* PARAMETERS (1)                                                              *
*  void                                                                       *
*                                                                             *
* PARAMETERS (2)                                                              *
*  @param rhs                                                                 *
*           An instance of Mesh to be copied to this mesh.                    *
*                                                                             *
*******************************************************************************
* RETURNS (1)                                                                 *
*  void                                                                       *
*                                                                             *
* RETURNS (2)                                                                 *
*  void                                                                       *
*                                                                             *
*******************************************************************************
* DESCRIPTION (1)                                                             *
*  Default constructor which initializes all members of Mesh to 0 (NULL)      *
*  except for members with default values (numBuffers and drawMode).          *
*                                                                             *
* DESCRIPTION (2)                                                             *
*  Copy constructor which copis all of the data from one Mesh to a new one.   *
*                                                                             *
*******************************************************************************/
Mesh::Mesh() :
    /* Constructor Initialization. */
    vertices(0), numVertices(0),
    indices(0), numIndices(0),
    textureID(-1),
    numBuffers(DEFAULT_NUM_BUFFERS), bufferIDs(0), vertexArrayID(0),
    drawMode(DEFAULT_DRAW_MODE) 
{
	/* Empty. */
}
Mesh::Mesh(const Mesh& rhs) :
    /* Constructor Initialization. */
	numVertices(rhs.getNumVertices()),
	numIndices(rhs.getNumIndices()),
	textureID(rhs.getTextureID()),
	numBuffers(rhs.getNumBuffers()),
	vertexArrayID(rhs.getVertexArrayID()),
	drawMode(rhs.getDrawMode())
{
	/* Allocate space for the vertices, indices, and buffers on the heap. */
	vertices = new Vertex[rhs.getNumVertices()];
	indices  = new GLushort[rhs.getNumIndices()];
	bufferIDs = new GLuint[rhs.getNumBuffers()];

	/* Copy the bytes over from the rhs mesh. */
	memcpy(vertices, rhs.getVertices(), rhs.getNumVertices() * sizeof(Vertex));
	memcpy(indices, rhs.getIndices(), rhs.getNumIndices() * sizeof(GLushort));
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
*                                 Mesh::loadObj                               *
*                                                                             *
*******************************************************************************
* PARAMETERS                                                                  *
*  @param objFile                                                             *
*        The path to the OBJ file that is to be loaded. Must point to a valid *
*        OBJ file.                                                            *
*  @param textureFile (optional)                                              *
*        The path to the texture file to be loaded. Can point to any valid    *
*        image format. By default this parameter is NULL.                     *
*                                                                             *
*******************************************************************************
* RETURNS                                                                     *
*  void                                                                       *
*                                                                             *
*******************************************************************************
* DESCRIPTION                                                                 *
*  Loads an OBJ file and generates a Mesh object based on the Vertex and      *
*  Index data. This function also allows the option for the caller to apply a *
*  texture to the newly loaded Mesh via the textureFile parameter.            *
*                                                                             *
*******************************************************************************/
Mesh* Geometry::loadObj(const char* objFile, const char* textureFile)
{
	/* Create a new Mesh object on the heap. */
	Mesh* obj = new Mesh();

	/* Declare the Vectors for shape and material data. */
	std::vector<tinyobj::shape_t>    shapes;
	std::vector<tinyobj::material_t> materials;

	/* Load the indicated OBJ file and get the error message. */
	std::string errMsg = tinyobj::LoadObj(shapes, materials, objFile);

	/* If there was an error message, prompt the user and exit function. */
	if (!errMsg.empty())
	{
		std::cerr << "Error loading obj: " << errMsg << std::endl;
		return nullptr;
	}

	/* Get the shape from the file. */
	tinyobj::shape_t  s = shapes.at(0);

	/* Copy over the Vertex data. */
	std::vector<Vertex> localVertices;
	for(GLuint i = 0; i < (s.mesh.positions.size() / 3); i++) 
		localVertices.push_back({

			/* Vertex Position. */
			{s.mesh.positions.at((3 * i) + 0), 
			 s.mesh.positions.at((3 * i) + 1), 
			 s.mesh.positions.at((3 * i) + 2)},

			/* Vertex Color. */
			DEFAULT_VERTEX_COLOR,

			/* Vertex Normal. */
			{s.mesh.normals.at((3 * i) + 0), 
			 s.mesh.normals.at((3 * i) + 1), 
			 s.mesh.normals.at((3 * i) + 2)},

			/* U, V Coordinate. */
			{    s.mesh.texcoords.at((2 * i) + 0), 
			 1 - s.mesh.texcoords.at((2 * i) + 1) }

		});

	/* Copy the Index data. */
	std::vector<GLushort> localIndices;
	for(GLuint i = 0; i < s.mesh.indices.size(); i++) 
		localIndices.push_back(
			(GLushort) s.mesh.indices.at(i)
		);

	/* Set the vertices and indices of this mesh. */
	obj->setVertices(&localVertices);
	obj->setIndices(&localIndices);
	
	/* Generate buffer and vertex arrays. */
	obj->genBufferArrayID();
	obj->genVertexArrayID();

	/* If the texture file was provided, generate the texture. */
	if (textureFile != NULL)
		obj->genTextureID(textureFile);

	/* Return the mesh. */
	return obj;
}

/******************************************************************************
*                                                                             *
*                               Mesh::genTextureID                            *
*                                                                             *
*******************************************************************************
* PARAMETERS                                                                  *
*  filename                                                                   *
*        The path to the texture that is to be loaded. Can be of any valid    *
*        image format.                                                        *
*                                                                             *
*******************************************************************************
* RETURNS                                                                     *
*  void                                                                       *
*                                                                             *
*******************************************************************************
* DESCRIPTION                                                                 *
*  Generates the texture buffer and sends the data from the indicated file    *
*  down to the graphics hardware. The texture ID is saved to the indicated    *
*  parameter for this Mesh.                                                   *
*                                                                             *
*******************************************************************************/
void Mesh::genTextureID(const char* filename)
{
	/* Enable Texture 2D. */
	glEnable(GL_TEXTURE_2D);

	/* If the filename is null, do nothing. */
	if(filename != NULL) 
	{
		/* Load the SDL_Surface from the file. */
		SDL_Surface* textureSurface = IMG_Load(filename);

		/* If the image was not loaded correctly, do nothing. */
		if(textureSurface != NULL) 
		{
			/* The default color scheme is RGB. */
			GLenum colorScheme = GL_RGB;

			/* If the file is a bitmap, change the color scheme to BGR. */
			std::string file(filename);
			std::string ext = file.substr(file.find('.'), file.length() - 1);
			if(ext == ".bmp") 
				colorScheme = GL_BGR;

			/* Generate the texture buffer. */
			glGenTextures(1, &textureID);

			/* Bind the texture ID to the appropriate binding point. */
			glBindTexture(GL_TEXTURE_2D, textureID);

			/* Send the image data down to the graphics card. */
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureSurface->w, 
				textureSurface->h, 0, colorScheme, GL_UNSIGNED_BYTE, 
				textureSurface->pixels);

			/* Set the desred texture parameters. */
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
	}
}
/******************************************************************************
*                                                                             *
*                            Mesh::genBufferArrayID                           *
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
*                            Mesh::genVertexArrayID                           *
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

	/* Bind the vertex buffer. */
	glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[0]);

	/* Enable the vertex attributes. */
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	
	/* Bind the attributes to the locations in the shader program. */
	//glBindAttribLocation(Geometry::shader->getProgram(), 0, "modelPostion");
	//glBindAttribLocation(Geometry::shader->getProgram(), 1, "modelColor");
	//glBindAttribLocation(Geometry::shader->getProgram(), 2, "modelNormal");
	//glBindAttribLocation(Geometry::shader->getProgram(), 3, "modelTexCoord");

	/* Vertex position attribute. */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
		(void*) ATTRIBUTE_0_OFFSET);

	/* Vertex color attribute. */
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,	sizeof(Vertex), 
		(void*) ATTRIBUTE_1_OFFSET);

	/* Vertex normal attribute. */
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*)ATTRIBUTE_2_OFFSET);

	/* Vertex texture coordinate attribute. */
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*)ATTRIBUTE_3_OFFSET);
}

/******************************************************************************
*                                                                             *
*                             Mesh::cleanUp (Destructor)                      *
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
