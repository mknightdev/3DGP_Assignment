#include <GL/glew.h>
#include <string>

const GLsizei LOG_LENGTH = 512;


struct ShaderProgram
{
public:
	GLuint getId();	// Returns the program id 
	GLuint GetSource(const GLchar* source, GLuint type);	// Get the source of the shaders
	void CreateShader(const GLchar* vertSrc, const GLchar* fragSrc);	// Create Shaders
private:
	GLuint programID;	// Main program id 
};