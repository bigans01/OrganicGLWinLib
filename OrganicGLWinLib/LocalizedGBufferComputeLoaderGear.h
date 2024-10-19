#pragma once

#ifndef LOCALIZEDGBUFFERCOMPUTELOADERGEAR_H
#define LOCALIZEDGBUFFERCOMPUTELOADERGEAR_H

#include "Gear.h"
#include "OrganicGLWinUtils.h"
#include "TerrainJobResults.h"

/*

Description: Similiar in fashion to TerrainLightingComputeGearBP, but designed for ShaderMachines that will render GPU coordinates in a camera-oriented
(localized) mode.

*/

class LocalizedGBufferComputeLoaderGear : public Gear
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
		void interpretMessage(Message in_message) {};
		void sendMessageAndBufferDataToGear(Message in_messageToSend, int in_arraySize, GLfloat* in_arrayRef) {};
	private:
		// Below: this struct contains necessary data to render localized blueprint graphics, when the time comes to render it;
		// data is stored in a fashion similiar to LocalizedHighlighterGearT1::LBPHighlightBufferData.
		struct LocalizedGBufferBPData
		{
			LocalizedGBufferBPData() {};
			LocalizedGBufferBPData(EnclaveKeyDef::EnclaveKey in_keyForLocalization,
								   GLuint in_mvpHandle,
								   GLuint in_textureUniform,
								   GLuint in_worldPosUniform,
								   GLuint in_atlasWidthUniform,
								   GLuint in_atlasTileWidthUniform,
								   GLuint in_globalAmbienceMultiplierUniform) :
				keyForLocalization(in_keyForLocalization),
				mvpHandle(in_mvpHandle),
				textureUniform(in_textureUniform),
				worldPosUniform(in_worldPosUniform),
				atlasWidthUniform(in_atlasWidthUniform),
				atlasTileWidthUniform(in_atlasTileWidthUniform),
				globalAmbienceMultiplierUniform(in_globalAmbienceMultiplierUniform)
			{};

			void generateBlueprintBuffer();	// generate the buffer that we will load data into; must be called before loading data into the buffer.
			void insertBlueprintDataIntoBuffer(int in_totalTriangles, int in_arraySize, GLfloat* in_arrayRef); // insert data into buffer.

			// draw the buffer; the parameters passed are for the camera uniformns in the shader.
			void drawBlueprintData(GLuint in_programID, 
								GLUniformRegistry* in_registryPtr,
								EnclaveKeyDef::EnclaveKey in_cameraBPKey, 
								glm::vec3 in_cameraBPLocalCoord, 
								glm::mat4 in_MVPvalue);

			void deleteBlueprintBufferAndData();	// cleanup the buffer and VAO

			EnclaveKeyDef::EnclaveKey keyForLocalization;	// represents the key of the blueprint to be rendered; 
															// required for uniform variable
			int numberOfLocalizedVertices = 0;
			GLuint blueprintBufferID = 0;
			GLuint blueprintVAOID = 0;

			// various other uniforms for textures, MVP, etc (typically found in the Gear itself)
			GLuint mvpHandle;
			GLuint textureUniform;
			GLuint worldPosUniform;
			GLuint atlasWidthUniform;
			GLuint atlasTileWidthUniform;
			GLuint globalAmbienceMultiplierUniform;
		};

		// Below: a container for blueprint data; the elements of this container can be called to be rendered.
		struct LocalizedGBufferBPDataMap
		{
			std::unordered_map<EnclaveKeyDef::EnclaveKey, LocalizedGBufferBPData, EnclaveKeyDef::KeyHasher> localizedBlueprintsMap;

			// Below: use this when creating a new buffer, and needing to populate it with data.
			void insertNewRenderableBlueprint(EnclaveKeyDef::EnclaveKey in_localizedBlueprintKey,
											int in_totalTriangles,							// finally insert the appropriate data for rendering 
											int in_arraySize,								// into the LBPHighlightBufferData.
											GLfloat* in_arrayRef,
											LocalizedGBufferBPData in_bpData);

			void reloadExistingBlueprintBuffer(EnclaveKeyDef::EnclaveKey in_localizedBlueprintKey,
											int in_totalTriangles,							// finally insert the appropriate data for rendering 
											int in_arraySize,								// into the LBPHighlightBufferData.
											GLfloat* in_arrayRef,
											LocalizedGBufferBPData in_bpData);

			void deleteBlueprintFromRendering(EnclaveKeyDef::EnclaveKey in_localizedBlueprintKey);

			// Call drawing on all highlights.
			void drawBlueprints(GLuint in_programID,
				GLUniformRegistry* in_registryPtr,
				EnclaveKeyDef::EnclaveKey in_cameraBPKey,
				glm::vec3 in_cameraBPLocalCoord,
				glm::mat4 in_MVP);
		};

		LocalizedGBufferBPDataMap localizedBPs;

		// shader uniforms 
		GLuint mvpHandle;
		GLuint textureUniform;
		GLuint worldPosUniform;
		GLuint atlasWidthUniform;
		GLuint atlasTileWidthUniform;
		GLuint globalAmbienceMultiplierUniform;

		// deferred specific functions
		GLuint deferredFBO = 0;

		void setMatrices();
		void insertLocalizedBPData(TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef);
};

#endif