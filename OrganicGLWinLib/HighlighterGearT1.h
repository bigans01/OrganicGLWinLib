#pragma once

#ifndef HIGHLIGHTERGEART1_H
#define HIGHLIGHTERGEART1_H

#include "Gear.h"
#include "OrganicGLWinUtils.h"
#include "GLMultiDrawArrayJob.h"

/*

Description: this gear is designed to draw highlights that don't fade in/out, and remain solid; 
examples include the block highlight cursor, and the current blueprint borders that show up 
as the camera moves around in the world. 

The OrganicGLManager class in OrganicCoreLib puts all the data for highlighting, into its "highlights" member,
which puts all the highlighting data into a single buffer and then sends it to the shader.

*/

class HighlighterGearT1 : public Gear
{
	public:
		// defined virtual functions (from MachineShader base class)
		void initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef, ShaderMachineBase* in_shaderMachineBasePtr);
		void render();
		void passGLuintValue(std::string in_identifier, GLuint in_gluInt);
		void executeGearFunction(std::string in_identifier);

		// unused, but required public virtual functions for Gear
		void printData() {};
		void interpretMessage(Message in_message) {};
		void sendTerrainDataToGear(TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef) {};
		void removeTerrainDataFromGear(EnclaveKeyDef::EnclaveKey in_keyToRemove) {};

	private:

		void setupHighlighterVAO();
		GLuint highlighterVAO;
};

#endif
