#include "stdafx.h"
#include "WaveHighlighterGearT1.h"
#include "ShaderMachineBase.h"

void WaveHighlighterGearT1::initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef, ShaderMachineBase* in_shaderMachineBasePtr)
{
	width = in_width;
	height = in_height;
	window = in_windowRef;

	programIndex.insertDefaultEntry(in_programID); 
	programID = programIndex.selectProgram();

	gearMachinePtr = in_shaderMachineBasePtr;

	// set up the uniform requests
	GLUniformRequest reqMVP(GLDataType::MAT4, "MVP");
	uniformRequests.push_back(reqMVP);
}
void WaveHighlighterGearT1::render()
{
	useProgram();
	auto wavesBegin = waveJobMap.begin();
	auto wavesEnd = waveJobMap.end();
	for (; wavesBegin != wavesEnd; wavesBegin++)
	{
		wavesBegin->second.renderWavedHighlight(programID, &gearUniformRegistry);		// render each dynamic buffer
	}
}

void WaveHighlighterGearT1::passGLuintValue(std::string in_identifier, GLuint in_gluInt)
{

}

void WaveHighlighterGearT1::executeGearFunction(std::string in_identifier)
{

}

void WaveHighlighterGearT1::printData()
{

}

void WaveHighlighterGearT1::interpretMessage(Message in_message)
{
	// below was for functional prototype testing only, and development reference, as of 4/25/2021; delete whenever convenient (unless finding another way to use)
	Message messageCopy = std::move(in_message);
	switch (messageCopy.messageType)
	{
		case (MessageType::OPENGL_REGISTER_DYN_BUFFER_IN_GEAR):
		{
			messageCopy.open();
			std::string waveJobName = messageCopy.readString();
			GLuint waveJobBufferID = messageCopy.readInt();
			WaveDrawJob newWaveDrawJob(waveJobBufferID);
			waveJobMap[waveJobName] = newWaveDrawJob;
			std::cout << "(WaveHighlighterGearT1): wave job name is: " << waveJobName << std::endl;
			std::cout << "(WaveHighlighterGearT1): created and initialized wave for WaveDrawJob, " << waveJobName << std::endl;
			break;
		}
		case (MessageType::OPENGL_REGISTER_DYN_BUFFER_MULTIDRAW_JOB_IN_GEAR):
		{
			messageCopy.open();
			std::string bufferName = messageCopy.readString();
			//GLMultiDrawArrayJob testGrab = accessProxy.proxyFetchDynamicMultiDrawArrayJob(bufferName);
			//GLMultiDrawArrayJob testGrab = fetchGearMachinePtr()->fetchDynamicMultiDrawArrayJobCopy(bufferName);
			GLMultiDrawArrayJob testGrab = gearBaseFetchDynamicMultiDrawArrayJobCopy(bufferName);

			waveJobMap[bufferName].setDrawJobDataAndBuildVAO(testGrab);
			//std::cout << "(WaveHighlighterGearT1): testGrab draw count is: " << testGrab.drawCount << std::endl;
			break;
		}
		case (MessageType::OPENGL_DEREGISTER_DYN_BUFFER_MULTIDRAW_JOB_IN_GEAR):
		{
			messageCopy.open();
			std::string bufferName = messageCopy.readString();
			waveJobMap.erase(bufferName);
			std::cout << "(WaveHighlighterGearT1): deleted VAOs tied to buffer with the name: " << bufferName << std::endl;
			break;
		} 
	}
}

// ++++++++++++++++++++++++++++++++++++++++
// Start: functions for WaveHighlighterGearT1::WaveDrawJob
// ++++++++++++++++++++++++++++++++++++++++

WaveHighlighterGearT1::WaveDrawJob::WaveDrawJob()
{

}

WaveHighlighterGearT1::WaveDrawJob::~WaveDrawJob()
{
	if (isWaveReady == true)	// memory management; only delete the VAO if it was actually created.
	{
		glDeleteVertexArrays(1, &waveVAO);
	}
}

void WaveHighlighterGearT1::WaveDrawJob::setDrawJobDataAndBuildVAO(GLMultiDrawArrayJob in_waveDrawJobData)
{
	waveDrawJobData = in_waveDrawJobData;
	buildVAO();
	isWaveReady = true;
}

void WaveHighlighterGearT1::WaveDrawJob::buildVAO()
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

void WaveHighlighterGearT1::WaveDrawJob::renderWavedHighlight(int in_programID, GLUniformRegistry* in_gearUniformRegistryRef)
{
	if (isWaveReady == true)
	{
		//std::cout << "Wave is READY!!!!!!! -- draw count is: " << waveDrawJobData.drawCount << std::endl;
		glBindVertexArray(waveVAO);
		GLuint mvpUniform = glGetUniformLocation(in_programID, "MVP");		// find the MVP uniform
		glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, &in_gearUniformRegistryRef->getMat4("MVP")[0][0]);		// set the uniform
		GLuint waveUniform = glGetUniformLocation(in_programID, "alphaWaveValue");

		//std::cout << "waveUniform value is: " << waveUniform << std::endl;

		glUniform1f(waveUniform, wave.get()->calculateWaveYValue());

		// Remember: GL_BLEND must be enabled, and then immediately disabled, when we are done. 
		// Not calling glDisable(GL_BLEND) is generally bad practice, and will lead to the compute shaders and similiar gears not working
		// correctly in SMDeferredLightingComputeV1.
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glMultiDrawArrays(GL_TRIANGLES, waveDrawJobData.multiStartIndices.get(), waveDrawJobData.multiVertexCount.get(), waveDrawJobData.drawCount);
		glDisable(GL_BLEND);
	}
}


