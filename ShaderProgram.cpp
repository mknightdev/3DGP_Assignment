#include "ShaderProgram.h"

#include <iostream>

ShaderProgram::ShaderProgram()
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


void ShaderProgram::createShaderProgram(const GLchar* vertSrc, const GLchar* fragSrc)
{
	GLuint vertShaderID = getSource(vertSrc, GL_VERTEX_SHADER);
	GLuint fragShaderID = getSource(fragSrc, GL_FRAGMENT_SHADER);
	id = glCreateProgram();

	glAttachShader(id, vertShaderID);
	glAttachShader(id, fragShaderID);

	GLint success;
	glLinkProgram(id);
	glGetProgramiv(id, GL_LINK_STATUS, &success);

	if (!success)
	{
		std::cout << "[ShaderProgram] Error linking shader" << std::endl;
		throw std::exception();
	}

	glBindAttribLocation(id, 0, "a_Position");
	glBindAttribLocation(id, 1, "a_TexCoord");
	glBindAttribLocation(id, 2, "a_Normal");

	glDetachShader(id, vertShaderID);
	glDeleteShader(vertShaderID);
	glDetachShader(id, fragShaderID);
	glDeleteShader(fragShaderID);
}

GLuint ShaderProgram::getSource(const GLchar* source, GLuint type)
{
	GLint success;

	GLuint shaderID = glCreateShader(type);
	glShaderSource(shaderID, 1, &source, NULL);
	glCompileShader(shaderID);

	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		if (type == GL_VERTEX_SHADER)
		{
			std::cout << "[ShaderProgram] Vertex Shader" << std::endl;
		}
		else { std::cout << "[ShaderProgram] Fragment Shader" << std::endl; }

		throw std::exception();
	}

	return shaderID;
}






