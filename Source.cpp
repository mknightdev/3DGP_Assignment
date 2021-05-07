#define SDL_MAIN_HANDLED

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "ShaderProgram.h"
#include "Texture.h"

/* Lab 4 - Start*/
// Needs to be defined before the include in exactly
// one compilation unit.
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
/* Lab 4 - End */

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

	// Same thing as: 
	// VertexBuffer* positionsVbo = new VertexBuffer();
	//
	// Automatically deletes 
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
	//	[UI] Arrows - VBO & VAO
	//*****************************************************

	// LEFT ARROW
	std::shared_ptr<VertexBuffer> leftArrowPosVBO = std::make_shared<VertexBuffer>();

	// Added to data vector (floats)
	// Tri 1
	leftArrowPosVBO->add(glm::vec3(-0.5f, -0.5f, 0.0f));	// Bottom Left
	leftArrowPosVBO->add(glm::vec3(0.5f, -0.5f, 0.0f));	// Bottom Right
	leftArrowPosVBO->add(glm::vec3(-0.5f, 0.5f, 0.0f));	// Top Right

	// Tri 2
	leftArrowPosVBO->add(glm::vec3(0.5f, 0.5f, 0.0f));		// Top Right
	leftArrowPosVBO->add(glm::vec3(0.5f, -0.5f, 0.0f));	// Bottom Right
	leftArrowPosVBO->add(glm::vec3(-0.5f, 0.5f, 0.0f));	// Top Left

	std::shared_ptr<VertexBuffer> leftArrowTexturesVBO = std::make_shared<VertexBuffer>();

	leftArrowTexturesVBO->add(glm::vec2(0.0f, 0.0f));
	leftArrowTexturesVBO->add(glm::vec2(1.0f, 0.0f));
	leftArrowTexturesVBO->add(glm::vec2(0.0f, -1.0f));
	leftArrowTexturesVBO->add(glm::vec2(1.0f, -1.0f));
	leftArrowTexturesVBO->add(glm::vec2(1.0f, 0.0f));
	leftArrowTexturesVBO->add(glm::vec2(0.0f, -1.0f));

	std::shared_ptr<VertexArray> leftArrowVAO = std::make_shared<VertexArray>();
	leftArrowVAO->setBuffer(0, leftArrowPosVBO);
	leftArrowVAO->setBuffer(1, leftArrowTexturesVBO);

	// RIGHT ARROW
	std::shared_ptr<VertexBuffer> rightArrowPosVBO = std::make_shared<VertexBuffer>();

	// Added to data vector (floats)
	// Tri 1
	rightArrowPosVBO->add(glm::vec3(-0.5f, -0.5f, 0.0f));	// Bottom Left
	rightArrowPosVBO->add(glm::vec3(0.5f, -0.5f, 0.0f));	// Bottom Right
	rightArrowPosVBO->add(glm::vec3(-0.5f, 0.5f, 0.0f));	// Top Right

	// Tri 2
	rightArrowPosVBO->add(glm::vec3(0.5f, 0.5f, 0.0f));		// Top Right
	rightArrowPosVBO->add(glm::vec3(0.5f, -0.5f, 0.0f));	// Bottom Right
	rightArrowPosVBO->add(glm::vec3(-0.5f, 0.5f, 0.0f));	// Top Left

	std::shared_ptr<VertexBuffer> rightArrowTexturesVBO = std::make_shared<VertexBuffer>();

	rightArrowTexturesVBO->add(glm::vec2(0.0f, 0.0f));
	rightArrowTexturesVBO->add(glm::vec2(1.0f, 0.0f));
	rightArrowTexturesVBO->add(glm::vec2(0.0f, -1.0f));
	rightArrowTexturesVBO->add(glm::vec2(1.0f, -1.0f));
	rightArrowTexturesVBO->add(glm::vec2(1.0f, 0.0f));
	rightArrowTexturesVBO->add(glm::vec2(0.0f, -1.0f));

	std::shared_ptr<VertexArray> rightArrowVAO = std::make_shared<VertexArray>();
	rightArrowVAO->setBuffer(0, rightArrowPosVBO);
	rightArrowVAO->setBuffer(1, rightArrowTexturesVBO);

	//*****************************************************
	//	[UI] ICONS - VBO & VAO
	//*****************************************************

	// MODEL ICON
	std::shared_ptr<VertexBuffer> modelIconPosVBO = std::make_shared<VertexBuffer>();

	// Added to data vector (floats)
	// Tri 1
	modelIconPosVBO->add(glm::vec3(-0.5f, -0.5f, 0.0f));	// Bottom Left
	modelIconPosVBO->add(glm::vec3(0.5f, -0.5f, 0.0f));	// Bottom Right
	modelIconPosVBO->add(glm::vec3(-0.5f, 0.5f, 0.0f));	// Top Right

	// Tri 2
	modelIconPosVBO->add(glm::vec3(0.5f, 0.5f, 0.0f));		// Top Right
	modelIconPosVBO->add(glm::vec3(0.5f, -0.5f, 0.0f));	// Bottom Right
	modelIconPosVBO->add(glm::vec3(-0.5f, 0.5f, 0.0f));	// Top Left

	std::shared_ptr<VertexBuffer> modelIconTexturesVBO = std::make_shared<VertexBuffer>();

	modelIconTexturesVBO->add(glm::vec2(0.0f, 0.0f));
	modelIconTexturesVBO->add(glm::vec2(1.0f, 0.0f));
	modelIconTexturesVBO->add(glm::vec2(0.0f, -1.0f));
	modelIconTexturesVBO->add(glm::vec2(1.0f, -1.0f));
	modelIconTexturesVBO->add(glm::vec2(1.0f, 0.0f));
	modelIconTexturesVBO->add(glm::vec2(0.0f, -1.0f));

	std::shared_ptr<VertexArray> modelIconVAO = std::make_shared<VertexArray>();
	modelIconVAO->setBuffer(0, modelIconPosVBO);
	modelIconVAO->setBuffer(1, modelIconTexturesVBO);

	//*****************************************************
	//	READ IN FROM FILE
	//*****************************************************

	// Model destination
	std::string modelSettings;

	// Model settings
	std::ifstream readFile("Model_Settings.txt");

	while (readFile >> modelSettings)
	{
		std::cout << modelSettings << std::endl;
	}

	// Close the file, as it's no longer needed. 
	readFile.close();

	// path is the file location
	//std::shared_ptr<VertexArray> cat = std::make_shared<VertexArray>(modelSettings);
	std::shared_ptr<VertexArray> cat = std::make_shared<VertexArray>("models/curuthers/curuthers.obj");
	std::shared_ptr<VertexArray> skeleton = std::make_shared<VertexArray>("models/skeleton/skeleton.obj");
	std::shared_ptr<VertexArray> croc = std::make_shared<VertexArray>("models/croc/croc.obj");

	std::vector<std::shared_ptr<VertexArray>> models;

	models.push_back(cat);
	models.push_back(skeleton);
	models.push_back(croc);

	//*****************************************************
	//	[VERT SHADER] SPECULAR LIGHTING
	//*****************************************************
	const GLchar* vertexShaderSrc =
		"attribute vec3 a_Position;														" \
		"attribute vec2 a_TexCoord;														" \
		"attribute vec3 a_Normal;														" \
		"uniform mat4 u_Projection;														" \
		"uniform mat4 u_Model;															" \
		"uniform mat4 u_View;															" \
		"																				" \
		"																				" \
		"varying vec2 v_TexCoord;														" \
		"varying vec3 v_FragPos;														" \
		"varying vec3 v_Normal;															" \
		"																				" \
		"void main()																	" \
		"{																				" \
		" v_FragPos = vec3(u_Model * vec4(a_Position, 1.0));							" \
		" gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);		" \
		" v_TexCoord = a_TexCoord;														" \
		" v_Normal = vec3(u_Model * vec4(a_Normal, 0));									" \
		"}																				" \
		"																				";

	//*****************************************************
	//	[FRAG SHADER] SPECULAR LIGHTING
	//*****************************************************
	const GLchar* fragmentShaderSrc =
		"#version 410\n													" \
		"uniform sampler2D u_Texture;									" \
		"varying vec2 v_TexCoord;										" \
		"varying vec3 v_FragPos;										" \
		"varying vec3 v_Normal;											" \
		"uniform float u_Pulse;											" \
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
		" gl_FragColor = vec4(diffuse + specular, 1.0) * tex;					" \
		"}																";

	//*****************************************************
	//	FRAGMENT SHADER DEBUGGING & Notes
	//	- " vec4 tex = texture2D(u_Texture, v_TexCoord); " \
	//	- " vec4 tex = vec4(v_TexCoord, 0, 1);	" \
	//  Note:
	//	For diffuse, set "gl_FragColor = vec4(diffuse, 1);"
	//	For ambient, set "gl_FragColor = vec4(light, 1) * tex;"
	//	For specular, set "gl_FragColor = vec4(diffuse + specular, 1.0);"
	//	For shininess, 32 is good. 8 makes it look metallic.
	//
	//	Diffuse:
	//	" vec3 lightPos = vec3(10, 10, 0);					" \
	//	" vec3 norm = normalize(v_Normal);					" \
	//	" vec3 lightDir = normalize(lightPos - v_FragPos);	" \
	//	"													" \
	//	" float diff = max(dot(norm, lightDir), 0.0);		" \
	//	" vec3 diffuse = vec3(1, 1, 1) * diff;				" \
	//	
	//	Ambient:
	//	" float ambientStrength = 0.1;						" \
	//	" vec3 lightColour(0, 1, 0);						" \
	//	"													" \
	//	" vec3 ambient = ambientStrength * lightColour;		" \
	//	" vec3 light = ambient;								" \
	//
	//	Specular:
	//	" vec3 viewPos = vec3(0, 0, 15);								" \
	//	" vec3 viewDir = normalize(viewPos - v_FragPos);				" \
	//	" vec3 reflectDir = reflect(-lightDir, norm);					" \
	//	" float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);	" \
	//	" vec3 specular = spec * vec3(1, 1, 1); 						" \
	//
	//*****************************************************

	std::shared_ptr<ShaderProgram> shaderProgram = std::make_shared<ShaderProgram>();
	shaderProgram->CreateShader(vertexShaderSrc, fragmentShaderSrc);

	//*****************************************************
	//	[VERT SHADER] DIFFUSE LIGHTING
	//*****************************************************
	const GLchar* vertexShaderSrc2 =
		"attribute vec3 a_Position;														" \
		"attribute vec2 a_TexCoord;														" \
		"attribute vec3 a_Normal;														" \
		"uniform mat4 u_Projection;														" \
		"uniform mat4 u_Model;															" \
		"uniform mat4 u_View;															" \
		"																				" \
		"																				" \
		"varying vec2 v_TexCoord;														" \
		"varying vec3 v_FragPos;														" \
		"varying vec3 v_Normal;															" \
		"																				" \
		"void main()																	" \
		"{																				" \
		" v_FragPos = vec3(u_Model * vec4(a_Position, 1.0));							" \
		" gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);		" \
		" v_TexCoord = a_TexCoord;														" \
		" v_Normal = vec3(u_Model * vec4(a_Normal, 0));									" \
		"}																				" \
		"																				";

	//*****************************************************
	//	[FRAG SHADER] DIFFUSE LIGHTING
	//*****************************************************
	const GLchar* fragmentShaderSrc2 =
		"#version 410\n													" \
		"uniform sampler2D u_Texture;									" \
		"varying vec2 v_TexCoord;										" \
		"varying vec3 v_FragPos;										" \
		"varying vec3 v_Normal;											" \
		"uniform float u_Pulse;											" \
		"uniform mat4 u_View;											" \
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
		" vec3 diffuse = vec3(1, 1, 0) * diff;							" \
		"																" \
		" 																" \
		"																" \
		" gl_FragColor = vec4(diffuse, 1.0) * tex;							" \
		"}																";

	std::shared_ptr<ShaderProgram> shaderProgram2 = std::make_shared<ShaderProgram>();
	shaderProgram2->CreateShader(vertexShaderSrc2, fragmentShaderSrc2);

	//*****************************************************
	//	[VERT SHADER] TEXTURE ONLY
	//*****************************************************
	const GLchar* vertexShaderSrc3 =
		"attribute vec3 a_Position;														" \
		"attribute vec2 a_TexCoord;														" \
		"uniform mat4 u_Projection;														" \
		"uniform mat4 u_Model;															" \
		"uniform mat4 u_View;															" \
		"																				" \
		"																				" \
		"varying vec2 v_TexCoord;														" \
		"																				" \
		"void main()																	" \
		"{																				" \
		" gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);		" \
		" v_TexCoord = a_TexCoord;														" \
		"}																				" \
		"																				";

	//*****************************************************
	//	[FRAG SHADER] TEXTURE ONLY
	//*****************************************************
	const GLchar* fragmentShaderSrc3 =
		"uniform sampler2D u_Texture;									" \
		"varying vec2 v_TexCoord;										" \
		"uniform float u_Pulse;											" \
		"																" \
		"void main()													" \
		"{																" \
		" vec4 tex = texture2D(u_Texture, v_TexCoord);					" \
		" gl_FragColor = tex;											" \
		"}																";

	std::shared_ptr<ShaderProgram> shaderProgram3 = std::make_shared<ShaderProgram>();
	shaderProgram3->CreateShader(vertexShaderSrc3, fragmentShaderSrc3);

	//*****************************************************
	//	CREATE UI VERTEX SHADER 
	//*****************************************************
	const GLchar* vertexShaderSrcUI =
		"attribute vec3 a_Position;														" \
		"attribute vec2 a_TexCoord;														" \
		"uniform mat4 u_Projection;														" \
		"uniform mat4 u_Model;															" \
		"																				" \
		"																				" \
		"varying vec2 v_TexCoord;														" \
		"																				" \
		"void main()																	" \
		"{																				" \
		" gl_Position = u_Projection * u_Model * vec4(a_Position, 1.0);		" \
		" v_TexCoord = a_TexCoord;														" \
		"}																				" \
		"																				";

	//*****************************************************
	//	CREATE UI FRAGMENT SHADER 
	//*****************************************************
	const GLchar* fragmentShaderSrcUI =
		"uniform sampler2D u_Texture;									" \
		"varying vec2 v_TexCoord;										" \
		"																" \
		"void main()													" \
		"{																" \
		" vec4 tex = texture2D(u_Texture, v_TexCoord);					" \
		" gl_FragColor = tex;											" \
		"}																";

	std::shared_ptr<ShaderProgram> shaderProgramUI = std::make_shared<ShaderProgram>();
	shaderProgramUI->CreateShader(vertexShaderSrcUI, fragmentShaderSrcUI);

	//******************************
	//	Group Perspective Shaders
	//******************************
	// Vector of Shaders
	std::vector<std::shared_ptr<ShaderProgram>> shaders;

	shaders.push_back(shaderProgram);
	shaders.push_back(shaderProgram2);
	shaders.push_back(shaderProgram3);

	//*****************************************************
	//	OBTAIN UNIFORM LOCATION
	//*****************************************************
	GLint pulseLoc = glGetUniformLocation(shaderProgram->getId(), "u_Pulse");

	if (pulseLoc == -1)
	{
		//throw std::exception();
	}

	// Find uniform locations
	GLint modelLoc = glGetUniformLocation(shaderProgram->getId(), "u_Model");
	GLint projectionLoc = glGetUniformLocation(shaderProgram->getId(), "u_Projection");
	GLint viewLoc = glGetUniformLocation(shaderProgram->getId(), "u_View");
	GLint inverseViewLoc = glGetUniformLocation(shaderProgram->getId(), "u_InverseView");

	if (modelLoc == -1)
	{
		throw std::exception();
	}
	if (projectionLoc == -1)
	{
		throw std::exception();
	}
	if (viewLoc == -1)
	{
		throw std::exception();
	}
	if (inverseViewLoc == -1)
	{
		throw std::exception();
	}

	//*****************************************************
	//	OBTAIN UNIFORM LOCATION 2
	//*****************************************************
	GLint pulseLoc2 = glGetUniformLocation(shaderProgram2->getId(), "u_Pulse");

	if (pulseLoc2 == -1)
	{
		//throw std::exception();
	}

	// Find uniform locations
	GLint modelLoc2 = glGetUniformLocation(shaderProgram2->getId(), "u_Model");
	GLint projectionLoc2 = glGetUniformLocation(shaderProgram2->getId(), "u_Projection");
	GLint viewLoc2 = glGetUniformLocation(shaderProgram2->getId(), "u_View");

	if (modelLoc2 == -1)
	{
		throw std::exception();
	}
	if (projectionLoc2 == -1)
	{
		throw std::exception();
	}
	if (viewLoc2 == -1)
	{
		throw std::exception();
	}

	//*****************************************************
	//	OBTAIN UNIFORM LOCATION 3
	//*****************************************************
	GLint pulseLoc3 = glGetUniformLocation(shaderProgram3->getId(), "u_Pulse");

	if (pulseLoc3 == -1)
	{
		//throw std::exception();
	}

	// Find uniform locations
	GLint modelLoc3 = glGetUniformLocation(shaderProgram3->getId(), "u_Model");
	GLint projectionLoc3 = glGetUniformLocation(shaderProgram3->getId(), "u_Projection");
	GLint viewLoc3 = glGetUniformLocation(shaderProgram3->getId(), "u_View");

	if (modelLoc3 == -1)
	{
		throw std::exception();
	}
	if (projectionLoc3 == -1)
	{
		throw std::exception();
	}
	if (viewLoc3 == -1)
	{
		throw std::exception();
	}

	//*****************************************************
	//	OBTAIN UI UNIFORM LOCATION 
	//*****************************************************
	GLint pulseLocUI = glGetUniformLocation(shaderProgramUI->getId(), "u_Pulse");

	if (pulseLocUI == -1)
	{
		//throw std::exception();
	}

	// Find uniform locations
	GLint modelLocUI = glGetUniformLocation(shaderProgramUI->getId(), "u_Model");
	GLint projectionLocUI = glGetUniformLocation(shaderProgramUI->getId(), "u_Projection");

	if (modelLocUI == -1)
	{
		throw std::exception();
	}
	if (projectionLocUI == -1)
	{
		throw std::exception();
	}


	// VECTORS
	std::vector<GLint> pulseLocs;
	std::vector<GLint> modelLocs;
	std::vector<GLint> projectionLocs;
	std::vector<GLint> viewLocs;
	std::vector<GLint> inverseViewLocs;

	pulseLocs.push_back(pulseLoc);
	pulseLocs.push_back(pulseLoc2);
	pulseLocs.push_back(pulseLoc3);

	modelLocs.push_back(modelLoc);
	modelLocs.push_back(modelLoc2);
	modelLocs.push_back(modelLoc3);

	projectionLocs.push_back(projectionLoc);
	projectionLocs.push_back(projectionLoc2);
	projectionLocs.push_back(projectionLoc3);

	viewLocs.push_back(viewLoc);
	viewLocs.push_back(viewLoc2);
	viewLocs.push_back(viewLoc3);

	inverseViewLocs.push_back(inverseViewLoc);
	inverseViewLocs.push_back(-1);
	inverseViewLocs.push_back(-1);

	//*****************************************************
	//	[IMAGE] LOAD  
	//	- File needs to be placed next to the project file. 
	//*****************************************************

	int w = 0;
	int h = 0;



	std::shared_ptr<Texture> catTexture = std::make_shared<Texture>(stbi_load("models/curuthers/Whiskers_diffuse.png", &w, &h, NULL, 4), w, h);
	std::shared_ptr<Texture> skeletonTexture = std::make_shared<Texture>(stbi_load("models/skeleton/skeleton_diffuse.png", &w, &h, NULL, 4), w, h);
	std::shared_ptr<Texture> crocTexture = std::make_shared<Texture>(stbi_load("models/croc/croc_diffuse.png", &w, &h, NULL, 4), w, h);

	std::vector<std::shared_ptr<Texture>> modelTextures;
	modelTextures.push_back(catTexture);
	modelTextures.push_back(skeletonTexture);
	modelTextures.push_back(crocTexture);


	//*****************************************************
	//	[IMAGE] LEFT ARROW   
	//*****************************************************

	std::shared_ptr<Texture> leftArrow = std::make_shared<Texture>(stbi_load("models/leftarrow.png", &w, &h, NULL, 4), w, h);
	std::shared_ptr<Texture> leftArrowActive = std::make_shared<Texture>(stbi_load("models/leftarrowactive.png", &w, &h, NULL, 4), w, h);

	//*****************************************************
	//	[IMAGE] RIGHT ARROW   
	//*****************************************************

	std::shared_ptr<Texture> rightArrow = std::make_shared<Texture>(stbi_load("models/rightarrow.png", &w, &h, NULL, 4), w, h);
	std::shared_ptr<Texture> rightArrowActive = std::make_shared<Texture>(stbi_load("models/rightarroveactive.png", &w, &h, NULL, 4), w, h);

	//*****************************************************
	//	[IMAGE] MODEL ICON
	//*****************************************************

	std::shared_ptr<Texture> modelIcon = std::make_shared<Texture>(stbi_load("models/modelicon.png", &w, &h, NULL, 4), w, h);

	//*****************************************************
	//	[IMAGE] SHADER ICON
	//*****************************************************

	std::shared_ptr<Texture> shaderIcon = std::make_shared<Texture>(stbi_load("models/shadericon.png", &w, &h, NULL, 4), w, h);

	//*****************************************************
	//	[IMAGE] ROTATE ICON
	//*****************************************************

	std::shared_ptr<Texture> rotateIcon = std::make_shared<Texture>(stbi_load("models/rotate.png", &w, &h, NULL, 4), w, h);
	std::shared_ptr<Texture> rotateActiveIcon = std::make_shared<Texture>(stbi_load("models/rotateactive.png", &w, &h, NULL, 4), w, h);

	//*****************************************************
	//	MAIN LOOP
	//*****************************************************
	bool stopped = false;
	float pulse = 0;

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
		//glEnable(GL_CULL_FACE);	// Shows the triangles separately

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

		pulse += 1.0f;
		if (pulse > 1)
		{
			pulse = 0;
		}

		// Update
		if (intersect(mouse, glm::vec4(50, 50, 100, 100)) && mouseButtonDown)
		{
			std::cout << "Texture intersect True" << std::endl;
			mouseButtonDown = false;
		}

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
			glBindTexture(GL_TEXTURE_2D, rightArrowActive->GetId());
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
		if (intersect(mouse, glm::vec4(50, 125, 100, 100)) && mouseButtonDown && !rotateOn)
		{
			rotateOn = true;
			std::cout << "Rotate intersect True" << std::endl;
			mouseButtonDown = false;
		}
		else if (intersect(mouse, glm::vec4(50, 125, 100, 100)) && mouseButtonDown && rotateOn)
		{
			rotateOn = false;
			std::cout << "Rotate intersect True" << std::endl;
			mouseButtonDown = false;
		}

		// Set background to Cyan 
		//glClearColor(0.0f, 0.33f, 0.5f, 1.0f);
		glClearColor(0.33f, 0.33f, 0.33f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Drawing
		// Instruct OpenGL to use our shader program and our VAO
		glUseProgram(shaders.at(shaderSelector)->getId());
		//glBindVertexArray(vao->getId());
		// Cat:
		//glBindTexture(GL_TEXTURE_2D, data->GetId());
		glBindTexture(GL_TEXTURE_2D, modelTextures.at(modelSelector)->GetId());
		glBindVertexArray(models.at(modelSelector)->getId());

		//glUniform1f(pulseLocs.at(shaderSelector), pulse);

		//*****************************************************
		//	PERSPECTIVE PATH
		//*****************************************************

		/* Lab 3 - Start */
		// Prepare the perspective projection matrix
		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
			(float)width / (float)height, 0.1f, 100.0f);

		//	MODEL MATRIX  //

		// Prepare the model matrix
		glm::mat4 model(1.0f);
		//model = glm::rotate(model, glm::radians(rot), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(0, 0, 0));
		model = glm::scale(model, glm::vec3(scale, scale, scale));

		//glm::vec3 diff = glm::vec3(model * glm::vec4(0, 0, 0, 1));

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
		/* LAB 3 - End */

		// Draw 3 vertices (a triangle)
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		// Cat:
		glDrawArrays(GL_TRIANGLES, 0, models.at(modelSelector)->getVertCount());

		glDisable(GL_DEPTH_TEST);

		//*****************************************************
		//	ORTHOGRAPHIC PATH
		//*****************************************************

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindVertexArray(vao->getId());
		glUseProgram(shaderProgramUI->getId());

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
		// Draw 3 vertices (a triangle)
		glDrawArrays(GL_TRIANGLES, 0, vao->getVertCount());

		//*****************************************************
		// [HUD] Left Arrow (Model)
		//*****************************************************

		glBindTexture(GL_TEXTURE_2D, leftArrow->GetId());
		glBindVertexArray(leftArrowVAO->getId());

		glm::mat4 leftArrowModel(1.0f);
		leftArrowModel = glm::translate(leftArrowModel, glm::vec3(100, height - 500, 0));
		leftArrowModel = glm::scale(leftArrowModel, glm::vec3(100, 100, 1));

		// Upload the model matrix
		glUniformMatrix4fv(modelLocUI, 1, GL_FALSE, glm::value_ptr(leftArrowModel));

		// Upload the projection matrix
		glUniformMatrix4fv(projectionLocUI, 1, GL_FALSE,
			glm::value_ptr(projection));

		glDrawArrays(GL_TRIANGLES, 0, leftArrowVAO->getVertCount());

		//*****************************************************
		// [HUD] Right Arrow (Model)
		//*****************************************************

		glBindTexture(GL_TEXTURE_2D, rightArrow->GetId());
		glBindVertexArray(rightArrowVAO->getId());

		glm::mat4 rightArrowModel(1.0f);
		rightArrowModel = glm::translate(rightArrowModel, glm::vec3(250, height - 500, 0));
		rightArrowModel = glm::scale(rightArrowModel, glm::vec3(100, 100, 1));

		// Upload the model matrix
		glUniformMatrix4fv(modelLocUI, 1, GL_FALSE, glm::value_ptr(rightArrowModel));

		// Upload the projection matrix
		glUniformMatrix4fv(projectionLocUI, 1, GL_FALSE,
			glm::value_ptr(projection));

		glDrawArrays(GL_TRIANGLES, 0, rightArrowVAO->getVertCount());

		//*****************************************************
		// [HUD] Model Icon (left)
		//*****************************************************

		glBindTexture(GL_TEXTURE_2D, modelIcon->GetId());
		glBindVertexArray(modelIconVAO->getId());

		glm::mat4 modelIconModelLeft(1.0f);
		modelIconModelLeft = glm::translate(modelIconModelLeft, glm::vec3(175, height - 500, 0));
		modelIconModelLeft = glm::scale(modelIconModelLeft, glm::vec3(40, 40, 1));

		// Upload the model matrix
		glUniformMatrix4fv(modelLocUI, 1, GL_FALSE, glm::value_ptr(modelIconModelLeft));

		// Upload the projection matrix
		glUniformMatrix4fv(projectionLocUI, 1, GL_FALSE,
			glm::value_ptr(projection));

		glDrawArrays(GL_TRIANGLES, 0, modelIconVAO->getVertCount());

		//*****************************************************
		// [HUD] Left Arrow (Shader)
		//*****************************************************

		glBindTexture(GL_TEXTURE_2D, leftArrow->GetId());
		glBindVertexArray(leftArrowVAO->getId());

		leftArrowModel = glm::mat4(1.0f);
		leftArrowModel = glm::translate(leftArrowModel, glm::vec3(width - 250, height - 500, 0));
		leftArrowModel = glm::scale(leftArrowModel, glm::vec3(100, 100, 1));

		// Upload the model matrix
		glUniformMatrix4fv(modelLocUI, 1, GL_FALSE, glm::value_ptr(leftArrowModel));

		// Upload the projection matrix
		glUniformMatrix4fv(projectionLocUI, 1, GL_FALSE,
			glm::value_ptr(projection));

		glDrawArrays(GL_TRIANGLES, 0, leftArrowVAO->getVertCount());

		//*****************************************************
		// [HUD] Right Arrow (Shader)
		//*****************************************************

		glBindTexture(GL_TEXTURE_2D, rightArrow->GetId());
		glBindVertexArray(rightArrowVAO->getId());

		rightArrowModel = glm::mat4(1.0f);
		rightArrowModel = glm::translate(rightArrowModel, glm::vec3(width - 100, height - 500, 0));
		rightArrowModel = glm::scale(rightArrowModel, glm::vec3(100, 100, 1));

		// Upload the model matrix
		glUniformMatrix4fv(modelLocUI, 1, GL_FALSE, glm::value_ptr(rightArrowModel));

		// Upload the projection matrix
		glUniformMatrix4fv(projectionLocUI, 1, GL_FALSE,
			glm::value_ptr(projection));

		glDrawArrays(GL_TRIANGLES, 0, rightArrowVAO->getVertCount());

		//*****************************************************
		// [HUD] Shader Icon (Right)
		//*****************************************************

		glBindTexture(GL_TEXTURE_2D, shaderIcon->GetId());
		glBindVertexArray(vao->getId());

		glm::mat4 shaderIconModel(1.0f);
		shaderIconModel = glm::translate(shaderIconModel, glm::vec3(width - 175, height - 500, 0));
		shaderIconModel = glm::scale(shaderIconModel, glm::vec3(40, 40, 1));

		// Upload the model matrix
		glUniformMatrix4fv(modelLocUI, 1, GL_FALSE, glm::value_ptr(shaderIconModel));

		// Upload the projection matrix
		glUniformMatrix4fv(projectionLocUI, 1, GL_FALSE,
			glm::value_ptr(projection));

		glDrawArrays(GL_TRIANGLES, 0, vao->getVertCount());

		//*****************************************************
		// [HUD] Rotate Icon
		//*****************************************************

		if (rotateOn)
		{
			glBindTexture(GL_TEXTURE_2D, rotateActiveIcon->GetId());
		}
		else if (!rotateOn)
		{
			glBindTexture(GL_TEXTURE_2D, rotateIcon->GetId());
		}
		glBindVertexArray(vao->getId());

		glm::mat4 rotateIconModel(1.0f);
		rotateIconModel = glm::translate(rotateIconModel, glm::vec3(100, height - 175, 0));
		rotateIconModel = glm::scale(rotateIconModel, glm::vec3(40, 40, 1));

		// Upload the model matrix
		glUniformMatrix4fv(modelLocUI, 1, GL_FALSE, glm::value_ptr(rotateIconModel));

		// Upload the projection matrix
		glUniformMatrix4fv(projectionLocUI, 1, GL_FALSE,
			glm::value_ptr(projection));

		glDrawArrays(GL_TRIANGLES, 0, vao->getVertCount());

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