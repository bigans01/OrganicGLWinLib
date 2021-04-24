#include "stdafx.h"
#include "DeferredComputeResultsGearT1.h"
#include "ShaderMachineBase.h"

void DeferredComputeResultsGearT1::initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef, ShaderMachineBase* in_shaderMachineBasePtr)
{
	width = in_width;
	height = in_height;
	window = in_windowRef;
	programID = in_programID;
	accessProxy.setMachinePtr(in_shaderMachineBasePtr);


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
void DeferredComputeResultsGearT1::render()
{
	useProgram();
	setDrawMatrices();
	drawQuad();
}


void DeferredComputeResultsGearT1::passGLuintValue(std::string in_identifier, GLuint in_gluInt)
{
	if (in_identifier == "compute_quad_buffer")
	{
		std::cout << "!!!!! ++++ SETTING UP COMPUTE QUAD BUFFER" << std::endl;
		registerNewBuffer(in_identifier, in_gluInt);			// register the "render_quad_buffer" NON-PERSISTENT buffer
		setUpRenderQuad();			// prepare the render quad
	}
	else if (in_identifier == "deferred_FBO")
	{
		registerNewFBO(in_identifier, in_gluInt);
	}
}

void DeferredComputeResultsGearT1::executeGearFunction(std::string in_identifier)
{

}

void DeferredComputeResultsGearT1::printData()
{

}

void DeferredComputeResultsGearT1::setUpRenderQuad()
{
	OrganicGLWinUtils::createAndBindVertexArray(&quadVaoID);	// create/bind the VAO to quadVaoID
	glBindBuffer(GL_ARRAY_BUFFER, getBufferID("compute_quad_buffer"));	// bind

	// quad points, and UVs
	GLfloat quadData[] =
	{												// first 3 floats = position, 4th and 5th = texture coords. 
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,		// 1st point, lower left
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,		// 2nd point, lower right
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,				// 3rd point, upper right
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,		// 4th point, lower left
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,				// 5th point, upper right
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,				// 6th point, upper left
	};

	glBufferData(GL_ARRAY_BUFFER, 6 * 5 * sizeof(float), quadData, GL_STATIC_DRAW);		// populate the data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)12);
	glEnableVertexAttribArray(1);
}

void DeferredComputeResultsGearT1::drawQuad()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);	// bind back to the default framebuffer
	glBindVertexArray(quadVaoID);
	glUniform1i(screenWidthUniform, gearUniformRegistry.getInt("screenWidth"));
	glUniform1i(screenHeightUniform, gearUniformRegistry.getInt("screenHeight"));
	glDrawArrays(GL_TRIANGLES, 0, 6);		// draw the quad

	// copy the depth from the deferred FBO that we put the G-buffers into, to the default FBO, but only AFTER we draw the triangles.
	glBindFramebuffer(GL_READ_FRAMEBUFFER, getFBOID("deferred_FBO"));		
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height,
		GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredComputeResultsGearT1::setDrawMatrices()
{
	glm::mat4 quadMatrix = glm::mat4(1.0);
	GLuint mvpUniform = glGetUniformLocation(programID, "MVP");			// find the MVP uniform
	glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, &quadMatrix[0][0]);		// set the uniform
}