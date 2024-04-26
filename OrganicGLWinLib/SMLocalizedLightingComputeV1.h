#pragma once

#ifndef SMLOCALIZEDLIGHTINGCOMPUTEV1_H
#define SMLOCALIZEDLIGHTINGCOMPUTEV1_H

#include "ShaderMachineBase.h"
#include "Gear.h"
#include "OrganicGLWinUtils.h"

#include "LocalizedHighlighterGearT1.h"

/*

Description: This SM, is the first of it's kind, designed for localized translation of vertex data. "localized translation" refers 
to the act of translating both the camera position and the vertices position, by the inverse value of the camera position. This 
allows the buffered vertex data to be rendered as if it was close to the origin point of 0,0,0; this allows us to have far render-distances,
without worrying about the strange artifacts that begin to appear when storing large float values above 1000 or below -1000 up close.

This SM will go under development for several months, as new Gear classes designed for localized translation are added. 

As of 4/18/2024, the first big test of the localized translation methodology was complete -- the LocalizedHighlighterGearT1 
appears to correctly translate basic highlight data.

*/

class SMLocalizedLightingComputeV1 : public ShaderMachineBase
{
	public:
		// used virtual functions (defined, inherited)
		void initialize(int in_windowWidth, int in_windowHeight, int in_immutableBufferSize);
		void setupTextureAtlases();	// STEP 2
		void runAllShadersAndSwap();
		void runAllShadersNoSwap();
		void multiDrawTerrain(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, int in_numberOfCollections);		// called by OrganicSystem
		void printDataForGears();
		void insertCollectionGLData(TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef);
		void flagCollectionGLDataForRemoval(EnclaveKeyDef::EnclaveKey in_keyForRemoval);

		// unused, but required public virtual functions for ShaderMachineBase
		void insertCollectionGLDataIntoGearBuffer(std::string in_gearName, std::string in_gearBufferName, TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef) {}
		void shutdownGL() {}
		void removeUnusedReplaceables() {}
		void insertWorldLight(std::string in_stringedContainerName, int in_lightID, WorldLight in_worldLight) {}

	private:
		void insertLocalizedHighlighterGear(int in_gearID, GLuint in_programID);
		void updateUniformRegistry();
};

#endif
