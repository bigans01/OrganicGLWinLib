#pragma once

#ifndef SMDEFERREDV2_H
#define SMDEFERREDV2_H

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

Description: An alternative to the "mode 4" shader, SMDeferredV1; structured in a similiar manner, but deprecated and unused.

This can be refactored/cleaned when it's been resolved (see the bug tracker, B-007 for tis related issue)

*/

class SMDeferredV2 : public ShaderMachineBase
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
		void insertWorldLight(std::string in_stringedContainerName, int in_lightID, WorldLight in_worldLight) {};
		void shutdownGL() {};

	private:
		void insertTerrainGear(int in_gearID, GLuint in_programID);
		void insertHighlighterGear(int in_gearID, GLuint in_programID);
		void setupDeferredFBO();		// sets up the deferred FBO, according to this Machine's needs/specifications
		void createGBufText(GLenum texUnit, GLenum  format, GLuint &texid);
		void updateUniformRegistry();	// updates the uniform registry with all the appropriate values

		// unused, but required private virtual functions for ShaderMachineBase
		void insertCollectionGLDataIntoGearBuffer(std::string in_gearName, std::string in_gearBufferName, TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef) {}

};

#endif
