#pragma once

#ifndef MSDEFERREDV1TERRAIN_H
#define MSDEFERREDV1TERRAIN_H

#include "Gear.h"
#include "OrganicGLWinUtils.h"

class TerrainComputeGearT1 : public Gear
{
public:

	// defined virtual functions (from MachineShader base class)
	void initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef, ShaderMachineBase* in_shaderMachineBasePtr);
	void render();
	void passGLuintValue(std::string in_identifier, GLuint in_gluInt);
	void executeGearFunction(std::string in_identifier);
	void printData();
private:

	//GLuint terrainBufferID = 0;				// the primary terrain buffer
	//GLuint terrainSwapID = 0;				// the swap terrain buffer;

	// shader uniforms 
	GLuint mvpHandle;
	GLuint textureUniform;
	GLuint worldPosUniform;
	GLuint atlasWidthUniform;
	GLuint atlasTileWidthUniform;

	// shader uniform values
	//glm::mat4 mv;

	// deferred specific functions
	GLuint deferredFBO = 0;
	GLuint terrainVaoID = 0;
	GLuint pass1index, pass2index;						// IDs of the program pass subroutine calls

	// terrain vao set up
	void setupTerrainVAO();

	// subRoutine index acquisition
	//void acquireSubroutineIndices();

	void writeToGBuffers();
	void setMatrices();
};

#endif
#pragma once
