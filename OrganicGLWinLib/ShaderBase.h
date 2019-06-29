#pragma once

#ifndef SHADERBASE_H
#define SHADERBASE_H

#include <GL/glew.h>
//#define GLFW_DLL		// only used when linking to a DLL version of GLFW.
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "AtlasMap.h"
#include "AtlasPropertiesGL.h"
#include "OrganicGLWinUtils.h"

class ShaderBase
{
	public:

		// misc
		GLFWwindow* window;									// pointer to openGL window
		int width, height;									// screen height, width

		// for the camera
		glm::vec3 position = glm::vec3(30, 0, 5);									// Initial position of camera : on +30x, 0y, +5z
		glm::vec3 direction;														// direction camera is facing
		glm::vec3 up;
		glm::mat4 projection;														// (temporary) OpenGL projection matrix
		glm::mat4 view;																// (temporary) OpenGL view matrix
		glm::mat4 model;															// (temporary) OpenGL model matrix
		glm::mat4 MVP;
		float horizontalAngle = 3.14f;												// Initial horizontal angle : toward -Z
		float verticalAngle = 0.0f;													// Initial vertical angle : none
		float initialFoV = 45.0f;													// Initial Field of view
		float speed = 10.0f;															// 3 units / second
		float mouseSpeed = 0.005f;

		// terrain and buffer parameters
		int OGLMVertexSubBufferSize = 0;		// the size of the terrain buffer, will be determined based on corresponding GL properties value
		GLuint shaderProgramID = 0;				// the ID of the shader program
		GLuint terrainBufferID = 0;				// the primary terrain buffer
		GLuint terrainSwapID = 0;				// the swap terrain buffer;
		GLuint organicGLVertexArrayID = 0;
		GLuint organicGLIndirectBufferID = 0;

		// misc handles
		GLuint mvpHandle = 0;

		// time
		static double lastTime;
		double currentTime;
		float deltaTime;

		// atlas
		GLuint textID;
		float atlasTileWidth = 0.0f;
		float atlasWidth = 0.0f;


		// virtual functions
		virtual void initialize(int in_windowWidth, int in_windowHeight) = 0;
		virtual void initializeShader(int in_windowWidth, int in_windowHeight, int in_terrainBufferSize) = 0;
		virtual void render() = 0;
		virtual void multiDrawTerrain(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, int in_numberOfCollections) = 0;	// for rendering terrain in each shader
		virtual void shutdownGL() = 0;

		// non-virtual functions
		GLuint* getTerrainBufferID();
		GLuint* getTerrainSwapID();
		void computeMatricesFromInputs();
		void updateMatricesAndDelta();
		GLFWwindow* getWindow();
		void setupTextureAtlas(AtlasMap* in_atlasMapRef, AtlasPropertiesGL* in_atlasPropertiesGLRef);
		int getBufferSize();
		glm::vec3* getPosition();
		glm::vec3* getDirection();
};

#endif
