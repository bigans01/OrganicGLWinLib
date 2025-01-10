#include "stdafx.h"
#include "MSBasicGrayscale.h"

void MSBasicGrayscale::setupMShaderRequestsAndName()
{
	mShaderName = "MSBasicGrayscale";
	mShaderLocalValueRegistry.insertVec3("background_clear_color", glm::vec3(0.0f, 0.50f, 0.0f));

	// Below: simple test to verify buffer creation request works (can be used as a reference/removed when needed)
	MAPIObjectRequest testRequest(MAPIObjectType::BUFFER, "testbuffer");
	bindingRequests.push_back(testRequest);
}

Message MSBasicGrayscale::checkRequiredBindings()
{
	Message returnMessage;
	return returnMessage;
}