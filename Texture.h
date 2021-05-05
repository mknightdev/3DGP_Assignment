#include <GL/glew.h>
#include <string>

struct Texture
{
public:
	Texture(unsigned char* image, int x, int y);
	GLuint GetId();
private:
	GLuint id;
	int w = 0;
	int h = 0;
};
