#include "stdafx.h"
#include "LocalizedGBufferComputeLoaderGear.h"

void LocalizedGBufferComputeLoaderGear::initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef, ShaderMachineBase* in_shaderMachineBasePtr)
{
	width = in_width;
	height = in_height;
	window = in_windowRef;

	programIndex.insertDefaultEntry(in_programID);
	programID = programIndex.selectProgram();

	gearMachinePtr = in_shaderMachineBasePtr;

	// get the uniforms; program must have been compiled before this
	mvpHandle = glGetUniformLocation(programID, "MVP");
	globalAmbienceMultiplierUniform = glGetUniformLocation(programID, "globalAmbienceMultiplier");
	worldPosUniform = glGetUniformLocation(programID, "worldPosition");
	atlasWidthUniform = glGetUniformLocation(programID, "atlasTextureWidth");
	atlasTileWidthUniform = glGetUniformLocation(programID, "atlasTileTextureWidth");

	// ***************************** the below two requests are Required for localization ***************************
	
	// These uniforms should be calculated every frame, and come from the call to ShaderMachineBase::runMatrixAndDeltaLocalComputations

	// The world camera's true blueprint key, before it was translated to 0,0,0
	GLUniformRequest reqCameraBlueprintKey(GLDataType::KEY3D, "worldCoordBPLocalCameraKey");
	uniformRequests.push_back(reqCameraBlueprintKey);

	// The world camera's true local coordinate, before it was translated to 0,0,0
	GLUniformRequest reqCameraLocalCoord(GLDataType::VEC3, "worldCoordBPLocalCameraCoord");
	uniformRequests.push_back(reqCameraLocalCoord);

	// ***************************** **************************************************** ************************** 

	// set up the uniform requests

	// The values of reqMVP, reqNormalMatrix and worldPosition should 
	// be already updated in the call to SMLocalizedLightingComputeV1::updateUniformRegistry(),
	// before sending data to the gears that need them.

	// reqMVP should be calculated with a view mat4 that is calculated from an origin of 0,0,0
	GLUniformRequest reqMVP(GLDataType::MAT4, "MVP");
	uniformRequests.push_back(reqMVP);

	GLUniformRequest reqNormalMatrix(GLDataType::MAT3, "NormalMatrix");
	uniformRequests.push_back(reqNormalMatrix);

	//GLUniformRequest modelViewMatrix(GLDataType::MAT4, "ModelViewMatrix");
	//uniformRequests.push_back(modelViewMatrix);

	GLUniformRequest worldPosition(GLDataType::VEC3, "worldPosition");
	uniformRequests.push_back(worldPosition);


	// Below: globalAmbienceMultipler is

	GLUniformRequest globalAmbience(GLDataType::FLOAT, "globalAmbienceMultiplier");
	uniformRequests.push_back(globalAmbience);

	GLUniformRequest atlasTextureWidth(GLDataType::FLOAT, "atlasTextureWidth");
	uniformRequests.push_back(atlasTextureWidth);

	GLUniformRequest atlasTileTextureWidth(GLDataType::FLOAT, "atlasTileTextureWidth");
	uniformRequests.push_back(atlasTileTextureWidth);
}

void LocalizedGBufferComputeLoaderGear::render()
{
	useProgram();

	// bind to the custom G-buffer FBO; clear the color/depth bits anad enabled the depth test
	glBindFramebuffer(GL_FRAMEBUFFER, deferredFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// remember, GL clear sets the depth buffer values to 1.0f, meaning they are the furthest away (closest to screen is 0.0f)
	glEnable(GL_DEPTH_TEST);

	// set common uniform values that would be shared across all blueprints being drawn
	setMatrices();

	
	// set blueprint localization uniforms, and draw each blueprint in it's localized space
	localizedBPs.drawBlueprints(programID,
								&gearUniformRegistry,
								gearUniformRegistry.get3DKey("worldCoordBPLocalCameraKey"),
								gearUniformRegistry.getVec3("worldCoordBPLocalCameraCoord"),
								gearUniformRegistry.getMat4("MVP")
							);


	// disable the depth buffer test for now
	glDisable(GL_DEPTH_TEST);
	
}

void LocalizedGBufferComputeLoaderGear::setMatrices()
{

	//glm::mat4 mv = view * model;
	//GLuint mvUniform = glGetUniformLocation(programID, "ModelViewMatrix");
	//glUniformMatrix4fv(mvUniform, 1, GL_FALSE, &gearUniformRegistry.getMat4("ModelViewMatrix")[0][0]);

	// all drawable blueprints should be using the same MVP, normal, and atlas values; there should be no need to set them per-draw call
	GLuint mvpUniform = glGetUniformLocation(programID, "MVP");	// find the MVP uniform
	glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, &gearUniformRegistry.getMat4("MVP")[0][0]);		// set the uniform

	GLuint normalMatrixUniform = glGetUniformLocation(programID, "NormalMatrix");
	glUniformMatrix3fv(normalMatrixUniform, 1, GL_FALSE, &gearUniformRegistry.getMat3("NormalMatrix")[0][0]);

	glUniform3fv(worldPosUniform, 1, &gearUniformRegistry.getVec3("worldPosUniform")[0]);
	glUniform1f(atlasWidthUniform, gearUniformRegistry.getFloat("atlasTextureWidth"));
	glUniform1f(atlasTileWidthUniform, gearUniformRegistry.getFloat("atlasTileTextureWidth"));
	glUniform1f(globalAmbienceMultiplierUniform, gearUniformRegistry.getFloat("globalAmbienceMultiplier"));
}

void LocalizedGBufferComputeLoaderGear::passGLuintValue(std::string in_identifier, GLuint in_gluInt)
{
	if (in_identifier == "deferred_FBO")
	{
		registerNewFBO(in_identifier, in_gluInt);
		deferredFBO = in_gluInt;

		std::cout << "!!!!! LocalizedGBufferComputeLoaderGear: deferredFBO value is: " << deferredFBO << std::endl;
		int defVal = 3;
		std::cin >> defVal;
	}
}

void LocalizedGBufferComputeLoaderGear::executeGearFunction(std::string in_identifier)
{

}

void LocalizedGBufferComputeLoaderGear::printData()
{

}

void LocalizedGBufferComputeLoaderGear::sendTerrainDataToGear(TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef)
{
	std::cout << "!!! ---> called LocalizedGBufferComputeLoaderGear::sendTerrainDataToGear. " << std::endl;
	insertLocalizedBPData(in_jobResults, in_arraySize, in_arrayRef);
}

void LocalizedGBufferComputeLoaderGear::insertLocalizedBPData(TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef)
{
	std::cout << " >>> Called LocalizedGBufferComputeLoaderGear::insertLocalizedBPData. " << std::endl;

	LocalizedGBufferBPData newData(in_jobResults.collectionKey,
							mvpHandle,
							textureUniform,
							worldPosUniform,
							atlasWidthUniform,
							atlasTileWidthUniform,
							globalAmbienceMultiplierUniform);

	// if the buffer doesn't exist, create the buffer AND insert the data.
	auto doesBlueprintExist = localizedBPs.localizedBlueprintsMap.find(in_jobResults.collectionKey);
	if (doesBlueprintExist == localizedBPs.localizedBlueprintsMap.end())
	{
		std::cout << "LocalizedGBufferBPDataMap: didn't find key, ";
		in_jobResults.collectionKey.printKey();
		std::cout << "; inserting new entry into LocalizedGBufferBPDataMap. " << std::endl;



		// remember, the call to insertNewRenderableBlueprint below will also generat the buffer
		localizedBPs.insertNewRenderableBlueprint(in_jobResults.collectionKey,
												in_jobResults.numberOfTrianglesProduced,
												in_arraySize, 
												in_arrayRef,
												newData);

	}

	// if buffer was already allocated, don't bother generating it, and insert blueprint data
	else
	{
		std::cout << "LocalizedGBufferBPDataMap: key, ";
		in_jobResults.collectionKey.printKey();
		std::cout << " was already found! " << std::endl;

		localizedBPs.reloadExistingBlueprintBuffer(in_jobResults.collectionKey,
			in_jobResults.numberOfTrianglesProduced,
			in_arraySize,
			in_arrayRef,
			newData);
	}

}

void LocalizedGBufferComputeLoaderGear::removeTerrainDataFromGear(EnclaveKeyDef::EnclaveKey in_keyToRemove)
{
	localizedBPs.deleteBlueprintFromRendering(in_keyToRemove);
}

void LocalizedGBufferComputeLoaderGear::LocalizedGBufferBPData::generateBlueprintBuffer()
{
	// Create the buffer, and a VAO; build the VAO
	OrganicGLWinUtils::createBuffer(&blueprintBufferID);
	OrganicGLWinUtils::createAndBindVertexArray(&blueprintVAOID);	// create/bind the highlighter VAO
	glBindBuffer(GL_ARRAY_BUFFER, blueprintBufferID);	// bind to the highlighter buffer

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 10, (void*)0);	// world position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 10, (void*)12);   // normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 10, (void*)24);	// calculated UV coordinates from OrganicSystem
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 10, (void*)32);	// tile ID in the atlas
	glEnableVertexAttribArray(3);
}

void LocalizedGBufferComputeLoaderGear::LocalizedGBufferBPData::insertBlueprintDataIntoBuffer(int in_totalTriangles, int in_arraySize, GLfloat* in_arrayRef)
{
	glBindBuffer(GL_ARRAY_BUFFER, blueprintBufferID);
	glBufferData(GL_ARRAY_BUFFER, in_arraySize, in_arrayRef, GL_STATIC_DRAW);
	numberOfLocalizedVertices = in_totalTriangles * 3;
}

void LocalizedGBufferComputeLoaderGear::LocalizedGBufferBPData::drawBlueprintData(GLuint in_programID, 
																				GLUniformRegistry* in_registryPtr, 
																				EnclaveKeyDef::EnclaveKey in_cameraBPKey, 
																				glm::vec3 in_cameraBPLocalCoord, 
																				glm::mat4 in_MVPvalue)
{
	glBindBuffer(GL_ARRAY_BUFFER, blueprintBufferID);
	glBindVertexArray(blueprintVAOID);

	//std::cout << "!! Binding data and drawing, via lbpHighlightDraw; lbp key is: ";
	//lbpHighlightBPKey.printKey();
	//std::cout << std::endl;

	// ******************************************************** Part 1: Localization uniforms

	GLuint mvpUniform = glGetUniformLocation(in_programID, "MVP");	// find the MVP uniform
	glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, &in_MVPvalue[0][0]);		// set the uniform

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
	// This value is static, and doesn't change every frame; the keyForLocalization represents the blueprint key that is used
	// as an offset when comparing to the camera blueprint key (see the logic used in the vertex shader for more info, or check out
	// task F-011)
	GLuint coordinateBlueprintXUniform = glGetUniformLocation(in_programID, "coordinateBlueprintX");
	glUniform1i(coordinateBlueprintXUniform, keyForLocalization.x);

	GLuint coordinateBlueprintYUniform = glGetUniformLocation(in_programID, "coordinateBlueprintY");
	glUniform1i(coordinateBlueprintYUniform, keyForLocalization.y);

	GLuint coordinateBlueprintZUniform = glGetUniformLocation(in_programID, "coordinateBlueprintZ");
	glUniform1i(coordinateBlueprintZUniform, keyForLocalization.z);



	// ******************************************************** Part 2: Deferred uniforms
	
	//GLuint mvpUniform = glGetUniformLocation(in_programID, "MVP");	// find the MVP uniform
	glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, &in_registryPtr->getMat4("MVP")[0][0]);		// set the uniform

	//glm::mat4 mv = view * model;
	GLuint mvUniform = glGetUniformLocation(in_programID, "ModelViewMatrix");
	glUniformMatrix4fv(mvUniform, 1, GL_FALSE, &in_registryPtr->getMat4("ModelViewMatrix")[0][0]);

	GLuint normalMatrixUniform = glGetUniformLocation(in_programID, "NormalMatrix");
	glUniformMatrix3fv(normalMatrixUniform, 1, GL_FALSE, &in_registryPtr->getMat3("NormalMatrix")[0][0]);

	glUniform3fv(worldPosUniform, 1, &in_registryPtr->getVec3("worldPosUniform")[0]);
	glUniform1f(atlasWidthUniform, in_registryPtr->getFloat("atlasTextureWidth"));
	glUniform1f(atlasTileWidthUniform, in_registryPtr->getFloat("atlasTileTextureWidth"));
	glUniform1f(globalAmbienceMultiplierUniform, in_registryPtr->getFloat("globalAmbienceMultiplier"));
	


	glDrawArrays(GL_TRIANGLES, 0, numberOfLocalizedVertices * 4);		// draw.
}

void LocalizedGBufferComputeLoaderGear::LocalizedGBufferBPData::deleteBlueprintBufferAndData()
{
	glDeleteVertexArrays(1, &blueprintVAOID);
	OrganicGLWinUtils::deleteBuffer(&blueprintBufferID);
}

void LocalizedGBufferComputeLoaderGear::LocalizedGBufferBPDataMap::insertNewRenderableBlueprint(EnclaveKeyDef::EnclaveKey in_localizedBlueprintKey,
	int in_totalTriangles,							// finally insert the appropriate data for rendering 
	int in_arraySize,								// into the LBPHighlightBufferData.
	GLfloat* in_arrayRef,
	LocalizedGBufferBPData in_bpData)
{
	localizedBlueprintsMap[in_localizedBlueprintKey] = in_bpData;
	localizedBlueprintsMap[in_localizedBlueprintKey].generateBlueprintBuffer();
	localizedBlueprintsMap[in_localizedBlueprintKey].insertBlueprintDataIntoBuffer(in_totalTriangles,
		in_arraySize,
		in_arrayRef);
}

void LocalizedGBufferComputeLoaderGear::LocalizedGBufferBPDataMap::reloadExistingBlueprintBuffer(EnclaveKeyDef::EnclaveKey in_localizedBlueprintKey,
	int in_totalTriangles,							// finally insert the appropriate data for rendering 
	int in_arraySize,								// into the LBPHighlightBufferData.
	GLfloat* in_arrayRef,
	LocalizedGBufferBPData in_bpData)
{
	localizedBlueprintsMap[in_localizedBlueprintKey].deleteBlueprintBufferAndData();
	localizedBlueprintsMap[in_localizedBlueprintKey] = in_bpData;
	localizedBlueprintsMap[in_localizedBlueprintKey].generateBlueprintBuffer();
	localizedBlueprintsMap[in_localizedBlueprintKey].insertBlueprintDataIntoBuffer(in_totalTriangles,
		in_arraySize,
		in_arrayRef);
}

void LocalizedGBufferComputeLoaderGear::LocalizedGBufferBPDataMap::drawBlueprints(GLuint in_programID,
	GLUniformRegistry* in_registryPtr,
	EnclaveKeyDef::EnclaveKey in_cameraBPKey,
	glm::vec3 in_cameraBPLocalCoord,
	glm::mat4 in_MVP)
{
	for (auto& currentBlueprint : localizedBlueprintsMap)
	{
		currentBlueprint.second.drawBlueprintData(in_programID,
			in_registryPtr,
			in_cameraBPKey,
			in_cameraBPLocalCoord,
			in_MVP);
	}
}

void LocalizedGBufferComputeLoaderGear::LocalizedGBufferBPDataMap::deleteBlueprintFromRendering(EnclaveKeyDef::EnclaveKey in_localizedBlueprintKeyToErase)
{
	std::cout << ">>>>>>>>>> Called: LocalizedGBufferComputeLoaderGear::LocalizedGBufferBPDataMap::deleteBlueprintFromRendering. " << std::endl;
	auto doesBlueprintExist = localizedBlueprintsMap.find(in_localizedBlueprintKeyToErase);
	if (doesBlueprintExist != localizedBlueprintsMap.end())
	{
		localizedBlueprintsMap[in_localizedBlueprintKeyToErase].deleteBlueprintBufferAndData();
		localizedBlueprintsMap.erase(in_localizedBlueprintKeyToErase);
	}
}