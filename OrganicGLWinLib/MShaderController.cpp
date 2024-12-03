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

	return true;
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