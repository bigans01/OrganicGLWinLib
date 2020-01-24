#include "stdafx.h"
#include "DeferredComputeGearT1.h"


void DeferredComputeGearT1::initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef)
{
	width = in_width;
	height = in_height;
	window = in_windowRef;
	programID = in_programID;
}
void DeferredComputeGearT1::render()
{
	useProgram();
	GLuint computeGroupsX = width / 16;	// 16 threads on x per work group
	GLuint computeGroupsY = height / 16;	// 16 threads on y per work group
	glDispatchCompute(computeGroupsX, computeGroupsY, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);		// don't forget the memory barrier
}
void DeferredComputeGearT1::passGLuintValue(std::string in_identifier, GLuint in_gluInt)
{

}
void DeferredComputeGearT1::executeGearFunction(std::string in_identifier)
{

}
void DeferredComputeGearT1::printData()
{

}
