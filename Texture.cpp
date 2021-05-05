#include "Texture.h"

#include <exception>

Texture::Texture(unsigned char* image, int x, int y)
{
	w = x;
	h = y;
	glGenTextures(1, &id);

	if (!id)
	{
		throw std::exception();
	}

	glBindTexture(GL_TEXTURE_2D, id);

	// Upload the image data to the bound texture unit in the GPU
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, image);

	// Free the loaded data because we now have a copy on the GPU
	free(image);

	// Generate Mipmap so the texture can be mapped correctly
	glGenerateMipmap(GL_TEXTURE_2D);

	// Unbind the texture because we are done operating on it
	glBindTexture(GL_TEXTURE_2D, 0);

}

GLuint Texture::GetId()
{
	return id;
}
