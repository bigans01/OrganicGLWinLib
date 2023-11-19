#pragma once

#ifndef SMFORWARD_V1
#define SMFORWARD_V1

#include "ShaderMachineBase.h"
#include "TerrainSimpleGearT1.h"
#include "HighlighterGearT1.h"

/*

Description: this shader uses theTerrainMemoryTracker to attempt to render tiled terrain data, but does not offer UV coordinates.
So expect to see tile bleeding (i.e, white or other colored area around some textured triangles, when viewed from particular angles.)

*/

class SMForwardV1 : public ShaderMachineBase
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

		// unused, but required public virtual functions for ShaderMachine
		void insertWorldLight(std::string in_stringedContainerName, int in_lightID, WorldLight in_worldLight) {};
		void shutdownGL() {};

	private:
		void insertTerrainGear(int in_gearID, GLuint in_programID);
		void insertHighlighterGear(int in_gearID, GLuint in_programID);
		void updateUniformRegistry();	// updates the uniform registry with all the appropriate values

		// unused, but required private virtual functions for ShaderMachine
		void insertCollectionGLDataIntoGearBuffer(std::string in_gearName, std::string in_gearBufferName, TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef) {}
};

#endif