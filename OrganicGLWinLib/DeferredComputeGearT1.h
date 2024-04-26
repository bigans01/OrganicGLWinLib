#pragma once

#ifndef DEFERREDCOMPUTEGEART1_H
#define DEFERREDCOMPUTEGEART1_H

#include "Gear.h"

class DeferredComputeGearT1 : public Gear
{

public:
	// defined virtual functions (from MachineShader base class)
	void initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef, ShaderMachineBase* in_shaderMachineBasePtr);
	void render();
	void passGLuintValue(std::string in_identifier, GLuint in_gluInt);
	void executeGearFunction(std::string in_identifier);
	void printData();
	void interpretMessage(Message in_message);

	// unused, but required public virtual functions for Gear
	void sendTerrainDataToGear(TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef) {};
	void removeTerrainDataFromGear(EnclaveKeyDef::EnclaveKey in_keyToRemove) {};
	void sendMessageAndBufferDataToGear(Message in_messageToSend, int in_arraySize, GLfloat* in_arrayRef) {};

private:

	int screenWidthUniform;
	int screenHeightUniform;
};


#endif
