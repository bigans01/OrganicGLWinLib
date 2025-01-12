#include "stdafx.h"
#include "MSBasicCompute.h"

void MSBasicCompute::setupMShaderRequestsAndName()
{
	mShaderName = "MSBasicCompute";
	mShaderLocalValueRegistry.insertVec3("background_clear_color", glm::vec3(0.49f, 0.0f, 0.47f));

	// Below: simple test to verify buffer creation request works (can be used as a reference/removed when needed)
	MAPIObjectRequest testRequest(MAPIObjectType::BUFFER, "testbuffer");
	bindingRequests.push_back(testRequest);
}

Message MSBasicCompute::checkRequiredBindings()
{
	Message returnMessage;
	return returnMessage;
}