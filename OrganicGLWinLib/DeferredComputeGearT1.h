#pragma once

#ifndef DEFERREDCOMPUTEGEART1_H
#define DEFERREDCOMPUTEGEART1_H

#include "Gear.h"

class DeferredComputeGearT1 : public Gear
{

public:
	// defined virtual functions (from MachineShader base class)
	void initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef);
	void render();
	void passGLuintValue(std::string in_identifier, GLuint in_gluInt);
	void executeGearFunction(std::string in_identifier);
	void printData();

private:
};


#endif
