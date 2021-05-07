#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>


struct Model
{
public:
	void Bind(GLuint textureId, GLuint vaoId);
	void SetPosition(int tX, int tY, int sX, int sY);
	void Draw(GLint modelLoc, GLint projectionLoc, glm::mat4 projection, size_t vao);
	glm::mat4 GetModel();
private:
	glm::mat4 model;
	int translateX = 0;
	int translateY = 0;
	int scaleX = 0;
	int scaleY = 0;
};