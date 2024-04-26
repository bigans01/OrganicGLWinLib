#pragma once

#ifndef TERRAINFORWARDGEART1_H
#define TERRAINFORWARDGEART1_H

#include "Gear.h"
#include "OrganicGLWinUtils.h"

/*

Description: renders terrain texture data from TerrainMemoryTracker, but does UV correction before it hits the FBO.

Pretty much only used by the SMForwardV2 ShaderMachine.

*/

class TerrainForwardGearT1 : public Gear
{
	public:
		// defined virtual functions (from MachineShader base class)
		void initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef, ShaderMachineBase* in_shaderMachineBasePtr);
		void render();
		void passGLuintValue(std::string in_identifier, GLuint in_gluInt);
		void executeGearFunction(std::string in_identifier);
		void printData();

		// unused, but required public virtual functions for Gear
		void interpretMessage(Message in_message) {};
		void sendTerrainDataToGear(TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef) {};
		void removeTerrainDataFromGear(EnclaveKeyDef::EnclaveKey in_keyToRemove) {};
		void sendMessageAndBufferDataToGear(Message in_messageToSend, int in_arraySize, GLfloat* in_arrayRef) {};
	private:
		// shader uniforms 
		GLuint mvpHandle;
		GLuint textureUniform;
		GLuint worldPosUniform;
		GLuint atlasWidthUniform;
		GLuint atlasTileWidthUniform;

		// VAO 
		GLuint terrainVaoID = 0;

		// terrain VAO setup
		void setupTerrainVAO();

		// render terrain
		void renderTerrain();

		// set matrices
		void setMatrices();
};

#endif
