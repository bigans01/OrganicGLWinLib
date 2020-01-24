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

	// For the next frame, the "last time" will be "now"
	model = glm::mat4(1.0);
	MVP = projection * view;		// model is NOT needed here for terrain drawing, since terrain data is in world space already.


	lastTime = currentTime;
}

void ShaderMachineBase::runGearTrain()
{
	auto gearTrainBegin = gearTrain.begin();
	auto gearTrainEnd = gearTrain.end();
	for (gearTrainBegin; gearTrainBegin != gearTrainEnd; gearTrainBegin++)
	{
		gearTrainBegin->second->render();
	}
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
			// below are sample calls; save these somewhere
			/*
			glm::vec2 someVec2;
			gearTrainBegin->second.get()->sendUniform<GLDataType::VEC2, glm::vec2>("vec2", someVec2);

			glm::vec3 someVec;
			gearTrainBegin->second.get()->sendUniform<GLDataType::VEC3, glm::vec3>("world", someVec);

			glm::mat4 someMat = glm::mat4(1.0);
			gearTrainBegin->second.get()->sendUniform<GLDataType::MAT4, glm::mat4>("MVP", someMat);

			float someFloat = 0.0f;
			gearTrainBegin->second.get()->sendUniform<GLDataType::FLOAT, float>("testFloat", someFloat);
			*/
			
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
			
		}
	}
}

void ShaderMachineBase::sendDrawJobs()
{
	auto gearTrainBegin = gearTrain.begin();
	auto gearTrainEnd = gearTrain.end();
	for (gearTrainBegin; gearTrainBegin != gearTrainEnd; gearTrainBegin++)
	{
		std::vector<std::string>* currentDrawJobRequests = gearTrainBegin->second.get()->getMultiDrawArrayJobRequests();
		std::vector<std::string> deRefedRequests = *currentDrawJobRequests;
		auto currentBegin = deRefedRequests.begin();
		auto currentEnd = deRefedRequests.end();
		for (currentBegin; currentBegin != currentEnd; currentBegin++)
		{
			GLMultiDrawArrayJob jobToSend = getMultiDrawArrayJob(*currentBegin);	// get the job to send
			gearTrainBegin->second.get()->insertMultiDrawArrayJob(*currentBegin, jobToSend);
		}
	}
}

void ShaderMachineBase::registerDrawJob(std::string in_drawJobName, GLint* in_startArray, GLsizei* in_vertexCount, int in_numberOfCollections)
{
	GLMultiDrawArrayJob newJob;
	newJob.updateDrawArrayData(0, in_startArray, in_vertexCount, in_numberOfCollections);
	insertNewMultiDrawArrayJob(in_drawJobName, newJob);
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

void ShaderMachineBase::insertNewMultiDrawArrayJob(std::string in_jobName)
{
	int currentSize = multiDrawArrayJobMap.size();
	GLMultiDrawArrayJob arrayJob;
	multiDrawArrayJobMap[currentSize] = arrayJob;	// copy the job into the map (it will be empty at first, remember we are just registering it)
	multiDrawArrayJobLookup[in_jobName] = currentSize;
}

void ShaderMachineBase::insertNewMultiDrawArrayJob(std::string in_jobName, GLMultiDrawArrayJob in_job)
{
	int currentSize = multiDrawArrayJobMap.size();
	multiDrawArrayJobMap[currentSize] = in_job;	// copy the job into the map (it will be empty at first, remember we are just registering it)
	multiDrawArrayJobLookup[in_jobName] = currentSize;
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