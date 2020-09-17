#include "stdafx.h"
#include "ShaderMachineBase.h"

double ShaderMachineBase::lastTime = glfwGetTime();
GLuint ShaderMachineBase::getPersistentBufferID(std::string in_bufferName)
{
	GLuint returnGLuint;
	int lookupID = persistentBufferLookup[in_bufferName];
	returnGLuint = persistentBufferMap[lookupID];
	return returnGLuint;
}

GLuint ShaderMachineBase::getFBOID(std::string in_fboName)
{
	GLuint returnGLuint;
	int lookupID = fboLookup[in_fboName];
	returnGLuint = fboMap[lookupID];
	return returnGLuint;
}

GLuint ShaderMachineBase::getTextureID(std::string in_textureName)
{
	GLuint returnGLuint;
	int lookupID = textureLookup[in_textureName];
	returnGLuint = textureMap[lookupID];
	return returnGLuint;
}

GLuint& ShaderMachineBase::getTextureLValueRef(std::string in_textureName)
{
	int lookupID = textureLookup[in_textureName];
	GLuint& returnLValue = textureMap[lookupID];
	return returnLValue;
}

AtlasMap* ShaderMachineBase::getAtlasMapRef(std::string in_atlasName)
{
	int lookupID = atlasMapLookup[in_atlasName];
	return &atlasMapMap[lookupID];
}

GLuint* ShaderMachineBase::getTextureRef(std::string in_textureName)
{
	GLuint* returnGLuint;
	int lookupID = textureLookup[in_textureName];
	returnGLuint = &textureMap[lookupID];
	return returnGLuint;
}

GLuint* ShaderMachineBase::getTerrainBufferRef()
{
	GLuint* returnGLuint;
	int lookupID = persistentBufferLookup["terrain_main"];
	returnGLuint = &persistentBufferMap[lookupID];
	return returnGLuint;
}

GLuint* ShaderMachineBase::getTerrainSwapRef()
{
	GLuint* returnGLuint;
	int lookupID = persistentBufferLookup["terrain_swap"];
	returnGLuint = &persistentBufferMap[lookupID];
	return returnGLuint;
}

GLuint ShaderMachineBase::getTerrainBufferID()
{
	GLuint returnGLuint;
	int lookupID = persistentBufferLookup["terrain_main"];
	returnGLuint = persistentBufferMap[lookupID];
	return returnGLuint;
}
GLuint ShaderMachineBase::getTerrainSwapID()
{
	GLuint returnGLuint;
	int lookupID = persistentBufferLookup["terrain_swap"];
	returnGLuint = persistentBufferMap[lookupID];
	return returnGLuint;
}

GLMultiDrawArrayJob* ShaderMachineBase::getMultiDrawArrayJobRef(std::string in_jobName)
{
	GLMultiDrawArrayJob* returnJob;
	int lookupID = multiDrawArrayJobLookup[in_jobName];
	returnJob = &multiDrawArrayJobMap[lookupID];
	return returnJob;
}


GLMultiDrawArrayJob ShaderMachineBase::getMultiDrawArrayJob(std::string in_jobName)
{
	GLMultiDrawArrayJob returnJob;
	int lookupID = multiDrawArrayJobLookup[in_jobName];
	returnJob = multiDrawArrayJobMap[lookupID];
	return returnJob;
}

GLDrawElementsInstancedJob ShaderMachineBase::getDrawElementsInstancedJob(std::string in_jobName)
{
	GLDrawElementsInstancedJob returnJob;
	int lookupID = drawElementsInstancedJobLookup[in_jobName];
	returnJob = drawElementsInstancedJobMap[lookupID];
	return returnJob;
}

GLuint ShaderMachineBase::getBufferID(std::string in_bufferName)
{
	GLuint returnGLuint;
	int lookupID = bufferLookup[in_bufferName];
	returnGLuint = bufferMap[lookupID];
	return returnGLuint;
}

void ShaderMachineBase::sendDataToPersistentBuffer(std::string in_bufferName, int in_offset, int in_byteSizeToWrite, GLfloat* in_dataArray)
{
	glBindBuffer(GL_ARRAY_BUFFER, getPersistentBufferID(in_bufferName));				// bind to the specified buffer
	glBufferSubData(GL_ARRAY_BUFFER, in_offset, in_byteSizeToWrite, in_dataArray);		// send the data to the buffer
}

void ShaderMachineBase::sendDataToBuffer(std::string in_bufferName, int in_offset, int in_byteSizeToWrite, GLfloat* in_dataArray)
{
	glBindBuffer(GL_ARRAY_BUFFER, getBufferID(in_bufferName));				// bind to the specified buffer
	//glBufferSubData(GL_ARRAY_BUFFER, in_offset, in_byteSizeToWrite, in_dataArray);		// send the data to the buffer
	//glBufferData(GL_ARRAY_BUFFER, 6 * 7 * sizeof(float), quadData, GL_STATIC_DRAW);		// populate the data
	glBufferData(GL_ARRAY_BUFFER, in_byteSizeToWrite, in_dataArray, GL_STATIC_DRAW);
}

void ShaderMachineBase::sendMat4DataToBuffer(std::string in_bufferName, int in_offset, int in_byteSizeToWrite, glm::mat4* in_dataArray)
{
	glBindBuffer(GL_ARRAY_BUFFER, getBufferID(in_bufferName));				// bind to the specified buffer
	//glBufferSubData(GL_ARRAY_BUFFER, in_offset, in_byteSizeToWrite, in_dataArray);		// send the data to the buffer
	//glBufferData(GL_ARRAY_BUFFER, 6 * 7 * sizeof(float), quadData, GL_STATIC_DRAW);		// populate the data
	glBufferData(GL_ARRAY_BUFFER, in_byteSizeToWrite, in_dataArray, GL_STATIC_DRAW);
}

int ShaderMachineBase::getVaoAttribMode()
{
	return vaoAttribMode;
}
int ShaderMachineBase::getVaoAttribByteSize()
{
	return vaoAttribByteSize;
}

void ShaderMachineBase::computeMatricesFromInputs()
{
	// Compute time difference between current and last frame
	currentTime = glfwGetTime();
	deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	int isFocused = glfwGetWindowAttrib(window, GLFW_FOCUSED);

	if (isFocused == 1)
	{
		glfwGetCursorPos(window, &xpos, &ypos);

		// Reset mouse position for next frame
		glfwSetCursorPos(window, width / 2, height / 2);





		//std::cout << "OpenGL is FOCUSED!" << std::endl;


		// Compute new orientation
		horizontalAngle += mouseSpeed * float(width / 2 - xpos);
		verticalAngle += mouseSpeed * float(height / 2 - ypos);

		// Direction : Spherical coordinates to Cartesian coordinates conversion
		direction = glm::vec3(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);
	}

	//std::cout << "Direction vector: " << direction.x << ", " << direction.y << ", " << direction.z << endl;

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	// Up vector
	up = glm::cross(right, direction);

	if (isFocused == 1)
	{
		// Move forward
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			position += direction * deltaTime * speed;
		}
		// Move backward
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			position -= direction * deltaTime * speed;
		}
		// Strafe right
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			position += right * deltaTime * speed;
		}
		// Strafe left
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			position -= right * deltaTime * speed;
		}
	}
}

void ShaderMachineBase::updateMatricesAndDelta()
{
	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// projection matrix : 45° Field of view, 4:3 ratio, display range : 0.1 unit <-> 100 units
	projection = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 400.0f);

	// Camera matrix

	
	view = glm::lookAt(
		position,           // Camera is here
		position + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
	);
	

	// new
	// REWORK BEGINS HERE; USE THIS NOW. (8/23/2020)
	
	/*
	glm::vec3 dummyposition;
	dummyposition.x = 0.0f;
	dummyposition.y = 0.0f;
	dummyposition.z = 0.0f;
	view = glm::lookAt(
		dummyposition,
		dummyposition + direction,
		up
	);
	*/

	// For the next frame, the "last time" will be "now"
	model = glm::mat4(1.0);
	MVP = projection * view;		// model is NOT needed here for terrain drawing, since terrain data is in world space already.


	lastTime = currentTime;
}

void ShaderMachineBase::runGearTrain()
{
	//std::cout << "## Running gear train... " << std::endl;
	auto gearTrainBegin = gearTrain.begin();
	auto gearTrainEnd = gearTrain.end();
	for (gearTrainBegin; gearTrainBegin != gearTrainEnd; gearTrainBegin++)
	{
		//std::cout << "Requesting render for gear: " << gearTrainBegin->first << std::endl;
		gearTrainBegin->second->render();
	}
	//std::cout << "## Gear train run complete. " << std::endl;
}

void ShaderMachineBase::sendGearUniforms()
{
	auto gearTrainBegin = gearTrain.begin();
	auto gearTrainEnd = gearTrain.end();
	for (gearTrainBegin; gearTrainBegin != gearTrainEnd; gearTrainBegin++)
	{
		std::vector<GLUniformRequest>* currentRequests = gearTrainBegin->second.get()->getUniformRequests();
		std::vector<GLUniformRequest> deRefedRequests = *currentRequests;
		auto currentBegin = deRefedRequests.begin();
		auto currentEnd = deRefedRequests.end();
		
		for (currentBegin; currentBegin != currentEnd; currentBegin++)
		{
			GLUniformRequest currentRequest = *currentBegin;
			if (currentRequest.dataType == GLDataType::VEC2)
			{
				glm::vec2 dataToSend = uniformRegistry.getVec2(currentRequest.uniformName);
				gearTrainBegin->second.get()->sendUniform<GLDataType::VEC2, glm::vec2>(currentRequest.uniformName, dataToSend);
			}
			else if (currentRequest.dataType == GLDataType::VEC3)
			{
				glm::vec3 dataToSend = uniformRegistry.getVec3(currentRequest.uniformName);
				gearTrainBegin->second.get()->sendUniform<GLDataType::VEC3, glm::vec3>(currentRequest.uniformName, dataToSend);
			}
			else if (currentRequest.dataType == GLDataType::MAT3)
			{
				glm::mat3 dataToSend = uniformRegistry.getMat3(currentRequest.uniformName);
				gearTrainBegin->second.get()->sendUniform<GLDataType::MAT3, glm::mat3>(currentRequest.uniformName, dataToSend);
			}

			else if (currentRequest.dataType == GLDataType::MAT4)
			{
				glm::mat4 dataToSend = uniformRegistry.getMat4(currentRequest.uniformName);
				gearTrainBegin->second.get()->sendUniform<GLDataType::MAT4, glm::mat4>(currentRequest.uniformName, dataToSend);
			}
			else if (currentRequest.dataType == GLDataType::FLOAT)
			{
				float dataToSend = uniformRegistry.getFloat(currentRequest.uniformName);
				gearTrainBegin->second.get()->sendUniform<GLDataType::FLOAT, float>(currentRequest.uniformName, dataToSend);
			}
			else if (currentRequest.dataType == GLDataType::INT)
			{
				int dataToSend = uniformRegistry.getInt(currentRequest.uniformName);
				gearTrainBegin->second.get()->sendUniform<GLDataType::INT, int>(currentRequest.uniformName, dataToSend);
			}
			
		}
	}
}

void ShaderMachineBase::sendDrawJobs()
{
	auto gearTrainBegin = gearTrain.begin();
	auto gearTrainEnd = gearTrain.end();
	for (gearTrainBegin; gearTrainBegin != gearTrainEnd; gearTrainBegin++)
	{
		sendMultiDrawArrayJobRequests(gearTrainBegin->second.get());				// if the gear has any multi draw array requests, send them.
		sendDrawElementsInstancedRequests(gearTrainBegin->second.get());			// if the gear has any instanced drawing requests, send them.
	}
}

void ShaderMachineBase::sendMultiDrawArrayJobRequests(Gear* in_gearRef)
{
	auto currentDrawJobRequests = in_gearRef->getMultiDrawArrayJobRequests();
	auto deRefedRequests = *currentDrawJobRequests;
	auto currentBegin = deRefedRequests.begin();
	auto currentEnd = deRefedRequests.end();
	for (currentBegin; currentBegin != currentEnd; currentBegin++)
	{
		auto foundResult = multiDrawArrayJobLookup.find(*currentBegin);			// only send if the requested job actually exists (will produce erratic OpenGL data if we send a job that is built but hasn't been initialized yet, 
																				// which is what would happen below if we didnt' do this, since getMultiDrawArrayJob assumes that it exists already)
		if (foundResult != multiDrawArrayJobLookup.end())
		{
			GLMultiDrawArrayJob jobToSend = getMultiDrawArrayJob(*currentBegin);	// get the job to send
			in_gearRef->insertMultiDrawArrayJob(*currentBegin, jobToSend);
		}
	}
}

void ShaderMachineBase::sendDrawElementsInstancedRequests(Gear* in_gearRef)
{
	auto currentDrawJobRequests = in_gearRef->getDrawElementsInstancedRequests();
	auto deRefedRequests = *currentDrawJobRequests;
	auto currentBegin = deRefedRequests.begin();
	auto currentEnd = deRefedRequests.end();
	for (currentBegin; currentBegin != currentEnd; currentBegin++)
	{
		auto foundResult = drawElementsInstancedJobLookup.find(*currentBegin);			// only send if the requested job actually exists (will produce erratic OpenGL data if we send a job that is built but hasn't been initialized yet, 
																				// which is what would happen below if we didnt' do this, since getMultiDrawArrayJob assumes that it exists already)
		if (foundResult != drawElementsInstancedJobLookup.end())
		{
			//std::cout << "Sending instanced job request! " << std::endl;
			GLDrawElementsInstancedJob jobToSend = getDrawElementsInstancedJob(*currentBegin);	// get the job to send
			in_gearRef->insertDrawElementsInstancedJob(*currentBegin, jobToSend);
		}
	}
}


void ShaderMachineBase::registerDrawJob(std::string in_drawJobName, GLint* in_startArray, GLsizei* in_vertexCount, int in_numberOfCollections)
{
	GLMultiDrawArrayJob newJob;
	newJob.updateDrawArrayData(0, in_startArray, in_vertexCount, in_numberOfCollections);
	//std::cout << "!!!!!!!!! (Machine) job vertex count: " << in_vertexCount[0] << std::endl;
	insertNewMultiDrawArrayJob(in_drawJobName, newJob);
}

void ShaderMachineBase::registerDrawElementsInstancedJob(std::string in_instancedJobName, int in_numberOfElements)
{
	GLDrawElementsInstancedJob newJob;
	newJob.numberOfElementsToRender = in_numberOfElements;
	insertNewDrawElementsInstancedJob(in_instancedJobName, newJob);
}

void ShaderMachineBase::swapAndPoll()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void ShaderMachineBase::insertNewPersistentBuffer(std::string in_bufferName, int in_size)
{
	int currentSize = persistentBufferMap.size();	// get the index we will use
	persistentBufferMap[currentSize] = 0;			// initialize to 0; but it will get changed.
	OrganicGLWinUtils::createImmutableBuffer(&persistentBufferMap[currentSize], in_size, 1);	// pass a reference to the GLuint to bind to
	persistentBufferLookup[in_bufferName] = currentSize;								// insert the lookup value
	std::cout << "!!!!!!! ################ persistent buffer ID is: " << persistentBufferMap[currentSize] << std::endl;
}

void ShaderMachineBase::insertNewFBO(std::string in_fboName)
{
	int currentSize = fboMap.size();
	fboMap[currentSize] = 0;	// initialize to 0; but will be changed in next function call
	OrganicGLWinUtils::createFBO(&fboMap[currentSize]);	// create the FBO; only creates, doesn't do anything special
	fboLookup[in_fboName] = currentSize;	// insert the lookup value
}

void ShaderMachineBase::insertNewBuffer(std::string in_bufferName)
{
	int currentSize = bufferMap.size();
	bufferMap[currentSize] = 0;
	OrganicGLWinUtils::createBuffer(&bufferMap[currentSize]);
	bufferLookup[in_bufferName] = currentSize;
}

void ShaderMachineBase::insertNewTexture(std::string in_textureName)
{
	int currentSize = textureMap.size();
	textureMap[currentSize] = 0;
	textureLookup[in_textureName] = currentSize;
}

void ShaderMachineBase::insertAndBuildNewAtlas(std::string in_atlasFolderName, GLuint* in_atlasTextureRef, float* in_atlasTileWidth, float* in_atlasWidth)
{
	int currentSize = atlasMapMap.size();
	AtlasMap newMap;
	atlasMapMap[currentSize] = newMap;
	atlasMapMap[currentSize].buildAtlas(in_atlasFolderName, in_atlasTextureRef, in_atlasTileWidth, in_atlasWidth);

	atlasMapLookup[in_atlasFolderName] = currentSize;
}

void ShaderMachineBase::insertAndBuildNewAtlasToSpecifiedTextureChannel(GLenum in_texUnit, std::string in_atlasFolderName, GLuint* in_atlasTextureRef, float* in_atlasTileWidth, float* in_atlasWidth)
{
	int currentSize = atlasMapMap.size();
	AtlasMap newMap;
	atlasMapMap[currentSize] = newMap;
	atlasMapMap[currentSize].buildAtlasOnTextureUnit(in_texUnit, in_atlasFolderName, in_atlasTextureRef, in_atlasTileWidth, in_atlasWidth);

	atlasMapLookup[in_atlasFolderName] = currentSize;
}

void ShaderMachineBase::insertNewMultiDrawArrayJob(std::string in_jobName, GLMultiDrawArrayJob in_job)
{
	int currentSize = multiDrawArrayJobMap.size();
	int targetKeyValue = currentSize;


	auto doesJobExist = multiDrawArrayJobLookup.find(in_jobName);
	if (doesJobExist != multiDrawArrayJobLookup.end())
	{

		targetKeyValue = doesJobExist->second;
		multiDrawArrayJobMap[targetKeyValue] = in_job;
		//multiDrawArrayJobLookup[in_jobName] = targetKeyValue;
	}
	else
	{
		multiDrawArrayJobMap[targetKeyValue] = in_job;
		multiDrawArrayJobLookup[in_jobName] = targetKeyValue;
	}
}

void ShaderMachineBase::insertNewDrawElementsInstancedJob(std::string in_jobName, GLDrawElementsInstancedJob in_job)
{
	int currentSize = drawElementsInstancedJobMap.size();
	int targetKeyValue = currentSize;

	auto doesJobExist = drawElementsInstancedJobLookup.find(in_jobName);
	if (doesJobExist != drawElementsInstancedJobLookup.end())
	{
		targetKeyValue = doesJobExist->second;
		drawElementsInstancedJobMap[targetKeyValue] = in_job;
	}
	else
	{
		drawElementsInstancedJobMap[targetKeyValue] = in_job;
		drawElementsInstancedJobLookup[in_jobName] = targetKeyValue;
	}
}

void ShaderMachineBase::createProgram(std::string in_programName)
{
	int currentSize = programMap.size();
	programMap[currentSize] = 0;
	OrganicGLWinUtils::loadShadersViaMode(&programMap[currentSize], in_programName);
	programLookup[in_programName] = programMap[currentSize];
}

void ShaderMachineBase::createComputeProgram(std::string in_programName)
{
	int currentSize = programMap.size();
	programMap[currentSize] = 0;
	OrganicGLWinUtils::loadComputeShader(&programMap[currentSize], in_programName);
	programLookup[in_programName] = programMap[currentSize];
}

glm::vec3* ShaderMachineBase::getPosition()
{
	return &position;
}

glm::vec3* ShaderMachineBase::getDirection()
{
	return &direction;
}

GLFWwindow* ShaderMachineBase::getWindow()
{
	return window;
}

int ShaderMachineBase::getBufferSize()
{
	return terrainBufferSize;
}