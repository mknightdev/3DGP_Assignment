#define SDL_MAIN_HANDLED

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Model.h"

// Needs to be defined before the include in exactly
// one compilation unit.
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream> 
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <memory>
#include <fstream>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

bool intersect(glm::vec2 mouse, glm::vec4 rectangle)
{
	if (mouse.x < rectangle.x)
	{
		return false;
	}

	if (mouse.x > rectangle.x + rectangle.z)
	{
		return false;
	}

	if (mouse.y < rectangle.y)
	{
		return false;
	}

	// W is for vec4 height
	if (mouse.y > rectangle.y + rectangle.w)
	{
		return false;
	}
	return true;
}

int main()
{
	//*****************************************************
	//	CREATE APPLICATION WINDOW
	//*****************************************************
	SDL_Window* window = SDL_CreateWindow("3D Model Viewer",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	if (!SDL_GL_CreateContext(window))
	{
		throw std::exception();
	}

	if (glewInit() != GLEW_OK)
	{
		throw std::exception();
	}

	//*****************************************************
	//	CREATE BUFFER ARRAY
	//*****************************************************

	// Created positionsVBO
	std::shared_ptr<VertexBuffer> positionsVbo = std::make_shared<VertexBuffer>();

	// Added to data vector (floats)
	// Tri 1
	positionsVbo->add(glm::vec3(-0.5f, -0.5f, 0.0f));	// Bottom Left
	positionsVbo->add(glm::vec3(0.5f, -0.5f, 0.0f));	// Bottom Right
	positionsVbo->add(glm::vec3(-0.5f, 0.5f, 0.0f));	// Top Right

	// Tri 2
	positionsVbo->add(glm::vec3(0.5f, 0.5f, 0.0f));		// Top Right
	positionsVbo->add(glm::vec3(0.5f, -0.5f, 0.0f));	// Bottom Right
	positionsVbo->add(glm::vec3(-0.5f, 0.5f, 0.0f));	// Top Left

	//*****************************************************
	// CREATE TEXTURES ARRAY
	//*****************************************************

	std::shared_ptr<VertexBuffer> texturesVbo = std::make_shared<VertexBuffer>();

	texturesVbo->add(glm::vec2(0.0f, 0.0f));
	texturesVbo->add(glm::vec2(1.0f, 0.0f));
	texturesVbo->add(glm::vec2(0.0f, -1.0f));
	texturesVbo->add(glm::vec2(1.0f, -1.0f));
	texturesVbo->add(glm::vec2(1.0f, 0.0f));
	texturesVbo->add(glm::vec2(0.0f, -1.0f));

	//*****************************************************
	//	CREATE VERTEX ARRAY
	//*****************************************************
	std::shared_ptr<VertexArray> vao = std::make_shared<VertexArray>();
	vao->setBuffer(0, positionsVbo);
	vao->setBuffer(1, texturesVbo);

	//*****************************************************
	//	OBTAIN OBJECTS
	//*****************************************************

	int w = 0;
	int h = 0;

	// Model destination
	std::string modelObject;
	std::string modelTextures;

	std::vector<std::shared_ptr<VertexArray>> models;
	std::vector<std::shared_ptr<Texture>> textures;

	// Model settings
	std::ifstream readFile("Model_Settings.txt");

	while (readFile >> modelObject >> modelTextures)
	{
		std::cout << modelObject << std::endl;
		std::cout << modelTextures << std::endl;
		std::shared_ptr<VertexArray> object = std::make_shared<VertexArray>(modelObject);
		std::shared_ptr<Texture> texture = std::make_shared<Texture>(modelTextures, w, h);
		models.push_back(object);
		textures.push_back(texture);
	}

	// Close the file, as it's no longer needed. 
	readFile.close();

	//*****************************************************
	// UI ICONS  
	//*****************************************************

	// Left Arrows
	std::shared_ptr<Texture> leftArrowTexture = std::make_shared<Texture>(stbi_load("models/icons/leftarrow.png", &w, &h, NULL, 4), w, h);
	std::shared_ptr<Texture> leftArrowActiveTexture = std::make_shared<Texture>(stbi_load("models/icons/leftarrowactive.png", &w, &h, NULL, 4), w, h);

	// Right Arrows
	std::shared_ptr<Texture> rightArrowTexture = std::make_shared<Texture>(stbi_load("models/icons/rightarrow.png", &w, &h, NULL, 4), w, h);
	std::shared_ptr<Texture> rightArrowActiveTexture = std::make_shared<Texture>(stbi_load("models/icons/rightarroveactive.png", &w, &h, NULL, 4), w, h);

	// Model Icon
	std::shared_ptr<Texture> modelIconTexture = std::make_shared<Texture>(stbi_load("models/icons/modelicon.png", &w, &h, NULL, 4), w, h);

	// Shader Icon
	std::shared_ptr<Texture> shaderIconTexture = std::make_shared<Texture>(stbi_load("models/icons/shadericon.png", &w, &h, NULL, 4), w, h);
	
	// Rotate Icons
	std::shared_ptr<Texture> rotateIconTexture = std::make_shared<Texture>(stbi_load("models/icons/rotate.png", &w, &h, NULL, 4), w, h);
	std::shared_ptr<Texture> rotateActiveIconTexture = std::make_shared<Texture>(stbi_load("models/icons/rotateactive.png", &w, &h, NULL, 4), w, h);

	// Scale Icons
	std::shared_ptr<Texture> addScaleTexture = std::make_shared<Texture>(stbi_load("models/icons/add.png", &w, &h, NULL, 4), w, h);
	std::shared_ptr<Texture> minusScaleTexture = std::make_shared<Texture>(stbi_load("models/icons/minus.png", &w, &h, NULL, 4), w, h);

	// Undo Icon
	std::shared_ptr<Texture> undoTexture = std::make_shared<Texture>(stbi_load("models/icons/undo.png", &w, &h, NULL, 4), w, h);

	//*****************************************************
	//	[VERT SHADER] SPECULAR LIGHTING
	//*****************************************************
	const GLchar* vertShaderSpecular =
		"attribute vec3 a_Position;													" \
		"attribute vec2 a_TexCoord;													" \
		"attribute vec3 a_Normal;													" \
		"uniform mat4 u_Projection;													" \
		"uniform mat4 u_Model;														" \
		"uniform mat4 u_View;														" \
		"																			" \
		"																			" \
		"varying vec2 v_TexCoord;													" \
		"varying vec3 v_FragPos;													" \
		"varying vec3 v_Normal;														" \
		"																			" \
		"void main()																" \
		"{																			" \
		" v_FragPos = vec3(u_Model * vec4(a_Position, 1.0));						" \
		" gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);	" \
		" v_TexCoord = a_TexCoord;													" \
		" v_Normal = vec3(u_Model * vec4(a_Normal, 0));								" \
		"}																			" \
		"																			";

	//*****************************************************
	//	[FRAG SHADER] SPECULAR LIGHTING
	//*****************************************************
	const GLchar* fragShaderSpecular =
		"#version 410\n													" \
		"uniform sampler2D u_Texture;									" \
		"varying vec2 v_TexCoord;										" \
		"varying vec3 v_FragPos;										" \
		"varying vec3 v_Normal;											" \
		"uniform mat4 u_View;											" \
		"uniform mat4 u_InverseView;									" \
		"																" \
		"void main()													" \
		"{																" \
		" vec4 tex = texture2D(u_Texture, v_TexCoord);					" \
		"																" \
		" vec3 lightPos = vec3(10, 10, 0);								" \
		" vec3 norm = normalize(v_Normal);								" \
		" vec3 lightDir = normalize(lightPos - v_FragPos);				" \
		"																" \
		" float diff = max(dot(norm, lightDir), 0.0);					" \
		" vec3 diffuse = vec3(0, 1, 1) * diff;							" \
		"																" \
		" vec3 viewPos = vec3(u_InverseView * vec4(0, 0, 0, 1));		" \
		" vec3 viewDir = normalize(viewPos - v_FragPos);				" \
		" vec3 reflectDir = reflect(-lightDir, norm);					" \
		" float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);	" \
		" vec3 specular = spec * vec3(1, 1, 1); 						" \
		" 																" \
		"																" \
		" gl_FragColor = vec4(diffuse + specular, 1.0) * tex;			" \
		"}																";

	std::shared_ptr<ShaderProgram> shaderSpecular = std::make_shared<ShaderProgram>();
	shaderSpecular->CreateShader(vertShaderSpecular, fragShaderSpecular);

	//*****************************************************
	//	[VERT SHADER] DIFFUSE LIGHTING
	//*****************************************************
	const GLchar* vertShaderDiffuse =
		"attribute vec3 a_Position;													" \
		"attribute vec2 a_TexCoord;													" \
		"attribute vec3 a_Normal;													" \
		"uniform mat4 u_Projection;													" \
		"uniform mat4 u_Model;														" \
		"uniform mat4 u_View;														" \
		"																			" \
		"																			" \
		"varying vec2 v_TexCoord;													" \
		"varying vec3 v_FragPos;													" \
		"varying vec3 v_Normal;														" \
		"																			" \
		"void main()																" \
		"{																			" \
		" v_FragPos = vec3(u_Model * vec4(a_Position, 1.0));						" \
		" gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);	" \
		" v_TexCoord = a_TexCoord;													" \
		" v_Normal = vec3(u_Model * vec4(a_Normal, 0));								" \
		"}																			" \
		"																			";

	//*****************************************************
	//	[FRAG SHADER] DIFFUSE LIGHTING
	//*****************************************************
	const GLchar* fragShaderDiffuse =
		"#version 410\n										" \
		"uniform sampler2D u_Texture;						" \
		"varying vec2 v_TexCoord;							" \
		"varying vec3 v_FragPos;							" \
		"varying vec3 v_Normal;								" \
		"uniform mat4 u_View;								" \
		"													" \
		"void main()										" \
		"{													" \
		" vec4 tex = texture2D(u_Texture, v_TexCoord);		" \
		"													" \
		" vec3 lightPos = vec3(10, 10, 0);					" \
		" vec3 norm = normalize(v_Normal);					" \
		" vec3 lightDir = normalize(lightPos - v_FragPos);	" \
		"													" \
		" float diff = max(dot(norm, lightDir), 0.0);		" \
		" vec3 diffuse = vec3(1, 1, 0) * diff;				" \
		"													" \
		" 													" \
		"													" \
		" gl_FragColor = vec4(diffuse, 1.0) * tex;			" \
		"}													";

	std::shared_ptr<ShaderProgram> shaderDiffuse = std::make_shared<ShaderProgram>();
	shaderDiffuse->CreateShader(vertShaderDiffuse, fragShaderDiffuse);

	//*****************************************************
	//	[VERT SHADER] AMBIENT LIGHTING
	//*****************************************************
	const GLchar* vertShaderAmbient =
		"attribute vec3 a_Position;													" \
		"attribute vec2 a_TexCoord;													" \
		"attribute vec3 a_Normal;													" \
		"uniform mat4 u_Projection;													" \
		"uniform mat4 u_Model;														" \
		"uniform mat4 u_View;														" \
		"																			" \
		"																			" \
		"varying vec2 v_TexCoord;													" \
		"varying vec3 v_FragPos;													" \
		"varying vec3 v_Normal;														" \
		"																			" \
		"void main()																" \
		"{																			" \
		" v_FragPos = vec3(u_Model * vec4(a_Position, 1.0));						" \
		" gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);	" \
		" v_TexCoord = a_TexCoord;													" \
		" v_Normal = vec3(u_Model * vec4(a_Normal, 0));								" \
		"}																			" \
		"																			";

	//*****************************************************
	//	[FRAG SHADER] AMBIENT LIGHTING
	//*****************************************************
	const GLchar* fragShaderAmbient =
		"uniform sampler2D u_Texture;							" \
		"varying vec2 v_TexCoord;								" \
		"varying vec3 v_FragPos;								" \
		"varying vec3 v_Normal;									" \
		"uniform mat4 u_View;									" \
		"														" \
		"void main()											" \
		"{														" \
		" vec4 tex = texture2D(u_Texture, v_TexCoord);			" \
		"														" \
		" vec3 lightPos = vec3(10, 10, 0);						" \
		" vec3 norm = normalize(v_Normal);						" \
		" vec3 lightDir = normalize(lightPos - v_FragPos);		" \
		"														" \
		" float diff = max(dot(norm, lightDir), 0.0);			" \
		" vec3 diffuse = vec3(1, 0, 0) * diff;					" \
		" float ambient = 0.20f;								" \
		" 														" \
		"														" \
		" gl_FragColor = vec4((diffuse + ambient), 1.0) * tex;	" \
		"}														";

	std::shared_ptr<ShaderProgram> shaderAmbient = std::make_shared<ShaderProgram>();
	shaderAmbient->CreateShader(vertShaderAmbient, fragShaderAmbient);

	//*****************************************************
	//	[VERT SHADER] TEXTURE ONLY
	//*****************************************************
	const GLchar* vertShaderNoLight =
		"attribute vec3 a_Position;													" \
		"attribute vec2 a_TexCoord;													" \
		"uniform mat4 u_Projection;													" \
		"uniform mat4 u_Model;														" \
		"uniform mat4 u_View;														" \
		"																			" \
		"																			" \
		"varying vec2 v_TexCoord;													" \
		"																			" \
		"void main()																" \
		"{																			" \
		" gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);	" \
		" v_TexCoord = a_TexCoord;													" \
		"}																			" \
		"																			";

	//*****************************************************
	//	[FRAG SHADER] TEXTURE ONLY
	//*****************************************************
	const GLchar* fragShaderNoLight =
		"uniform sampler2D u_Texture;					" \
		"varying vec2 v_TexCoord;						" \
		"												" \
		"void main()									" \
		"{												" \
		" vec4 tex = texture2D(u_Texture, v_TexCoord);	" \
		" gl_FragColor = tex;							" \
		"}												";

	std::shared_ptr<ShaderProgram> shaderNoLight = std::make_shared<ShaderProgram>();
	shaderNoLight->CreateShader(vertShaderNoLight, fragShaderNoLight);

	//*****************************************************
	//	CREATE UI VERTEX SHADER 
	//*****************************************************
	const GLchar* vertShaderUI =
		"attribute vec3 a_Position;											" \
		"attribute vec2 a_TexCoord;											" \
		"uniform mat4 u_Projection;											" \
		"uniform mat4 u_Model;												" \
		"																	" \
		"																	" \
		"varying vec2 v_TexCoord;											" \
		"																	" \
		"void main()														" \
		"{																	" \
		" gl_Position = u_Projection * u_Model * vec4(a_Position, 1.0);		" \
		" v_TexCoord = a_TexCoord;											" \
		"}																	" \
		"																	";

	//*****************************************************
	//	CREATE UI FRAGMENT SHADER 
	//*****************************************************
	const GLchar* fragShaderUI =
		"uniform sampler2D u_Texture;					" \
		"varying vec2 v_TexCoord;						" \
		"												" \
		"void main()									" \
		"{												" \
		" vec4 tex = texture2D(u_Texture, v_TexCoord);	" \
		" gl_FragColor = tex;							" \
		"}												";

	std::shared_ptr<ShaderProgram> shaderUI = std::make_shared<ShaderProgram>();
	shaderUI->CreateShader(vertShaderUI, fragShaderUI);

	//******************************
	//	Group Perspective Shaders
	//******************************
	std::vector<std::shared_ptr<ShaderProgram>> shaders;

	shaders.push_back(shaderNoLight);
	shaders.push_back(shaderDiffuse);
	shaders.push_back(shaderAmbient);
	shaders.push_back(shaderSpecular);

	//*****************************************************
	//	OBTAIN UNIFORM LOCATION 3
	//*****************************************************

	// Find uniform locations
	GLint modelLocNoLight = glGetUniformLocation(shaderNoLight->getId(), "u_Model");
	GLint projectionLocNoLight = glGetUniformLocation(shaderNoLight->getId(), "u_Projection");
	GLint viewLocNoLight = glGetUniformLocation(shaderNoLight->getId(), "u_View");

	if (modelLocNoLight == -1)
	{
		throw std::exception();
	}
	if (projectionLocNoLight == -1)
	{
		throw std::exception();
	}
	if (viewLocNoLight == -1)
	{
		throw std::exception();
	}

	//*****************************************************
	//	OBTAIN DIFFUSE UNIFORM LOCATION
	//*****************************************************

	// Find uniform locations
	GLint modelLocDiffuse = glGetUniformLocation(shaderDiffuse->getId(), "u_Model");
	GLint projectionLocDiffuse = glGetUniformLocation(shaderDiffuse->getId(), "u_Projection");
	GLint viewLocDiffuse = glGetUniformLocation(shaderDiffuse->getId(), "u_View");

	if (modelLocDiffuse == -1)
	{
		throw std::exception();
	}
	if (projectionLocDiffuse == -1)
	{
		throw std::exception();
	}
	if (viewLocDiffuse == -1)
	{
		throw std::exception();
	}

	//*****************************************************
	//	OBTAIN AMBIENT UNIFORM LOCATION
	//*****************************************************

	// Find uniform locations
	GLint modelLocAmbient = glGetUniformLocation(shaderAmbient->getId(), "u_Model");
	GLint projectionLocAmbient = glGetUniformLocation(shaderAmbient->getId(), "u_Projection");
	GLint viewLocAmbient = glGetUniformLocation(shaderAmbient->getId(), "u_View");

	if (modelLocAmbient == -1)
	{
		throw std::exception();
	}
	if (projectionLocAmbient == -1)
	{
		throw std::exception();
	}
	if (viewLocAmbient == -1)
	{
		throw std::exception();
	}

	//*****************************************************
	//	OBTAIN SPECULAR UNIFORM LOCATION
	//*****************************************************

	// Find uniform locations
	GLint modelLocSpecular = glGetUniformLocation(shaderSpecular->getId(), "u_Model");
	GLint projectionLocSpecular = glGetUniformLocation(shaderSpecular->getId(), "u_Projection");
	GLint viewLocSpecular = glGetUniformLocation(shaderSpecular->getId(), "u_View");
	GLint inverseViewLocSpecular = glGetUniformLocation(shaderSpecular->getId(), "u_InverseView");

	if (modelLocSpecular == -1)
	{
		throw std::exception();
	}
	if (projectionLocSpecular == -1)
	{
		throw std::exception();
	}
	if (viewLocSpecular == -1)
	{
		throw std::exception();
	}
	if (inverseViewLocSpecular == -1)
	{
		throw std::exception();
	}

	//*****************************************************
	//	OBTAIN UI UNIFORM LOCATION 
	//*****************************************************

	// Find uniform locations
	GLint modelLocUI = glGetUniformLocation(shaderUI->getId(), "u_Model");
	GLint projectionLocUI = glGetUniformLocation(shaderUI->getId(), "u_Projection");

	if (modelLocUI == -1)
	{
		throw std::exception();
	}
	if (projectionLocUI == -1)
	{
		throw std::exception();
	}


	// VECTORS
	std::vector<GLint> modelLocs;
	std::vector<GLint> projectionLocs;
	std::vector<GLint> viewLocs;
	std::vector<GLint> inverseViewLocs;

	modelLocs.push_back(modelLocNoLight);
	modelLocs.push_back(modelLocDiffuse);
	modelLocs.push_back(modelLocAmbient);
	modelLocs.push_back(modelLocSpecular);

	projectionLocs.push_back(projectionLocNoLight);
	projectionLocs.push_back(projectionLocDiffuse);
	projectionLocs.push_back(projectionLocAmbient);
	projectionLocs.push_back(projectionLocSpecular);

	viewLocs.push_back(viewLocNoLight);
	viewLocs.push_back(viewLocDiffuse);
	viewLocs.push_back(viewLocAmbient);
	viewLocs.push_back(viewLocSpecular);

	inverseViewLocs.push_back(-1);
	inverseViewLocs.push_back(-1);
	inverseViewLocs.push_back(-1);
	inverseViewLocs.push_back(inverseViewLocSpecular);

	//*****************************************************
	//	MAIN LOOP
	//*****************************************************
	bool stopped = false;

	float angle = 0;
	float xPos = 0;
	float yPos = 1;

	float rot = 0;
	float deltaTime = 0.0001f;
	float prevTime = 0;

	float pos = 0;
	float speed = 1.0f;

	int modelSelector = 0;
	int shaderSelector = 0;

	float scale = 1.0f;

	glm::vec3 camPos(0, 0, 15);

	glm::vec2 mouse(0, 0);
	bool mouseButtonDown = false;
	bool rotateOn = true;

	while (!stopped)
	{
		int width = 0;
		int height = 0;

		glEnable(GL_DEPTH_TEST);

		SDL_GetWindowSize(window, &width, &height);

		SDL_Event event = { 0 };

		//*****************************************************
		//	KEYBOARD EVENTS
		//*****************************************************
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				stopped = true;
			}
			else if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_a:
					std::cout << "A Key Pressed" << std::endl;

					if (modelSelector == 0)
					{
						/* if selector equals zero, set selector to be the value of the end of the model vector,
						* this prevents us from going out of bounds. */
						modelSelector = models.size() - 1;
						std::cout << "Model Selector: " << modelSelector << std::endl;
					}
					else
					{
						// Otherwise, continue decreasing selector to view previous models.
						modelSelector--;
						std::cout << "Model Selector: " << modelSelector << std::endl;
					}

					break;
				case SDLK_d:
					std::cout << "D Key Pressed" << std::endl;

					if (modelSelector == models.size() - 1)
					{
						/* If selector is the same as our vector of models (-1),
						* set selector back to zero so we don't go out of bounds.*/
						modelSelector = 0;
						std::cout << "Model Selector: " << modelSelector << std::endl;
					}
					else
					{
						// Otherwise, continue increasing selector to view further models. 
						modelSelector++;
						std::cout << "Model Selector: " << modelSelector << std::endl;
					}
					break;
				case SDLK_q:
					std::cout << "Q Key Pressed" << std::endl;

					if (shaderSelector == 0)
					{
						/* if selector equals zero, set selector to be the value of the end of the shader vector,
						* this prevents us from going out of bounds. */
						shaderSelector = shaders.size() - 1;
						std::cout << "Selector: " << shaderSelector << std::endl;
					}
					else
					{
						// Otherwise, continue decreasing selector to view previous shader.
						shaderSelector--;
						std::cout << "Selector: " << shaderSelector << std::endl;
					}

					break;
				case SDLK_e:
					std::cout << "E Key Pressed" << std::endl;

					if (shaderSelector == shaders.size() - 1)
					{
						/* If selector is the same as our vector of shader (-1),
						* set selector back to zero so we don't go out of bounds.*/
						shaderSelector = 0;
						std::cout << "Selector: " << shaderSelector << std::endl;
					}
					else
					{
						// Otherwise, continue increasing selector to view further shader. 
						shaderSelector++;
						std::cout << "Selector: " << shaderSelector << std::endl;
					}
					break;
				case SDLK_UP:
					// Move the model up
					std::cout << "UP Arrow" << std::endl;
					camPos.y -= 50.0f * deltaTime;
					break;
				case SDLK_DOWN:
					// Move the model down
					std::cout << "DOWN Arrow" << std::endl;
					camPos.y += 50.0f * deltaTime;
					break;
				case SDLK_LEFT:
					// Move the model left
					std::cout << "LEFT Arrow" << std::endl;
					camPos.x += 50.0f * deltaTime;
					break;
				case SDLK_RIGHT:
					// Move the model right
					std::cout << "RIGHT Arrow" << std::endl;
					camPos.x -= 50.0f * deltaTime;
					break;
				case SDLK_EQUALS:
					std::cout << "Scale Increased" << std::endl;
					scale += 0.05f;
					break;
				case SDLK_MINUS:
					std::cout << "Scale Decreased" << std::endl;
					if (scale <= 0.05f)
					{
						scale = 0.05f;
					}
					else
					{
						scale -= 0.05f;
					}
					std::cout << scale << std::endl;
					break;
				default:
					std::cout << "No valid input" << std::endl;
					break;
				}
			}
			else if (event.type == SDL_MOUSEMOTION)
			{
				mouse = glm::vec2(event.motion.x, event.motion.y);

			}
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				mouseButtonDown = true;
			}
			else if (event.type == SDL_MOUSEBUTTONUP)
			{
				mouseButtonDown = false;
			}
		}

		float currTime = SDL_GetTicks();
		float diffTime = currTime - prevTime;
		deltaTime = diffTime / 1000.0f;
		// Makes sure it has the latest prev time. Otherwise it will make things go faster and faster
		prevTime = currTime;

		// Shader Left Arrow
		if (intersect(mouse, glm::vec4(width - 300, 450, 100, 100)) && mouseButtonDown)
		{
			if (shaderSelector == 0)
			{
				/* if selector equals zero, set selector to be the value of the end of the shader vector,
				* this prevents us from going out of bounds. */
				shaderSelector = shaders.size() - 1;
				std::cout << "Selector: " << shaderSelector << std::endl;
			}
			else
			{
				// Otherwise, continue decreasing selector to view previous shader.
				shaderSelector--;
				std::cout << "Selector: " << shaderSelector << std::endl;
			}

			std::cout << "Left arrow intersect True" << std::endl;
			mouseButtonDown = false;	// Prevents holding down mouse button 
		}

		// Shader Right Arrow
		if (intersect(mouse, glm::vec4(width - 150, 450, 100, 100)) && mouseButtonDown)
		{
			glBindTexture(GL_TEXTURE_2D, rightArrowActiveTexture->GetId());
			if (shaderSelector == shaders.size() - 1)
			{
				/* If selector is the same as our vector of shader (-1),
				* set selector back to zero so we don't go out of bounds.*/
				shaderSelector = 0;
				std::cout << "Selector: " << shaderSelector << std::endl;
			}
			else
			{
				// Otherwise, continue increasing selector to view further shader. 
				shaderSelector++;
				std::cout << "Selector: " << shaderSelector << std::endl;
			}

			std::cout << "Right arrow intersect True" << std::endl;
			mouseButtonDown = false;	// Prevents holding down mouse button 
		}

		// Left Arrow
		if (intersect(mouse, glm::vec4(50, 450, 100, 100)) && mouseButtonDown)
		{

			if (modelSelector == 0)
			{
				/* if selector equals zero, set selector to be the value of the end of the model vector,
				* this prevents us from going out of bounds. */
				modelSelector = models.size() - 1;
				std::cout << "Model Selector: " << modelSelector << std::endl;
			}
			else
			{
				// Otherwise, continue decreasing selector to view previous models.
				modelSelector--;
				std::cout << "Model Selector: " << modelSelector << std::endl;
			}

			std::cout << "Left arrow intersect True" << std::endl;
			mouseButtonDown = false;	// Prevents holding down mouse button 
		}

		// Right Arrow
		if (intersect(mouse, glm::vec4(200, 450, 100, 100)) && mouseButtonDown)
		{
			if (modelSelector == models.size() - 1)
			{
				/* If selector is the same as our vector of models (-1),
				* set selector back to zero so we don't go out of bounds.*/
				modelSelector = 0;
				std::cout << "Model Selector: " << modelSelector << std::endl;
			}
			else
			{
				// Otherwise, continue increasing selector to view further models. 
				modelSelector++;
				std::cout << "Model Selector: " << modelSelector << std::endl;
			}

			std::cout << "Right arrow intersect True" << std::endl;
			mouseButtonDown = false;	// Prevents holding down mouse button 
		}

		// Rotate Icon 
		if (intersect(mouse, glm::vec4(45, 210, 50, 50)) && mouseButtonDown && !rotateOn)
		{
			rotateOn = true;
			std::cout << "Rotate intersect True" << std::endl;
			mouseButtonDown = false;
		}
		else if (intersect(mouse, glm::vec4(45, 210, 50, 50)) && mouseButtonDown && rotateOn)
		{
			rotateOn = false;
			std::cout << "Rotate intersect True" << std::endl;
			mouseButtonDown = false;
		}

		// Add scale icon
		if (intersect(mouse, glm::vec4(45, 155, 50, 50)) && mouseButtonDown)
		{
			scale += 0.1f;
			std::cout << "Add scale intersect True" << std::endl;
			mouseButtonDown = false;
		}

		// Minus scale icon
		if (intersect(mouse, glm::vec4(115, 155, 50, 50)) && mouseButtonDown)
		{
			if (scale <= 0.1f)
			{
				scale = 0.1f;
			}
			else
			{
				scale -= 0.1f;
			}
			std::cout << "Minus scale intersect True" << std::endl;
			mouseButtonDown = false;
		}

		// Undo icon
		if (intersect(mouse, glm::vec4(115, 210, 50, 50)) && mouseButtonDown)
		{
			scale = 1.0f;
			shaderSelector = 0;
			std::cout << "Undo intersect True" << std::endl;
			mouseButtonDown = false;
		}
		// Set background to grey 
		glClearColor(0.33f, 0.33f, 0.33f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Drawing
		// Instruct OpenGL to use our shader program and our VAO
		glUseProgram(shaders.at(shaderSelector)->getId());
		glBindTexture(GL_TEXTURE_2D, textures.at(modelSelector)->GetId());
		glBindVertexArray(models.at(modelSelector)->getId());

		//*****************************************************
		//	PERSPECTIVE PATH
		//*****************************************************

		// Prepare the perspective projection matrix
		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
			(float)width / (float)height, 0.1f, 100.0f);

		//	MODEL MATRIX  //
		// Prepare the model matrix
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0, 0, 0));
		model = glm::scale(model, glm::vec3(scale, scale, scale));

		// Upload the model matrix
		glUniformMatrix4fv(modelLocs.at(shaderSelector), 1, GL_FALSE, glm::value_ptr(model));


		//	VIEW MATRIX  //
		// If we do rotate then translate, it will look like it will orbit around the model
		glm::mat4 view(1.0f);
		if (rotateOn)
		{
			view = glm::rotate(view, glm::radians(rot), glm::vec3(0, 1, 0));
		}
		else
		{
			view = glm::rotate(view, glm::radians(0.0f), glm::vec3(0, 1, 0));
			rot = 0;
		}

		view = glm::translate(view, glm::vec3(0, 0, 15));
		//view = glm::translate(view, camPos);
		glUniformMatrix4fv(viewLocs.at(shaderSelector), 1, GL_FALSE, glm::value_ptr(glm::inverse(view)));

		if (inverseViewLocs.at(shaderSelector) != -1)
		{
			glUniformMatrix4fv(inverseViewLocs.at(shaderSelector), 1, GL_FALSE, glm::value_ptr(view));
		}

		// Increase the float angle so next frame the triangle rotates further
		// How fast the object rotates
		rot += 45.0f * deltaTime;

		// Make sure the current program is bound
		// Upload the projection matrix
		glUniformMatrix4fv(projectionLocs.at(shaderSelector), 1, GL_FALSE, glm::value_ptr(projection));

		// Draw 3 vertices (a triangle)
		glDrawArrays(GL_TRIANGLES, 0, models.at(modelSelector)->getVertCount());

		glDisable(GL_DEPTH_TEST);

		//*****************************************************
		//	ORTHOGRAPHIC PATH
		//*****************************************************

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindVertexArray(vao->getId());
		glUseProgram(shaderUI->getId());

		// Prepare the orthographic projection matrix (reusing the variable)
		projection = glm::ortho(0.0f, (float)width, 0.0f,
			(float)height, 0.0f, 1.0f);

		// Prepare model matrix. The scale is important because now our triangle
		// would be the size of a single pixel when mapped to an orthographic
		// projection.
		glm::mat4 UIModel(1.0f);
		UIModel = glm::translate(UIModel, glm::vec3(100, height - 100, 0));	// Position of the orthographic view
		UIModel = glm::scale(UIModel, glm::vec3(100, 100, 1));

		// Upload the model matrix
		glUniformMatrix4fv(modelLocUI, 1, GL_FALSE, glm::value_ptr(UIModel));

		// Upload the projection matrix
		glUniformMatrix4fv(projectionLocUI, 1, GL_FALSE,
			glm::value_ptr(projection));

		// Draw shape as before
		glDrawArrays(GL_TRIANGLES, 0, vao->getVertCount());

		//*****************************************************
		// [HUD] Left Arrow (Model)
		//*****************************************************

		std::shared_ptr<Model> leftArrow = std::make_shared<Model>();
		leftArrow->Bind(leftArrowTexture->GetId(), vao->getId());
		leftArrow->SetPosition(100, height - 500, 0);
		leftArrow->SetScale(100, 100, 1);
		leftArrow->Draw(modelLocUI, projectionLocUI, projection, vao->getVertCount());

		//*****************************************************
		// [HUD] Right Arrow (Model)
		//*****************************************************

		std::shared_ptr<Model> rightArrow = std::make_shared<Model>();
		rightArrow->Bind(rightArrowTexture->GetId(), vao->getId());
		rightArrow->SetPosition(250, height - 500, 0);
		rightArrow->SetScale(100, 100, 1);
		rightArrow->Draw(modelLocUI, projectionLocUI, projection, vao->getVertCount());

		//*****************************************************
		// [HUD] Model Icon
		//*****************************************************

		std::shared_ptr<Model> modelIcon = std::make_shared<Model>();
		modelIcon->Bind(modelIconTexture->GetId(), vao->getId());
		modelIcon->SetPosition(175, height - 500, 0);
		modelIcon->SetScale(40, 40, 1);
		modelIcon->Draw(modelLocUI, projectionLocUI, projection, vao->getVertCount());

		//*****************************************************
		// [HUD] Left Arrow (Shader)
		//*****************************************************

		leftArrow->Bind(leftArrowTexture->GetId(), vao->getId());
		leftArrow->SetPosition(width - 250, height - 500, 0);
		leftArrow->SetScale(100, 100, 1);
		leftArrow->Draw(modelLocUI, projectionLocUI, projection, vao->getVertCount());

		//*****************************************************
		// [HUD] Right Arrow (Shader)
		//*****************************************************

		rightArrow->Bind(rightArrowTexture->GetId(), vao->getId());
		rightArrow->SetPosition(width - 100, height - 500, 0);
		rightArrow->SetScale(100, 100, 1);
		rightArrow->Draw(modelLocUI, projectionLocUI, projection, vao->getVertCount());

		//*****************************************************
		// [HUD] Shader Icon
		//*****************************************************

		std::shared_ptr<Model> shaderIcon = std::make_shared<Model>();
		shaderIcon->Bind(shaderIconTexture->GetId(), vao->getId());
		shaderIcon->SetPosition(width - 175, height - 500, 0);
		shaderIcon->SetScale(40, 40, 1);
		shaderIcon->Draw(modelLocUI, projectionLocUI, projection, vao->getVertCount());
			
		//*****************************************************
		// [HUD] Rotate Icon
		//*****************************************************

		std::shared_ptr<Model> rotateIcon = std::make_shared<Model>();

		if (rotateOn)
		{
			rotateIcon->Bind(rotateActiveIconTexture->GetId(), vao->getId());
		}
		else if (!rotateOn)
		{
			rotateIcon->Bind(rotateIconTexture->GetId(), vao->getId());
		}

		rotateIcon->SetPosition(70, height - 235, 0);
		rotateIcon->SetScale(50, 50, 1);
		rotateIcon->Draw(modelLocUI, projectionLocUI, projection, vao->getVertCount());

		//*****************************************************
		// [HUD] Add Scale Icon
		//*****************************************************

		std::shared_ptr<Model> addScaleIcon = std::make_shared<Model>();
		addScaleIcon->Bind(addScaleTexture->GetId(), vao->getId());
		addScaleIcon->SetPosition(70, height - 180, 0);
		addScaleIcon->SetScale(50, 50, 1);
		addScaleIcon->Draw(modelLocUI, projectionLocUI, projection, vao->getVertCount());

		//*****************************************************
		// [HUD] Minus Scale Icon
		//*****************************************************

		std::shared_ptr<Model> minusScaleIcon = std::make_shared<Model>();
		minusScaleIcon->Bind(minusScaleTexture->GetId(), vao->getId());
		minusScaleIcon->SetPosition(130, height - 180, 0);
		minusScaleIcon->SetScale(50, 50, 1);
		minusScaleIcon->Draw(modelLocUI, projectionLocUI, projection, vao->getVertCount());

		//*****************************************************
		// [HUD] Undo Icon
		//*****************************************************
		std::shared_ptr<Model> undoIcon = std::make_shared<Model>();
		undoIcon->Bind(undoTexture->GetId(), vao->getId());
		undoIcon->SetPosition(130, height - 235, 0);
		undoIcon->SetScale(50, 50, 1);
		undoIcon->Draw(modelLocUI, projectionLocUI, projection, vao->getVertCount());

		//*****************************************************
		//	RESET THE STATE
		//*****************************************************
		glBindVertexArray(0);
		glUseProgram(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glViewport(0, 0, width, height);
		SDL_GL_SwapWindow(window);
	}
	return 0;
}