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
******************************************************************************/
class Shader
{
/*Public Members.*/
public:
	/* Constructor. */
	Shader(std::string vertexShaderFilepath, std::string fragmentShaderFilepath);
	/* Destructor. */
	~Shader() {}
	/* use */
	void use();
	GLuint getProgram() const { return program; }
/* Private Members.*/
private:
	/* Shader handles (Vertex, Fragment). */
	GLuint shaders[2];
	/* Program handle. */
	GLuint program;
	/* loadShaderSource */
	std::string loadShaderSource(std::string shaderFilepath);
	/* compileShader */
	void compileShader(std::string sourceCode, GLenum shaderType);
	/* checkShaderError */
	bool checkShaderError(GLuint shaderID);
	/* checkProgramError */
	bool checkProgramError(GLuint program);
};

