#include "Model.h"

void Model::Bind(GLuint textureId, GLuint vaoId)
{
	model = glm::mat4(1.0f);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glBindVertexArray(vaoId);
}

void Model::SetPosition(int x, int y, int z)
{
	translateX = x;
	translateY = y;
	translateZ = z;

	model = glm::translate(model, glm::vec3(translateX, translateY, translateZ));
}

void Model::SetScale(int x, int y, int z)
{
	scaleX = x;
	scaleY = y;
	scaleZ = z;

	model = glm::scale(model, glm::vec3(scaleX, scaleY, scaleZ));
}

void Model::Draw(GLint modelLoc, GLint projectionLoc, glm::mat4 projection, size_t vao)
{
	// Upload the model matrix
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	// Upload the projection matrix
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE,
		glm::value_ptr(projection));

	glDrawArrays(GL_TRIANGLES, 0, vao);
}

glm::mat4 Model::GetModel()
{
	return model;
}


