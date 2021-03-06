#pragma once

#ifndef DEFERREDLIGHTINGCOMPUTEGEART1_H
#define DEFERREDLIGHTINGCOMPUTEGEART1_H

#include "Gear.h"

class DeferredLightingComputeGearT1 : public Gear
{

public:
	// defined virtual functions (from MachineShader base class)
	void initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef, ShaderMachineBase* in_shaderMachineBasePtr);
	void render();
	void passGLuintValue(std::string in_identifier, GLuint in_gluInt);
	void executeGearFunction(std::string in_identifier);
	void printData();
	void interpretMessage(Message in_message);

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
