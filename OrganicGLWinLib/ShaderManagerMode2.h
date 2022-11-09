#pragma once

#ifndef SHADERMANAGERMODE2_H
#define SHADERMANAGERMODE2_H


#include <GL/glew.h>
//#define GLFW_DLL		// only used when linking to a DLL version of GLFW.
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>
#include "OrganicGLWinUtils.h"
#include "ShaderBase.h"

class ShaderManagerMode2 : public ShaderBase
{
	// shader-specific variables
	//GLuint textID;	// ID for the typical texture
	GLuint textureUniform;

	// virtual function overrides
	void initialize(int in_windowWidth, int in_windowHeight);
	void initializeShader(int in_windowWidth, int in_windowHeight, int in_terrainBufferSize);
	void render();
	void multiDrawTerrain(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, int in_numberOfCollections);
	void shutdownGL();
};

#endif
