#pragma once

#ifndef SHADERMANAGERMODE3_H
#define SHADERMANAGERMODE3_H

#include <string>

#include <GL/glew.h>
//#define GLFW_DLL		// only used when linking to a DLL version of GLFW.
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>
#include "OrganicGLWinUtils.h"
#include "ShaderBase.h"

class ShaderManagerMode3 : public ShaderBase
{
	// shader-specific variables
	//GLuint textID;	// ID for the typical texture
	GLuint textureUniform;		// ID for the "albedo" texture atlas
	GLuint worldPosUniform;		// ID for world position uniform
	glm::vec3 worldPosition;	// a vec3 that will be used to update worldPosUniform
	GLuint atlasWidthUniform;	// ID for the uniform that maintains the current atlas width
	//float atlasWidth;			// the actual width of the atlas; this will be placed in the atlas uniform
	GLuint atlasTileWidthUniform;	// the ID of the atlas tile width uniform
	//float atlasTileWidth;		// the width of a tile in the atlas; this wil be placed in the atlas tile width uniform

	// virtual function overrides
	void initialize(int in_windowWidth, int in_windowHeight);
	void initializeShader(int in_windowWidth, int in_windowHeight, int in_terrainBufferSize);
	void render();
	void multiDrawTerrain(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, int in_numberOfCollections);
	void shutdownGL();
};

#endif
