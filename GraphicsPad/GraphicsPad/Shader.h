#pragma once

#include <string>
#include <gl\glew.h>

class Shader
{
public:
	Shader(std::string vertexShaderFilepath, std::string fragmentShaderFilepath);
	~Shader();
private:
	GLuint shaders[2];
	GLuint program;
	std::string loadShaderSource(std::string shaderFilepath);
	void compileShader(std::string sourceCode, GLenum shaderType);
	bool checkShaderError(GLuint shaderID);
	bool checkProgramError(GLuint program);
};

