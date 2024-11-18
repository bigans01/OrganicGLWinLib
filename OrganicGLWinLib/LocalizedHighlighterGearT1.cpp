#include "stdafx.h"
#include "LocalizedHighlighterGearT1.h"

void LocalizedHighlighterGearT1::initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef, ShaderMachineBase* in_shaderMachineBasePtr)
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

void LocalizedHighlighterGearT1::sendMessageAndBufferDataToGear(Message in_messageToSend, int in_arraySize, GLfloat* in_arrayRef)
{
	// The Message to read should be of the type MessageType::SHADERMACHINE_BUFFER_DATA_COLLECTION_HIGHLIGHT.
	in_messageToSend.open();
	switch (in_messageToSend.messageType)
	{
		// The contents of the Message should be, in the following order:
		// -EnclaveKey 
		// -number of triangles to render
		case MessageType::SHADERMACHINE_BUFFER_DATA_COLLECTION_HIGHLIGHT:
		{
			EnclaveKeyDef::EnclaveKey collectionHighlightKey = in_messageToSend.readEnclaveKey();
			int totalTriangles = in_messageToSend.readInt();

			// scan for and remove the existing collection highlight, if it exists. Then, insert the necessary data.
			highlightsIndex.scanForAndRemoveCollectionHighlight();
			highlightsIndex.insertNewHighlight(collectionHighlightKey,
												UniqueHighlightEnum::CAMERA_COLLECTION,
												totalTriangles,
												in_arraySize,
												in_arrayRef);

			break;
		}

		case MessageType::SHADERMACHINE_BUFFER_DATA_TARGETBLOCK_HIGHLIGHT:
		{
			EnclaveKeyDef::EnclaveKey collectionHighlightKey = in_messageToSend.readEnclaveKey();
			int totalTriangles = in_messageToSend.readInt();

			// scan for and remove the existing collection highlight, if it exists. Then, insert the necessary data.
			highlightsIndex.scanForAndRemoveTargetedBlockHighlight();
			//std::cout << "LocalizedHighlighterGearT1::sendMessageAndBufferDataToGear received data to render...total triangles to render will be: " << totalTriangles << std::endl;
			highlightsIndex.insertNewHighlight(collectionHighlightKey,
				UniqueHighlightEnum::TARGETED_BLOCK,
				totalTriangles,
				in_arraySize,
				in_arrayRef);
			break;
		}
	}
}

void LocalizedHighlighterGearT1::interpretMessage(Message in_message)
{
	in_message.open();
	switch (in_message.messageType)
	{
		case MessageType::SHADERMACHINE_REMOVE_TARGETBLOCK_HIGHLIGHT:
		{
			// scan for and remove the existing collection highlight, if it exists. 
			std::cout << "+++++++++++++++++++ Found signal to remove target block highlight. " << std::endl;
			highlightsIndex.scanForAndRemoveTargetedBlockHighlight();
			break;
		}
	}
}

void LocalizedHighlighterGearT1::LBPHighlightBufferData::lbpHighlightGenerateBuffer()
{
	// Create the buffer, and a VAO; build the VAO
	OrganicGLWinUtils::createBuffer(&blueprintBufferID);
	OrganicGLWinUtils::createAndBindVertexArray(&blueprintVAOID);	// create/bind the highlighter VAO
	glBindBuffer(GL_ARRAY_BUFFER, blueprintBufferID);	// bind to the highlighter buffer


	//glVertexAttribPointer(
	//	0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	//	3,                  // size
	//	GL_FLOAT,           // type
	//	GL_FALSE,           // normalized?
	//	0,                  // stride = 0 (tightly packed); bytes offset between consecutive generic vertex attributes is 0.
	//	(void*)0            /* array buffer offset. Number following (void*) indicates offset point to begin reading from in the pointed-to buffer, measured in bytes;
	//						For instance, if the data begins at byte 10000, you would put (void*)10000 in the array you are reading.
	//						*/
	//);


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)0);		// First attribute: a vec3 representing the point data, before it is translated by MVP.
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)12);    // Second attribute: a vec3 representing the output color.

	glEnableVertexAttribArray(0);
}

void LocalizedHighlighterGearT1::LBPHighlightBufferData::lbpHighightInsertDataIntoBuffer(int in_totalTriangles, 
																						int in_arraySize, 
																						GLfloat* in_arrayRef)
{
	glBindBuffer(GL_ARRAY_BUFFER, blueprintBufferID);
	glBufferData(GL_ARRAY_BUFFER, in_arraySize, in_arrayRef, GL_STATIC_DRAW);
	numberOfVertices = in_totalTriangles * 3;
}

void LocalizedHighlighterGearT1::LBPHighlightBufferData::lbpHighlightDraw(GLuint in_programID, 
																		EnclaveKeyDef::EnclaveKey in_cameraBPKey, 
																		glm::vec3 in_cameraBPLocalCoord, 
																		glm::mat4 in_MVPvalue)
{
	glBindBuffer(GL_ARRAY_BUFFER, blueprintBufferID);
	glBindVertexArray(blueprintVAOID);

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
	glUniform1i(coordinateBlueprintXUniform, lbpHighlightBPKey.x);

	GLuint coordinateBlueprintYUniform = glGetUniformLocation(in_programID, "coordinateBlueprintY");
	glUniform1i(coordinateBlueprintYUniform, lbpHighlightBPKey.y);

	GLuint coordinateBlueprintZUniform = glGetUniformLocation(in_programID, "coordinateBlueprintZ");
	glUniform1i(coordinateBlueprintZUniform, lbpHighlightBPKey.z);

	glDrawArrays(GL_TRIANGLES, 0, numberOfVertices * 4);		// draw.

}


void LocalizedHighlighterGearT1::LBPHighlightBufferData::deleteLbpHighlightBuffer()
{
	glDeleteVertexArrays(1, &blueprintVAOID);
	OrganicGLWinUtils::deleteBuffer(&blueprintBufferID);
}

bool LocalizedHighlighterGearT1::LBPUniqueHighlightMap::containsUniqueHighlight(UniqueHighlightEnum in_highlightType)
{
	bool hasHighlight = false;
	auto highlightFinder = uniqueHighlights.find(in_highlightType);
	if (highlightFinder != uniqueHighlights.end())
	{
		hasHighlight = true;
	}
	return hasHighlight;
}

void LocalizedHighlighterGearT1::LBPUniqueHighlightMap::removeHighlight(UniqueHighlightEnum in_highlightType)
{
	// check if the specified highlight exists
	bool highlightFound = false;
	auto highlightFinder = uniqueHighlights.find(in_highlightType);
	if (highlightFinder != uniqueHighlights.end())
	{
		highlightFound = true;
	}

	// if it exists, delete the buffer data, then remove the highlight itself.
	if (highlightFound)
	{
		auto highlightFinder = uniqueHighlights.find(in_highlightType);
		highlightFinder->second.deleteLbpHighlightBuffer();
		uniqueHighlights.erase(in_highlightType);
	}
}

void LocalizedHighlighterGearT1::BPUniqueHighlights::insertNewHighlight(EnclaveKeyDef::EnclaveKey in_bpKey,				// This function will create a new LBPHighlightBufferData,
	UniqueHighlightEnum in_uniqueHighlightValue,	// map it, call the buffer creation buffer, and then
	int in_totalTriangles,							// finally insert the appropriate data for rendering 
	int in_arraySize,								// into the LBPHighlightBufferData.
	GLfloat* in_arrayRef)							//
{
	LBPHighlightBufferData newBufferData(in_bpKey);
	bpHighlights[in_bpKey].uniqueHighlights[in_uniqueHighlightValue] = newBufferData;
	bpHighlights[in_bpKey].uniqueHighlights[in_uniqueHighlightValue].lbpHighlightGenerateBuffer();
	bpHighlights[in_bpKey].uniqueHighlights[in_uniqueHighlightValue].lbpHighightInsertDataIntoBuffer(in_totalTriangles,
		in_arraySize,
		in_arrayRef);

}

void LocalizedHighlighterGearT1::BPUniqueHighlights::scanForAndRemoveCollectionHighlight()	// scan for wherever the UniqueHighlightEnum::CAMERA_COLLECTION might be, and erases it
														// (this is done so that we don't render multiple UniqueHighlightEnum::CAMERA_COLLECTION highlights
														//	from different blueprints)
{
	for (auto& currentBPHighlights : bpHighlights)
	{
		currentBPHighlights.second.removeHighlight(UniqueHighlightEnum::CAMERA_COLLECTION);
	}
}

void LocalizedHighlighterGearT1::BPUniqueHighlights::scanForAndRemoveTargetedBlockHighlight()
{
	for (auto& currentBPHighlights : bpHighlights)
	{
		currentBPHighlights.second.removeHighlight(UniqueHighlightEnum::TARGETED_BLOCK);
	}
}

void LocalizedHighlighterGearT1::BPUniqueHighlights::drawHighlights(GLuint in_programID,
	EnclaveKeyDef::EnclaveKey in_cameraBPKey,
	glm::vec3 in_cameraBPLocalCoord,
	glm::mat4 in_MVP)
{
	GLuint mvpUniform = glGetUniformLocation(in_programID, "MVP");	// find the MVP uniform
	glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, &in_MVP[0][0]);		// set the uniform

	for (auto& currentBPHighlights : bpHighlights)
	{
		for (auto& currentHighlight : currentBPHighlights.second.uniqueHighlights)
		{
			//std::cout << "!! Found highlight to draw!. " << std::endl;
			//std::cout << "Camera key: "; in_cameraBPKey.printKey(); std::cout << std::endl;
			//std::cout << "Camera local: " << in_cameraBPLocalCoord.x << ", " << in_cameraBPLocalCoord.y << ", " << in_cameraBPLocalCoord.z << std::endl;

			currentHighlight.second.lbpHighlightDraw(in_programID,
				in_cameraBPKey,
				in_cameraBPLocalCoord,
				in_MVP);
		}
	}
}


void LocalizedHighlighterGearT1::render()
{
	useProgram();

	// we should be writing to and/or manipulating the default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glEnable(GL_DEPTH_TEST);
	highlightsIndex.drawHighlights(programID,
									gearUniformRegistry.get3DKey("worldCoordBPLocalCameraKey"),
									gearUniformRegistry.getVec3("worldCoordBPLocalCameraCoord"),
									gearUniformRegistry.getMat4("MVP")
								);

	// TEMPORARY: when done drawing all highlights, clear the buffers.
	glDisable(GL_DEPTH_TEST);

}