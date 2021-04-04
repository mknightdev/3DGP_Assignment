#define SDL_MAIN_HANDLED

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "ShaderProgram.h"

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


int main()
{
	//*****************************************************
	//	CREATE APPLICATION WINDOW
	//*****************************************************
	SDL_Window* window = SDL_CreateWindow("Triangle",
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
	positionsVbo->add(glm::vec3(-0.5f, -0.5f, 0.0f));
	positionsVbo->add(glm::vec3(0.5f, -0.5f, 0.0f));
	positionsVbo->add(glm::vec3(-0.5f, 0.5f, 0.0f));
	positionsVbo->add(glm::vec3(0.5f, 0.5f, 0.0f));
	positionsVbo->add(glm::vec3(0.5f, -0.5f, 0.0f));
	positionsVbo->add(glm::vec3(-0.5f, 0.5f, 0.0f));

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

	// Model destination
	std::string modelSettings;

	// Model settings
	std::ifstream readFile("Model_Settings.txt");

	while (readFile >> modelSettings)
	{
		std::cout << modelSettings;
	}

	// Close the file, as it's no longer needed. 
	readFile.close();

	// path is the file location
	std::shared_ptr<VertexArray> cat = std::make_shared<VertexArray>(modelSettings);
	std::shared_ptr<VertexArray> block = std::make_shared<VertexArray>("models/block.obj");

	std::vector<std::shared_ptr<VertexArray>> models;

	models.push_back(cat);
	models.push_back(block);


	//*****************************************************
	//	CREATE VERTEX SHADER
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
	//	CREATE FRAGMENT SHADER
	//*****************************************************
	const GLchar* fragmentShaderSrc =
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
		" vec3 diffuse = vec3(0, 1, 1) * diff;							" \
		"																" \
		" vec3 viewPos = vec3(inverse(u_View) * vec4(0, 0, 0, 1));		" \
		" vec3 viewDir = normalize(viewPos - v_FragPos);				" \
		" vec3 reflectDir = reflect(-lightDir, norm);					" \
		" float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);	" \
		" vec3 specular = spec * vec3(1, 1, 1); 						" \
		" 																" \
		"																" \
		" gl_FragColor = vec4(diffuse + specular, 1.0);					" \
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

	//*****************************************************
	//	LOAD IMAGE
	//	- File needs to be placed next to the project file. 
	//*****************************************************

	int w = 0;
	int h = 0;

	//unsigned char* data = stbi_load("image.png", &w, &h, NULL, 4);
	// Cat: 
	unsigned char* data = stbi_load("models/curuthers/Whiskers_diffuse.png", &w, &h, NULL, 4);

	if (!data)
	{
		throw std::exception();
	}


	//*****************************************************
	//	UPLOAD TO GPU
	//*****************************************************

	// Create and bind a texture.
	GLuint textureId = 0;
	glGenTextures(1, &textureId);

	if (!textureId)
	{
		throw std::exception();
	}

	glBindTexture(GL_TEXTURE_2D, textureId);

	// Upload the image data to the bound texture unit in the GPU
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, data);

	// Free the loaded data because we now have a copy on the GPU
	free(data);

	// Generate Mipmap so the texture can be mapped correctly
	glGenerateMipmap(GL_TEXTURE_2D);

	// Unbind the texture because we are done operating on it
	glBindTexture(GL_TEXTURE_2D, 0);

	//*****************************************************
	//	MAIN LOOP
	//*****************************************************
	bool stopped = false;
	float pulse = 0;
	/* Lab 3 - Start */
	float angle = 0;
	float xPos = 0;
	/* Lab 3 - End */

	float rot = 0;
	float deltaTime = 0.0001f;
	float prevTime = 0;
	
	float pos = 0;
	float speed = 1.0f;

	int selector = 0;

	glm::vec3 camPos(0, 0, 15);

	while (!stopped)
	{
		int width = 0;
		int height = 0;

		glEnable(GL_DEPTH_TEST);

		SDL_GetWindowSize(window, &width, &height);

		SDL_Event event = { 0 };

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
					case SDLK_w:
						std::cout << "W Key Pressed" << std::endl;
						camPos.z += 50.0f * deltaTime;
						break;
					case SDLK_a:
						std::cout << "A Key Pressed" << std::endl;
						break;
					case SDLK_s:
						std::cout << "S Key Pressed" << std::endl;
						break;
					case SDLK_d:
						std::cout << "D Key Pressed" << std::endl;
						break;
					case SDLK_q:
						std::cout << "Q Key Pressed" << std::endl;

						if (selector == 0)
						{
							/* if selector equals zero, set selector to be the value of the end of the model vector,
							* this prevents us from going out of bounds. */
							selector = models.size() - 1;
							std::cout << "Selector: " << selector << std::endl;
						}
						else
						{
							// Otherwise, continue decreasing selector to view previous models.
							selector--;
							std::cout << "Selector: " << selector << std::endl;
						}

						break;
					case SDLK_e:
						std::cout << "E Key Pressed" << std::endl;

						if (selector == models.size() - 1)
						{
							/* If selector is the same as our vector of models (-1), 
							* set selector back to zero so we don't go out of bounds.*/
							selector = 0;
							std::cout << "Selector: " << selector << std::endl;
						}
						else
						{
							// Otherwise, continue increasing selector to view further models. 
							selector++;
							std::cout << "Selector: " << selector << std::endl;
						}
						break;
				}
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

		// Set background to Red 
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Drawing
		// Instruct OpenGL to use our shader program and our VAO
		glUseProgram(shaderProgram->getId());
		//glBindVertexArray(vao->getId());
		// Cat:
		glBindVertexArray(models.at(selector)->getId());
		glBindTexture(GL_TEXTURE_2D, textureId);

		glUniform1f(pulseLoc, pulse);

		//*****************************************************
		//	PERSPECTIVE PATH
		//*****************************************************

		/* Lab 3 - Start */
		// Prepare the perspective projection matrix
		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
			(float)width / (float)height, 0.1f, 100.0f);

		//*****************************************************
		//	MODEL MATRIX
		//*****************************************************

		// Prepare the model matrix
		glm::mat4 model(1.0f);
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(0, 0, 0));

		glm::vec3 diff = glm::vec3(model * glm::vec4(0, 0, 0, 1));

		// Upload the model matrix
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		//*****************************************************
		//	VIEW MATRIX
		//*****************************************************
		
		// If we do rotate then translate, it will look like it will orbit around the model
		glm::mat4 view(1.0f);
		//view = glm::rotate(view, glm::radians(rot), glm::vec3(0, 1, 0));
		//view = glm::translate(view, glm::vec3(0, 0, 15));
		view = glm::translate(view, camPos);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(glm::inverse(view)));

		//glUniform3f(camPos, )

		// Increase the float angle so next frame the triangle rotates further
		// How fast the object rotates
		rot += 45.0f * deltaTime;

		// Make sure the current program is bound
		// Upload the projection matrix
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		/* LAB 3 - End */

		// Draw 3 vertices (a triangle)
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		// Cat:
		glDrawArrays(GL_TRIANGLES, 0, models.at(selector)->getVertCount());

		glDisable(GL_DEPTH_TEST);

		//*****************************************************
		//	ORTHOGRAPHIC PATH
		//*****************************************************

		// Prepare the orthographic projection matrix (reusing the variable)
		projection = glm::ortho(0.0f, (float)WINDOW_HEIGHT, 0.0f, (float)WINDOW_WIDTH, 0.0f, 1.0f);

		// Prepare model matrix. The scale is important because now our triangle
		// would be the size of a single pixel when mapped to an orthographic
		// projection.
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(100, /*height - */100, 1));
		model = glm::scale(model, glm::vec3(100, 100, 1));

		// Upload the model matrix
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Upload the projection matrix
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Draw shape as before
		// Draw 3 vertices (a triangle)
		glDrawArrays(GL_TRIANGLES, 0, models.at(selector)->getVertCount());


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