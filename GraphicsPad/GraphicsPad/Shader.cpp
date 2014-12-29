#include "Shader.h"
#include <iostream>
#include <fstream>

Shader::Shader(std::string vertexShaderFilepath, std::string fragmentShaderFilepath)
{
	std::string vertexShaderSource = loadShaderSource(vertexShaderFilepath);
	std::string fragmentShaderSource = loadShaderSource(fragmentShaderFilepath);

	program = glCreateProgram();
	/* Compile and check the shaders. */
	compileShader(vertexShaderSource, GL_VERTEX_SHADER);
	compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
	if (checkShaderError(shaders[0]) || checkShaderError(shaders[1]))
		return;

	/* Link and check the program. */
	glAttachShader(program, shaders[0]);
	glAttachShader(program, shaders[1]);
	glBindAttribLocation(program, 0, "position");
	glBindAttribLocation(program, 1, "color");
	glLinkProgram(program);
	if (checkProgramError(program))
		return;

	glUseProgram(program);
}


Shader::~Shader()
{
}

std::string Shader::loadShaderSource(std::string shaderFilepath)
{
	std::ifstream file;
	std::string output;
	file.open(shaderFilepath.c_str());
	if (file.is_open())
	{
		std::string line;
		while (file.good())
		{
			std::getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to load shader: " << shaderFilepath << std::endl;
	}
	
	//std::cout << output << std::endl; /* For ensuring the source code is read correctly. */
	return output;
}

void Shader::compileShader(std::string sourceCode, GLenum shaderType)
{
	int index;
	switch (shaderType)
	{
	case GL_VERTEX_SHADER:
		index = 0;
		break;
	case GL_FRAGMENT_SHADER:
		index = 1;
		break;
	}
	shaders[index] = glCreateShader(shaderType);
	const char* input[1];
	input[0] = sourceCode.c_str();
	glShaderSource(shaders[index], 1, input, 0);
	glCompileShader(shaders[index]);
}

bool Shader::checkShaderError(GLuint shaderID)
{
	GLint compileStatus;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE)
	{
		GLint infoLogLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		glGetShaderInfoLog(shaderID, infoLogLength, &bufferSize, buffer);
		std::cout << buffer << std::endl;

		delete [] buffer;
		return true;
	}
	return false;
}

bool Shader::checkProgramError(GLuint programID)
{
	GLint linkStatus;
	glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE)
	{
		GLint infoLogLength;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		glGetProgramInfoLog(programID, infoLogLength, &bufferSize, buffer);
		std::cout << buffer << std::endl;

		delete[] buffer;
		return true;
	}
	return false;
}