#pragma once

#ifndef TERRAINSIMPLEGEART1_H
#define TERRAINSIMPLEGEART1_H

#include "Gear.h"
#include "OrganicGLWinUtils.h"

class TerrainSimpleGearT1 : public Gear
{
public:
	// defined virtual functions (from MachineShader base class)
	void initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef, ShaderMachineBase* in_shaderMachineBasePtr);
	void render();
	void passGLuintValue(std::string in_identifier, GLuint in_gluInt);
	void executeGearFunction(std::string in_identifier);
	void printData();

private:

	// shader uniforms 
	GLuint mvpHandle;

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
