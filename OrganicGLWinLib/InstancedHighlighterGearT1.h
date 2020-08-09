#pragma once

#ifndef INSTANCEDHIGHLIGHTERGEART1_H
#define INSTANCEDHIGHLIGHTERGEART1_h

#include "Gear.h"
#include "OrganicGLWinUtils.h"
#include "GLDrawElementsInstancedJob.h"

class InstancedHighlighterGearT1 : public Gear
{
	public:
		// defined virtual functions (from MachineShader base class)
		void initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef);
		void render();
		void passGLuintValue(std::string in_identifier, GLuint in_gluInt);
		void executeGearFunction(std::string in_identifier);
		void printData();
	private:
		void setupInstancingBuffersAndVAO();
		GLuint instancingVAO;
};

#endif
