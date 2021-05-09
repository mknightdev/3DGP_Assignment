#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
/// A struct to create vertex buffer objects. 
/// 
/// Stores coordinate positions that get used within the VertexArray struct. 
struct VertexBuffer
{
public:
	VertexBuffer(); // Constructor
	~VertexBuffer(); // Deconstructor

	/// Adds a vec2 to the Vertex Buffer. 
	/// 
	/// Used for position, texture or colour coordinates. 
	/// @param value Needs an X and Y component. 
	/// @see add(glm::vec3(value)
	/// @see add(glm::vec4(value)
	void add(glm::vec2 value);
	
	/// Adds a vec3 to the Vertex Buffer. 
	/// 
	/// Used for position, texture or colour coordinates. 
	/// @param value Needs an X, Y and Z component. 
	/// @see add(glm::vec2(value)
	/// @see add(glm::vec4(value)
	void add(glm::vec3 value);
	
	/// Adds a vec4 to the Vertex Buffer. 
	/// 
	/// Used for position, texture or colour coordinates. 
	/// @param value Needs an X, Y, Z and W component. 
	/// @see add(glm::vec2(value)
	/// @see add(glm::vec3(value)
	void add(glm::vec4 value);

	/// Returns the id of the Vertex Buffer. 
	///
	/// @returns GLuint
	GLuint getId();

	/// Returns the components of the Vertex Buffer. 
	/// 
	/// Gets used within the VertexArray.h getId function. 
	/// @returns int
	int getComponents();
private:
	GLuint id;
	std::vector<float> data;
	bool dirty; // Trigger lazy uploading
	int components;
};