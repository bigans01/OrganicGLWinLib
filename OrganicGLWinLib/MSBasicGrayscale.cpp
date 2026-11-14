#include "stdafx.h"
#include "MSBasicGrayscale.h"

void MSBasicGrayscale::setupMShaderRequestsAndName()
{
	// First thing, always: make sure the MGearManager is initialized properly.
	mShaderGearManager.initializeMGearManager(parentBindingMapPtr, parentValueRegistryPtr);

	// Create necessary MGear objects in the mShaderGearManager:
	// -BasicGrayscaleMG (sequence location 0)
	mShaderGearManager.insertMGear(0, "BasicGrayscaleMG", std::move(std::unique_ptr<MGearBase>(new BasicGrayscaleMG())));

	mShaderName = "MSBasicGrayscale";

	// Setup local, preferred values that are common between other shaders (such as background color, some depth value, etc)
	mShaderLocalValueRegistry.insertVec3("background_clear_color", glm::vec3(0.0f, 0.50f, 0.0f));

	// Below: simple test to verify buffer creation request works (can be used as a reference/removed when needed)
	MAPIObjectRequest bufferRequest(MAPIObjectType::BUFFER, "testbuffer");
	MAPIObjectRequest bufferRequest2(MAPIObjectType::BUFFER, "testRenderBuffer");
	MAPIObjectRequest textureRequest(MAPIObjectType::TEXTURE, "testTexture");
	MAPIObjectRequest fboRequest(MAPIObjectType::FBO, "testFBO");
	MAPIObjectRequest vaoRequest(MAPIObjectType::VAO, "commonVAO");

	bindingRequests.push_back(bufferRequest);
	bindingRequests.push_back(bufferRequest2);
	bindingRequests.push_back(textureRequest);
	bindingRequests.push_back(fboRequest);
	bindingRequests.push_back(vaoRequest);

	// Create the expected matrix values we need to find, such as the basicMVP.
	GLUniformRequest basicMVPRequest(GLDataType::MAT4, "absolute_MVP_matrix");
	mShaderUniformRequests.push_back(basicMVPRequest);
}

Message MSBasicGrayscale::checkRequiredBindings()
{
	Message returnMessage;
	return returnMessage;
}

void MSBasicGrayscale::prepareAndRender()
{
	// Check if we to scan for updates this tick; if we must do that, have all the gears do their scans (if they have any to do),
	// and reset the flag.
	if (scanForUpdatesFlag)
	{
		// ... do stuff
		mShaderGearManager.sendScanSignalToGears(runFullScan, &updatesToProcess);
		resetUpdateObjects();
	}

	// Testing only: get the current value of the background_clear_color from 
	// the MShader's registry, and use that in a call to glClear when all gears are rendered.
	// (this was done to check if gradient smoothing was functional, and how it looked.)
	auto currentClearColor = parentValueRegistryPtr->getVec3("background_clear_color");
	glClearColor(currentClearColor.x, currentClearColor.y, currentClearColor.z, 0.0f);

	// Call the following gears:
	// BasicGrayscaleMG
	renderAllGears();

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}