#pragma once

#ifndef TERRAINFORWARDGEART1_H
#define TERRAINFORWARDGEART1_H

#include "Gear.h"
#include "OrganicGLWinUtils.h"

class TerrainForwardGearT1 : public Gear
{
public:
	// defined virtual functions (from MachineShader base class)
	void initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef);
	void render();
	void passGLuintValue(std::string in_identifier, GLuint in_gluInt);
	void executeGearFunction(std::string in_identifier);
	void printData();
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
