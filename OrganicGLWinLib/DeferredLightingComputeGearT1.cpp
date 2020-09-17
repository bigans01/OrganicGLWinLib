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

	// compute width
	GLUniformRequest reqWidth(GLDataType::INT, "screenWidth");
	uniformRequests.push_back(reqWidth);

	// compute height
	GLUniformRequest reqHeight(GLDataType::INT, "screenHeight");
	uniformRequests.push_back(reqHeight);


}

void DeferredLightingComputeGearT1::render()
{
	useProgram();
	GLuint computeGroupsX = width / 16;	// 16 threads on x per work group
	GLuint computeGroupsY = height / 16;	// 16 threads on y per work group
	glUniform3fv(worldPosUniform, 1, &gearUniformRegistry.getVec3("worldPosition")[0]);
	glDispatchCompute(computeGroupsX, computeGroupsY, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);		// don't forget the memory barrier

	// test, apply the single light twice.
	//swapImageTargetBindings();
	//glUniform3fv(worldPosUniform, 1, &gearUniformRegistry.getVec3("worldPosition")[0]);
	//glDispatchCompute(computeGroupsX, computeGroupsY, 1);
	//glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);		// don't forget the memory barrier

	//resetSwapImage();		// may not be used
}

void DeferredLightingComputeGearT1::passGLuintValue(std::string in_identifier, GLuint in_gluInt)
{
	if (in_identifier == "original_image_unit_1_texture_ID")
	{
		originalImageUnit1TextureID = in_gluInt;
	}
	else if (in_identifier == "original_image_unit_2_texture_ID")
	{
		originalImageUnit2TextureID = in_gluInt;
	}
}

void DeferredLightingComputeGearT1::executeGearFunction(std::string in_identifier)
{

}

void DeferredLightingComputeGearT1::printData()
{

}

void DeferredLightingComputeGearT1::swapImageTargetBindings()
{
	// initial values when SM starts are:
	// GL_TEXTURE31, image unit 1, texture id 7
	// GL_TEXTURE11, image unit 0, texture id 9 

	if (currentTargetBinding == 1)
	{
		// bind image unit 0 to texture id 9
		//glBindImageTexture(0, 9, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);	// bind to image unit 0, for this texture (it can now be sampled in the compute shader)
		glBindImageTexture(0, originalImageUnit2TextureID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);	// bind to image unit 0, for this texture (it can now be sampled in the compute shader)

		// bind image unit 1 to texture id 8
		//glBindImageTexture(1, 8, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);	// bind to image unit 0, for this texture (it can now be sampled in the compute shader)
		glBindImageTexture(1, originalImageUnit1TextureID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);	// bind to image unit 0, for this texture (it can now be sampled in the compute shader)
		currentTargetBinding = 0;
	}
	else if (currentTargetBinding == 0)
	{
		//glBindImageTexture(in_imageUnit, getTextureID(in_imageName), 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);	// bind to image unit 0, for this texture (it can now be sampled in the compute shader)
		
		// bind image unit 0 to texture id 8
		//glBindImageTexture(0, 8, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);	// bind to image unit 0, for this texture (it can now be sampled in the compute shader)
		glBindImageTexture(0, originalImageUnit1TextureID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);	// bind to image unit 0, for this texture (it can now be sampled in the compute shader)

		// bind image unit 1 to texture id 9
		//glBindImageTexture(1, 9, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);	// bind to image unit 0, for this texture (it can now be sampled in the compute shader)
		glBindImageTexture(1, originalImageUnit2TextureID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);	// bind to image unit 0, for this texture (it can now be sampled in the compute shader)
		currentTargetBinding = 1;
	};
}

void DeferredLightingComputeGearT1::resetSwapImage()
{
	if (currentTargetBinding == 0)
	{
		//glBindImageTexture(in_imageUnit, getTextureID(in_imageName), 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);	// bind to image unit 0, for this texture (it can now be sampled in the compute shader)

		// bind image unit 0 to texture id 8
		glBindImageTexture(0, originalImageUnit1TextureID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);	// bind to image unit 0, for this texture (it can now be sampled in the compute shader)

		// bind image unit 1 to texture id 9
		glBindImageTexture(1, originalImageUnit2TextureID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);	// bind to image unit 0, for this texture (it can now be sampled in the compute shader)
		currentTargetBinding = 1;
	};
}