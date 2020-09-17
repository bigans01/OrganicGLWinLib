#pragma once

#ifndef DEFERREDCOMPUTERESULTSGEART1_H
#define DEFERREDCOmPUTERESULTSGEART1_H

#include "Gear.h"
#include "OrganicGLWinUtils.h"

class DeferredComputeResultsGearT1 : public Gear
{
public:
	// defined virtual functions (from MachineShader base class)
	void initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef);
	void render();
	void passGLuintValue(std::string in_identifier, GLuint in_gluInt);
	void executeGearFunction(std::string in_identifier);
	void printData();

private:
	void setUpRenderQuad();
	void drawQuad();
	void setDrawMatrices();
	GLuint quadVaoID = 0;

	int screenWidthUniform;
	int screenHeightUniform;
};

#endif