#include "stdafx.h"
#include "MShaderController.h"

MShaderController::MShaderController()
{
	mShaderCycler.setRefs(&catalog,
		&controllerValueRegistry,
		&controllerMGCI);

	inputListener.initializeFeedbackListener(&controllerInputTracker, &controllerMachineFeedback);

	// trigger setup, basic testing
	inputListener.createTrigger("LCTRL", Message(MessageType::NOVAL), GUIInteractionCondition::NOT_DEPENDENT, GLFW_KEY_LEFT_CONTROL);
	inputListener.createTrigger("LSHIFT", Message(MessageType::NOVAL), GUIInteractionCondition::NOT_DEPENDENT, GLFW_KEY_LEFT_SHIFT);

	// testing only: use J and K keys to dynamically switch between MShaders at runtime.
	inputListener.createTrigger("SWITCH_TO_GRAYSCALE", Message(MessageType::MSHADER_SWITCH_SHADER_NAME, "MSBasicGrayscale"), GUIInteractionCondition::NOT_DEPENDENT, GLFW_KEY_J);
	inputListener.createTrigger("SWITCH_TO_MSBASICCOMPUTE", Message(MessageType::MSHADER_SWITCH_SHADER_NAME, "MSBasicCompute"), GUIInteractionCondition::NOT_DEPENDENT, GLFW_KEY_K);
}

MShaderController::~MShaderController()
{
	std::cout << "Calling destructor for MShaderController..." << std::endl;

	// Remove GLFW Window, if it was active.
	if (isGLFWWindowActive)
	{
		glfwDestroyWindow(mainWindowPtr);
		std::cout << "GLFW window destroyed." << std::endl;
	};

	// Cleanup MGears
	catalog.cleanupMShaderMGears();

}

bool MShaderController::switchMShader(std::string in_shaderToSwitchTo)
{
	Message attemptMessage = mShaderCycler.switchToMShader(in_shaderToSwitchTo);
	attemptMessage.open();

	int attemptStatus = attemptMessage.readInt();
	std::string attemptInfo = attemptMessage.readString();

	mShaderInfoQueue.push(Message(MessageType::MSHADER_INFO, attemptInfo));

	if (attemptStatus == 0)
	{
		return false;
	}

	return true;
}

void MShaderController::attemptSwitchOnNextTick(std::string in_shaderToSwitchTo)
{
	nextShaderToSwitchTo = in_shaderToSwitchTo;
	shaderSwitchAttemptFlag = true;
}

bool MShaderController::addMShaderToCatalog(std::string in_shaderToSwitchTo)
{
	return true;
}

void MShaderController::setupSharedComponents()
{

}

void MShaderController::instantiateAndSetupShader(std::string in_shaderToInstantiate)
{

}

void MShaderController::processMessage(Message in_messageToRead)
{
	switch (in_messageToRead.messageType)
	{
		case MessageType::MSHADER_SET_COMPUTE_RESOLUTION: 
		{ 
			mShaderInfoQueue.push(setComputeResolution(std::move(in_messageToRead))); 
			break;
		}

		case MessageType::MSHADER_CREATE_MSBASICCOMPUTE:
		{
			mShaderSetupQueue.push(in_messageToRead);
			break;
		}

		case MessageType::MSHADER_CREATE_MSBASICGRAYSCALE:
		{
			mShaderSetupQueue.push(in_messageToRead);
			break;
		}

		case MessageType::MSHADER_TRANSITIONAL_HINT:
		{
			mShaderInfoQueue.push(controllerHintIndexer.processTransitionalHintMessage(in_messageToRead));
			break;
		}
	}
};

Message MShaderController::setComputeResolution(Message in_messageToRead)
{
	in_messageToRead.open();

	// read order, as described in MessageType header is:
	//	-(int) The dimensions of the compute shaders to use
	//  -(int) desired screen width
	//  -(int) desired screen height 
	int computeDim = in_messageToRead.readInt();
	int desiredScreenWidth = in_messageToRead.readInt();
	int desiredScreenHeight = in_messageToRead.readInt();

	ComputeResolution mResolution(desiredScreenWidth, desiredScreenHeight, computeDim, computeDim);
	mainScreenWidth = mResolution.computeScreenWidth;		// should be represented as "screen_pixel_width" in the registry
	mainScreenHeight = mResolution.computeScreenHeight;		//  "screen_pixel_height"
	mainComputeDim = computeDim;							//  "screen_compute_group_dim"
	mainScreenHorizonalComputeGroups = mResolution.computeScreenWidth / computeDim;	// "screen_compute_columns"
	mainScreenVerticalComputeGroups = mResolution.computeScreenHeight / computeDim;	// "screen_compute_rows"

	updateScreenDimensionUniforms();	// update the registry with all the new screen dimension metadata

	isResolutionSet = true;
	return Message(MessageType::MSHADER_INFO, std::string("Compute resolution set"));
}

void MShaderController::updateScreenDimensionUniforms()
{
	controllerValueRegistry.insertInt("screen_pixel_width", mainScreenWidth);
	controllerValueRegistry.insertInt("screen_pixel_height", mainScreenHeight);
	controllerValueRegistry.insertInt("screen_compute_group_dim", mainComputeDim);
	controllerValueRegistry.insertInt("screen_compute_columns", mainScreenHorizonalComputeGroups);
	controllerValueRegistry.insertInt("screen_compute_rows", mainScreenVerticalComputeGroups);
}

void MShaderController::initializeMandatoryItems()
{
	// GLFW setup
	if (OrganicGLWinUtils::initializeLibraryAndSetHints())
	{
		mShaderInfoQueue.push(Message(MessageType::MSHADER_INFO, std::string("GLFW setup OK.")));
	}
	else { mShaderInfoQueue.push(Message(MessageType::MSHADER_INFO, std::string("GLFW setup FAILED.")));}

	// OpenGL window creation
	mainWindowPtr = OrganicGLWinUtils::createGLFWWindow(mainScreenWidth, mainScreenHeight);
	if (OrganicGLWinUtils::checkWindowValidity(mainWindowPtr))
	{
		isGLFWWindowActive = true;
		mShaderInfoQueue.push(Message(MessageType::MSHADER_INFO, std::string("OpenGL window created OK.")));
	}
	else { mShaderInfoQueue.push(Message(MessageType::MSHADER_INFO, std::string("OpenGL window creation FAILED."))); }

	// Context must be current before calling GLEW.
	OrganicGLWinUtils::makeContextCurrent(mainWindowPtr);

	// GLEW setup
	if (OrganicGLWinUtils::initializeGlew())
	{
		mShaderInfoQueue.push(Message(MessageType::MSHADER_INFO, std::string("GLEW setup OK.")));
	}
	else { mShaderInfoQueue.push(Message(MessageType::MSHADER_INFO, std::string("GLEW setup FAILED."))); }

	// keyboard and other feedbacks must be setup after GLFW/GLEW has been setup; attempting to do this
	// on the constructor of this class would have no effect, as nothing would have been initialized yet before this
	// function was called.
	OrganicGLWinUtils::setGLFWInputMode(mainWindowPtr);		// set input mode for feedback from the keyboard
	glfwSetWindowUserPointer(mainWindowPtr, this);			// required, before glfwGetWindowUserPointer is called
	glfwSetKeyCallback(mainWindowPtr, controllerKeyboardCallbackWrapper);	// keyboard
}

void MShaderController::createMShaders()
{
	while (!mShaderSetupQueue.empty())
	{
		auto currentShaderMessage = mShaderSetupQueue.front();

		std::string insertAttemptShaderName = "";
		bool wasInsertSuccessful = false;
		switch (currentShaderMessage.messageType)
		{
			// For MSBasicCompute shader attempt
			case MessageType::MSHADER_CREATE_MSBASICCOMPUTE:
			{
				insertAttemptShaderName = "MSBasicCompute";
				wasInsertSuccessful = catalog.insertMShader(insertAttemptShaderName, std::move(std::unique_ptr<MShaderBase>(new MSBasicCompute())));
				break;
			}

			// For MSBasicGrayscale shader attempt
			case MessageType::MSHADER_CREATE_MSBASICGRAYSCALE:
			{
				insertAttemptShaderName = "MSBasicGrayscale";
				wasInsertSuccessful = catalog.insertMShader(insertAttemptShaderName, std::move(std::unique_ptr<MShaderBase>(new MSBasicGrayscale())));
				break;
			}
		}

		// Only bother continuing the initialization of the shader, if we know for a fact that it was inserted into the catalog.
		if (wasInsertSuccessful)
		{
			//std::cout << "Attempting creation of " + insertAttemptShaderName + " MShader. " << std::endl;
			mShaderInfoQueue.push(Message(MessageType::MSHADER_INFO, "Attempting creation of " + insertAttemptShaderName + " MShader. "));
			catalog.getShaderRef(insertAttemptShaderName)->setSharedObjectPointers(&controllerButtonPanelContainer,
				&controllerSliderPanelContainer,
				&controllerInputPanelContainer,
				&controllerMachineFeedback,
				&controllerBindings,
				&controllerValueRegistry
			);

			catalog.getShaderRef(insertAttemptShaderName)->setupMShaderRequestsAndName();
			mShaderInfoQueue.push(Message(MessageType::MSHADER_INFO, std::string("Verifying bindings on " + insertAttemptShaderName + "...")));

			// Fetch the required binding requests, and process each one.
			auto fetchedBindingRequests = catalog.getShaderRef(insertAttemptShaderName)->fetchMShaderBindingRequests();
			for (auto& currentRequest : fetchedBindingRequests)
			{
				processAPIObjectRequest(currentRequest);
			}

			// For safety, verify that the underlying MGear objects in the newly spawned MShader have all their requested resources,
			// that would be found in the MAPIObjectManager
			bool allGearObjectsFound = catalog.getShaderRef(insertAttemptShaderName)->checkForRequiredGearObjects(&mShaderInfoQueue);
			if (allGearObjectsFound)
			{
				mShaderInfoQueue.push(Message(MessageType::MSHADER_INFO, "All required MGear resources found for " + insertAttemptShaderName));
			}
			else
			{
				mShaderInfoQueue.push(Message(MessageType::MSHADER_INFO, "Missing some required MGear resources for " + insertAttemptShaderName));
			}
		}
		else
		{
			mShaderInfoQueue.push(Message(MessageType::MSHADER_INFO, std::string("Failed to create " + insertAttemptShaderName +  "; ...did it already exist?")));
		}

		mShaderSetupQueue.pop();
	}
}

void MShaderController::processAPIObjectRequest(MAPIObjectRequest in_bindingRequestToProcess)
{
	std::cout << "!!! calling processAPIObjectRequest..." << std::endl;
	Message requestResult = controllerBindings.handleMAPIObjectRequest(in_bindingRequestToProcess);
	mShaderInfoQueue.push(requestResult);	
}

void MShaderController::writeOutInformationalMessages()
{
	std::cout << "Writing out current MShaderController messages...." << std::endl;
	while (!mShaderInfoQueue.empty())
	{
		auto currentInfo = mShaderInfoQueue.front();
		currentInfo.open();
		std::string infoString = currentInfo.readString();
		std::cout << infoString << std::endl;

		mShaderInfoQueue.pop();
	}
}


void MShaderController::insertNewGradient(Message in_gradientInsertionMessage, bool in_overwriteGradientFlag)
{
	// Open Message; read the enum'd int value
	in_gradientInsertionMessage.open();

	float singleCyclePiValue = 0.0f;
	float singleCycleDivisorMs = 0.0f;

	std::string gradientName = in_gradientInsertionMessage.readString();
	std::cout << "!! Finished reading string..." << std::endl;

	float cycleDuration = 0.0f;	// only used with finite


	// Step 1: read the single cycle pi, divisor value, and finite duration (if doing finite.
	// These should always be the first floats of the Message.
	switch (in_gradientInsertionMessage.messageType)
	{
		case MessageType::MSHADER_SETUP_CYCLICAL_MGRADIENT:
		{
			std::cout << "!! Fonund cyclical MGradient. " << std::endl;
			singleCyclePiValue = in_gradientInsertionMessage.readFloat();
			singleCycleDivisorMs = in_gradientInsertionMessage.readFloat();
			break;
		}

		case MessageType::MSHADER_SETUP_FINITE_MGRADIENT:
		{
			std::cout << "!! Fonund finite MGradient. " << std::endl;
			singleCyclePiValue = in_gradientInsertionMessage.readFloat();
			singleCycleDivisorMs = in_gradientInsertionMessage.readFloat();

			// specific to finite only, read the cycleDuration
			cycleDuration = in_gradientInsertionMessage.readFloat();
			break;
		}
	};

	// Step 2: determine the type of gradient input, by reading the next int available and 
	// casting it to MessageType. The MessageType being read should be some MGRADIENT-flavor of MessageType;
	// Anything else might cause exceptions and/or other errors.
	MessageType gradientTypeToBuild = MessageType(in_gradientInsertionMessage.readInt());	// read the int, cast it.

	Message messageForMGC;	// will contain all data for the Message that contains the metadata for the gradient;
							// this data will be used in step 3, when we insert into the controllerMGCI.

	switch (gradientTypeToBuild)
	{
		case MessageType::MGRADIENT_VEC3_INPUT:
		{
			std::cout << "!! Attempting creation of MGRADIENT_VEC3_INPUT Message..." << std::endl;

			ECBPolyPoint pointA = in_gradientInsertionMessage.readPoint();
			std::cout << "~~~~point A value, prior to insert: "; pointA.printPointCoords(); std::cout << std::endl;

			ECBPolyPoint pointB = in_gradientInsertionMessage.readPoint();
			std::cout << "~~~~point B value, prior to insert: "; pointB.printPointCoords(); std::cout << std::endl;

			messageForMGC = Message(MessageType::MGRADIENT_VEC3_INPUT,
				pointA,	// point A
				pointB	// point B
			);

			/*
			// Do not insert Message data like this; the two points have no particular order. Use pre-defined variables instead
			// I should probably put a note of this behavior in Message.h...
			messageForMGC = Message(MessageType::MGRADIENT_VEC3_INPUT,
								in_gradientInsertionMessage.readPoint(),	// point A
								in_gradientInsertionMessage.readPoint()	// point B
								);
			*/
			break;
		}
	};

	// Step 3: based off the messageType of the input Message into this function, 
	switch (in_gradientInsertionMessage.messageType)
	{

		case MessageType::MSHADER_SETUP_CYCLICAL_MGRADIENT:
		{
			std::cout << "Sending cyclical gradient to controllerMGCI..." << std::endl;
			controllerMGCI.insertCyclicalGradient(gradientName,
												messageForMGC,
												singleCyclePiValue,
												singleCycleDivisorMs, 
												in_overwriteGradientFlag);
			break;
		}

		case MessageType::MSHADER_SETUP_FINITE_MGRADIENT:
		{
			std::cout << "Sending finite gradient to controllerMGCI..." << std::endl;
			controllerMGCI.insertFiniteGradient(gradientName,
												messageForMGC,
												singleCyclePiValue,
												singleCycleDivisorMs, 
												cycleDuration,
												in_overwriteGradientFlag);

			break;
		}
	}
}

void MShaderController::runTick()
{
	// ---------- Phase 1: Check for requests,  calculate passed time, update variables and gradients, reset shader feedback
	processShaderChangeRequests();		// Step 1:	if attemptSwitchOnNextTick was called, attempt to switch
										//			to the shader given by the current value of nextShaderToSwitchTo.
										//		 	Check if there are any transitional hints to process as well, if the
										//			switch was sucessful, to create new gradients.								

	calculatePassedTime();				// Step 2: fetch the total time that has elapsed, to use as a value for updateAndApplyGradients
	updateMVPVariables();				// Step 3: Calculate any MVP matrices or other similiar values that have been setup
	updateAndapplyGradients(secondsSinceLastTimestamp * 1000);		// Step 4: Update gradients, and apply them (gradients work in milliseconds)
	controllerMGCI.deleteExpiredFiniteGradients();		// Step 5: remove expired gradients.

	// ---------- Phase 2: Render non ImGui components (but do not swap buffers)
	// TODO: This is where the currently selected MShader will be called upon to do its rendering, somehow.
	
	// ---------- Phase 3: Render ImGui components, process any clicks/interactions from ImGui
	// TODO: Somehow: start a new ImGui frame, read feedback, then render ImGui

	// ---------- Phase 4: swap the buffers
	glfwSwapBuffers(mainWindowPtr);		// TEMPORARY: swap buffers to render a new frame; realistically, this will get done by the currently selected
										// MShader, but this is just here for now.





	// ----------- Phase 5: poll for input feedback events (i.e, from keyboard); take actions based off any input somehow.
	glfwPollEvents();	// required, so that we may check for keyboard inputs and other similiar events that are waiting in the queue...

	// 5.1: Feedback gathering.
	controllerInputTracker.resetChangeState();			// required: reset change state back to false
	controllerInputTracker.flagStillPressedStates();	// check for inputs still pressed from the previous tick.
	controllerInputTracker.handleKeyPressTransfers(false);	// allow keyboard input as feedback, if clicks weren't made in ImGui
	controllerInputTracker.destroyCyclesAtEndOfLife();	// destroy inputs no longer being pressed, before calling scanForTriggerRemovalConditions below.
	inputListener.scanForTriggerFiringConditions();		// look for fired fired triggers
	inputListener.scanForTriggerRemovalConditions();
	controllerInputTracker.destroyOneoffs();			// destroy one off inputs, such as mouse scroll

	// 5.2: checking the contents of inputListener.firedTriggerMessages, and
	// other relevant objects should be done here
	// TODO: need to read and process input feedback that occurred in this tick
	auto fetchedKeyboardMessages = inputListener.fetchTriggerMessages();
	processInputFeedback(fetchedKeyboardMessages);

	// 5.3: cleanup / erase the feedback queues in the inputListener
	inputListener.clearFeedbackMessages();
}

void MShaderController::processInputFeedback(std::queue<Message> in_feedbackQueue)
{
	while (!in_feedbackQueue.empty())
	{
		Message currentMessage = in_feedbackQueue.front();
		switch (currentMessage.messageType) 
		{
			// Below: make an attempt to switch to the designated MShader on the next tick.
			case MessageType::MSHADER_SWITCH_SHADER_NAME:
			{
				currentMessage.open();
				std::string shaderName = currentMessage.readString();
				std::cout << "!!! Found shader name for attempted shader switch: " << shaderName << std::endl;
				attemptSwitchOnNextTick(shaderName);
				break;
			}
		}

		in_feedbackQueue.pop();
	}
}

void MShaderController::processShaderChangeRequests()
{
	// This function will only really attempt to do anything if the shaderSwitchAttemptFlag
	// is set to true.
	if (shaderSwitchAttemptFlag)
	{
		// Now, use the value in nextShaderToSwitchTo to attempt a switch.
		// If the result is true, AND there are two shaders in the mShaderCycler proceed 
		// to check for transitional hints.
		bool wasSwitchSuccessful = switchMShader(nextShaderToSwitchTo);
		if (wasSwitchSuccessful)
		{
			// If we're processing the very first call to attemptSwitchOnNextTick,
			// there is no need to check transitional hints.
			if (mShaderCycler.getNumberOfLoadedShaders() == 1)
			{
				std::cout << "Initial shader selected; ignoring transitional hint scan." << std::endl;
				// copy over uniform/other stored values from selected shader, into the controllerValueRegistry;
				// gradients cannot be used when only one shader is in the mShaderCycler.
			}

			// ...but if we're not on the first, check the hints.
			else if (mShaderCycler.getNumberOfLoadedShaders() == 2)
			{
				// copy over uniform/other stored values from selected shader, into the controllerValueRegistry;
				std::cout << "########## TEST, getNumberOfLoadedShaders() ###########: "; ECBPolyPoint(controllerValueRegistry.getVec3("background_clear_color")).printPointCoords(); std::cout << std::endl;
				std::cout << "## Old shader name: " << mShaderCycler.getPreviousShaderRef()->fetchMShaderName() << std::endl;
				std::cout << "## New shader name: " << mShaderCycler.getTargetShaderRef()->fetchMShaderName() << std::endl;

				// Whenever there are two shaders loaded in the mShaderCycler,
				// we can attempt gradient transitioning.

				std::cout << "!!! Scanning for hints." << std::endl;
				auto transitionalHints = controllerHintIndexer.fetchTransitionalHints();
				for (auto& currentHint : transitionalHints)
				{
					parseHintAndCreateGradient(currentHint);
				}
			}
		}


		// Once all is said and done, reset the flag, and nextShaderToSwitchTo value
		nextShaderToSwitchTo = "";
		shaderSwitchAttemptFlag = false;
	}
}

void MShaderController::controllerKeyboardCallbackWrapper(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	std::cout << "!! Caling callback!" << std::endl;
	static_cast<MShaderController*>(glfwGetWindowUserPointer(window))->controllerKeyboardCallback(window, key, scancode, action, mods);
}

void MShaderController::controllerKeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	std::cout << "!!! Found some input..." << std::endl;
	if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS)
	{
		//toggleCameraBoundToMousePointer();
		std::cout << "!! Found condition to stop running." << std::endl;
		continueRunning = false;
	}

	// new, for KeyPressTracker keyTracker:

	if (action == GLFW_PRESS)
	{
		controllerInputTracker.insertCycle(key);
	}
	else if (action == GLFW_RELEASE)
	{
		controllerInputTracker.killCycle(key);
	}
}

bool MShaderController::checkIfRunning()
{
	return continueRunning;
}

void MShaderController::parseHintAndCreateGradient(MShaderHintEnum in_enumValue)
{
	// each hint should be mapped to 1 gradient attempt.
	//
	// If the gradient does not exist:
	//	-Value A comes from the old shader, via getPreviousShaderRef.
	//  -Value B comes from the new shader, via getTargetShaderRef.

	// If the gradient DOES exist:
	//  -Value A should come from the MShaderController's registry
	//  -Value B comes from the new shader, via getTargetShaderRef.
	//	-replace the old gradient of the same name with the new one.
	

	switch (in_enumValue)
	{
		case MShaderHintEnum::TRANSIT_CLEAR_COLOR:
		{
			std::cout << "!! Found TRANSIT_CLEAR_COLOR to use during transit..." << std::endl;
			formClearColorGradient();
			break;
		}
	}
}

void MShaderController::formClearColorGradient()
{
	// Condition 1: If both the old and new shaders have the "background_clear_color" value in their uniforms,
	// AND a gradient with that name doesn't exist, insert a finite gradient using the background colors of both the
	// old and new shader.
	if (
		(mShaderCycler.getPreviousShaderRef()->getLocalValueRegistryRef()->doesVec3Exist("background_clear_color"))
		&&
		(mShaderCycler.getTargetShaderRef()->getLocalValueRegistryRef()->doesVec3Exist("background_clear_color"))
		&&
		(!controllerMGCI.doesGradientExist("background_clear_color"))
	   )
	{
		Message finiteClearColor(MessageType::MSHADER_SETUP_FINITE_MGRADIENT,
								int(MessageType::MGRADIENT_VEC3_INPUT),
								"background_clear_color",	// must be named this
								fullCircleRadians,	// 2 pi radians
								2000.0f,			// cycle duration, in ms
								1000.0f,			// finite time duration, in ms					
								ECBPolyPoint(mShaderCycler.getPreviousShaderRef()->getLocalValueRegistryRef()->getVec3("background_clear_color")),
								ECBPolyPoint(mShaderCycler.getTargetShaderRef()->getLocalValueRegistryRef()->getVec3("background_clear_color"))
							);

		std::cout << "!! Creating new background_clear_color gradient..." << std::endl;
		auto bgColorA = mShaderCycler.getPreviousShaderRef()->getLocalValueRegistryRef()->getVec3("background_clear_color");
		std::cout << "Old background color: " << bgColorA.x << ", " << bgColorA.y << ", " << bgColorA.z << std::endl;
		auto bgColorB = mShaderCycler.getTargetShaderRef()->getLocalValueRegistryRef()->getVec3("background_clear_color");
		std::cout << "New background color: " << bgColorB.x << ", " << bgColorB.y << ", " << bgColorB.z << std::endl;
		insertNewGradient(finiteClearColor, false);
	}

	// Condition 2: If the new shader has a value of "background_clear_color" in it's uniform, AND the gradient already exists,
	// use the existing background color sitting in this controller's registry as the start value, and the preferred background color of
	// the new target MShader, and create a new gradient with those values.
	else if
	(
		(mShaderCycler.getTargetShaderRef()->getLocalValueRegistryRef()->doesVec3Exist("background_clear_color"))
		&&
		(controllerMGCI.doesGradientExist("background_clear_color"))
	)
	{

		Message finiteClearColor(MessageType::MSHADER_SETUP_FINITE_MGRADIENT,
								int(MessageType::MGRADIENT_VEC3_INPUT),
								"background_clear_color",	// must be named this
								fullCircleRadians,	// 2 pi radians
								2000.0f,			// cycle duration, in ms
								1000.0f,			// finite time duration, in ms					
								ECBPolyPoint(controllerValueRegistry.getVec3("background_clear_color")),
								ECBPolyPoint(mShaderCycler.getTargetShaderRef()->getLocalValueRegistryRef()->getVec3("background_clear_color"))
							);

		std::cout << "!! Updating existing background_clear_color gradient..." << std::endl;
		auto bgColorA = controllerValueRegistry.getVec3("background_clear_color");
		std::cout << "Old background color: " << bgColorA.x << ", " << bgColorA.y << ", " << bgColorA.z << std::endl;
		auto bgColorB = mShaderCycler.getTargetShaderRef()->getLocalValueRegistryRef()->getVec3("background_clear_color");
		std::cout << "New background color: " << bgColorB.x << ", " << bgColorB.y << ", " << bgColorB.z << std::endl;

		/*
		int someVal = 3;
		std::cout << "FOUND EXISTING COLOR GRADIENT TO UPDATE; Enter number to continue. " << std::endl;
		std::cin >> someVal;
		*/

		insertNewGradient(finiteClearColor, true);	// the second argument needs to be true, to overwrite hte existing gradient.
	}
}


void MShaderController::calculatePassedTime()
{
	// Find what the current time is, and subtract the lastTimeStamp value from it, to get
	// the updated value for secondsSinceLastTimestamp. Once calculated, set the lastTimeStamp = currentTimeStamp.
	//
	// This function should be called every tick that the MShaderController is running.

	currentTimeStamp = glfwGetTime();

	// If this the very first time this function is called, the timestamps will be equal;
	// we must then set wasInitialTimeCalculated to true so that this doesn't happen again.
	if (!wasInitialTimeCalculated)
	{
		lastTimeStamp = currentTimeStamp;
		wasInitialTimeCalculated = true;
	}

	secondsSinceLastTimestamp = float(currentTimeStamp - lastTimeStamp);
	lastTimeStamp = currentTimeStamp;
}

void MShaderController::updateCameraDirectionAndPosition(bool in_imguiFocusedFlag)
{
	// This function should be setup similiar to 
	// Get mouse position ShaderMachineBase::computeCameraDirectionAndPosition.
	//
	// It should be called immediately before the updateMVPVariables function.
	//
	// The in_imguiFocusedFlag is used to check if an imgui window is being focused, in which case
	// we want to focus input on that object.
	double xpos, ypos;
	int isFocused = glfwGetWindowAttrib(mainWindowPtr, GLFW_FOCUSED);

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
		glfwSetCursorPos(mainWindowPtr, mainScreenWidth / 2, mainScreenHeight / 2);
		wasFocusedPreviousFrame = true;
		cameraBoundToMousePointer = true;
	}

	// if the window is focused, and the camera is working with the mouse pointer,
	// we must update it if there was any movement.
	if
	(
		(isFocused == 1)
		&&
		(cameraBoundToMousePointer == true)
	)
	{
		// For below/TODO: is this really necessary? can't we just do glfwSetCursorPos?
		glfwGetCursorPos(mainWindowPtr, &xpos, &ypos);

		// Reset mouse position for next frame
		glfwSetCursorPos(mainWindowPtr, mainScreenWidth / 2, mainScreenHeight / 2);





		//std::cout << "OpenGL is FOCUSED!" << std::endl;


		// Compute new orientation
		horizontalAngle += mouseSpeed * float(mainScreenWidth / 2 - xpos);
		verticalAngle += mouseSpeed * float(mainScreenHeight / 2 - ypos);

		// Direction : Spherical coordinates to Cartesian coordinates conversion
		direction = glm::vec3(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);
	}

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	// Up vector
	up = glm::cross(right, direction);


}

void MShaderController::updateMVPVariables()
{

}

void MShaderController::updateAndapplyGradients(float in_ms)
{
	std::vector<Message> gradientMessageVector = controllerMGCI.updateGradientsAndGetOutputs(in_ms);
	for (auto& currentGradientMessage : gradientMessageVector)
	{
		currentGradientMessage.open();
		std::string variableName = currentGradientMessage.readString();

		// For all fetched gradients, apply their values into the MShaderController's GLUniformRegistry (controllverValueRegistry)
		switch (currentGradientMessage.messageType)
		{
			case MessageType::MGRADIENT_VEC3_OUTPUT:
			{
				ECBPolyPoint ppoint = currentGradientMessage.readPoint();
				std::cout << "## Found vec3 value to insert. Name: " << variableName << " | Value: "; ppoint.printPointCoords(); std::cout << std::endl;
				glm::vec3 vec3Point(ppoint.x, ppoint.y, ppoint.z);
				controllerValueRegistry.insertVec3(variableName, vec3Point);
				break;
			};
		}
	}
}

