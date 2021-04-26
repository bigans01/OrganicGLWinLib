#pragma once

#ifndef WAVEHIGHLIGHTERGEART1_H
#define WAVEHIGHLIGHTERGEART1_H

#include "Gear.h"
#include "GLMultiDrawArrayJob.h"
#include "OrganicGLWinUtils.h"
#include "SinTBW.h"
#include <unordered_map>
#include <mutex>

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
	private:
		struct WaveDrawJob
		{
			public:
				WaveDrawJob() {};
				WaveDrawJob(int in_waveDrawJobBufferID) :
					waveDrawJobBufferID(in_waveDrawJobBufferID)
				{
					wave.reset(new SinTBW);
					wave.get()->initialize();
					isWaveInitialized = true;
				};
				~WaveDrawJob() 
				{
					if (isWaveReady == true)	// memory management; only delete the VAO if it was actually created.
					{
						glDeleteVertexArrays(1, &waveVAO);
					}
				};

				void setDrawJobDataAndBuildVAO(GLMultiDrawArrayJob in_waveDrawJobData)
				{
					waveDrawJobData = in_waveDrawJobData;
					buildVAO();
					isWaveReady = true;
				}
				void buildVAO()
				{
					OrganicGLWinUtils::createAndBindVertexArray(&waveVAO);	// create/bind the highlighter VAO
					glBindBuffer(GL_ARRAY_BUFFER, waveDrawJobBufferID);	// bind to the highlighter buffer
					//glVertexAttribPointer(
					//	0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
					//	3,                  // size
					//	GL_FLOAT,           // type
					//	GL_FALSE,           // normalized?
					//	0,                  // stride = 0 (tightly packed); bytes offset between consecutive generic vertex attributes is 0.
					//	(void*)0             array buffer offset. Number following (void*) indicates offset point to begin reading from in the pointed-to buffer, measured in bytes;
					//						For instance, if the data begins at byte 10000, you would put (void*)10000 in the array you are reading.
					//						
					//);


					glEnableVertexAttribArray(0);
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)0);		// First attribute: a vec3 representing the point data, before it is translated by MVP.
					glEnableVertexAttribArray(1);
					glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)12);    // Second attribute: a vec3 representing the output color.

					glEnableVertexAttribArray(0);
				}

				void renderWavedHighlight(int in_programID, GLUniformRegistry* in_gearUniformRegistryRef)
				{
					if (isWaveReady == true)
					{
						//std::cout << "Wave is READY!!!!!!! -- draw count is: " << waveDrawJobData.drawCount << std::endl;
						glBindVertexArray(waveVAO);
						GLuint mvpUniform = glGetUniformLocation(in_programID, "MVP");		// find the MVP uniform
						glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, &in_gearUniformRegistryRef->getMat4("MVP")[0][0]);		// set the uniform
						GLuint waveUniform = glGetUniformLocation(in_programID, "alphaWaveValue");
						glUniform1f(waveUniform, wave.get()->calculateWaveYValue());

						glMultiDrawArrays(GL_TRIANGLES, waveDrawJobData.multiStartIndices.get(), waveDrawJobData.multiVertexCount.get(), waveDrawJobData.drawCount);
					}
				}



				GLMultiDrawArrayJob waveDrawJobData;
				GLuint waveVAO;
				int waveDrawJobBufferID = 0;
				bool isWaveInitialized = false;
				std::shared_ptr<TimeBasedWaveBase> wave;
				bool isWaveReady = false;

		};
		
		std::unordered_map<std::string, WaveDrawJob> waveJobMap;
		
};

#endif
