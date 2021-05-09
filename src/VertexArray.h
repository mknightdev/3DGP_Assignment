#include <GL/glew.h>

#include <vector>
#include <memory>
#include <string>

struct VertexBuffer; // Forward declare 

/// VertexArray struct to set the vertex array object buffer to vertex buffer objects. 
///
/// This struct takes in a location integer and vertex buffer object to create a means to 
/// display objects to the screen by getting their vertex counts individually. 
struct VertexArray
{
public:
	/// Default constructor. 
	/// 
	/// A default constructor that sets the vert count to 6, enough to display a quad. 
	/// @attention This is the default version! 
	/// @see VertexArray(std::string path)
	VertexArray();

	/// Overloaded constructor. 
	/// 
	/// An overloaded constructor that sets the vertex count using a custom model loader.  
	/// @attention This is the overloaded version! 
	/// @see VertexArray()
	VertexArray(std::string path);

	/// Sets the buffer. 
	/// 
	/// A function to set the buffer of the vertex array object with the integer location and 
	/// vertex buffer object. 
	/// @param location
	/// @param buffer
	/// @returns void
	void setBuffer(int location, std::shared_ptr<VertexBuffer> buffer);

	/// Returns the id of the Vertex Array. 
	/// 
	/// @returns GLuint
	GLuint getId();

	/// Gets the vertex count of the model. 
	/// 
	/// Returns the vertex count and is used to display the model correctly. 
	/// @returns size_t
	size_t getVertCount();
private:
	GLuint id;
	std::vector<std::shared_ptr<VertexBuffer>> buffers;
	bool dirty;
	size_t vertCount;
};
