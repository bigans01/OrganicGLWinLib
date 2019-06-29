#include "stdafx.h"
#include "ShaderManagerMode0.h"

void ShaderManagerMode0::initialize(int in_windowWidth, int in_windowHeight)
{
	
}

void ShaderManagerMode0::initializeShader(int in_windowWidth, int in_windowHeight, int in_terrainBufferSize)
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
	OrganicGLWinUtils::loadShadersViaMode(&shaderProgramID, 0);		// load mode 0 shaders
	OGLMVertexSubBufferSize = in_terrainBufferSize * 1000000;		// set terrain buffer size
	OrganicGLWinUtils::createImmutableBufferMode0(&terrainBufferID, &terrainSwapID, OGLMVertexSubBufferSize);
	mvpHandle = glGetUniformLocation(shaderProgramID, "MVP");
	glUseProgram(shaderProgramID);

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

void ShaderManagerMode0::render()
{

}

void ShaderManagerMode0::multiDrawTerrain(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, int in_numberOfCollections)
{
	OrganicGLWinUtils::multiDrawArraysMode0(in_drawArrayID, in_startArray, in_vertexCount, &mvpHandle, &MVP, in_numberOfCollections);
}

void ShaderManagerMode0::shutdownGL()
{
	OrganicGLWinUtils::shutdownOpenGLBasic(&terrainBufferID, &organicGLVertexArrayID, &shaderProgramID);
}