#pragma once

#ifndef SMDEFERREDCOMPUTEV1_H
#define SMDEFERREDCOMPUTEV1_H

#include <GL/glew.h>
//#define GLFW_DLL		// only used when linking to a DLL version of GLFW.
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "ShaderMachineBase.h"
#include "Gear.h"
#include "OrganicGLWinUtils.h"
#include "TerrainComputeGearT1.h"
#include "DeferredComputeGearT1.h"
#include "DeferredComputeResultsGearT1.h"
#include "HighlighterGearT1.h"
#include "InstancedHighlighterGearT1.h"
#include "ComputeResolution.h"
#include "WaveHighlighterGearT1.h"

/*

Description: this shader was meant as an experiment to see how loading deferred draw data, into an image, and then FBO would work.
Nothing special, just an experimental shader for basic compute shader observations and behavior.

Does provide wave/instanced/highlighter gears.

*/

class SMDeferredComputeV1 : public ShaderMachineBase
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
		void insertComputeGear(int in_gearID, GLuint in_programID);
		void insertWaveHighlighterGear(int in_gearID, GLuint in_programID);
		void insertComputeResultsGear(int in_gearID, GLuint in_programID);
		void insertInstancedHighlighterGear(int in_gearID, GLuint in_programID);
		void setupDeferredFBO();								// sets up the deferred FBO, according to this Machine's needs/specifications
		void createComputeImage(std::string in_imageName);		// creates a read/write OpenGL image
		void createGBufText(GLenum texUnit, GLenum  format, GLuint &texid);		// creates a G-Buffer texture for use by the deferred shader.
		void updateUniformRegistry();	// updates the uniform registry with all the appropriate values

		// unused, but required private virtual functions for ShaderMachineBase
		void insertCollectionGLDataIntoGearBuffer(std::string in_gearName, std::string in_gearBufferName, TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef) {};


		float globalAmbienceMultiplier = 1.0f; // global ambience multiplier; used in DeferredLightingComputeLightContributionsT1
};

#endif