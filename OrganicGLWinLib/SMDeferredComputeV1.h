#pragma once

#ifndef SMDEFERREDCOMPUTEV1_H
#define SMDEFERREDCOMPUTEV1_H

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
#include "TerrainComputeGearT1.h"
#include "DeferredComputeGearT1.h"
#include "DeferredComputeResultsGearT1.h"
#include "HighlighterGearT1.h"
#include "InstancedHighlighterGearT1.h"
#include "ComputeResolution.h"

class SMDeferredComputeV1 : public ShaderMachineBase
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
	void insertWorldLight(std::string in_stringedContainerName, int in_lightID, WorldLight in_worldLight);
private:
	void insertTerrainGear(int in_gearID, GLuint in_programID);
	void insertHighlighterGear(int in_gearID, GLuint in_programID);
	void insertComputeGear(int in_gearID, GLuint in_programID);
	void insertComputeResultsGear(int in_gearID, GLuint in_programID);
	void insertInstancedHighlighterGear(int in_gearID, GLuint in_programID);

	void setupDeferredFBO();								// sets up the deferred FBO, according to this Machine's needs/specifications
	void createComputeImage(std::string in_imageName);		// creates a read/write OpenGL image

	void createGBufText(GLenum texUnit, GLenum  format, GLuint &texid);		// creates a G-Buffer texture for use by the deferred shader.
	void updateUniformRegistry();	// updates the uniform registry with all the appropriate values
};

#endif