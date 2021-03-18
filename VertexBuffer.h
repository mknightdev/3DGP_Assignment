#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

struct VertexBuffer
{
public:
	VertexBuffer(); // Constructor
	~VertexBuffer(); // Deconstructor
	void add(glm::vec2 value);
	void add(glm::vec3 value);
	void add(glm::vec4 value);
	GLuint getId();
	int getComponents();
private:
	GLuint id;
	std::vector<float> data;
	bool dirty; // Trigger lazy uploading
	int components;
};