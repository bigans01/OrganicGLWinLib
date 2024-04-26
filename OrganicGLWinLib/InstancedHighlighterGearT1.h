#pragma once

#ifndef INSTANCEDHIGHLIGHTERGEART1_H
#define INSTANCEDHIGHLIGHTERGEART1_h

#include "Gear.h"
#include "OrganicGLWinUtils.h"
#include "GLDrawElementsInstancedJob.h"

/*

Description: This shader has the ability to render an instance of an object; it is experimental. It uses MAt4 values to render individual 
instances of an object.

*/

class InstancedHighlighterGearT1 : public Gear
{
	public:
		// defined virtual functions (from MachineShader base class)
		void initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef, ShaderMachineBase* in_shaderMachineBasePtr);
		void render();
		void passGLuintValue(std::string in_identifier, GLuint in_gluInt);
		void executeGearFunction(std::string in_identifier);

		// unused, but required public virtual functions for Gear
		void printData() {};
		void interpretMessage(Message in_message) {};
		void sendTerrainDataToGear(TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef) {};
		void removeTerrainDataFromGear(EnclaveKeyDef::EnclaveKey in_keyToRemove) {};
		void sendMessageAndBufferDataToGear(Message in_messageToSend, int in_arraySize, GLfloat* in_arrayRef) {};
	private:
		void setupInstancingBuffersAndVAO();
		GLuint instancingVAO;
};

#endif
