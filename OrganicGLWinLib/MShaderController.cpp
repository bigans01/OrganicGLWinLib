#include "stdafx.h"
#include "MShaderController.h"

MShaderController::MShaderController()
{
	mShaderCycler.setCatalogRef(&catalog);
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
	mainScreenWidth = mResolution.computeScreenWidth;
	mainScreenHeight = mResolution.computeScreenHeight;
	mainComputeDim = computeDim;

	isResolutionSet = true;
	return Message(MessageType::MSHADER_INFO, std::string("Compute resolution set"));
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

}

void MShaderController::createMShaders()
{
	while (!mShaderSetupQueue.empty())
	{
		auto currentShaderMessage = mShaderSetupQueue.front();
		switch (currentShaderMessage.messageType)
		{
			// Basic compute shader
			case MessageType::MSHADER_CREATE_MSBASICCOMPUTE:
			{
				// Attempt to insert, only bother continuing if true.
				if (catalog.insertMShader("MSBasicCompute", std::move(std::unique_ptr<MShaderBase>(new MSBasicCompute()))))
				{
					catalog.getShaderRef("MSBasicCompute")->setSharedObjectPointers(&controllerButtonPanelContainer,
															&controllerSliderPanelContainer,
															&controllerInputPanelContainer,
															&controllerMachineFeedback,
															&controllerBindings,
															&controllerValueRegistry
														);

					catalog.getShaderRef("MSBasicCompute")->setupMShaderRequestsAndName();
					mShaderInfoQueue.push(Message(MessageType::MSHADER_INFO, std::string("Verifying bindings on MSBasicCompute...")));
				}
				else
				{
					mShaderInfoQueue.push(Message(MessageType::MSHADER_INFO, std::string("Failed to create MSBasicCompute; ...did it already exist?")));
				}
				break;
			};

			// Grayscale shader 
			case MessageType::MSHADER_CREATE_MSBASICGRAYSCALE:
			{
				// Attempt to insert, only bother continuing if true.
				std::cout << "!! Found MessageType::MSHADER_CREATE_MSBASICGRAYSCALE; will attempt grayscale shader creation." << std::endl;
				if (catalog.insertMShader("MSBasicGrayscale", std::move(std::unique_ptr<MShaderBase>(new MSBasicGrayscale()))))
				{
					catalog.getShaderRef("MSBasicGrayscale")->setSharedObjectPointers(&controllerButtonPanelContainer,
															&controllerSliderPanelContainer,
															&controllerInputPanelContainer,
															&controllerMachineFeedback,
															&controllerBindings,
															&controllerValueRegistry
														);
					catalog.getShaderRef("MSBasicGrayscale")->setupMShaderRequestsAndName();
					mShaderInfoQueue.push(Message(MessageType::MSHADER_INFO, std::string("Verifying bindings on MSBasicGrayscale...")));
				}
				else
				{
					mShaderInfoQueue.push(Message(MessageType::MSHADER_INFO, std::string("Failed to create MSBasicGrayscale; ...did it already exist?")));
				}

				break;
			};
		}
		mShaderSetupQueue.pop();
	}
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


void MShaderController::insertNewGradient(Message in_gradientInsertionMessage)
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
												singleCycleDivisorMs);
			break;
		}

		case MessageType::MSHADER_SETUP_FINITE_MGRADIENT:
		{
			std::cout << "Sending finite gradient to controllerMGCI..." << std::endl;
			controllerMGCI.insertFiniteGradient(gradientName,
												messageForMGC,
												singleCyclePiValue,
												singleCycleDivisorMs, 
												cycleDuration);

			break;
		}
	}
}

void MShaderController::runTick()
{
	processShaderChangeRequests();		// Step 1:	if attemptSwitchOnNextTick was called, attempt to switch
										//			to the shader given by the current value of nextShaderToSwitchTo.
										//		 	Check if there are any transitional hints to process as well, if the
										//			switch was sucessful, to create new gradients.								

	calculatePassedTime();				// Step 2: fetch the total time that has elapsed, to use as a value for updateAndApplyGradients
	updateMVPVariables();				// Step 3: Calculate any MVP matrices or other similiar values that have been setup
	updateAndapplyGradients(millisecondsSinceLastTimestamp);		// Step 4: Update gradients, and apply them
	controllerMGCI.deleteExpiredFiniteGradients();		// Step 5: remove expired gradients.
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
				std::cout << "Initial shader selected; ignoring transitional hint scan.";
			}

			// ...but if we're not on the first, check the hints.
			else if (mShaderCycler.getNumberOfLoadedShaders() == 2)
			{
				std::cout << "!!! Scanning for hints." << std::endl;
				auto transitionalHints = controllerHintIndexer.fetchTransitionalHints();
				for (auto& currentHint : transitionalHints)
				{
					// each hint should be mapped to 1 gradient attempt.
				}
			}
		}


		// Once all is said and done, reset the flag, and nextShaderToSwitchTo value
		nextShaderToSwitchTo = "";
		shaderSwitchAttemptFlag = false;
	}
}


void MShaderController::calculatePassedTime()
{
	// Find what the current time is, and subtract the lastTimeStamp value from it, to get
	// the updated value for millisecondsSinceLastTimestamp. Once calculated, set the lastTimeStamp = currentTimeStamp.
	//
	// This function should be called every tick that the MShaderController is running.

	currentTimeStamp = std::chrono::steady_clock::now();	

	// If this the very first time this function is called, the timestamps will be equal;
	// we must then set wasInitialTimeCalculated to true so that this doesn't happen again.
	if (!wasInitialTimeCalculated)
	{
		lastTimeStamp = currentTimeStamp;
		wasInitialTimeCalculated = true;
	}

	millisecondsSinceLastTimestamp = std::chrono::duration_cast<std::chrono::milliseconds>(currentTimeStamp - lastTimeStamp).count();
	lastTimeStamp = currentTimeStamp;
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

