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

void ShaderMachineBase::checkForClickedButtons()
{
	machineFeedback += buttonPanelContainer.checkAllPanelsForClickResults();
}

void ShaderMachineBase::checkForTextInput()
{
	machineFeedback += inputPanelContainer.checkForInput();
}

ShaderMachineFeedback ShaderMachineBase::retrieveShaderInputs()
{
	return std::move(machineFeedback);
}

void ShaderMachineBase::renderSliders()
{

	machineFeedback += sliderPanelContainer.runAllSliders();
}

void ShaderMachineBase::renderWorldLocation(float world_precise[3], int world_organicLoc[9])
{
	// render it, and get the return value. If true, it means the mouse was in this specific window.

	machineFeedback += OrganicGLWinUtils::IMGuiPrepWorldLocation(world_precise, world_organicLoc);
}

void ShaderMachineBase::renderTargetedBlockLocation(int world_organicLoc[9])
{
	// render it, and get the return value. If true, it means the mouse was in this specific window.

	machineFeedback += OrganicGLWinUtils::IMGuiPrepBlockLocation(world_organicLoc);
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

void ShaderMachineBase::sendDataToBuffer(std::string in_bufferName, int in_byteSizeToWrite, GLfloat* in_dataArray)
{
	glBindBuffer(GL_ARRAY_BUFFER, getBufferID(in_bufferName));				// bind to the specified buffer
	//glBufferSubData(GL_ARRAY_BUFFER, in_offset, in_byteSizeToWrite, in_dataArray);		// send the data to the buffer
	//glBufferData(GL_ARRAY_BUFFER, 6 * 7 * sizeof(float), quadData, GL_STATIC_DRAW);		// populate the data
	glBufferData(GL_ARRAY_BUFFER, in_byteSizeToWrite, in_dataArray, GL_STATIC_DRAW);
}

void ShaderMachineBase::sendMat4DataToBuffer(std::string in_bufferName, int in_byteSizeToWrite, glm::mat4* in_dataArray)
{
	glBindBuffer(GL_ARRAY_BUFFER, getBufferID(in_bufferName));				// bind to the specified buffer
	//glBufferSubData(GL_ARRAY_BUFFER, in_offset, in_byteSizeToWrite, in_dataArray);		// send the data to the buffer
	//glBufferData(GL_ARRAY_BUFFER, 6 * 7 * sizeof(float), quadData, GL_STATIC_DRAW);		// populate the data
	glBufferData(GL_ARRAY_BUFFER, in_byteSizeToWrite, in_dataArray, GL_STATIC_DRAW);
}

void ShaderMachineBase::sendDataToSSBOBuffer(std::string in_bufferName, int in_byteSizeToWrite, GLfloat* in_dataArray)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, getBufferID(in_bufferName));
	glBufferData(GL_SHADER_STORAGE_BUFFER, in_byteSizeToWrite, in_dataArray, GL_DYNAMIC_COPY);
}

void ShaderMachineBase::sendDataToSSBOBufferSub(std::string in_bufferName, int in_offset, int in_byteSizeToWrite, GLfloat* in_dataArray)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, getBufferID(in_bufferName));
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, in_offset, in_byteSizeToWrite, in_dataArray);
}

int ShaderMachineBase::getVaoAttribMode()
{
	return vaoAttribMode;
}
int ShaderMachineBase::getVaoAttribByteSize()
{
	return vaoAttribByteSize;
}

void ShaderMachineBase::computeMatricesFromInputs(bool in_imguiFocusedFlag)
{

	// Compute time difference between current and last frame
	currentTime = glfwGetTime();
	deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	int isFocused = glfwGetWindowAttrib(window, GLFW_FOCUSED);

	// always set wasFocusedPreviousFrame to false if it isn't focused.
	if (isFocused == 0)
	{
		wasFocusedPreviousFrame = false;
	}


	// do this check when the gets re-focused; i.e., the user clicks the window.
	if
	(
		(isFocused == 1)
		&&
		(wasFocusedPreviousFrame == false)
		//&&
		//(cameraBoundToMousePointer == true)
	)
	{
		glfwSetCursorPos(window, width / 2, height / 2);
		wasFocusedPreviousFrame = true;
		cameraBoundToMousePointer = true;
	}

	if 
	(
		(isFocused == 1)
		&&
		(cameraBoundToMousePointer == true)
	)
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

	int someVal = 3;
	//std::cout << "Direction vector: " << direction.x << ", " << direction.y << ", " << direction.z << std::endl;
	//std::cout << "Horizontal angle: " << horizontalAngle << std::endl;
	//std::cout << "Vertical angle: " << verticalAngle << std::endl;

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

		// camera toggling
		//if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		//{
			//toggleCameraBoundToMousePointer();
		//}
	}
}

void ShaderMachineBase::keyCallbackWrapper(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	static_cast<ShaderMachineBase*>(glfwGetWindowUserPointer(window))->keyCallback(window, key, scancode, action, mods);
}

void ShaderMachineBase::resetFeedback()
{
	ShaderMachineFeedback newFeedback;
	machineFeedback = newFeedback;
}

void ShaderMachineBase::handleKeyPressInputs()
{
	bool voidOutKeyPresses = false;
	if (machineFeedback.wasInputTextModified == true)
	{
		voidOutKeyPresses = true;	// void out key presses if any input text was received.
	}
	keyTracker.handleKeyPressTransfers(voidOutKeyPresses);
}

void ShaderMachineBase::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	int isFocused = glfwGetWindowAttrib(window, GLFW_FOCUSED);

	// only listen for input from keyboard, when the GLFW window is focused.
	//if (isFocused == 1)
	//{
		if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS)
		{
			toggleCameraBoundToMousePointer();
		}

		// new, for KeyPressTracker keyTracker:

		if (action == GLFW_PRESS)
		{
			keyTracker.insertCycle(key);
		}
		else if (action == GLFW_RELEASE)
		{
			keyTracker.killCycle(key);
		}
	//}
}

void ShaderMachineBase::mouseScrollCallbackWrapper(GLFWwindow* window, double xoffset, double yoffset)
{
	static_cast<ShaderMachineBase*>(glfwGetWindowUserPointer(window))->mouseScrollCallback(window, xoffset, yoffset);
}

void ShaderMachineBase::mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	std::cout << "!!! Scroll detected; yoffset is: " << yoffset << std::endl;
	machineFeedback.mouseFeedback[ShaderMachineFeedbackType::MOUSE_SCROLL] = float(yoffset);
	//Message scrollValue(MessageType::REQUEST_FROM_CLIENT_MOUSE_SCROLL_INPUT_GENERATED);
	if (yoffset == 1.0f)
	{
		std::cout << "!!! Positive mouse scroll detected. " << std::endl;
		keyTracker.insertOneOffCycle(GLFW_MOUSE_SCROLL_POSITIVE);
	}
	else if (yoffset == -1.0f)
	{
		std::cout << "!!! Negative mouse scroll detected. " << std::endl;
		keyTracker.insertOneOffCycle(GLFW_MOUSE_SCROLL_NEGATIVE);
	}
}

void ShaderMachineBase::mouseButtonCallbackWrapper(GLFWwindow* window, int button, int action, int mods)
{
	static_cast<ShaderMachineBase*>(glfwGetWindowUserPointer(window))->mouseButtonCallback(window, button, action, mods);
}

void ShaderMachineBase::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		keyTracker.insertCycle(button);
	}
	else if (action == GLFW_RELEASE)
	{
		keyTracker.killCycle(button);
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

void ShaderMachineBase::updateWaveUniforms()
{
	waveManager.updateWaveValuesInRegistry(&uniformRegistry);
}

void ShaderMachineBase::sendDrawJobs()
{
	auto gearTrainBegin = gearTrain.begin();
	auto gearTrainEnd = gearTrain.end();
	for (gearTrainBegin; gearTrainBegin != gearTrainEnd; gearTrainBegin++)
	{
		sendMultiDrawArrayJobRequests(gearTrainBegin->first, gearTrainBegin->second.get());				// if the gear has any multi draw array requests, send them.
		sendDrawElementsInstancedRequests(gearTrainBegin->first, gearTrainBegin->second.get());			// if the gear has any instanced drawing requests, send them.
	}
}

void ShaderMachineBase::sendMultiDrawArrayJobRequests(int in_gearID, Gear* in_gearRef)
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
			if (jobToSend.isEnabled == true)		// only send if it is enabled.
			{
				multiDrawArrayItineraries[*currentBegin] = in_gearID;				// make sure to flag the itinerary.
				in_gearRef->insertMultiDrawArrayJob(*currentBegin, jobToSend);		// send a copy to the Gear.
			}
		}
	}
}

void ShaderMachineBase::disableMultiDrawArrayJob(std::string in_drawJobName)
{
	// first, find the Gear that has the draw job in it, and erase it from that gear...if there's even an itinerary for it.
	auto gearIter = multiDrawArrayItineraries.find(in_drawJobName);
	if (gearIter != multiDrawArrayItineraries.end())
	{
		int gearID = gearIter->second;
		auto targetGear = gearTrain.find(gearID);
		targetGear->second.get()->removeMultiDrawArrayJob(in_drawJobName);
		multiDrawArrayItineraries.erase(in_drawJobName); // erase the itinerary entry, after we've removed the gear's data.
	}

	// second, set the draw job's isEnabled flag to false, if it exists.
	auto lookupAttempt = multiDrawArrayJobLookup.find(in_drawJobName);
	if (lookupAttempt != multiDrawArrayJobLookup.end())
	{
		multiDrawArrayJobMap[lookupAttempt->second].isEnabled = false;
	}
}

void ShaderMachineBase::enableMultiDrawArrayJob(std::string in_drawJobName)
{
	// set the draw job's isEnabled flag to true, if it exists.
	auto lookupAttempt = multiDrawArrayJobLookup.find(in_drawJobName);
	if (lookupAttempt != multiDrawArrayJobLookup.end())
	{
		multiDrawArrayJobMap[lookupAttempt->second].isEnabled = true;
	}
}

void ShaderMachineBase::sendDrawElementsInstancedRequests(int in_gearID, Gear* in_gearRef)
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

float ShaderMachineBase::retrieveFloatUniform(std::string in_floatUniformName)
{
	return uniformRegistry.getFloat(in_floatUniformName);
}


void ShaderMachineBase::registerMultiDrawArrayJob(std::string in_drawJobName, GLint* in_startArray, GLsizei* in_vertexCount, int in_numberOfCollections)
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

void ShaderMachineBase::registerTBW(std::string in_tbwName, TimeBasedWaveType in_timeBasedWaveType)
{
	waveManager.createTimeBasedWave(in_tbwName, in_timeBasedWaveType);
}


ShaderMachineBase::GearFindResult ShaderMachineBase::findGear(std::string in_programName)
{
	GearFindResult searchResult;
	auto programFinder = programLookup.find(in_programName);
	if (programFinder != programLookup.end())
	{
		int programIDToFind = programFinder->second;	// the ID to check against each program.
		auto gearsBegin = gearTrain.begin();
		auto gearsEnd = gearTrain.end();
		bool wasSelectedGearFound = false;
		int selectedGearID = 0;							// this will be set when 
		for (; gearsBegin != gearsEnd; gearsBegin++)
		{
			if (gearsBegin->second.get()->programID == programFinder->second)	// find the Gear with a program ID that matches the program ID we're looking for.
			{
				// the gear with the corresponding program was found, set it and return it.
				GearFindResult foundResult(true, gearsBegin->first, gearsBegin->second.get());
				searchResult = foundResult;
				break;
			}
		}
	}
	return searchResult;
}

void ShaderMachineBase::sendMessageToGLProgram(std::string in_programName, Message in_message)
{
	GearFindResult programToSearch = findGear(in_programName);
	if (programToSearch.wasResultFound == true)
	{
		programToSearch.foundGear->interpretMessage(in_message);
	}
}

void ShaderMachineBase::createDynamicBufferAndSendToGear(std::string in_bufferName, std::string in_programName)
{
	// first, attempt to find the program; don't bother continuing if we can't find the program.
	GearFindResult programToSearch = findGear(in_programName);
	if (programToSearch.wasResultFound == true)
	{
		GLuint dynamicBufferID = dynBufferManager.attemptCreateOfDynamicBufferForGear(in_bufferName, programToSearch.foundGearIndex);
		Message requestMessage(MessageType::OPENGL_REGISTER_DYN_BUFFER_IN_GEAR);	// create a new message for the Gear, to let it know to handle the dyn buffer request.
		requestMessage.insertString(in_bufferName);									// insert the buffer name for dyn buffer request.
		requestMessage.insertInt(dynamicBufferID);									// insert the OpenGL buffer ID 
		programToSearch.foundGear->interpretMessage(requestMessage);					// send the message to the Gear; have it interpret it.
		//std::cout << "Created new buffer; it's ID is: " << dynamicBufferID << std::endl;
	}
}

void ShaderMachineBase::createDynamicBufferMultiDrawArrayJobAndSendToGear(std::string in_bufferName,
																			std::string in_programName,
																			GLint* in_startArray,
																			GLsizei* in_vertexCount,
																			int in_drawCount)
{
	// first, attempt to find the program; don't bother continuing if we can't find the program.
	GearFindResult programToSearch = findGear(in_programName);
	if (programToSearch.wasResultFound == true)
	{
		dynBufferManager.createDynamicMultiDrawArrayJob(in_bufferName, in_startArray, in_vertexCount, in_drawCount);
		Message requestMessage(MessageType::OPENGL_REGISTER_DYN_BUFFER_MULTIDRAW_JOB_IN_GEAR);	// tell the gear that it needs to fetch a multi draw array job
																						// from the DynamicBufferManager (dynBufferManager)
		requestMessage.insertString(in_bufferName);
		programToSearch.foundGear->interpretMessage(requestMessage);
		//std::cout << "Sent dynamic multi draw array job message to gear. " << std::endl;
	}
}

void ShaderMachineBase::sendDataToDynamicBuffer(std::string in_bufferName, int in_byteSizeToWrite, GLfloat* in_dataArray)
{
	dynBufferManager.insertDataIntoDynBuffer(in_bufferName, in_byteSizeToWrite, in_dataArray);
}

void ShaderMachineBase::deleteDynamicBuffer(std::string in_bufferName)
{
	// first, go to any Gear that uses the buffer, and let it know to unbind any VAO, or other object, that was using it.
	auto destinationGears = dynBufferManager.getBufferDestinationGears(in_bufferName);
	auto destinationGearsBegin = destinationGears.intSet.begin();
	auto destinationGearsEnd = destinationGears.intSet.end();
	for (; destinationGearsBegin != destinationGearsEnd; destinationGearsBegin++)
	{
		// "destinationGears" contains the Gear IDs that data was sent to; cycle through each gear we sent to.
		Message dynBufferRemovalMessage(MessageType::OPENGL_DEREGISTER_DYN_BUFFER_MULTIDRAW_JOB_IN_GEAR);
		dynBufferRemovalMessage.insertString(in_bufferName);
		gearTrain[*destinationGearsBegin]->interpretMessage(dynBufferRemovalMessage);
	}

	// second, remove the dynamically created buffer from the dynBufferManager.
	dynBufferManager.attemptDeleteOfDynamicBuffer(in_bufferName);
}

void ShaderMachineBase::registerTBWAndSendRequestToProgramGear(std::string in_tbwName, TimeBasedWaveType in_timeBasedWaveType, std::string in_programName)
{
	// first, attempt to find the program; don't bother continuing if we can't find the program.
	auto programFinder = programLookup.find(in_programName);
	if (programFinder != programLookup.end())
	{
		waveManager.createTimeBasedWave(in_tbwName, in_timeBasedWaveType);
		int programIDToFind = programFinder->second;	// the ID to check against each program.
		auto gearsBegin = gearTrain.begin();
		auto gearsEnd = gearTrain.end();
		bool wasSelectedGearFound = false;
		int selectedGearID = 0;							// this will be set when 
		for (; gearsBegin != gearsEnd; gearsBegin++)
		{
			if (gearsBegin->second.get()->programID == programFinder->second)	// find the Gear with a program ID that matches the program ID we're looking for.
			{
				gearsBegin->second.get()->insertUniformRequest(GLDataType::FLOAT, in_tbwName);	// send the newly created request to the selected Gear; waves should always be a float type.
				wasSelectedGearFound = true;
				selectedGearID = gearsBegin->first;
			}
		}

		// make sure to put the appropriate value in the tbw itinerary.
		if (wasSelectedGearFound == true)
		{
			tbwDestinationItineraries[in_tbwName] = selectedGearID;
		}
	}
}

void ShaderMachineBase::deregisterTBWANdRemoveRequestFromProgramGear(std::string in_tbwName)
{
	// first, attempt to find the itinerary associated with the tbwName.
	auto tbwNameFinder = tbwDestinationItineraries.find(in_tbwName);
	if (tbwNameFinder != tbwDestinationItineraries.end())
	{
		int targetGearKey = tbwNameFinder->second;
		gearTrain[targetGearKey].get()->deleteUniformRequest(in_tbwName);
		tbwDestinationItineraries.erase(in_tbwName);
	}
}

void ShaderMachineBase::swapAndPoll()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void ShaderMachineBase::insertNewPersistentBuffer(std::string in_bufferName, int in_size)
{
	int indexToUse = persistentBufferMap.getNextAvailableKey();	// get the index we will use
	persistentBufferMap[indexToUse] = 0;			// initialize to 0; but it will get changed.
	OrganicGLWinUtils::createImmutableBuffer(&persistentBufferMap[indexToUse], in_size, 1);	// pass a reference to the GLuint to bind to
	persistentBufferLookup[in_bufferName] = indexToUse;								// insert the lookup value
	std::cout << "!!!!!!! ################ persistent buffer ID is: " << persistentBufferMap[indexToUse] << std::endl;
}

void ShaderMachineBase::insertNewFBO(std::string in_fboName)
{
	int indexToUse = fboMap.getNextAvailableKey();
	fboMap[indexToUse] = 0;	// initialize to 0; but will be changed in next function call
	OrganicGLWinUtils::createFBO(&fboMap[indexToUse]);	// create the FBO; only creates, doesn't do anything special
	fboLookup[in_fboName] = indexToUse;	// insert the lookup value
}

void ShaderMachineBase::insertNewBuffer(std::string in_bufferName)
{
	int indexToUse = bufferMap.getNextAvailableKey();
	bufferMap[indexToUse] = 0;
	OrganicGLWinUtils::createBuffer(&bufferMap[indexToUse]);
	bufferLookup[in_bufferName] = indexToUse;
}

void ShaderMachineBase::insertNewTexture(std::string in_textureName)
{
	int indexToUse = textureMap.getNextAvailableKey();
	textureMap[indexToUse] = 0;
	textureLookup[in_textureName] = indexToUse;
}

void ShaderMachineBase::insertAndBuildNewAtlas(std::string in_atlasFolderName, GLuint* in_atlasTextureRef, float* in_atlasTileWidth, float* in_atlasWidth)
{
	int indexToUse = atlasMapMap.getNextAvailableKey();
	AtlasMap newMap;
	atlasMapMap[indexToUse] = newMap;
	atlasMapMap[indexToUse].buildAtlas(in_atlasFolderName, in_atlasTextureRef, in_atlasTileWidth, in_atlasWidth);

	atlasMapLookup[in_atlasFolderName] = indexToUse;
}

void ShaderMachineBase::insertAndBuildNewAtlasToSpecifiedTextureChannel(GLenum in_texUnit, std::string in_atlasFolderName, GLuint* in_atlasTextureRef, float* in_atlasTileWidth, float* in_atlasWidth)
{
	int indexToUse = atlasMapMap.getNextAvailableKey();
	AtlasMap newMap;
	atlasMapMap[indexToUse] = newMap;
	atlasMapMap[indexToUse].buildAtlasOnTextureUnit(in_texUnit, in_atlasFolderName, in_atlasTextureRef, in_atlasTileWidth, in_atlasWidth);

	atlasMapLookup[in_atlasFolderName] = indexToUse;
}

void ShaderMachineBase::insertNewMultiDrawArrayJob(std::string in_jobName, GLMultiDrawArrayJob in_job)
{
	int indexToUse = multiDrawArrayJobMap.getNextAvailableKey();
	int targetKeyValue = indexToUse;


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
	int indexToUse = drawElementsInstancedJobMap.getNextAvailableKey();
	int targetKeyValue = indexToUse;

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
	int indexToUse = programMap.getNextAvailableKey();
	programMap[indexToUse] = 0;
	OrganicGLWinUtils::loadShadersViaMode(&programMap[indexToUse], in_programName);
	programLookup[in_programName] = programMap[indexToUse];
}

void ShaderMachineBase::createComputeProgram(std::string in_programName)
{
	int indexToUse = programMap.getNextAvailableKey();
	programMap[indexToUse] = 0;
	OrganicGLWinUtils::loadComputeShader(&programMap[indexToUse], in_programName);
	programLookup[in_programName] = programMap[indexToUse];
}

void ShaderMachineBase::toggleCameraBoundToMousePointer()
{
	if (cameraBoundToMousePointer == true)
	{
		cameraBoundToMousePointer = false;
	}
	else if (cameraBoundToMousePointer == false)
	{
		glfwSetCursorPos(window, width / 2, height / 2); // when setting back to true, set the cursor to center of the screen.
		cameraBoundToMousePointer = true;
	}
}

void ShaderMachineBase::setDirection(float in_x, float in_y, float in_z)
{
	direction.x = in_x;
	direction.y = in_y;
	direction.z = in_z;

	glm::vec3 directionVec3(in_x, in_y, in_z);
	glm::vec3 dirNormalized = glm::normalize(directionVec3);

	const float doublePi = 3.14f * 2;

	// set horizontal angle -- first value is x, second is z.
	float horizontalSet = atan2(dirNormalized.x, dirNormalized.z);
	//float horizontalSet = atan2(dirNormalized.z, dirNormalized.x);
	//horizontalAngle = doublePi - horizontalSet;
	horizontalAngle = horizontalSet;

	// set vertical angle -- first value is y, second is z.
	float verticalSet = atan2(dirNormalized.y, dirNormalized.z);
	//float verticalSet = atan2(dirNormalized.z, dirNormalized.y);
	//verticalAngle = doublePi - verticalSet;
	verticalAngle = verticalSet;


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