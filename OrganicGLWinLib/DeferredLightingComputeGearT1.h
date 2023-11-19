#pragma once

#ifndef DEFERREDLIGHTINGCOMPUTEGEART1_H
#define DEFERREDLIGHTINGCOMPUTEGEART1_H

#include "Gear.h"

/*

Description: This experimental Gear uses a compute shader (that is very much still experimental) in an attempt to apply
light data to an area. The resulting light data is output to the "output" 2d image (see binding = 0 in the corresponding compute shader)

*/

class DeferredLightingComputeGearT1 : public Gear
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

	private:
		GLuint worldPosUniform;
		int currentTargetBinding = 1;		// always starts at 1.
		void swapImageTargetBindings();
		void resetSwapImage();

		int screenWidthUniform;
		int screenHeightUniform;

		int originalImageUnit1TextureID = 0;	// the original texture ID (before swapping occurs) of the first image unit, set by passGLuintValue
		int originalImageUnit2TextureID = 0;	// the original texture ID (before swapping occurs) of the first image unit, set by passGLuintValue
		int worldLightsSSBOBufferID = 0;		// the buffer ID of the WorldLights SSBO
};

#endif
