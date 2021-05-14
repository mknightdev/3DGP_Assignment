#include <GL/glew.h>
#include <string>


/// Texture struct to streamline loading textures and storing. 
/// 
/// The texture struct provides file loading and returning of its id. 
struct Texture
{
public:
	
	/// Enables the loading of a custom texture. 
	/// 
	/// Requires a string for the file name, and it's width and height. 
	/// @param image The loaded image of the texture. 
	/// @param x The width of the texture.
	/// @param y The height of the texture. 
	/// @attention This is the unsigned char* version!
	/// @see Texture(std::string fileName, int x, int y)

	Texture(unsigned char* image, int x, int y);

	/// Enables the loading of a custom texture. 
	/// 
	/// Requires a string for the file name, and it's width and height. 
	/// @param fileName The file path of the texture. 
	/// @param x The width of the texture.
	/// @param y The height of the texture. 
	/// @attention This is the std::string version!
	/// @see Texture(unsigned char* image, int x, int y)
	Texture(std::string fileName, int x, int y);

	/// Texture deconstructor
	///
	/// Deconstructor to clean up memory to prevent any leaks.
	/// It deletes the generated textures using glDeleteTextures.
	~Texture();

	/// Returns the id of the current texture.
	/// 
	/// Gets the GLuint of the current texture object and returns it. 
	/// @returns GLuint
	GLuint GetId();
private:
	GLuint id;
	int width = 0;
	int height = 0;
	unsigned char* data = NULL;
};
