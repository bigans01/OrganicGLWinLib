#pragma once

#ifndef ORGANICGLWINUTILS
#define ORGANICGLWINUTILS

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

#include <GL/glew.h>
//#define GLFW_DLL		// only used when linking to a DLL version of GLFW.
#include <GLFW/glfw3.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>
#include <math.h>

#include <iostream>
#include <map>
#include <vector>

#include "OrganicShaderLoader.h"
#include "AtlasMetaData.h"
#include "AtlasMap.h"
#include "AtlasPropertiesGL.h"
#include "TileLoadData.h"
#include "stb_image.h"

class OrganicGLWinUtils
{
public:
	static void createImmutableBufferMode1(GLuint* in_bufferID, int in_bufferSize, int in_numberOfBuffers);		// creates an immutable buffer, and sets appropriate vertexarray attributes for mode 0
	static void createImmutableBufferMode0(GLuint* in_bufferID, int in_bufferSize, int in_numberOfBuffers);		// creates an immutable buffer, and sets appropriate vertexarray attributes for mode 1
	static void createImmutableBufferMode2(GLuint* in_bufferID, int in_bufferSize, int in_numberOfBuffers,  GLuint* in_textureRef);
	static void createImmutableBuffer(GLuint* in_bufferID, int in_bufferSize, int in_numberOfBuffers);
	static void createImmutableBufferExperimental(GLuint* in_bufferID, int in_bufferSize, int in_numberOfBuffers, GLuint* in_textureRef, GLuint* in_textureRef2);
	static void createImmutableBufferExperimental2(GLuint* in_bufferID, int in_bufferSize, int in_numberOfBuffers, GLuint* in_textureRef);
	static void createImmutableBufferExperimental3(GLuint* in_bufferID, int in_bufferSize, int in_numberOfBuffers, GLuint* in_textureRef);
	static AtlasMetaData findAtlasMetadata(int in_atlasWidth, int in_tileWidth);
	static void createAndBindVertexArray(GLuint* in_bufferID);													// creates and binds a single vertex array
	static void createAndBindDrawIndirectBuffer(GLuint* in_bufferID);											// creates and binds a single indirect buffer
	static void initializeLibraryAndSetHints();	// initializes the OpenGL library and the window; adds input(keypress) listening for the window
	static void loadShadersViaMode(GLuint* in_programID, int in_mode);
	static void initializeGlew();
	static void computeMatricesFromInputs(GLFWwindow* in_windowRef, float* in_horizontalAngle, float* in_verticalAngle, glm::vec3* in_position, float*  in_initialFoV, float* in_speed, float in_mouseSpeed, glm::mat4* in_projectionMatrix, glm::mat4* in_viewMatrix, glm::mat4* in_MVPref);
	static void setGLFWInputMode(GLFWwindow* in_window);
	static int setupTextureAtlasJPEG(GLuint* in_atlasTextureRef, AtlasMap* in_atlasRef, AtlasPropertiesGL* in_atlasPropertiesGLRef);
	static int setupTextureAtlasJPEG2(GLuint* in_atlasTextureRef, AtlasMap* in_atlasRef, AtlasPropertiesGL* in_atlasPropertiesGLRef);
	static void multiDrawArraysMode0(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, GLuint* in_MVPuniformLocation, glm::mat4* in_MVPmat4ref, int in_numberOfCollections);
	static void multiDrawArraysMode1(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, GLuint* in_MVPuniformLocation, glm::mat4* in_MVPmat4ref, int in_numberOfCollections);
	static void multiDrawArraysMode2(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, GLuint* in_MVPuniformLocation, glm::mat4* in_MVPmat4ref, GLuint* in_textureRef, GLuint* in_textureUniformRef, int in_numberOfCollections);
	static void multiDrawArraysMode3(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, GLuint* in_MVPuniformLocation, glm::mat4* in_MVPmat4ref, GLuint* in_textureRef, GLuint* in_textureUniformRef, int in_numberOfCollections, GLuint* in_textureWidthRef, glm::vec3* in_textureWidth);
	static void shutdownOpenGLBasic(GLuint* in_terrainBufferID, GLuint* in_vertexArrayID, GLuint* in_programID);			// shuts down OpenGL for modes 0 through 2
	static GLFWwindow* createGLFWWindow(int in_width, int in_height);
	static void makeContextCurrent(GLFWwindow* in_window);
	static void checkWindowValidity(GLFWwindow* in_window);
	static void setBasicStates();																				// sets basic OpenGL states
	static void setClearColor(float in_red, float in_green, float in_blue, float in_alpha);						// sets the background color
	static GLuint loadDDS(const char* imagepath);
};

#endif