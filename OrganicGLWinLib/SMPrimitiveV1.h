#pragma once

#ifndef SMPRIMITIVEV1_H
#define SMPRIMITIVEV1_H

#include "ShaderMachineBase.h"
#include "TerrainPrimitiveGearT1.h"
#include "HighlighterGearT1.h"

/*

Description: the simplest shader ever; does not support any special highlgihting, and simply renders all terrain as red.

The TerrainMemoryTracker will still manage memory, however.

*/

class SMPrimitiveV1 : public ShaderMachineBase
{
	public:
		// virtual functions (defined, inherited)
		void initialize(int in_windowWidth, int in_windowHeight, int in_immutableBufferSize);
		void setupTextureAtlases();	// STEP 2
		void runAllShadersAndSwap();
		void runAllShadersNoSwap();
		void multiDrawTerrain(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, int in_numberOfCollections);		// called by OrganicSystem
		void printDataForGears();
		void insertCollectionGLData(TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef);
		void removeUnusedReplaceables();
		void flagCollectionGLDataForRemoval(EnclaveKeyDef::EnclaveKey in_keyForRemoval);

		// unused, but required private virtual functions for ShaderMachineBase
		void insertWorldLight(std::string in_stringedContainerName, int in_lightID, WorldLight in_worldLight) {};
		void shutdownGL() {};


	private:
		void insertTerrainGear(int in_gearID, GLuint in_programID);
		void insertHighlighterGear(int in_gearID, GLuint in_programID);
		void updateUniformRegistry();	// updates the uniform registry with all the appropriate values

		// unused, but required private virtual functions for ShaderMachineBase
		void insertCollectionGLDataIntoGearBuffer(std::string in_gearName, std::string in_gearBufferName, TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef) {};
};

#endif
