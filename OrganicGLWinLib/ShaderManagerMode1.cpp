#include "stdafx.h"
#include "ShaderManagerMode1.h"

void ShaderManagerMode1::initialize(int in_windowWidth, int in_windowHeight)
{

}

void ShaderManagerMode1::initializeShader(int in_windowWidth, int in_windowHeight, int in_terrainBufferSize)
{
	width = in_windowWidth;
	height = in_windowHeight;
	OrganicGLWinUtils::initializeLibraryAndSetHints();
	window = OrganicGLWinUtils::createGLFWWindow(width, height);
	OrganicGLWinUtils::checkWindowValidity(window);											// make sure the window is valid
	OrganicGLWinUtils::makeContextCurrent(window);											// set the context as current
	OrganicGLWinUtils::initializeGlew();							// Initialize GLEW
	OrganicGLWinUtils::setBasicStates();							// set basic states for rendering
	OrganicGLWinUtils::setGLFWInputMode(window);				// Ensure we can capture the escape key being pressed below
	OrganicGLWinUtils::setClearColor(0.0f, 0.0f, 0.7f, 0.0f);	// background color
	OrganicGLWinUtils::createAndBindVertexArray(&organicGLVertexArrayID);	// Create Vertex Array Object
	OrganicGLWinUtils::createAndBindDrawIndirectBuffer(&organicGLIndirectBufferID);		// indirect draw buffer

	// NEW ---> setup IMGui
	OrganicGLWinUtils::IMGuiInit(window);

	// load shaders
	OrganicGLWinUtils::loadShadersViaMode(&shaderProgramID, 1);		// load mode 1 shaders
	OGLMVertexSubBufferSize = in_terrainBufferSize * 1000000;		// set terrain buffer size
	OrganicGLWinUtils::createImmutableBufferMode0(&terrainBufferID, &terrainSwapID, OGLMVertexSubBufferSize);
	mvpHandle = glGetUniformLocation(shaderProgramID, "MVP");
	glUseProgram(shaderProgramID);

	glBindBuffer(GL_ARRAY_BUFFER, terrainBufferID);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)0);		// First attribute: a vec3 representing the point data, before it is translated by MVP.
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)12);    // Second attribute: a vec3 representing the output color.

	// set initial values for other things
	projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);		// projection matrix : 45° Field of view, 4:3 ratio, display range : 0.1 unit <-> 100 units

	view = glm::lookAt(													// Camera matrix
		glm::vec3(0, 0, 0),								 // Camera is at (4,3,-3), in World Space
		glm::vec3(0, 0, 0),									// and looks at the origin
		glm::vec3(0, 1, 0)									// Head is up (set to 0,-1,0 to look upside-down)
	);

	model = glm::mat4(1.0f);												// model matrix
	MVP = projection * view * model; // Remember, matrix multiplication is the other way around
}

void ShaderManagerMode1::render()
{

}

void ShaderManagerMode1::multiDrawTerrain(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, int in_numberOfCollections)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	OrganicGLWinUtils::multiDrawArraysMode1(in_drawArrayID, in_startArray, in_vertexCount, &mvpHandle, &MVP, in_numberOfCollections);
}

void ShaderManagerMode1::shutdownGL()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	OrganicGLWinUtils::shutdownOpenGLBasic(&terrainBufferID, &organicGLVertexArrayID, &shaderProgramID);
}