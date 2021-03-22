#include <GL/glew.h>
#include <string>

struct ShaderProgram
{
public:
	ShaderProgram(const GLchar* vertShader);	// Constructor
	~ShaderProgram();	// Deconstructor
	GLuint getId();
	void createVertexShader(const GLchar* vertShader);

	/*
		Need a SOURCE
		Need an ID --
		Need to BIND the shader -- 
		Need to UNBIND the shader -- 	
	*/


private:
	GLuint id;	// Main program id 
	GLint success = 0;

};