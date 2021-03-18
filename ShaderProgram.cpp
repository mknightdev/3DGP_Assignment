#include "ShaderProgram.h"




ShaderProgram::ShaderProgram(const GLchar* vertShader, const GLchar* fragShader)
{
	if (!success)
	{
		throw std::exception();
	}
}

ShaderProgram::~ShaderProgram()
{
	// Detach and destroy the shader objects. These are no longer needed
	// because we now have a complete shader program.
	//glDetachShader(programId, vertexShaderId);
	//glDeleteShader(vertexShaderId);
	//glDetachShader(programId, fragmentShaderId);
	//glDeleteShader(fragmentShaderId);
}

GLuint ShaderProgram::getId()
{
	return id;
}

void ShaderProgram::createVertShader()
{
	id = glCreateShader(GL_VERTEX_SHADER);
}

void ShaderProgram::createFragShader()
{
	id = glCreateShader(GL_FRAGMENT_SHADER);
}

