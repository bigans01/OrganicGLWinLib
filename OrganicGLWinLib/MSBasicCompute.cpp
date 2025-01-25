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
	bindingRequests.push_back(bufferRequest);
	bindingRequests.push_back(textureRequest);
	bindingRequests.push_back(fboRequest);
}

Message MSBasicCompute::checkRequiredBindings()
{
	Message returnMessage;
	return returnMessage;
}