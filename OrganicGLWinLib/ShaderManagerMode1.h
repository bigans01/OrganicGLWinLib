#pragma once

#ifndef SHADERMANAGERMODE1_H
#define SHADERMANAGERMODE1_H

#include <string>

#include <GL/glew.h>
//#define GLFW_DLL		// only used when linking to a DLL version of GLFW.
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>
#include "OrganicGLWinUtils.h"
#include "ShaderBase.h"

class ShaderManagerMode1 : public ShaderBase
{
public:
	//GLFWwindow* window;									// pointer to openGL window
	//int width, height;									// screen height, width

	// virtual function overrides
	void initialize(int in_windowWidth, int in_windowHeight);
	void initializeShader(int in_windowWidth, int in_windowHeight, int in_terrainBufferSize);
	void render();
	void multiDrawTerrain(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, int in_numberOfCollections);
	void shutdownGL();
};

#endif
