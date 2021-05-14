#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <memory>

/// Model struct to edit the attributes of the HUD display. 
/// 
/// The model struct provides binding, position and scale setting,
/// drawing the model object and returning the model.
struct VertexArray;

struct Model
{
public:

	/// Binding the model object. 
	/// 
	/// Passes through the texture and vertex array object to bind the model. 
	/// @param textureId The texture 
	/// @param vaoId The vertex array object id. 
	/// @returns void
	/// @attention The order of these values are important!
	void Bind(GLuint textureId, GLuint vaoId);

	/// Sets the position of the model. 
	///
	/// Requires three integers that determines the position of the current model. 
	/// @param x The x-coordinate of the model. 
	/// @param y The y-coordinate of the model.
	/// @param z The z-coordinate of the model.
	/// @returns void
	/// @attention The order of these values are important!
	/// @see SetScale()

	void SetPosition(int x, int y, int z);

	/// Sets the scale of the model. 
	///
	/// Requires three integers that determines the position of the current model. 
	/// @param x The x-coordinate of the model. 
	/// @param y The y-coordinate of the model.
	/// @param z The z-coordinate of the model.
	/// @returns void
	/// @attention The order of these values are important!
	/// @see SetPosition()

	void SetScale(int x, int y, int z);

	/// Enables the drawing of the model.
	/// 
	/// Requires model location, projection location, projection matrix and a vertex array object. 
	/// @param modelLoc The model uniform location.
	/// @param projectionLoc The projection uniform location.
	/// @param projection The projection matrix.
	/// @param vao The vertex array object.
	/// @returns void 
	/// @attention This is the size_t vao version!
	/// @see Draw(GLint modelLoc, GLint projectionLoc, glm::mat4 projection, GLsizei vao)
	void Draw(GLint modelLoc, GLint projectionLoc, glm::mat4 projection, size_t vao);

	/// Enables the drawing of the model.
	/// 
	/// Requires model location, projection location, projection matrix and a vertex array object. 
	/// @param modelLoc The model uniform location.
	/// @param projectionLoc The projection uniform location. 
	/// @param projection The projection matrix. 
	/// @param vao The vertex array object. 
	/// @returns void 
	/// @attention This is the GLsizei vao version!
	/// @see Draw(GLint modelLoc, GLint projectionLoc, glm::mat4 projection, size_t vao)
	void Draw(GLint modelLoc, GLint projectionLoc, glm::mat4 projection, GLsizei vao);

	/// Returns the current model object. 
	/// 
	/// Gets the 4x4 matrix of the current model object and returns it. 
	/// @returns glm::mat4 
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