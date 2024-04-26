#pragma once

#ifndef WAVEHIGHLIGHTERGEART1_H
#define WAVEHIGHLIGHTERGEART1_H

#include "Gear.h"
#include "GLMultiDrawArrayJob.h"
#include "OrganicGLWinUtils.h"
#include "SinTBW.h"

/*

Description:

The role of the WaveHighlighterGearT1 is to provide highlights that have the ability to "fade" in and out (i.e, go from solid to transparent, and back again, in a cycle)

Note that like other Gears, this doesn't manage its own buffers. It assumes they have been created.

This gear will call glEnable(GL_BLEND), and disable it via glDisable, after the rendering of a highlight has been completed.

*/

class WaveHighlighterGearT1 : public Gear
{
	public:
		// defined virtual functions (from MachineShader base class)
		void initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef, ShaderMachineBase* in_shaderMachineBasePtr);
		void render();
		void passGLuintValue(std::string in_identifier, GLuint in_gluInt);
		void executeGearFunction(std::string in_identifier);
		void printData();
		void interpretMessage(Message in_message);

		void sendTerrainDataToGear(TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef) {};
		void removeTerrainDataFromGear(EnclaveKeyDef::EnclaveKey in_keyToRemove) {};
		void sendMessageAndBufferDataToGear(Message in_messageToSend, int in_arraySize, GLfloat* in_arrayRef) {};
	private:
		struct WaveDrawJob
		{
			public:
				WaveDrawJob();
				WaveDrawJob(int in_waveDrawJobBufferID) :
					waveDrawJobBufferID(in_waveDrawJobBufferID)
				{
					wave.reset(new SinTBW);
					wave.get()->initialize();
					isWaveInitialized = true;
				};
				~WaveDrawJob();		// does required OpenGL operations for when the draw job gets deleted (i.e, glDeleteVertexArrays).
									// The vertex array only gets deleted if setDrawJobAndBuildVAO was called (see isWaveReady flag).

				void setDrawJobDataAndBuildVAO(GLMultiDrawArrayJob in_waveDrawJobData);	
				void buildVAO();	// builds the VAO (only needs to be called once on call to setDrawJobDataAndBuildVAO)
				void renderWavedHighlight(int in_programID, GLUniformRegistry* in_gearUniformRegistryRef);	// render the wave highlight. Blending will be enabled,
																											// the proper blending function will get used, and then 
																											// blending needs to be disabled after the draw call.
																											// Failing to disable blending will cause strangeness 
																											// on cards such as GTX 4070. This was the cause of bug
																											// B-006.

				GLMultiDrawArrayJob waveDrawJobData;
				GLuint waveVAO;							// stores the ID of the VAO that will be used during the call to glBindVertexArray. Utilized during call to
														// WaveDrawJob::buildVAO() above.

				int waveDrawJobBufferID = 0;			// stores the ID of the buffer that will be used to store this WaveDrawJob's data; 
														// follow the chain of execution that starts in the OrganicGLManager::jobConstructCameraEnclaveHighlight, where it calls the
														// createDynamicBufferAndSendToGear function.

				bool isWaveInitialized = false;		// used to indicate when setDrawJobDataAndBuildVAO() is called, and that the WaveDrawJob is ready for use.

				std::shared_ptr<TimeBasedWaveBase> wave;	// stores the SinTBW when it gets setup; the value of the SinTBW is based on time delta, to allow for a "fading" effect.

				bool isWaveReady = false;	// mainly used to indicate that it is OK to call glDeleteVertexArrays, when it comes time to delete the WaveDrawJob.

		};
		
		std::unordered_map<std::string, WaveDrawJob> waveJobMap;	// stores all possible WaveDrawJobs that can be run.
		
};

#endif
