/******************************************************************************
*                                                                             *
*                              Included Header Files                          *
*                                                                             *
******************************************************************************/
#include "Shader.h"
#include <iostream>
#include <fstream>

/******************************************************************************
*                                                                             *
*                          Shader::Shader (Constructor)                       *
*                                                                             *
*******************************************************************************
* PARAMETERS                                                                  *
*  @param vertexShaderFilepath                                                *
*           Path to the GLSL file containing the vertex shader source code.   *
*  @param fragmentShaderFilepath                                              *
*           Path to the GLSL file containing the fragment shader soruce code. *
*                                                                             *
*******************************************************************************
* RETURNS                                                                     *
*  void                                                                       *
*                                                                             *
*******************************************************************************
* DESCRIPTION                                                                 *
*  Constructor for the Shader class. This constructor loads the text from the *
*  indicated files into strings and compiles them into executable shader      *
*  objects. The function then links them into an executable shader program,   *
*  and tells OpenGL to use this program for shading.                          *
*                                                                             *
******************************************************************************/
Shader::Shader(std::string vertexShaderFilepath, 
               std::string fragmentShaderFilepath)
{
	/* Load the source code (GLSL) into the indicated strings.*/
	std::string vertexShaderSource = loadShaderSource(vertexShaderFilepath);
	std::string fragmentShaderSource = loadShaderSource(fragmentShaderFilepath);

	/* Create the shader program. */
	program = glCreateProgram();

	/* Create the vertex shader and the fragment shader */
	shaders[0] = glCreateShader(GL_VERTEX_SHADER);
	shaders[1] = glCreateShader(GL_FRAGMENT_SHADER);

	/* Add the source code to the shaders. */
	const char* input[1];
	input[0] = vertexShaderSource.c_str();
	glShaderSource(shaders[0], 1, input, 0);
	input[0] = fragmentShaderSource.c_str();
	glShaderSource(shaders[1], 1, input, 0);

	/* Compile the added source code. */
	glCompileShader(shaders[0]);
	glCompileShader(shaders[1]); 

	/* If either of the shaders did not compile correctly, return. */
	if (checkShaderError(shaders[0]) || checkShaderError(shaders[1]))
		return;

	/* Attach the shaders to the program. */
	glAttachShader(program, shaders[0]);
	glAttachShader(program, shaders[1]);

	/* Bind the indicated data attributes to the variables. */
	glBindAttribLocation(program, 0, "modelPosition");
	glBindAttribLocation(program, 1, "modelColor");
	glBindAttribLocation(program, 2, "modelNormal");
	glBindAttribLocation(program, 3, "modelTexCoord");

	/* Link the shader objects. */
	glLinkProgram(program);

	/* If linking failed, return. */
	if (checkProgramError(program))
		return;

	/* Tell OpenGL to use this program. */
	glUseProgram(program);
}

/******************************************************************************
*                                                                             *
*                           Shader::loadShaderSource                          *
*                                                                             *
*******************************************************************************
* PARAMETERS                                                                  *
*  @param shaderFilepath                                                      *
*           Path to the GLSL file containing the shader source code.          *
*                                                                             *
*******************************************************************************
* RETURNS                                                                     *
*  @return                                                                    *
*           A string containing the GLSL source code for the indicated file.  *
*                                                                             *
*******************************************************************************
* DESCRIPTION                                                                 *
*  This function opens a GLSL source code file and copies the entire contents *
*  of the file to a string and returns it to the caller.                      *
*                                                                             *
******************************************************************************/
std::string Shader::loadShaderSource(std::string shaderFilepath)
{
	/* Declare the input file and output string. */
	std::ifstream file;
	std::string output;

	/* Open the indicated file. */
	file.open(shaderFilepath.c_str());
	
	/* If the file opened, read the contents. */
	if (file.is_open())
	{
		/* Buffer for reading lines from the file. */
		std::string buffer;

		/* While not EOF / Error. */
		while (file.good())
		{
			/* Read the line and append it to the output string. */
			std::getline(file, buffer);
			output.append(buffer + "\n");
		}
	}
	/* If the file was not opened, output the error. */
	else
	{
		std::cerr << "Unable to load shader: " << shaderFilepath << std::endl;
	}

	/* For ensuring the source code is read correctly. */
	//std::cout << output << std::endl; 
	/* Return the string to the caller. */
	return output;
}

/******************************************************************************
*                                                                             *
*                           Shader::checkShaderError                          *
*                                                                             *
*******************************************************************************
* PARAMETERS                                                                  *
*  @param shaderID                                                            *
*           The handle of the shader to check for compile error.              *
*                                                                             *
*******************************************************************************
* RETURNS                                                                     *
*  @return                                                                    *
*           A boolean value indicating whether there was an error or not.     *
*                                                                             *
*******************************************************************************
* DESCRIPTION                                                                 *
*  This function tests a shader to see whether there was an error compiling.  *
*                                                                             *
******************************************************************************/
bool Shader::checkShaderError(GLuint shaderID)
{
	/* Get the compile status and store it in the GLint. */
	GLint compileStatus;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);

	/* If the compile status is not 1, output the error information. */
	if (compileStatus != GL_TRUE)
	{
		/* Get the handle of the info log. */
		GLint infoLogLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		/* Get the last entry in the info log. */
		GLsizei bufferSize;
		glGetShaderInfoLog(shaderID, infoLogLength, &bufferSize, buffer);

		/* Output the error log. */
		std::cout << buffer << std::endl;

		/* Delete dynamic memory and return that there was an error. */
		delete [] buffer;
		return true;
	}
	/* Return there was not an error. */
	return false;
}

/******************************************************************************
*                                                                             *
*                           Shader::checkProgramError                         *
*                                                                             *
*******************************************************************************
* PARAMETERS                                                                  *
*  @param programID                                                           *
*           The handle of the program to check for linking error.             *
*                                                                             *
*******************************************************************************
* RETURNS                                                                     *
*  @return                                                                    *
*           A boolean value indicating whether there was an error or not.     *
*                                                                             *
*******************************************************************************
* DESCRIPTION                                                                 *
*  This function tests a program to see whether there was an error linking.   *
*                                                                             *
******************************************************************************/
bool Shader::checkProgramError(GLuint programID)
{
	/* Get the link status and store it in the GLint. */
	GLint linkStatus;
	glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);

	/* If the link status is not 1, output the error information. */
	if (linkStatus != GL_TRUE)
	{
		/* Get the handle of the info log. */
		GLint infoLogLength;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		/* Get the last entry in the info log. */
		GLsizei bufferSize;
		glGetProgramInfoLog(programID, infoLogLength, &bufferSize, buffer);

		/* Output the error log. */
		std::cout << buffer << std::endl;

		/* Delete dynamic memory and return that there was an error. */
		delete[] buffer;
		return true;
	}
	/* Return there was not an error. */
	return false;
}

/******************************************************************************
*                                                                             *
*                                  Shader::use                                *
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
*  This function tells OpenGL to use this shader for rendering objects.       *
*                                                                             *
******************************************************************************/
void Shader::use()
{
	glUseProgram(program);
}
