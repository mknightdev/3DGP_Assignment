#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const GLchar* vertShader)
{

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

void ShaderProgram::createVertexShader(const GLchar* vertSrc)
{
	id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(id, 1, &vertSrc, NULL);
	glCompileShader(id);
}

GLuint ShaderProgram::getId()
{
	return id;
}






