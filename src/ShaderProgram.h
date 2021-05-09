#include <GL/glew.h>
#include <string>


/// ShaderProgram struct to bind into a shader program.
/// 
/// This struct creates a shader program, returns its id and source. It takes in a vertex shader
/// and fragment shader. 
const GLsizei LOG_LENGTH = 512;

struct ShaderProgram
{
public:
	/// Returns the program id. 
	/// 
	/// @returns GLuint 
	GLuint getId();

	/// Returns the shader source.
	/// 
	/// Takes in shader source and the shader type to return its id. 
	/// @returns GLuint
	GLuint GetSource(const GLchar* source, GLuint type);

	/// Creates the shader program. 
	/// 
	/// Takes in both vertex shader and fragment shader to combine into a shader program. 
	/// @param vertSrc The vertex shader. 
	/// @param fragSrc The fragment shader. 
	/// @returns void
	void CreateShader(const GLchar* vertSrc, const GLchar* fragSrc);
private:
	GLuint programID;
};