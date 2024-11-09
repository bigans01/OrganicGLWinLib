#pragma once

#ifndef LOCALIZEDHIGHLIGHTERGEAR_T1
#define LOCALIZEDHIGHLIGHTERGEAR_T1

#include "Gear.h"
#include "OrganicGLWinUtils.h"
#include "GLMultiDrawArrayJob.h"
#include "UniqueHighlightEnum.h"

/*

Description: similiar to the HighlighterGearT1, in that it draws highlights that don't fade in/out; however,
this gear is only designed to be used with ShaderMachines and that are operating in a LOCAL coordinate mode 
(that is, the value of  machineCoordMode = GPUCoordinateMode::COORDINATE_MODE_LOCAL).

This Gear requires that the following uniforms be updated every frame:

-MVP
-worldCoordBPLocalCameraKey
-worldCoordBPLocalCameraCoord

The "position" value used in the MVP calculation should always be at 0,0,0; this is because the vertex shader
will use a method known as localized translation, which applies the inverse value of the camera's world coordinate 
(which is used in translating) to all vertices in the vertex shader. Some of the values used for the localized translation should come 
from a calculation that is done by an instance of GPUWorldCoordinateProducer (which will produce uniform values per-frame
for the  "worldCoordBPLocalCameraKey" and "worldCoordBPLocalCameraCoord"). More explicit data on how uniforms are passed for
localized translation can be found in the LocalizedHighlighterGearT1::LBPHighlightBufferData::lbpHighlightDraw function.

*/

class LocalizedHighlighterGearT1 : public Gear
{
	public:
		// defined virtual functions (from MachineShader base class)
		void initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef, ShaderMachineBase* in_shaderMachineBasePtr);
		void render();
		void passGLuintValue(std::string in_identifier, GLuint in_gluInt);
		void executeGearFunction(std::string in_identifier);
		void sendMessageAndBufferDataToGear(Message in_messageToSend, int in_arraySize, GLfloat* in_arrayRef);	// read a Message and the corresponding buffer data, 
																												// and handle it appropriately; the Message type should be:
																												// MessageType::SHADERMACHINE_BUFFER_DATA_COLLECTION_HIGHLIGHT
																												
		void interpretMessage(Message in_message);	// used for handling MessageType::SHADERMACHINE_REMOVE_TARGETBLOCK_HIGHLIGHT.

		// unused, but required public virtual functions for Gear
		void printData() {};
		void sendTerrainDataToGear(TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef) {};
		void removeTerrainDataFromGear(EnclaveKeyDef::EnclaveKey in_keyToRemove) {};

	private:

		// Below: this struct contains necessary data to render a specific, categorized highlight.
		struct LBPHighlightBufferData
		{
			LBPHighlightBufferData() {};
			LBPHighlightBufferData(EnclaveKeyDef::EnclaveKey in_lbpHighlightBPKey) :
				lbpHighlightBPKey(in_lbpHighlightBPKey)
			{};

			void lbpHighlightGenerateBuffer();	// generate the buffer that we will load data into; must be called before loading data into the buffer.
			void lbpHighightInsertDataIntoBuffer(int in_totalTriangles, int in_arraySize, GLfloat* in_arrayRef);	// insert data into buffer.
			void lbpHighlightDraw(GLuint in_programID, EnclaveKeyDef::EnclaveKey in_cameraBPKey, glm::vec3 in_cameraBPLocalCoord, glm::mat4 in_MVPvalue);	// draw the buffer; the parameters
																												// passed are for the camera uniformns in the shader.

			void deleteLbpHighlightBuffer();	// delete the buffer and its contents

			EnclaveKeyDef::EnclaveKey lbpHighlightBPKey;	// required for the uniform
			int numberOfVertices = 0;	// the number of vertices for the draw call; should be equal to number of numberOfTrianglesProduced * 3.
			GLuint blueprintBufferID = 0;
			GLuint blueprintVAOID = 0;

		};

		// Below: a struct that contains a map of unique highlight data. The key should be the unique highlight enum class,
		// UniqueHighlightEnum, with one LBPHighlightBufferData per key.
		struct LBPUniqueHighlightMap
		{
			LBPUniqueHighlightMap() {};

			bool containsUniqueHighlight(UniqueHighlightEnum in_highlightType);
			void removeHighlight(UniqueHighlightEnum in_highlightType);

			std::map<UniqueHighlightEnum, LBPHighlightBufferData> uniqueHighlights;
		};

		// Below: an EnclaveKeyDef::EnclaveKey map of LBPUniqueHighlightMap objects.
		struct BPUniqueHighlights
		{
			std::unordered_map<EnclaveKeyDef::EnclaveKey, LBPUniqueHighlightMap, EnclaveKeyDef::KeyHasher> bpHighlights;

		
			void insertNewHighlight(EnclaveKeyDef::EnclaveKey in_bpKey,				// This function will create a new LBPHighlightBufferData,
									UniqueHighlightEnum in_uniqueHighlightValue,	// map it, call the buffer creation buffer, and then
									int in_totalTriangles,							// finally insert the appropriate data for rendering 
									int in_arraySize,								// into the LBPHighlightBufferData.
									GLfloat* in_arrayRef);

			void scanForAndRemoveCollectionHighlight();	// scan for wherever the UniqueHighlightEnum::CAMERA_COLLECTION might be, and erases it
														// (this is done so that we don't render multiple UniqueHighlightEnum::CAMERA_COLLECTION highlights
														//	from different blueprints)
			
			void scanForAndRemoveTargetedBlockHighlight();	// will remove the block highlighting from rendering
			
			// Call drawing on all highlights.
			void drawHighlights(GLuint in_programID,
								EnclaveKeyDef::EnclaveKey in_cameraBPKey,
								glm::vec3 in_cameraBPLocalCoord,
								glm::mat4 in_MVP);
		};

		BPUniqueHighlights highlightsIndex;

};

#endif