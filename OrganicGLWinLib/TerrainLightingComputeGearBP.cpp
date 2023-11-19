#include "stdafx.h"
#include "TerrainLightingComputeGearBP.h"
#include "ShaderMachineBase.h"

void TerrainLightingComputeGearBP::initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef, ShaderMachineBase* in_shaderMachineBasePtr)
{
	width = in_width;
	height = in_height;
	window = in_windowRef;

	programIndex.insertDefaultEntry(in_programID);
	programID = programIndex.selectProgram();

	gearMachinePtr = in_shaderMachineBasePtr;

	// get the uniforms; program must have been compiled before this
	mvpHandle = glGetUniformLocation(programID, "MVP");
	//textureUniform = glGetUniformLocation(programID, "terrainTexture");
	globalAmbienceMultiplierUniform = glGetUniformLocation(programID, "globalAmbienceMultiplier");
	worldPosUniform = glGetUniformLocation(programID, "worldPosition");
	atlasWidthUniform = glGetUniformLocation(programID, "atlasTextureWidth");
	atlasTileWidthUniform = glGetUniformLocation(programID, "atlasTileTextureWidth");

	// set up the uniform requests
	GLUniformRequest reqMVP(GLDataType::MAT4, "MVP");
	uniformRequests.push_back(reqMVP);

	GLUniformRequest reqNormalMatrix(GLDataType::MAT3, "NormalMatrix");
	uniformRequests.push_back(reqNormalMatrix);

	GLUniformRequest modelViewMatrix(GLDataType::MAT4, "ModelViewMatrix");
	uniformRequests.push_back(modelViewMatrix);

	GLUniformRequest worldPosition(GLDataType::VEC3, "worldPosition");
	uniformRequests.push_back(worldPosition);

	GLUniformRequest globalAmbience(GLDataType::FLOAT, "globalAmbienceMultiplier");
	uniformRequests.push_back(globalAmbience);

	GLUniformRequest atlasTextureWidth(GLDataType::FLOAT, "atlasTextureWidth");
	uniformRequests.push_back(atlasTextureWidth);

	GLUniformRequest atlasTileTextureWidth(GLDataType::FLOAT, "atlasTileTextureWidth");
	uniformRequests.push_back(atlasTileTextureWidth);

	//std::string deferredDrawRequest = "terrain";
	//multiDrawArrayJobRequests.push_back(deferredDrawRequest);
}

void TerrainLightingComputeGearBP::render()
{
	useProgram();
	writeToGBuffersV2();
	glFlush();
}

void TerrainLightingComputeGearBP::writeToGBuffersV2()
{
	/*
	// For reference, if needed: This was the old code, found in TerrainLightingComputeGearBP::writeToGBuffers()

	glBindFramebuffer(GL_FRAMEBUFFER, deferredFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// remember, GL clear sets the depth buffer values to 1.0f, meaning they are the furthest away (closest to screen is 0.0f)
	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(terrainVaoID);								// bind to the terrain VAO
	setMatrices();
	GLMultiDrawArrayJob jobToUse = getMultiDrawArrayJob("terrain");
	glMultiDrawArrays(GL_TRIANGLES, jobToUse.multiStartIndices.get(), jobToUse.multiVertexCount.get(), jobToUse.drawCount);
	glFinish();
	*/

	glBindFramebuffer(GL_FRAMEBUFFER, deferredFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// remember, GL clear sets the depth buffer values to 1.0f, meaning they are the furthest away (closest to screen is 0.0f)
	glEnable(GL_DEPTH_TEST);

	setMatrices();

	for (auto& currentBlueprint : renderableBlueprints)
	{
		EnclaveKeyDef::EnclaveKey drawKey = currentBlueprint.first;
		//std::cout << "Attempting draw: ";  drawKey.printKey(); std::cout << " || ";
		currentBlueprint.second.drawBuffer();
	}

	glFinish();
}

void TerrainLightingComputeGearBP::setMatrices()
{
	GLuint mvpUniform = glGetUniformLocation(programID, "MVP");	// find the MVP uniform
	glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, &gearUniformRegistry.getMat4("MVP")[0][0]);		// set the uniform

	//glm::mat4 mv = view * model;
	GLuint mvUniform = glGetUniformLocation(programID, "ModelViewMatrix");
	glUniformMatrix4fv(mvUniform, 1, GL_FALSE, &gearUniformRegistry.getMat4("ModelViewMatrix")[0][0]);

	GLuint normalMatrixUniform = glGetUniformLocation(programID, "NormalMatrix");
	glUniformMatrix3fv(normalMatrixUniform, 1, GL_FALSE, &gearUniformRegistry.getMat3("NormalMatrix")[0][0]);

	glUniform3fv(worldPosUniform, 1, &gearUniformRegistry.getVec3("worldPosUniform")[0]);
	glUniform1f(atlasWidthUniform, gearUniformRegistry.getFloat("atlasTextureWidth"));
	glUniform1f(atlasTileWidthUniform, gearUniformRegistry.getFloat("atlasTileTextureWidth"));
	glUniform1f(globalAmbienceMultiplierUniform, gearUniformRegistry.getFloat("globalAmbienceMultiplier"));
}

void TerrainLightingComputeGearBP::passGLuintValue(std::string in_identifier, GLuint in_gluInt)
{
	if (in_identifier == "terrain_main")
	{
		registerNewPersistentBuffer(in_identifier, in_gluInt);	// register the "terrain_main" PERSISTENT buffer
		//terrainBufferID = in_gluInt;		// set the main terrain buffer ID
	}
	else if (in_identifier == "terrain_swap")
	{
		registerNewPersistentBuffer(in_identifier, in_gluInt);	// register the "terrain_swap" PERSISTENT buffer
		//terrainSwapID = in_gluInt;			// set the swap terrain buffer ID
	}
	else if (in_identifier == "deferred_FBO")
	{
		registerNewFBO(in_identifier, in_gluInt);
		deferredFBO = in_gluInt;
	}
}

void TerrainLightingComputeGearBP::executeGearFunction(std::string in_identifier)
{
	if (in_identifier == "setup_terrain_VAO")
	{
		setupTerrainVAO();
	}
}

void TerrainLightingComputeGearBP::setupTerrainVAO()
{
	OrganicGLWinUtils::createAndBindVertexArray(&terrainVaoID);	// create/bind the VAO to terrainVaoID
	glBindBuffer(GL_ARRAY_BUFFER, getPersistentBufferID("terrain_main"));	// bind to the immutable buffer before setting the attribs


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 10, (void*)0);	// world position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 10, (void*)12);   // normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 10, (void*)24);	// calculated UV coordinates from OrganicSystem
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 10, (void*)32);	// tile ID in the atlas
	glEnableVertexAttribArray(3);

	/*
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, (void*)20);
	glEnableVertexAttribArray(2);
	*/
}

void TerrainLightingComputeGearBP::printData()
{
	std::cout << "Atlas width: " << gearUniformRegistry.getFloat("atlasTextureWidth") << std::endl;
	std::cout << "Atlas tile width: " << gearUniformRegistry.getFloat("atlasTileTextureWidth") << std::endl;
}


void TerrainLightingComputeGearBP::sendTerrainDataToGear(TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef)
{
	std::cout << "!!! ---> called TerrainLightingComputeGearBP::sendTerrainDataToGear. " << std::endl;
	insertBlueprintData(in_jobResults, in_arraySize, in_arrayRef);
}

void TerrainLightingComputeGearBP::removeTerrainDataFromGear(EnclaveKeyDef::EnclaveKey in_keyToRemove)
{
	deleteBlueprintData(in_keyToRemove);
}

void TerrainLightingComputeGearBP::BPGearBuffer::generateBuffer()
{

	OrganicGLWinUtils::createBuffer(&blueprintBufferID);

	OrganicGLWinUtils::createAndBindVertexArray(&blueprintVAOID);	// create/bind the VAO to terrainVaoID
	glBindBuffer(GL_ARRAY_BUFFER, blueprintBufferID);	// bind to the immutable buffer before setting the attribs


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 10, (void*)0);	// world position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 10, (void*)12);   // normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 10, (void*)24);	// calculated UV coordinates from OrganicSystem
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 10, (void*)32);	// tile ID in the atlas
	glEnableVertexAttribArray(3);
	bufferGenerated = true;
}

void TerrainLightingComputeGearBP::BPGearBuffer::insertBufferData(TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef)
{
	glBindBuffer(GL_ARRAY_BUFFER, blueprintBufferID);
	glBufferData(GL_ARRAY_BUFFER, in_arraySize, in_arrayRef, GL_STATIC_DRAW);

	// once data is set, create the data for the draw call.
	numberOfVertices = in_jobResults.numberOfTrianglesProduced * 3;	// 3 verts per triangle, obviously.
	std::cout << "!!! Total vertices to render will be: " << in_jobResults.numberOfTrianglesProduced << " times 3. " << std::endl;
	std::cout << "! GL buffer ID: " << blueprintBufferID << std::endl;
	std::cout << "! VAO ID: " << blueprintVAOID << std::endl;
}

void TerrainLightingComputeGearBP::BPGearBuffer::drawBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, blueprintBufferID);
	glBindVertexArray(blueprintVAOID);

	//std::cout << "!!! BPGearBuffer::drawBuffer() attempting draw, values are: blueprintBufferID: " << blueprintBufferID << " | blueprintvAOID: " << blueprintVAOID << " | number of vertices: " << numberOfVertices << std::endl;

	glDrawArrays(GL_TRIANGLES, 0, numberOfVertices);		// draw.
}

void TerrainLightingComputeGearBP::BPGearBuffer::deleteBuffer()
{
	glDeleteVertexArrays(1, &blueprintVAOID);
	OrganicGLWinUtils::deleteBuffer(&blueprintBufferID);
}

void TerrainLightingComputeGearBP::insertBlueprintData(TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef)
{
	std::cout << ">>>>>>>>> Called: TerrainLightingComputeGearBP::insertBlueprintData. " << std::endl;
	// Check if the blueprint exists already. If it doesn't create it.
	auto doesBlueprintExist = renderableBlueprints.find(in_jobResults.collectionKey);
	if (doesBlueprintExist == renderableBlueprints.end())	// it doesn't exist.
	{
		std::cout << "BPGearBuffer: didn't find key, ";
		in_jobResults.collectionKey.printKey();
		std::cout << "; inserting new BPGearBuffer. " << std::endl;

		BPGearBuffer newBuffer;
		renderableBlueprints[in_jobResults.collectionKey] = newBuffer;
		renderableBlueprints[in_jobResults.collectionKey].generateBuffer();
	}
	else
	{
		std::cout << "BPGearBuffer:  key, ";
		in_jobResults.collectionKey.printKey();
		std::cout << " was already found! " << std::endl;
	}

	renderableBlueprints[in_jobResults.collectionKey].insertBufferData(in_jobResults, in_arraySize, in_arrayRef);
}

void TerrainLightingComputeGearBP::deleteBlueprintData(EnclaveKeyDef::EnclaveKey in_keyToRemove)
{
	std::cout << ">>>>>>>>> Called: TerrainLightingComputeGearBP::deleteBlueprintData. " << std::endl;
	auto doesBlueprintExist = renderableBlueprints.find(in_keyToRemove);
	if (doesBlueprintExist != renderableBlueprints.end())	// it exists
	{
		std::cout << "BPGearBuffer: found key to delete: ";
		in_keyToRemove.printKey();
		std::cout << std::endl;

		renderableBlueprints[in_keyToRemove].deleteBuffer();
		renderableBlueprints.erase(in_keyToRemove);
	}
}