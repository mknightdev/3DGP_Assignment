#include <GL/glew.h>
#include <string>

struct ShaderProgram
{
public:
	ShaderProgram();	// Constructor
	~ShaderProgram();	// Deconstructor
	GLuint getId();
	void createVertexShader(const GLchar* vertShader);
	GLuint getSource(const GLchar* source, GLuint type);
	void createShaderProgram(const GLchar* vertSrc, const GLchar* fragSrc);

	/*
		Need a SOURCE
		Need an ID --
		Need to BIND the shader -- 
		Need to UNBIND the shader -- 	
	*/


private:
	GLuint id;	// Main program id 
	GLint success;

};