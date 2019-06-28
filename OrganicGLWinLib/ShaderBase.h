#pragma once

#ifndef SHADERBASE_H
#define SHADERBASE_H

#include <GL/glew.h>
//#define GLFW_DLL		// only used when linking to a DLL version of GLFW.
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class ShaderBase
{
	public:
		// for the camera
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

		// virtual functions
		virtual void initialize(int in_windowWidth, int in_windowHeight) = 0;
		virtual void render() = 0;
};

#endif
