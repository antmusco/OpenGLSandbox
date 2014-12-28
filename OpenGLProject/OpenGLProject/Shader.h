#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glew.h>
#include "Transform.h"
#include "Camera.h"

class Shader
{
public:
/* Public functions. */
	Shader(const std::string& filename);
	~Shader();
	void bind();
	void update(const Transform& transform, const Camera& camera);

private:
/* Private members. */
	enum
	{
		TRANSFORM_U,
		NUM_UNIFORMS
	};
	static const unsigned int NUM_SHADERS = 2;
	GLuint m_program;
	GLuint m_shaders[NUM_SHADERS];
	GLuint m_uniforms[NUM_UNIFORMS];

/* Private functions. */
	Shader(const Shader& other) {}	
	void operator=( const Shader& rhs ) {}
};

#endif // SHADER_H