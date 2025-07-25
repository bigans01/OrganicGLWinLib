#include "stdafx.h"
#include "MSBasicCompute.h"

void MSBasicCompute::setupMShaderRequestsAndName()
{
	// First thing, always: make sure the MGearManager is initialized properly.
	mShaderGearManager.initializeMGearManager(parentBindingMapPtr, parentValueRegistryPtr);

	// Create necessary MGear objects in the mShaderGearManager:
	// -BasicMG (sequence location 0)
	mShaderGearManager.insertMGear(0, "BasicMG", std::move(std::unique_ptr<MGearBase>(new BasicMG())));

	mShaderName = "MSBasicCompute";
	mShaderLocalValueRegistry.insertVec3("background_clear_color", glm::vec3(0.49f, 0.0f, 0.47f));

	// Below: simple test to verify buffer creation request works (can be used as a reference/removed when needed)
	MAPIObjectRequest bufferRequest(MAPIObjectType::BUFFER, "testbuffer");
	MAPIObjectRequest textureRequest(MAPIObjectType::TEXTURE, "testTexture");
	MAPIObjectRequest fboRequest(MAPIObjectType::FBO, "testFBO");

	// Below: testing only, to ensure that 3d-key retrieval works for BUFFER objects
	MAPIObjectRequest threeDTest(MAPIObjectType::BUFFER, EnclaveKeyDef::EnclaveKey(1, 1, 1), "3dTest");

	bindingRequests.push_back(bufferRequest);
	bindingRequests.push_back(textureRequest);
	bindingRequests.push_back(fboRequest);

	bindingRequests.push_back(threeDTest);
}

Message MSBasicCompute::checkRequiredBindings()
{
	Message returnMessage;
	return returnMessage;
}

void MSBasicCompute::prepareAndRender()
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

	renderAllGears();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}