#pragma once

#ifndef SHADERMANAGERDEFERRED1_H
#define SHADERMANAGERDEFERRED1_H

#include <string>

#include <GL/glew.h>
//#define GLFW_DLL		// only used when linking to a DLL version of GLFW.
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>
#include "OrganicGLWinUtils.h"
#include "ShaderBase.h"

class ShaderManagerDeferred1 : public ShaderBase
{
public:
	//GLFWwindow* window;									// pointer to openGL window
	//int width, height;									// screen height, width
	GLuint quadVaoID = 0;								// vertex array object ID
	GLuint quadBufferID = 0;							// quad vertex data object ID

	GLuint testTriangleVaoID;
	GLuint testTriangleBufferID;
	GLuint mainAlbedoText;

	GLuint pass1index, pass2index;						// IDs of the program pass subroutine calls

	// for the camera
	/*
	glm::vec3 position = glm::vec3(30, 0, 5);									// Initial position of camera : on +30x, 0y, +5z
	glm::vec3 direction;														// direction camera is facing
	glm::mat4 projection;														// (temporary) OpenGL projection matrix
	glm::mat4 view;																// (temporary) OpenGL view matrix
	glm::mat4 model;															// (temporary) OpenGL model matrix
	glm::mat4 MVP;
	float horizontalAngle = 3.14f;												// Initial horizontal angle : toward -Z
	float verticalAngle = 0.0f;													// Initial vertical angle : none
	float initialFoV = 45.0f;													// Initial Field of view
	float speed = 10.0f;															// 3 units / second
	float mouseSpeed = 0.005f;
	*/

	GLuint programID = 0;								// ID of the program to run
	GLuint testTriangleData = 0;						// handle for holding the buffer containing the test triangle data
	GLuint deferredFBO = 0;								// handle for the deferred FBO
	void setupAndRun();									// called once; sets everything up
	void setupGLWindow(int in_width, int in_height);	// sets up the GLFW window
	void createGBufText(GLenum texUnit, GLenum  format, GLuint &texid);
	void computeFromInputs();							// listen for input from each tick

	// pass-related functions/members
	GLuint mvpHandle;
	GLuint modelViewHandle;
	void setMatrices();
	void runPass1();
	void runPass2();

	// virtual functions (for later)
	void initialize(int in_windowWidth, int in_windowHeight);
	void initializeShader(int in_windowWidth, int in_windowHeight, int in_terrainBufferSize);
	void render();
	void multiDrawTerrain(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, int in_numberOfCollections);
	void shutdownGL();

	// independent, object specific values
	void setupRenderQuad();
	void setupFBO();
	void setupTestTriangle();
	void acquireSubroutineIndices();
	void setupImGui();
};

#endif
