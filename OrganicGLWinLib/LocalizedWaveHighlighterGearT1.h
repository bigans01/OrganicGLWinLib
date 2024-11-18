#pragma once

#ifndef LOCALIZEDWAVEHIGHLIGHTERGEART1_H
#define LOCALIZEDWAVEHIGHLIGHTERGEART1_H

#include "Gear.h"
#include "OrganicGLWinUtils.h"
#include "SinTBW.h"
#include "WaveHighlightCategories.h"

class LocalizedWaveHighlighterGearT1 : public Gear
{
	public:
		// defined virtual functions (from MachineShader base class)
		void initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef, ShaderMachineBase* in_shaderMachineBasePtr);
		void render();
		void sendMessageAndBufferDataToGear(Message in_messageToSend, int in_arraySize, GLfloat* in_arrayRef);	// read a Message and the corresponding buffer data, 
																												// and handle it appropriately; the Message type should be:
																												// MessageType::SHADERMACHINE_BUFFER_DATA_COLLECTION_HIGHLIGHT

		void interpretMessage(Message in_message);	// used for handling MessageType::SHADERMACHINE_REMOVE_TARGETBLOCK_HIGHLIGHT.

		// unused, but required public virtual functions for Gear
		void printData() {};
		void sendTerrainDataToGear(TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef) {};
		void removeTerrainDataFromGear(EnclaveKeyDef::EnclaveKey in_keyToRemove) {};
		void passGLuintValue(std::string in_identifier, GLuint in_gluInt) {};
		void executeGearFunction(std::string in_identifier) {};
	private:

		// Below: this struct contains everything needed to draw an individual wave highlight.
		struct LocalizedWaveHighlightData
		{
			LocalizedWaveHighlightData() {};	// copy constructor that is needed for working in the map
			LocalizedWaveHighlightData(EnclaveKeyDef::EnclaveKey in_localWaveHighlightKey) :
				localWaveHighlightKey(in_localWaveHighlightKey)
			{
				localWave.reset(new SinTBW);
				localWave.get()->initialize();
			};

			~LocalizedWaveHighlightData();	// used to automatically delete buffer and VAO when an instance of this class is erased from
											// the uniqueWaveHighlights map that's found in LocalizedUniqueWaveHighlights

			void generateLocalWaveHighlightBufferAndVAO();
			void insertDataIntoWaveHighlightBuffer(int in_totalTriangles, int in_arraySize, GLfloat* in_arrayRef);
			void drawWaveHighlight(GLuint in_programID, EnclaveKeyDef::EnclaveKey in_cameraBPKey, glm::vec3 in_cameraBPLocalCoord, glm::mat4 in_MVPvalue);

			void deleteWaveHighlightBuffer();


			EnclaveKeyDef::EnclaveKey localWaveHighlightKey;
			int numberOfVertices = 0;	// the number of vertices for the draw call; should be equal to number of numberOfTrianglesProduced * 3.
			GLuint waveDrawJobBufferID = 0;			// stores the ID of the buffer that will be used to store this WaveDrawJob's data; 
													// follow the chain of execution that starts in the OrganicGLManager::jobConstructCameraEnclaveHighlight, where it calls the
													// createDynamicBufferAndSendToGear function.
			GLuint waveVAOId;							// stores the ID of the VAO that will be used during the call to glBindVertexArray. Utilized during call to
											// WaveDrawJob::buildVAO() above.

			std::shared_ptr<TimeBasedWaveBase> localWave;	// a pointer to TimwBasedWaveBase-derived class such as SinTBW, that will
															// store the state of the "wave" value to use in the highlight to render.
		};

		// Below: a struct that stores multiple unique highlights, and maps each highlight type to
		// it's instance of LocalizedWaveHighlightData.
		struct LocalizedUniqueWaveHighlights
		{
			LocalizedUniqueWaveHighlights() {};

			bool containsUniqueHighlight(WaveHighlightCategories in_highlightType);
			void removeHighlight(WaveHighlightCategories in_highlightType);

			std::map<WaveHighlightCategories, LocalizedWaveHighlightData> uniqueWaveHighlights;
		};

		// Below: an EnclaveKeyDef::EnclaveKey map of LocalizedWaveHighlightData.
		struct BPKeyedUniqueWaveHighlights
		{
			std::unordered_map<EnclaveKeyDef::EnclaveKey, LocalizedUniqueWaveHighlights, EnclaveKeyDef::KeyHasher> bpKeyedWaveHighlights;

			void insertWaveHighlight(EnclaveKeyDef::EnclaveKey in_bpKey,				// This function will create a new LBPHighlightBufferData,
									WaveHighlightCategories in_uniqueHighlightValue,	// map it, call the buffer creation buffer, and then
									int in_totalTriangles,							// finally insert the appropriate data for rendering 
									int in_arraySize,								// into the LBPHighlightBufferData.
									GLfloat* in_arrayRef);

			void scanForAndRemoveORECompositionHighlight();

			void scanForAndRemoveCurrentEnclaveHighlight();

			// Call drawing on all wave highlights.
			void drawWaveHighlights(GLuint in_programID,
									EnclaveKeyDef::EnclaveKey in_cameraBPKey,
									glm::vec3 in_cameraBPLocalCoord,
									glm::mat4 in_MVP);
		};

		BPKeyedUniqueWaveHighlights waveHighlightsIndex;
};

#endif