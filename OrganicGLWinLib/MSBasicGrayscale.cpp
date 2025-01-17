#include "stdafx.h"
#include "MSBasicGrayscale.h"

void MSBasicGrayscale::setupMShaderRequestsAndName()
{
	mShaderName = "MSBasicGrayscale";
	mShaderLocalValueRegistry.insertVec3("background_clear_color", glm::vec3(0.0f, 0.50f, 0.0f));

	// Below: simple test to verify buffer creation request works (can be used as a reference/removed when needed)
	MAPIObjectRequest bufferRequest(MAPIObjectType::BUFFER, "testbuffer");
	MAPIObjectRequest bufferRequest2(MAPIObjectType::BUFFER, "testbuffer2");
	MAPIObjectRequest textureRequest(MAPIObjectType::TEXTURE, "testTexture");
	MAPIObjectRequest fboRequest(MAPIObjectType::FBO, "testFBO");
	bindingRequests.push_back(bufferRequest);
	bindingRequests.push_back(bufferRequest2);
	bindingRequests.push_back(textureRequest);
	bindingRequests.push_back(fboRequest);
}

Message MSBasicGrayscale::checkRequiredBindings()
{
	Message returnMessage;
	return returnMessage;
}