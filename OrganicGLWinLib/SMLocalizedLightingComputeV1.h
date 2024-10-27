#pragma once

#ifndef SMLOCALIZEDLIGHTINGCOMPUTEV1_H
#define SMLOCALIZEDLIGHTINGCOMPUTEV1_H

#include "ShaderMachineBase.h"
#include "Gear.h"
#include "OrganicGLWinUtils.h"

#include "LocalizedHighlighterGearT1.h"
#include "LocalizedGBufferComputeLoaderGear.h"
#include "ComputeCopyRBGFromTextureToImageGearT1.h"
#include "DeferredLightingComputeGearT1.h"
#include "DeferredComputeResultsGearT1.h"
#include "ComputeResolution.h"

/*

Description: This SM, is the first of it's kind, designed for localized translation of vertex data. "localized translation" refers 
to the act of translating both the camera position and the vertices position, by the inverse value of the camera position. This 
allows the buffered vertex data to be rendered as if it was close to the origin point of 0,0,0; this allows us to have far render-distances,
without worrying about the strange artifacts that begin to appear when storing large float values above 1000 or below -1000 in the GPU buffers,
which is likely due to the result of float precision loss.

This SM will go under development for several months, as new Gear classes designed for localized translation are added. 

As of 4/18/2024, the first big test of the localized translation methodology was complete -- the LocalizedHighlighterGearT1 
appears to correctly translate basic highlight data.

||||||||||||| Gear order:

	0:	 LocalizedGBufferComputeLoaderGear:
	1:   ComputeCopyRBGFromTextureToImageGearT1
	2:	 DeferredLightingComputeGearT1
	3:   DeferredComputeResultsGearT1
	4:   InstancedHighlighterGearT1

||||||||||||| Notable uniforms:

	I.	Uniforms shared across drawable blueprints, but updated every frame:

	MVP
	screenWidth
	screenHeight
	globalAmbienceMultiplier
	worldPosition

	II.	Uniforms shared across drawable blueprints, but that should remain static

	atlasTextureWidth
	atlasTileTextureWidth

*/

class SMLocalizedLightingComputeV1 : public ShaderMachineBase
{
	public:
		// used virtual functions (defined, inherited)
		void initialize(int in_windowWidth, int in_windowHeight, int in_immutableBufferSize);
		void setupTextureAtlases();	// sets up the texture atlases, and inserts the related uniforms that have static values that won't change 
									// (i.e., atlasTextureWidth and atlasTileTextureWidth.
		void runAllShadersNoSwap();		// should be the only call for running shaders used for this ShaderMachine; i.e, glfwSwapBuffers should be called by other code,
										// such as in the case of OrganicGLManager::renderReadyArrays().
		void printDataForGears();
		void insertCollectionGLData(TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef);	// will insert blueprint data into the LocalizedGBufferComputeLoaderGear,
																												// which should be the first gear, at index 0.

		void flagCollectionGLDataForRemoval(EnclaveKeyDef::EnclaveKey in_keyForRemoval);	// will remove blueprint data from the LocalizedGBufferComputeLoaderGear.

		// unused, but required public virtual functions for ShaderMachineBase
		void insertCollectionGLDataIntoGearBuffer(std::string in_gearName, std::string in_gearBufferName, TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef) {};
		void shutdownGL() {}	// needs to be revisited
		void removeUnusedReplaceables() {}	// not used
		void insertWorldLight(std::string in_stringedContainerName, int in_lightID, WorldLight in_worldLight) {}
		void multiDrawTerrain(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, int in_numberOfCollections) {}	// unused for this ShaderMachine; terrain data is called through render()
		void runAllShadersAndSwap() {}	// this ShaderMachine is not responsible for calling glfwSwapBuffers; this must be handled outside of an instance of this ShaderMachine.

	private:
		void insertTerrainGear(int in_gearID, GLuint in_programID);

		void insertComputeTransferGear(int in_gearID, GLuint in_programID);
		void insertComputeGear(int in_gearID, GLuint in_programID);
		void insertComputeResultsGear(int in_gearID, GLuint in_programID);
		void insertLocalizedHighlighterGear(int in_gearID, GLuint in_programID);

		void setupDeferredFBO();	// sets up the deferred FBO, according to this Machine's needs/specifications;
									// current intent as of 10/13/2024 is to set it up similiar to that of SMDeferredLightingComputeV2.
		void createGBufText(GLenum texUnit, GLenum  format, GLuint &texid);		// creates a G-Buffer texture for use by the deferred shader.
		void createComputeImage(GLenum texUnit, std::string in_imageName, int in_imageUnit);		// creates a read/write OpenGL image


		void updateUniformRegistry();

		float globalAmbienceMultiplier = 1.0f; // global ambience multiplier; used in DeferredLightingComputeLightContributionsT1
};

#endif
