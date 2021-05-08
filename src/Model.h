#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <memory>

struct VertexArray;

struct Model
{
public:
	void Bind(GLuint textureId, GLuint vaoId);
	void SetPosition(int x, int y, int z);
	void SetScale(int x, int y, int z);
	void Draw(GLint modelLoc, GLint projectionLoc, glm::mat4 projection, size_t vao);
	void Draw(GLint modelLoc, GLint projectionLoc, glm::mat4 projection, GLsizei vao);
	glm::mat4 GetModel();
private:
	glm::mat4 model;
	int translateX = 0;
	int translateY = 0;
	int translateZ = 0;
	int scaleX = 0;
	int scaleY = 0;
	int scaleZ = 0;
};