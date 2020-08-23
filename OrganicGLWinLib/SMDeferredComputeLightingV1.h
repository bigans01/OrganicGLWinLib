#pragma once

#ifndef SMDEFERREDCOMPUTELIGHTINGV1_H
#define SMDEFERREDCOMPUTELIGHTINGV1_H

#include <GL/glew.h>
//#define GLFW_DLL		// only used when linking to a DLL version of GLFW.
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ShaderMachineBase.h"
#include "Gear.h"
#include "OrganicGLWinUtils.h"
#include <map>
#include <unordered_map>
#include <mutex>

class SMDeferredComputeLightingV1 : public ShaderMachineBase
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

};

#endif


