#include "stdafx.h"
#include "LocalizedWaveHighlighterGearT1.h"

void LocalizedWaveHighlighterGearT1::initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef, ShaderMachineBase* in_shaderMachineBasePtr)
{
	width = in_width;
	height = in_height;
	window = in_windowRef;

	programIndex.insertDefaultEntry(in_programID);
	programID = programIndex.selectProgram();

	gearMachinePtr = in_shaderMachineBasePtr;

	// set up the uniform requests

	// reqMVP should be calculated with a view mat4 that is calculated from an origin of 0,0,0
	GLUniformRequest reqMVP(GLDataType::MAT4, "MVP");
	uniformRequests.push_back(reqMVP);

	// ***************************** the below two requests are Required for localization ***************************

	// These uniforms should be calculated every frame, and come from the call to ShaderMachineBase::runMatrixAndDeltaLocalComputations

	// The world camera's true blueprint key, before it was translated to 0,0,0
	GLUniformRequest reqCameraBlueprintKey(GLDataType::KEY3D, "worldCoordBPLocalCameraKey");
	uniformRequests.push_back(reqCameraBlueprintKey);

	// The world camera's true local coordinate, before it was translated to 0,0,0
	GLUniformRequest reqCameraLocalCoord(GLDataType::VEC3, "worldCoordBPLocalCameraCoord");
	uniformRequests.push_back(reqCameraLocalCoord);

	// ***************************** **************************************************** ************************** 

	// Remember, draw call data is kept internally in this gear.

}

void LocalizedWaveHighlighterGearT1::render()
{
	useProgram();

	// we should be writing to and/or manipulating the default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glEnable(GL_DEPTH_TEST);
	waveHighlightsIndex.drawWaveHighlights(programID,
		gearUniformRegistry.get3DKey("worldCoordBPLocalCameraKey"),
		gearUniformRegistry.getVec3("worldCoordBPLocalCameraCoord"),
		gearUniformRegistry.getMat4("MVP")
	);

	// TEMPORARY: when done drawing all highlights, clear the buffers.
	glDisable(GL_DEPTH_TEST);
}

void LocalizedWaveHighlighterGearT1::sendMessageAndBufferDataToGear(Message in_messageToSend, int in_arraySize, GLfloat* in_arrayRef)
{
	// The Message to read should be of the type MessageType::SHADERMACHINE_BUFFER_DATA_COLLECTION_HIGHLIGHT.
	in_messageToSend.open();
	switch (in_messageToSend.messageType)
	{
		case MessageType::SHADERMACHINE_BUFFER_DATA_WAVE_CURRENT_ORE:
		{
			EnclaveKeyDef::EnclaveKey collectionHighlightKey = in_messageToSend.readEnclaveKey();
			int totalTriangles = in_messageToSend.readInt();

			// scan for and remvoe the existing camera ORE highlight, if it exists
			waveHighlightsIndex.scanForAndRemoveCurrentEnclaveHighlight();
			waveHighlightsIndex.insertWaveHighlight(collectionHighlightKey,
													WaveHighlightCategories::WAVE_CURRENT_COLLECTION_ENCLAVE,
													totalTriangles,
													in_arraySize,
													in_arrayRef);

			break;
		}

		case MessageType::SHADERMACHINE_BUFFER_DATA_WAVE_ORE_COMPOSITION:
		{
			EnclaveKeyDef::EnclaveKey collectionHighlightKey = in_messageToSend.readEnclaveKey();
			int totalTriangles = in_messageToSend.readInt();

			// scan for and remove the existing ORE composition highlight, if it exists
			waveHighlightsIndex.scanForAndRemoveORECompositionHighlight();
			waveHighlightsIndex.insertWaveHighlight(collectionHighlightKey,
													WaveHighlightCategories::WAVE_CURRENT_ORE_COMPOSITION,
													totalTriangles,
													in_arraySize,
													in_arrayRef);
													
			break;
		}
	}
}

void LocalizedWaveHighlighterGearT1::interpretMessage(Message in_message)
{
	// Open the message for reading, just in case
	in_message.open();
	switch (in_message.messageType)
	{
		case MessageType::SHADERMACHINE_REMOVE_DATA_WAVE_CURRENT_ORE:
		{
			waveHighlightsIndex.scanForAndRemoveCurrentEnclaveHighlight();
			break;
		}

		case MessageType::SHADERMACHINE_REMOVE_DATA_WAVE_ORE_COMPOSITION:
		{
			waveHighlightsIndex.scanForAndRemoveORECompositionHighlight();
			break;
		}
	}
}


LocalizedWaveHighlighterGearT1::LocalizedWaveHighlightData::~LocalizedWaveHighlightData()
{
	deleteWaveHighlightBuffer();
}

void LocalizedWaveHighlighterGearT1::LocalizedWaveHighlightData::generateLocalWaveHighlightBufferAndVAO()
{
	// Create the buffer, and a VAO; build the VAO
	OrganicGLWinUtils::createBuffer(&waveDrawJobBufferID);
	OrganicGLWinUtils::createAndBindVertexArray(&waveVAOId);	// create/bind the highlighter VAO
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

void LocalizedWaveHighlighterGearT1::LocalizedWaveHighlightData::insertDataIntoWaveHighlightBuffer(int in_totalTriangles, int in_arraySize, GLfloat* in_arrayRef)
{
	glBindBuffer(GL_ARRAY_BUFFER, waveDrawJobBufferID);
	glBufferData(GL_ARRAY_BUFFER, in_arraySize, in_arrayRef, GL_STATIC_DRAW);
	numberOfVertices = in_totalTriangles * 3;
}

void LocalizedWaveHighlighterGearT1::LocalizedWaveHighlightData::drawWaveHighlight(GLuint in_programID, EnclaveKeyDef::EnclaveKey in_cameraBPKey, glm::vec3 in_cameraBPLocalCoord, glm::mat4 in_MVPvalue)
{
	glBindBuffer(GL_ARRAY_BUFFER, waveDrawJobBufferID);
	glBindVertexArray(waveVAOId);

	//std::cout << "!! Binding data and drawing, via lbpHighlightDraw; lbp key is: ";
	//lbpHighlightBPKey.printKey();
	//std::cout << std::endl;

	//GLuint mvpUniform = glGetUniformLocation(in_programID, "MVP");	// find the MVP uniform
	//glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, &in_MVPvalue[0][0]);		// set the uniform

	// |||||||||||||| send the uniforms:

	// Uniform set one: camera BP key. Requires the following uniforms to be in vertex shader:
	// The value of this uniform will change every frame, as it is requested by the gearUniformRegistry.
	//	-cameraBlueprintX
	//  -cameraBlueprintY
	//  -cameraBlueprintZ
	//
	// This value will change every frame, as it is requested by the gearUniformRegistry, so it must be resent
	// to the uniform of the vertex shader every frame as well. The value of in_cameraBPKey must be the value of
	// "worldCoordBPLocalCameraKey" in the gearUniformRegistry.
	//
	EnclaveKeyDef::EnclaveKey cameraBPKey = in_cameraBPKey;

	GLuint cameraBlueprintXUniform = glGetUniformLocation(in_programID, "cameraBlueprintX");
	glUniform1i(cameraBlueprintXUniform, cameraBPKey.x);

	GLuint cameraBlueprintYUniform = glGetUniformLocation(in_programID, "cameraBlueprintY");
	glUniform1i(cameraBlueprintYUniform, cameraBPKey.y);

	GLuint cameraBlueprintZUniform = glGetUniformLocation(in_programID, "cameraBlueprintZ");
	glUniform1i(cameraBlueprintZUniform, cameraBPKey.z);

	// Uniform two: camera local coordinates. Requires the following uniform to be in the vertex shader:
	//	-cameraLocalVec
	//
	// This value will change every frame, as it is requested by the gearUniformRegistry, so it must be resent
	// to the uniform of the vertex shader every frame as well. The value of in_cameraBPLocalCoord must be
	// the value of "worldCoordBPLocalCameraCoord" in the gearUniformRegistry.

	GLuint cameraLocalVecUniform = glGetUniformLocation(in_programID, "cameraLocalVec");
	glUniform3fv(cameraLocalVecUniform, 1, &in_cameraBPLocalCoord[0]);

	// Uniform set three: the blueprint key that corresponds to the data that is in the buffer 
	// (i.e, the key used for offseting localized data in the shader)
	//
	// This value is static, and doesn't change every frame; the lbpHighlightBPKey represents the blueprint key that is used
	// as an offset when comparing to the camera blueprint key (see the logic used in the vertex shader for more info, or check out
	// task F-011)
	GLuint coordinateBlueprintXUniform = glGetUniformLocation(in_programID, "coordinateBlueprintX");
	glUniform1i(coordinateBlueprintXUniform, localWaveHighlightKey.x);

	GLuint coordinateBlueprintYUniform = glGetUniformLocation(in_programID, "coordinateBlueprintY");
	glUniform1i(coordinateBlueprintYUniform, localWaveHighlightKey.y);

	GLuint coordinateBlueprintZUniform = glGetUniformLocation(in_programID, "coordinateBlueprintZ");
	glUniform1i(coordinateBlueprintZUniform, localWaveHighlightKey.z);


	// Lastly, send wave related uniforms, and enable blending before drawing.
	GLuint waveUniform = glGetUniformLocation(in_programID, "alphaWaveValue");
	glUniform1f(waveUniform, localWave.get()->calculateWaveYValue());
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_TRIANGLES, 0, numberOfVertices * 4);		// draw.
	glDisable(GL_BLEND);
}

void LocalizedWaveHighlighterGearT1::LocalizedWaveHighlightData::deleteWaveHighlightBuffer()
{
	glDeleteVertexArrays(1, &waveVAOId);
	OrganicGLWinUtils::deleteBuffer(&waveDrawJobBufferID);
}

bool LocalizedWaveHighlighterGearT1::LocalizedUniqueWaveHighlights::containsUniqueHighlight(WaveHighlightCategories in_highlightType)
{
	bool hasHighlight = false;
	auto highlightFinder = uniqueWaveHighlights.find(in_highlightType);
	if (highlightFinder != uniqueWaveHighlights.end())
	{
		hasHighlight = true;
	}
	return hasHighlight;
}

void LocalizedWaveHighlighterGearT1::LocalizedUniqueWaveHighlights::removeHighlight(WaveHighlightCategories in_highlightType)
{
	// check if the specified highlight exists
	auto highlightFound = false;
	auto highlightFinder = uniqueWaveHighlights.find(in_highlightType);
	if (highlightFinder != uniqueWaveHighlights.end())
	{
		highlightFound = true;
	}

	// if it exists, erase it...the desructor of LocalizedWaveHighlightData should automatically 
	// delete the buffer data, then remove the highlight itself.
	if (highlightFound)
	{
		auto highlightFinder = uniqueWaveHighlights.find(in_highlightType);
		uniqueWaveHighlights.erase(in_highlightType);
	}
}

void LocalizedWaveHighlighterGearT1::BPKeyedUniqueWaveHighlights::insertWaveHighlight(EnclaveKeyDef::EnclaveKey in_bpKey,				// This function will create a new LBPHighlightBufferData,
																					WaveHighlightCategories in_uniqueHighlightValue,	// map it, call the buffer creation buffer, and then
																					int in_totalTriangles,							// finally insert the appropriate data for rendering 
																					int in_arraySize,								// into the LBPHighlightBufferData.
																					GLfloat* in_arrayRef)
{
	LocalizedWaveHighlightData newWaveHighlightData(in_bpKey);
	bpKeyedWaveHighlights[in_bpKey].uniqueWaveHighlights[in_uniqueHighlightValue] = newWaveHighlightData;
	bpKeyedWaveHighlights[in_bpKey].uniqueWaveHighlights[in_uniqueHighlightValue].generateLocalWaveHighlightBufferAndVAO();
	bpKeyedWaveHighlights[in_bpKey].uniqueWaveHighlights[in_uniqueHighlightValue].insertDataIntoWaveHighlightBuffer(in_totalTriangles,
		in_arraySize,
		in_arrayRef);
}

void LocalizedWaveHighlighterGearT1::BPKeyedUniqueWaveHighlights::scanForAndRemoveORECompositionHighlight()
{
	for (auto& currentWaveHighlights : bpKeyedWaveHighlights)
	{
		currentWaveHighlights.second.removeHighlight(WaveHighlightCategories::WAVE_CURRENT_ORE_COMPOSITION);
	}
}

void LocalizedWaveHighlighterGearT1::BPKeyedUniqueWaveHighlights::scanForAndRemoveCurrentEnclaveHighlight()
{
	for (auto& currentWaveHighlights : bpKeyedWaveHighlights)
	{
		currentWaveHighlights.second.removeHighlight(WaveHighlightCategories::WAVE_CURRENT_COLLECTION_ENCLAVE);
	}
}

void LocalizedWaveHighlighterGearT1::BPKeyedUniqueWaveHighlights::drawWaveHighlights(GLuint in_programID,
	EnclaveKeyDef::EnclaveKey in_cameraBPKey,
	glm::vec3 in_cameraBPLocalCoord,
	glm::mat4 in_MVP)
{
	GLuint mvpUniform = glGetUniformLocation(in_programID, "MVP");	// find the MVP uniform
	glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, &in_MVP[0][0]);		// set the uniform

	for (auto& currentWaveHighlights : bpKeyedWaveHighlights)
	{
		for (auto& currentWaveHighlight : currentWaveHighlights.second.uniqueWaveHighlights)
		{
			currentWaveHighlight.second.drawWaveHighlight(in_programID,
				in_cameraBPKey,
				in_cameraBPLocalCoord,
				in_MVP);
		}
	}
}