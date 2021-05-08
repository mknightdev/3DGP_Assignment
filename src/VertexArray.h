#include <GL/glew.h>

#include <vector>
#include <memory>
#include <string>

// Forward declare 
struct VertexBuffer;

struct VertexArray
{
public:
	VertexArray();
	VertexArray(std::string path);
	void setBuffer(int location, std::shared_ptr<VertexBuffer> buffer);
	GLuint getId();
	size_t getVertCount();
private:
	GLuint id;
	std::vector<std::shared_ptr<VertexBuffer>> buffers;
	bool dirty;
	size_t vertCount;
};
