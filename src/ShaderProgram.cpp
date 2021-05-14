#include "ShaderProgram.h"

#include <iostream>
#include <vector>


GLuint ShaderProgram::getId()
{
	return programID;
}

void ShaderProgram::CreateShader(const GLchar* vertSrc, const GLchar* fragSrc)
{
	// Get shader source. 
	vertShader = GetSource(vertSrc, GL_VERTEX_SHADER);
	fragShader = GetSource(fragSrc, GL_FRAGMENT_SHADER);

	// Create a program for the shaders.
	programID = glCreateProgram();

	// Attach shaders to the program. 
	glAttachShader(programID, vertShader);
	glAttachShader(programID, fragShader);

	// Link the program.
	glLinkProgram(programID);

	// Error checking.
	GLint success;
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		// Retrieve error and store it. 
		int length;
		glGetShaderiv(programID, GL_INFO_LOG_LENGTH, &length);
		char message[LOG_LENGTH];
		glGetShaderInfoLog(programID, LOG_LENGTH, NULL, message);

		// Display error message.
		std::cout << "Shader Failed to Link" << std::endl;
		std::cout << message << std::endl;
	}

	// Bind Attributes.
	glBindAttribLocation(programID, 0, "a_Position");
	glBindAttribLocation(programID, 1, "a_TexCoord");
	glBindAttribLocation(programID, 2, "a_Normal");

	// Clean up.
	glDetachShader(programID, vertShader);
	glDeleteShader(vertShader);
	glDetachShader(programID, fragShader);
	glDeleteShader(fragShader);

	// Set the current program to be used.
	glUseProgram(programID);
}

GLuint ShaderProgram::GetSource(const GLchar* source, GLuint type)
{
	GLuint id = glCreateShader(type);
	glShaderSource(id, 1, &source, NULL);
	glCompileShader(id);

	// Error checking
	GLint success;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	// If result is false, it didn't compile successfully. 
	if (!success)
	{
		int length; 
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char message[LOG_LENGTH];
		glGetShaderInfoLog(id, LOG_LENGTH, NULL, message);

		// Display error
		if (type == GL_VERTEX_SHADER)
		{
			std::cout << "Failed to compile VERTEX shader" << std::endl;
			std::cout << message << std::endl;
		}
		else
		{
			std::cout << "Failed to compile FRAGMENT shader" << std::endl;
			std::cout << message << std::endl;
		}
		glDeleteShader(id);
		return 0;
	}

	return id;
}







