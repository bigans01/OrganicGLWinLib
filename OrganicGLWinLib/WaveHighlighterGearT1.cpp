#include "stdafx.h"
#include "WaveHighlighterGearT1.h"
#include "ShaderMachineBase.h"

void WaveHighlighterGearT1::initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef, ShaderMachineBase* in_shaderMachineBasePtr)
{
	width = in_width;
	height = in_height;
	window = in_windowRef;
	programID = in_programID;
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
			std::cout << "(WaveHighlighterGearT1): testGrab draw count is: " << testGrab.drawCount << std::endl;
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