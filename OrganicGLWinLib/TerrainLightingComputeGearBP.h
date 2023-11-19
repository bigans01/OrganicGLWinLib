#pragma once

#ifndef TERRAINLIGHTINGCOMPUTEGEARBP
#define TERRAINLIGHTINGCOMPUTEGEARBP

#include "Gear.h"
#include "OrganicGLWinUtils.h"
#include "TerrainJobResults.h"

/*

Description: this gear renders in a similiar way to the TerrainLightComputeGearT1; however, the notable exception
is that this gear manages its own OpenGL buffers for rendering, in an eventual attempt to localize all the OpenGL data 
around the world camera being 0.0f.

OpenGL buffer data is sent to this gear through calls to the sendTerrainToGear and removeTerrainDataFromGear required virtual functions.
All elements in the renderableBlueprints map are then rendered when render() is called.

*/

class TerrainLightingComputeGearBP : public Gear
{
	public:

		// defined virtual functions (from MachineShader base class)
		void initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef, ShaderMachineBase* in_shaderMachineBasePtr);
		void render();
		void passGLuintValue(std::string in_identifier, GLuint in_gluInt);
		void executeGearFunction(std::string in_identifier);
		void printData();
		void sendTerrainDataToGear(TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef);
		void removeTerrainDataFromGear(EnclaveKeyDef::EnclaveKey in_keyToRemove);

		// unused, but required public virtual functions for Gear
		void interpretMessage(Message in_message) {}

	private:

		// shader uniforms 
		GLuint mvpHandle;
		GLuint textureUniform;
		GLuint worldPosUniform;
		GLuint atlasWidthUniform;
		GLuint atlasTileWidthUniform;
		GLuint globalAmbienceMultiplierUniform;

		// deferred specific functions
		GLuint deferredFBO = 0;
		GLuint terrainVaoID = 0;

		// terrain vao set up
		void setupTerrainVAO();
		void setMatrices();

		struct BPGearBuffer
		{
			BPGearBuffer() {};

			void generateBuffer();
			void insertBufferData(TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef);
			void drawBuffer();
			void deleteBuffer();

			bool bufferGenerated = false;
			GLuint blueprintBufferID = 0;
			GLuint blueprintVAOID = 0;

			int numberOfVertices = 0;	// the number of vertices for the draw call; should be equal to number of numberOfTrianglesProduced * 3.
		};

		std::unordered_map<EnclaveKeyDef::EnclaveKey, BPGearBuffer, EnclaveKeyDef::KeyHasher> renderableBlueprints;

		// blueprint management functions
		void insertBlueprintData(TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef);

		void deleteBlueprintData(EnclaveKeyDef::EnclaveKey in_keyToRemove);

		// custom rendering
		void writeToGBuffersV2();	// for 1 draw call per blueprint style.

};

#endif