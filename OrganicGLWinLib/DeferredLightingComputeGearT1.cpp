#include "stdafx.h"
#include "DeferredLightingComputeGearT1.h"

void DeferredLightingComputeGearT1::initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef)
{
	width = in_width;
	height = in_height;
	window = in_windowRef;
	programID = in_programID;

	worldPosUniform = glGetUniformLocation(programID, "worldPosition");

	GLUniformRequest worldPosition(GLDataType::VEC3, "worldPosition");		// for lighting testing.
	uniformRequests.push_back(worldPosition);


}

void DeferredLightingComputeGearT1::render()
{
	useProgram();
	GLuint computeGroupsX = width / 16;	// 16 threads on x per work group
	GLuint computeGroupsY = height / 16;	// 16 threads on y per work group
	glUniform3fv(worldPosUniform, 1, &gearUniformRegistry.getVec3("worldPosition")[0]);
	glDispatchCompute(computeGroupsX, computeGroupsY, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);		// don't forget the memory barrie
}

void DeferredLightingComputeGearT1::passGLuintValue(std::string in_identifier, GLuint in_gluInt)
{

}

void DeferredLightingComputeGearT1::executeGearFunction(std::string in_identifier)
{

}

void DeferredLightingComputeGearT1::printData()
{

}