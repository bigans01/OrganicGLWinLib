#pragma once

#ifndef ORGANICGLWINUTILS
#define ORGANICGLWINUTILS

#include <GL/glew.h>
//#define GLFW_DLL		// only used when linking to a DLL version of GLFW.
#include <GLFW/glfw3.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>

#include <iostream>

#include "OrganicShaderLoader.h"

class OrganicGLWinUtils
{
public:
	static void createImmutableBufferMode1(GLuint* in_bufferID, int in_bufferSize, int in_numberOfBuffers);		// creates an immutable buffer, and sets appropriate vertexarray attributes for mode 0
	static void createImmutableBufferMode0(GLuint* in_bufferID, int in_bufferSize, int in_numberOfBuffers);		// creates an immutable buffer, and sets appropriate vertexarray attributes for mode 1
	static void createImmutableBufferMode2(GLuint* in_bufferID, int in_bufferSize, int in_numberOfBuffers,  GLuint* in_textureRef);
	static void createAndBindVertexArray(GLuint* in_bufferID);													// creates and binds a single vertex array
	static void createAndBindDrawIndirectBuffer(GLuint* in_bufferID);											// creates and binds a single indirect buffer
	static void initializeLibraryAndSetHints();	// initializes the OpenGL library and the window; adds input(keypress) listening for the window
	static void loadShadersViaMode(GLuint* in_programID, int in_mode);
	static void initializeGlew();
	static void setGLFWInputMode(GLFWwindow* in_window);
	static void multiDrawArraysMode0(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, GLuint* in_MVPuniformLocation, glm::mat4* in_MVPmat4ref, int in_numberOfCollections);
	static void multiDrawArraysMode1(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, GLuint* in_MVPuniformLocation, glm::mat4* in_MVPmat4ref, int in_numberOfCollections);
	static void multiDrawArraysMode2(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, GLuint* in_MVPuniformLocation, glm::mat4* in_MVPmat4ref, GLuint* in_textureRef, GLuint* in_textureUniformRef, int in_numberOfCollections);
	static GLFWwindow* createGLFWWindow(int in_width, int in_height);
	static void makeContextCurrent(GLFWwindow* in_window);
	static void checkWindowValidity(GLFWwindow* in_window);
	static void setBasicStates();																				// sets basic OpenGL states
	static void setClearColor(float in_red, float in_green, float in_blue, float in_alpha);						// sets the background color
};

#endif