#pragma once

#ifndef SHADERMANAGERDEFERRED2_H
#define SHADERMANAGERDEFERRED2_H

#include <string>

#include <GL/glew.h>
//#define GLFW_DLL		// only used when linking to a DLL version of GLFW.
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>
#include "OrganicGLWinUtils.h"
#include "ShaderBase.h"

class ShaderManagerDeferred2 : public ShaderBase
{
	public:

		// deferred specific members
		GLuint quadVaoID = 0;								// vertex array object ID
		GLuint terrainVaoID = 0;
		GLuint quadBufferID = 0;							// quad vertex data object ID
		GLuint pass1index, pass2index;						// IDs of the program pass subroutine calls
		GLuint deferredFBO = 0;								// handle for the deferred FBO

		// pass-related functions/members
		GLuint mvpHandle;
		GLuint modelViewHandle;
		void setMatrices();
		void runPass1(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, int in_numberOfCollections);
		void runPass2();

		// atlas/texture uniforms
		GLuint textureUniform;		// ID for the "albedo" texture atlas
		GLuint worldPosUniform;		// ID for world position uniform
		glm::vec3 worldPosition;	// a vec3 that will be used to update worldPosUniform
		GLuint atlasWidthUniform;	// ID for the uniform that maintains the current atlas width
		GLuint atlasTileWidthUniform;	// the ID of the atlas tile width uniform

		void createGBufText(GLenum texUnit, GLenum  format, GLuint &texid);

		// virtual functions (for later)
		void initialize(int in_windowWidth, int in_windowHeight);
		void initializeShader(int in_windowWidth, int in_windowHeight, int in_terrainBufferSize);
		void render();
		void multiDrawTerrain(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, int in_numberOfCollections);
		void shutdownGL();

		// independent, object specific values
		void setupRenderQuad();
		void setupFBO();
		void setupTerrainVAO();
		void updateMatricesForPass1();
		void acquireSubroutineIndices();
		void setupImGui();
};

#endif