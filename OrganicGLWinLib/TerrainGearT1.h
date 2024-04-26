#pragma once

#ifndef MSDEFERREDV1TERRAIN_H
#define MSDEFERREDV1TERRAIN_H

#include "Gear.h"
#include "OrganicGLWinUtils.h"

/*

Description: This Gear class is designed to send data to the FBO in a deferred manner; it doesn't do any lighting.
This Gear doesn't appear to work correctly on an Nvidia 4070; but appears to work on a laptop.

*/

class TerrainGearT1 : public Gear
{
	public:

		// defined virtual functions (from MachineShader base class)
		void initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef, ShaderMachineBase* in_shaderMachineBasePtr);
		void render();
		void passGLuintValue(std::string in_identifier, GLuint in_gluInt);
		void executeGearFunction(std::string in_identifier);
		void printData();

		// unused, but required public virtual functions for Gear
		void sendTerrainDataToGear(TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef) {};
		void removeTerrainDataFromGear(EnclaveKeyDef::EnclaveKey in_keyToRemove) {};
		void interpretMessage(Message in_message) {};
		void sendMessageAndBufferDataToGear(Message in_messageToSend, int in_arraySize, GLfloat* in_arrayRef) {};
	private:

		// shader uniforms 
		GLuint mvpHandle;
		GLuint textureUniform;
		GLuint worldPosUniform;
		GLuint atlasWidthUniform;
		GLuint atlasTileWidthUniform;

		// deferred specific functions
		GLuint deferredFBO = 0;

		//GLuint quadBufferID = 0;							// quad vertex data object ID
		GLuint quadVaoID = 0;
		GLuint terrainVaoID = 0;
		GLuint pass1index, pass2index;						// IDs of the program pass subroutine calls

		// render quad set up
		void setUpRenderQuad();

		// terrain vao set up
		void setupTerrainVAO();

		// subRoutine index acquisition
		void acquireSubroutineIndices();

		void runPass1();
		void runPass2();
		void setPass1Matrices();
		void setPass2Matrices();
};

#endif
