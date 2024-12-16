#include "stdafx.h"
#include "MSBasicGrayscale.h"

void MSBasicGrayscale::setupMShaderRequestsAndName()
{
	mShaderName = "MSBasicGrayscale";
	mShaderLocalValueRegistry.insertVec3("background_clear_color", glm::vec3(0.0f, 0.50f, 0.0f));
}

Message MSBasicGrayscale::checkRequiredBindings()
{
	Message returnMessage;
	return returnMessage;
}