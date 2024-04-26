#pragma once

#ifndef COMPUTECOPYRGBFROMTEXTURETOIMAGEGEART1_H
#define COMPUTECOPYRGBFROMTEXTURETOIMAGEGEART1_H

#include "Gear.h"
#include "OrganicGLWinUtils.h"

/*

Description: used in machines such as SMDeferredLightingComputeV1 and SMDeferredLightingComputeV2,
this gear reads from the color data texture used as part of the main FBO in those machines, 
using a full screen quad to put the data into the "read" texture.

Currently, GL_TEXTURE13 is the unit expected to contain the color data (see usage of layout(binding = 13) in the compute shader for reference)

Because the size of the screen dimensions is sent to the uniform, no explicit buffer data needs to be sent to this gear; it uses the shader code itself 
to handle it.

*/

class ComputeCopyRBGFromTextureToImageGearT1 : public Gear
{
	public:
		// defined virtual functions (from MachineShader base class)
		void initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef, ShaderMachineBase* in_shaderMachineBasePtr);
		void render();

		// unused, but required public virtual functions for Gear
		void passGLuintValue(std::string in_identifier, GLuint in_gluInt) {}
		void executeGearFunction(std::string in_identifier) {}
		void printData() {}
		void interpretMessage(Message in_message) {}
		void sendTerrainDataToGear(TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef) {};
		void removeTerrainDataFromGear(EnclaveKeyDef::EnclaveKey in_keyToRemove) {};
		void sendMessageAndBufferDataToGear(Message in_messageToSend, int in_arraySize, GLfloat* in_arrayRef) {};

	private:
		GLuint quadVaoID = 0;

		int screenWidthUniform;
		int screenHeightUniform;
};

#endif