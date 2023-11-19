#include "stdafx.h"
#include "ComputeCopyRBGFromTextureToImageGearT1.h"
#include "ShaderMachineBase.h"

void ComputeCopyRBGFromTextureToImageGearT1::initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef, ShaderMachineBase* in_shaderMachineBasePtr)
{
	width = in_width;
	height = in_height;
	window = in_windowRef;

	programIndex.insertDefaultEntry(in_programID); 
	programID = programIndex.selectProgram();

	gearMachinePtr = in_shaderMachineBasePtr;

	screenWidthUniform = glGetUniformLocation(programID, "screenWidth");
	screenHeightUniform = glGetUniformLocation(programID, "screenHeight");

	// set up the uniform requests
	GLUniformRequest reqMVP(GLDataType::MAT4, "MVP");
	uniformRequests.push_back(reqMVP);

	// compute width
	GLUniformRequest reqWidth(GLDataType::INT, "screenWidth");
	uniformRequests.push_back(reqWidth);

	// compute height
	GLUniformRequest reqHeight(GLDataType::INT, "screenHeight");
	uniformRequests.push_back(reqHeight);
}

void ComputeCopyRBGFromTextureToImageGearT1::render()
{
	useProgram();

	//std::cout << "!! Executing compute transfer... " << std::endl;
	//setDrawMatrices();
	//drawQuad();
	GLuint computeGroupsX = width / 16;	// 16 threads on x per work group
	GLuint computeGroupsY = height / 16;	// 16 threads on y per work group
	
	// send the screen resolution to the compute shader
	//glUniform1i

	//std::cout << "Screen width/height: " << gearUniformRegistry.getInt("screenWidth") << " / " << gearUniformRegistry.getInt("screenHeight") << std::endl;

	//gearUniformRegistry.getInt("screenWidth");
	glUniform1i(screenWidthUniform, gearUniformRegistry.getInt("screenWidth"));
	glUniform1i(screenHeightUniform, gearUniformRegistry.getInt("screenHeight"));
	glDispatchCompute(computeGroupsX, computeGroupsY, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);		// don't forget the memory barrier
}