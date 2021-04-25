#pragma once

#ifndef TERRAINLIGHTINGCOMPUTEGEAR_T1
#define TERRAINLIGHTINGCOMPUTEGEAR_T1

#include "Gear.h"
#include "OrganicGLWinUtils.h"

class TerrainLightingComputeGearT1 : public Gear
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

	//GLuint terrainBufferID = 0;				// the primary terrain buffer
	//GLuint terrainSwapID = 0;				// the swap terrain buffer;

	// shader uniforms 
	GLuint mvpHandle;
	GLuint textureUniform;
	GLuint worldPosUniform;
	GLuint atlasWidthUniform;
	GLuint atlasTileWidthUniform;
	GLuint globalAmbienceMultiplierUniform;

	// shader uniform values
	//glm::mat4 mv;

	// deferred specific functions
	GLuint deferredFBO = 0;
	GLuint terrainVaoID = 0;

	// terrain vao set up
	void setupTerrainVAO();

	void writeToGBuffers();
	void setMatrices();
};

#endif