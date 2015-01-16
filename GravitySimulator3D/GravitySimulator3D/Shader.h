#pragma once

/******************************************************************************
*                                                                             *
*                              Included Header Files                          *
*                                                                             *
******************************************************************************/
#include <string>
#include <gl\glew.h>

/******************************************************************************
*																			  *
*								Shader Class 								  *
*																			  *
 ******************************************************************************
 *  shaders                                                                   *
 *          Array containing the IDs of the shader binaries associated with   *
 *          this object.                                                      *
 *  program                                                                   *
 *          ID of the program associated with this object.                    *
 *                                                                            *
 ******************************************************************************
 * DESCRIPTION                                                                *
 *  Class which manages the compilation and linking of shader programs.       *
 *                                                                            *
 ******************************************************************************/
class Shader
{
/*Public Members.*/
public:

	/* Constructors. */
	       Shader(std::string vertexShaderFilepath, 
	              std::string fragmentShaderFilepath);
	       Shader() {}

	/* Tell OpenGL to use this program. */
	void   use();

	/* Getters. */
	GLuint getProgram() const { return program; }

	/* Destructor. */
	       ~Shader() {}

/* Private Members.*/
private:

	/* Shader handles (Vertex, Fragment). */
	GLuint      shaders[2];
	/* Program handle. */
	GLuint      program;
	/* loadShaderSource */
	std::string loadShaderSource(std::string shaderFilepath);
	/* compileShader */
	void        compileShader(std::string sourceCode, 
	                          GLenum      shaderType);
	/* checkShaderError */
	bool        checkShaderError(GLuint shaderID);
	/* checkProgramError */
	bool        checkProgramError(GLuint program);
};
