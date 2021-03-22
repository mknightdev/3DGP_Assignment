#include "ShaderProgram.h"

#include <iostream>


ShaderProgram::~ShaderProgram()
{
	glUseProgram(0);
}

GLuint ShaderProgram::getId()
{
	return id;
}

void ShaderProgram::createShaderProgram(const GLchar* vertSrc, const GLchar* fragSrc)
{
	GLuint _vertShaderID = getSource(vertSrc, GL_VERTEX_SHADER);
	GLuint _fragShaderID = getSource(fragSrc, GL_FRAGMENT_SHADER);
	id = glCreateProgram();

	glAttachShader(id, _vertShaderID);
	glAttachShader(id, _fragShaderID);

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

	glDetachShader(id, _vertShaderID);
	glDeleteShader(_vertShaderID);
	glDetachShader(id, _fragShaderID);
	glDeleteShader(_fragShaderID);

	glUseProgram(id);
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






