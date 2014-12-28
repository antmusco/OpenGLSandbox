#include "Shader.h"
#include "Transform.h"
#include <fstream>
#include <iostream>

/* Local static function prototypes. */
static void checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
static std::string fileToString(const std::string& fileName);
static GLuint createShader(const std::string& text, GLenum shaderType);

/**
 * Constructor for the Shader class. Builds the shader program based on the indicated filename.
 *
 * @param filename
 *			Name of the file for the vertex shader and fragment shader. The vertex shader is
 *			indicated as the file titled <filename>.vs, whereas the fragment shader is
 *			indicated as the file titled <filename>.fs.
 */
Shader::Shader(const std::string& filename)
{
	/* Create the shader program. */
	m_program = glCreateProgram();

	/* Create the vertex shader and the fragment shader from the indicated filename. */
	m_shaders[0] = createShader(fileToString(filename + ".vs"), GL_VERTEX_SHADER);
	m_shaders[1] = createShader(fileToString(filename + ".fs"), GL_FRAGMENT_SHADER);

	/* Attach the vertex shader and the fragment shader to the shader program. */
	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		glAttachShader(m_program, m_shaders[i]);
	}

	/* Name the vertex shader attributes
	 *   0 - Vertex Shader   = position
	 *   1 - Fragment Shader = texture */
	glBindAttribLocation(m_program, 0, "position");
	glBindAttribLocation(m_program, 1, "textCoord");

	/* Link the shaders together, and check for an error in linking. */
	glLinkProgram(m_program);
	checkShaderError(m_program, GL_LINK_STATUS, true, "Error: Program linking failed: ");

	/* Validate the new shader program, and check for an error in validation. */
	glValidateProgram(m_program);
	checkShaderError(m_program, GL_VALIDATE_STATUS, true, "Error: Program is invalid: ");

	/* Get a reference to the 'transform' variable from the shader program  *
	 * (see basicShader.vs )                                                */
	m_uniforms[TRANSFORM_U] = glGetUniformLocation(m_program, "transform");
}

/**
 * Destructor fo the Shader class. Detaches the shaders from the shader program and
 * deletes the shader program.
 */
Shader::~Shader()
{
	/* Detatch the shaders. */
	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		glDetachShader(m_program, m_shaders[i]);
		glDeleteShader(m_shaders[i]);
	}

	/* Delete the program. */
	glDeleteProgram(m_program);
}

/**
 * Calls OpenGL and tells it to use this specific shader program for shading.
 */
void Shader::bind()
{
	/* Use the shader. */
	glUseProgram(m_program);
}

/**
 * Performs the transformation of the 
 *
 * @param transform
 *			Transform object which indicates the combined transformation model for the rendered  
 *			objects.
 * @param camera
 *			Camera object which indicates the view projection of the rendered objects.
 */
void Shader::update(const Transform& transform, const Camera& camera)
{
	/* Multiply together the camera view projection by the transformation model. */
	glm::mat4 model = camera.getViewProjection() * transform.getModel();

	/* View https://www.khronos.org/opengles/sdk/docs/man/xhtml/glUniform.xml  for more	
	 * information regarding uniform transformations. */
	glUniformMatrix4fv(m_uniforms[TRANSFORM_U], 1, GL_FALSE, &model[0][0]);
}

/**
 * Creates a shader of the inidcated type based on the source code passed to the
 * function.
 *
 * @param sourceCode
 *			Source code (as a string object) for the shader.
 * @param shaderType
 *			Type of shader to be created (GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, etc.)
 */
static GLuint createShader(const std::string& sourceCode, GLenum shaderType)
{
	/* Shader ID to be returned to the caller. */
	GLuint shader = glCreateShader(shaderType);
	
	/* If the shader was not created, return 0. */
	if (shader == 0)
	{
		std::cerr << "Error: Shader creation failed!" << std::endl;
		return 0;
	}

	/* Arrays for passing the source coude and the length of the source code to the
	 * shader (there may be more than one, which is why they must be in an array). */
	const GLchar *shaderSourceStrings[1];
	GLint shaderSourceStringLengths[1];

	/* Put the indicated text and its length in the indicated arrays.*/
	shaderSourceStringLengths[0] = sourceCode.length();
	shaderSourceStrings[0] = sourceCode.c_str();
	
	/* Bind the source code to the shader, then compile the shader. */
	glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLengths);
	glCompileShader(shader);

	/* Validate that the shader compiled correctly. */
	checkShaderError(shader, GL_COMPILE_STATUS, false, "Error: Shader compilation failed: ");
	
	/* Return the compiled shader to the caller. */
	return shader;
}

/**
 * Loads the text from a file into a string and returns it to the caller.
 *
 * @param fileName
 *			Name of the file to load into a string.
 */
static std::string fileToString(const std::string& fileName)
{
	/* Create a file object and open the file with the given filename. */
	std::ifstream file;
	file.open(( fileName ).c_str()); // Convert string to c-style string.

	/* String which will be returned to the user. */
	std::string output;
	std::string line;

	/* If the file is open, read the file. */
	if (file.is_open())
	{
		/* Read line into output until end of file. */
		while (file.good()) // good() will also fail on logical error or io error.
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}

	/* Return string to the caller. */
	return output;
}

/**
* Simple validation function which checks to see if there has been an error during
* shader construction.
*
* @param shader
*			ID of the shader to be validated.
* @param flag    
*			Status to test for error.              
* @param isProgram
*			Whether or not a shader program is being tested.
* @param errorMessage
*			Message to display if validation fails.
* 
*/
static void checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	/* Success flag. */
	GLint success = 0;
	/* Buffer for the error message. */
	GLchar error[1024] = { 0 };

	/* Validate the shader or the program. */
	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	/* If the validation failed, get the info log and output failure. */
	if (success == GL_FALSE) 
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		/* Output the error message and info log to cerr. */
		std::cerr << errorMessage << ": " << error << "'" << std::endl;
	}
}
