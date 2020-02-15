#pragma once

#ifndef SMDEBUGV1_H
#define SMDEBUGV1_H

#include "ShaderMachineBase.h"
#include "TerrainDebugGearT1.h"
#include "HighlighterGearT1.h"

class SMDebugV1 : public ShaderMachineBase
{
public:
	// virtual functions (defined, inherited)
	void initialize(int in_windowWidth, int in_windowHeight, int in_immutableBufferSize);
	void setupTextureAtlas(AtlasMap* in_atlasMapRef, AtlasPropertiesGL* in_atlasPropertiesGLRef);	// STEP 2
	void runAllShadersAndSwap();
	void runAllShadersNoSwap();
	void shutdownGL();
	void multiDrawTerrain(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, int in_numberOfCollections);		// called by OrganicSystem
	void printDataForGears();
private:
	void insertTerrainGear(int in_gearID, GLuint in_programID);
	void insertHighlighterGear(int in_gearID, GLuint in_programID);

	void updateUniformRegistry();	// updates the uniform registry with all the appropriate values
};

#endif