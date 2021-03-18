#include <GL/glew.h>
#include <string>

struct ShaderProgram
{
public:
	ShaderProgram(const GLchar* vertShader, const GLchar* fragShader);	// Constructor
	~ShaderProgram();	// Deconstructor
	GLuint getId();
	void createVertShader();
	void createFragShader();
private:
	GLuint id; 
	GLint success = 0;

};