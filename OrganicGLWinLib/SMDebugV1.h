#pragma once

#ifndef SMDEBUGV1_H
#define SMDEBUGV1_H

#include "ShaderMachineBase.h"
#include "TerrainDebugGearT1.h"
#include "HighlighterGearT1.h"

/*

Description: this shader uses the terrain data stored in the TerrainMemoryTracker to render bright yellow-ish triangles for all terrain triangles.
It doesn't use textured triangles, and is mainly used to occasionally see the various size of triangles that are rendered at various LODs.

*/

class SMDebugV1 : public ShaderMachineBase
{
	public:
		// virtual functions (defined, inherited)
		void initialize(int in_windowWidth, int in_windowHeight, int in_immutableBufferSize);
		void setupTextureAtlases();
		void runAllShadersAndSwap();
		void runAllShadersNoSwap();
		void multiDrawTerrain(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, int in_numberOfCollections);		// called by OrganicSystem
		void printDataForGears();
		void insertCollectionGLData(TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef);
		void removeUnusedReplaceables();
		void flagCollectionGLDataForRemoval(EnclaveKeyDef::EnclaveKey in_keyForRemoval);

		// unused, but required public virtual functions for Gear
		void insertWorldLight(std::string in_stringedContainerName, int in_lightID, WorldLight in_worldLight) {};
		void shutdownGL() {};

	private:
		void insertTerrainGear(int in_gearID, GLuint in_programID);
		void insertHighlighterGear(int in_gearID, GLuint in_programID);
		void updateUniformRegistry();	// updates the uniform registry with all the appropriate values

		// unused, but required public virtual functions for Gear
		void insertCollectionGLDataIntoGearBuffer(std::string in_gearName, std::string in_gearBufferName, TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef) {};

};

#endif