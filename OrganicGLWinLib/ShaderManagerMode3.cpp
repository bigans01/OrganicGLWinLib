#include "stdafx.h"
#include "ShaderManagerMode3.h"

void ShaderManagerMode3::initialize(int in_windowWidth, int in_windowHeight)
{

}

void ShaderManagerMode3::initializeShader(int in_windowWidth, int in_windowHeight, int in_terrainBufferSize)
{
	width = in_windowWidth;
	height = in_windowHeight;

	// set shader specific VAO values
	vaoAttribMode = 3;
	vaoAttribByteSize = 28;

	OrganicGLWinUtils::initializeLibraryAndSetHints();
	window = OrganicGLWinUtils::createGLFWWindow(width, height);
	OrganicGLWinUtils::checkWindowValidity(window);											// make sure the window is valid
	OrganicGLWinUtils::makeContextCurrent(window);											// set the context as current
	OrganicGLWinUtils::initializeGlew();							// Initialize GLEW
	OrganicGLWinUtils::setBasicStates();							// set basic states for rendering
	OrganicGLWinUtils::setGLFWInputMode(window);				// Ensure we can capture the escape key being pressed below
	OrganicGLWinUtils::setClearColor(0.0f, 0.0f, 0.7f, 0.0f);	// background color
	OrganicGLWinUtils::createAndBindDrawIndirectBuffer(&organicGLIndirectBufferID);		// indirect draw buffer

	// NEW ---> setup IMGui
	OrganicGLWinUtils::IMGuiInit(window);

	// load shaders
	OrganicGLWinUtils::loadShadersViaMode(&shaderProgramID, 3);		// load mode 1 shaders
	OGLMVertexSubBufferSize = in_terrainBufferSize * 1000000;		// set terrain buffer size
	OrganicGLWinUtils::createImmutableBufferMode2(&terrainBufferID, &terrainSwapID, OGLMVertexSubBufferSize, &textID);
	mvpHandle = glGetUniformLocation(shaderProgramID, "MVP");
	textureUniform = glGetUniformLocation(shaderProgramID, "TextureSampler");
	worldPosUniform = glGetUniformLocation(shaderProgramID, "worldPosition");
	atlasWidthUniform = glGetUniformLocation(shaderProgramID, "atlasTextureWidth");
	atlasTileWidthUniform = glGetUniformLocation(shaderProgramID, "atlasTileTextureWidth");
	glUseProgram(shaderProgramID);

	// setup the VAO
	OrganicGLWinUtils::createAndBindVertexArray(&organicGLVertexArrayID);	// Create Vertex Array Object
	glBindBuffer(GL_ARRAY_BUFFER, terrainBufferID);							// NOTE: buffer to point the VAO's attributes to must be bound before doing so!
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, (void*)20);
	glEnableVertexAttribArray(2);

	// set initial values for other things
	projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);		// projection matrix : 45� Field of view, 4:3 ratio, display range : 0.1 unit <-> 100 units

	view = glm::lookAt(													// Camera matrix
		glm::vec3(0, 0, 0),								 // Camera is at (4,3,-3), in World Space
		glm::vec3(0, 0, 0),									// and looks at the origin
		glm::vec3(0, 1, 0)									// Head is up (set to 0,-1,0 to look upside-down)
	);

	model = glm::mat4(1.0f);												// model matrix
	MVP = projection * view * model; // Remember, matrix multiplication is the other way around
}

void ShaderManagerMode3::render()
{

}

void ShaderManagerMode3::multiDrawTerrain(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, int in_numberOfCollections)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	OrganicGLWinUtils::multiDrawArraysMode3(in_drawArrayID, in_startArray, in_vertexCount, &mvpHandle, &MVP, &textID, &textureUniform, in_numberOfCollections, &worldPosUniform, &position, &atlasWidthUniform, atlasWidth, &atlasTileWidthUniform, atlasTileWidth);
	int stupidThing = 3;
	std::cout << "(Base) Vertex count: " << in_vertexCount[0] << std::endl;
}

void ShaderManagerMode3::shutdownGL()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	OrganicGLWinUtils::shutdownOpenGLBasic(&terrainBufferID, &organicGLVertexArrayID, &shaderProgramID);
}