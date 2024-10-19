#pragma once

#ifndef SMDeferredLightingComputeV2_H
#define SMDeferredLightingComputeV2_H

#include <GL/glew.h>
//#define GLFW_DLL		// only used when linking to a DLL version of GLFW.
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "ShaderMachineBase.h"
#include "Gear.h"
#include "OrganicGLWinUtils.h"

// gears used by this machine; doesn't represent actual order of usage
#include "TerrainLightingComputeGearT1.h"
#include "TerrainLightingComputeGearBP.h"
#include "DeferredLightingComputeGearT1.h"
#include "DeferredComputeResultsGearT1.h"
#include "HighlighterGearT1.h"
#include "InstancedHighlighterGearT1.h"
#include "WaveHighlighterGearT1.h"
#include "ComputeCopyRBGFromTextureToImageGearT1.h"

#include "ComputeResolution.h"

/*

Description: This machine is similiar to SMDeferredLightingComputeV1, with the exception that the machine itself
doesn't allocate the main/swap buffers that are normally used for terrain rendering; instead, the TerrainLightingComputeGearBP 
manages the terrain data itself. 

As a result of this, the terrainMemoryTracker doesn't manage any terrain data or memory.

*/

class SMDeferredLightingComputeV2 : public ShaderMachineBase
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
		void shutdownGL() {}
		void removeUnusedReplaceables() {}
		void insertWorldLight(std::string in_stringedContainerName, int in_lightID, WorldLight in_worldLight) {}

	private:
		void insertTerrainGear(int in_gearID, GLuint in_programID);
		void insertComputeTransferGear(int in_gearID, GLuint in_programID);
		void insertComputeGear(int in_gearID, GLuint in_programID);
		void insertComputeResultsGear(int in_gearID, GLuint in_programID);
		void insertHighlighterGear(int in_gearID, GLuint in_programID);
		void insertInstancedHighlighterGear(int in_gearID, GLuint in_programID);
		void insertWaveHighlighterGear(int in_gearID, GLuint in_programID);

		void insertCollectionGLDataIntoGearBuffer(std::string in_gearName, std::string in_gearBufferName, TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef);

		void setupDeferredFBO();								// sets up the deferred FBO, according to this Machine's needs/specifications
		void createGBufText(GLenum texUnit, GLenum  format, GLuint &texid);		// creates a G-Buffer texture for use by the deferred shader.
		void createComputeImage(GLenum texUnit, std::string in_imageName, int in_imageUnit);		// creates a read/write OpenGL image
		void updateUniformRegistry();



		float globalAmbienceMultiplier = 1.0f; // global ambience multiplier; used in DeferredLightingComputeLightContributionsT1

};

#endif


