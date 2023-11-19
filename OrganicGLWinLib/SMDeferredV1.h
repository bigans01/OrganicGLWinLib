#pragma once

#ifndef SMDEFERREDV1_H
#define SMDEFERREDV1_H

#include <GL/glew.h>
//#define GLFW_DLL		// only used when linking to a DLL version of GLFW.
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "ShaderMachineBase.h"
#include "Gear.h"
#include "OrganicGLWinUtils.h"
#include "TerrainGearT1.h"
#include "HighlighterGearT1.h"

/*

Description: Also known as the "Mode 4" shader, this shader works on the laptop, but on the main PC, since switching to a Nvidia 4070.
When it does work, this shader uses deferred rendering to write the rendering data to Gbuffers.

This can be refactored/cleaned when it's been resolved (see the bug tracker, B-007 for tis related issue)

*/

class SMDeferredV1 : public ShaderMachineBase
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

		// unused, but required public virtual functions for ShaderMachineBase
		void shutdownGL() {};
		void insertWorldLight(std::string in_stringedContainerName, int in_lightID, WorldLight in_worldLight) {};
	private:
		void insertTerrainGear(int in_gearID, GLuint in_programID);
		void insertHighlighterGear(int in_gearID, GLuint in_programID);
		void setupDeferredFBO();		// sets up the deferred FBO, according to this Machine's needs/specifications
		void createGBufText(GLenum texUnit, GLenum  format, GLuint &texid);
		void updateUniformRegistry();	// updates the uniform registry with all the appropriate values

		// unused, but required private virtual functions for ShaderMachineBase
		void insertCollectionGLDataIntoGearBuffer(std::string in_gearName, std::string in_gearBufferName, TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef) {};
};

#endif
