#include "stdafx.h"
#include "MSBasicCompute.h"

void MSBasicCompute::setupMShaderRequestsAndName()
{
	mShaderName = "MSBasicCompute";
	mShaderLocalValueRegistry.insertVec3("background_clear_color", glm::vec3(0.49f, 0.0f, 0.47f));
}

Message MSBasicCompute::checkRequiredBindings()
{
	Message returnMessage;
	return returnMessage;
}