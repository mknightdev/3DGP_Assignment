#include "VertexBuffer.h"

#include <exception>

VertexBuffer::VertexBuffer()
{
	// Create a new VBO on the GPU and bind it
	glGenBuffers(1, &id);

	if (!id)
	{
		throw std::exception();
	}

	// False = Nothing on GPU
	/* True = If you do use it without uploading any vertices, 
	you end up uploading no data to the graphics card. */
	components = 0;
	dirty = true; 
}

VertexBuffer::~VertexBuffer()
{
	// Gets executed when the class goes out of scope. 

	// Delete object at the first position of id (array). 
	glDeleteBuffers(1, &id);
}

int VertexBuffer::getComponents()
{
	return components;
}

void VertexBuffer::add(glm::vec2 value)
{
	data.push_back(value.x);
	data.push_back(value.y);

	components = 2;
	dirty = true;
}

void VertexBuffer::add(glm::vec3 value)
{
	data.push_back(value.x);
	data.push_back(value.y);
	data.push_back(value.z);

	components = 3;
	dirty = true;
}

void VertexBuffer::add(glm::vec4 value)
{
	data.push_back(value.x);
	data.push_back(value.y);
	data.push_back(value.z);
	data.push_back(value.w);

	components = 4;
	dirty = true;
}
 
GLuint VertexBuffer::getId()
{
	// If it needs re-uploading
	if (dirty)
	{
		// Upload 
		glBindBuffer(GL_ARRAY_BUFFER, id);

		// Upload a copy of the data from memory into the new VBO
		/* Data will be 9, Float is 8 (bytes)
		*  9 * 8 = Total size of data.
		*  Then, supply a point at first location.
		*  Note: .at is safer to use, as it will throw an exception
		*  if we are out of its bounds. */
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(data.at(0)), &data.at(0), GL_STATIC_DRAW);

		// Reset the state
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		// Now been synchronised 
		// Future getIds won't re-synchronised, assumes data is up-to-date.
		dirty = false;
	}

	return id;
}