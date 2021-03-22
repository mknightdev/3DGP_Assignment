#include <GL/glew.h>
#include <string>

struct ShaderProgram
{
public:
	~ShaderProgram();	// Deconstructor
	GLuint getId();	// Returns the program id 
	GLuint getSource(const GLchar* source, GLuint type); // Get the source of the shaders
	void createShaderProgram(const GLchar* vertSrc, const GLchar* fragSrc);	// Create Shaders

private:
	GLuint id;	// Main program id 
	GLint success;

};