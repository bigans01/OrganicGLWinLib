#pragma once

#ifndef SMPRIMITIVEV1_H
#define SMPRIMITIVEV1_H

#include "ShaderMachineBase.h"
#include "TerrainPrimitiveGearT1.h"
#include "HighlighterGearT1.h"

class SMPrimitiveV1 : public ShaderMachineBase
{
public:
	// virtual functions (defined, inherited)
	void initialize(int in_windowWidth, int in_windowHeight, int in_immutableBufferSize);
	//void setupTextureAtlas(AtlasMap* in_atlasMapRef, AtlasPropertiesGL* in_atlasPropertiesGLRef);	// STEP 2
	void setupTextureAtlases();	// STEP 2
	void runAllShadersAndSwap();
	void runAllShadersNoSwap();
	void shutdownGL();
	void multiDrawTerrain(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, int in_numberOfCollections);		// called by OrganicSystem
	void printDataForGears();
	void insertCollectionGLData(TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef);
	void removeUnusedReplaceables();
private:
	void insertTerrainGear(int in_gearID, GLuint in_programID);
	void insertHighlighterGear(int in_gearID, GLuint in_programID);

	void updateUniformRegistry();	// updates the uniform registry with all the appropriate values
};

#endif
