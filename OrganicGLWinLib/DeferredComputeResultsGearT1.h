#pragma once

#ifndef DEFERREDCOMPUTERESULTSGEART1_H
#define DEFERREDCOmPUTERESULTSGEART1_H

#include "Gear.h"
#include "OrganicGLWinUtils.h"

/*

Description: this Gear is designed to load color data from an image that contains data from compute shaders,
into the main frame buffer.

It simply draws a quad across the screen, in order to sample each pixel that is being represented in the buffer;
the MVP uniform is just a simple 1.0 mat4 matrix.

*/

class DeferredComputeResultsGearT1 : public Gear
{
public:
	// defined virtual functions (from MachineShader base class)
	void initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef, ShaderMachineBase* in_shaderMachineBasePtr);
	void render();
	void passGLuintValue(std::string in_identifier, GLuint in_gluInt);

	// unused, but required public virtual functions for Gear
	void sendTerrainDataToGear(TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef) {};
	void removeTerrainDataFromGear(EnclaveKeyDef::EnclaveKey in_keyToRemove) {};
	void executeGearFunction(std::string in_identifier) {};
	void printData() {};
	void interpretMessage(Message in_message) {};
	void sendMessageAndBufferDataToGear(Message in_messageToSend, int in_arraySize, GLfloat* in_arrayRef) {};

private:
	void setUpRenderQuad();
	void drawQuad();
	void setDrawMatrices();	// simply sets the MVP uniform as a glm::mat4(1.0), for rendering the screen quad for sampling.
	GLuint quadVaoID = 0;

	int screenWidthUniform;
	int screenHeightUniform;
};

#endif