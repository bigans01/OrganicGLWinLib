#include "stdafx.h"
#include "DeferredComputeGearT1.h"
#include "ShaderMachineBase.h"

void DeferredComputeGearT1::initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef, ShaderMachineBase* in_shaderMachineBasePtr)
{
	width = in_width;
	height = in_height;
	window = in_windowRef;

	programIndex.insertDefaultEntry(in_programID); 
	programID = programIndex.selectProgram();

	gearMachinePtr = in_shaderMachineBasePtr;

	screenWidthUniform = glGetUniformLocation(programID, "screenWidth");
	screenHeightUniform = glGetUniformLocation(programID, "screenHeight");

	// compute width
	GLUniformRequest reqWidth(GLDataType::INT, "screenWidth");
	uniformRequests.push_back(reqWidth);

	// compute height
	GLUniformRequest reqHeight(GLDataType::INT, "screenHeight");
	uniformRequests.push_back(reqHeight);
}
void DeferredComputeGearT1::render()
{
	useProgram();
	GLuint computeGroupsX = width / 16;	// 16 threads on x per work group
	GLuint computeGroupsY = height / 16;	// 16 threads on y per work group

	glUniform1i(screenWidthUniform, gearUniformRegistry.getInt("screenWidth"));
	glUniform1i(screenHeightUniform, gearUniformRegistry.getInt("screenHeight"));

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

void DeferredComputeGearT1::interpretMessage(Message in_message)
{

}
