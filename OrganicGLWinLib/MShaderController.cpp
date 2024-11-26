#include "stdafx.h"
#include "MShaderController.h"

bool MShaderController::switchMShader(std::string in_shaderToSwitchTo)
{
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

bool MShaderController::BindingMap::doesBindingExist(std::string in_bindingToFind)
{
	bool wasFound = false;
	auto bindingFinder = bindings.find(in_bindingToFind);
	if (bindingFinder != bindings.end())
	{
		wasFound = true;
	}
	return wasFound;
}

void MShaderController::BindingMap::insertBinding(std::string in_bindingName, int in_bindingValue)
{
	bindings[in_bindingName] = in_bindingValue;
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

}