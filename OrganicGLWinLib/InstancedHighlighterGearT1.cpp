#include "stdafx.h"
#include "InstancedHighlighterGearT1.h"
#include "ShaderMachineBase.h"

void InstancedHighlighterGearT1::initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef, ShaderMachineBase* in_shaderMachineBasePtr)
{
	width = in_width;
	height = in_height;
	window = in_windowRef;

	programIndex.insertDefaultEntry(in_programID); 
	programID = programIndex.selectProgram();

	gearMachinePtr = in_shaderMachineBasePtr;

	// set up the uniform requests
	GLUniformRequest reqMVP(GLDataType::MAT4, "MVP");
	uniformRequests.push_back(reqMVP);

	// set up the instancing request
	std::string instancedDrawRequest = "instanced_highlight_job1";
	drawElementsInstancedRequests.push_back(instancedDrawRequest);
}

void InstancedHighlighterGearT1::render()
{
	useProgram();
	glBindVertexArray(instancingVAO);
	GLuint mvpUniform = glGetUniformLocation(programID, "MVP");		// find the MVP uniform
	glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, &gearUniformRegistry.getMat4("MVP")[0][0]);		// set the uniform
	GLDrawElementsInstancedJob jobToUse = getDrawElementsInstancedJob("instanced_highlight_job1");
	glDrawArraysInstanced(GL_TRIANGLES, 0, 3, jobToUse.numberOfElementsToRender);
}

void InstancedHighlighterGearT1::passGLuintValue(std::string in_identifier, GLuint in_gluInt)
{
	if (in_identifier == "mesh_buffer")
	{
		registerNewBuffer(in_identifier, in_gluInt);
	}

	if (in_identifier == "matrices_buffer")
	{
		registerNewBuffer(in_identifier, in_gluInt);
	}
}

void InstancedHighlighterGearT1::executeGearFunction(std::string in_identifier)
{
	if (in_identifier == "setup_instancing_buffers_and_VAO")
	{
		setupInstancingBuffersAndVAO();
	}
}

void InstancedHighlighterGearT1::setupInstancingBuffersAndVAO()
{
	std::cout << "########### CALLING INSTANCED HIGHLIGHT SET UP " << std::endl;



	OrganicGLWinUtils::createAndBindVertexArray(&instancingVAO);	// create/bind the instancing VAO
	glBindBuffer(GL_ARRAY_BUFFER, getBufferID("mesh_buffer"));	// Bind to the mesh buffer; contains data about the actual mesh triangles
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)0);		// First attribute: a vec3 representing the point data, before it is translated by MVP.
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)12);		// First attribute: a vec3 representing the point data, before it is translated by MVP.
	glEnableVertexAttribArray(0);

	
	glBindBuffer(GL_ARRAY_BUFFER, getBufferID("matrices_buffer"));						// Bind to the matrices buffer; these are just world-positiion matrices, remember

	// in order to use a mat4 as a uniform in the vertex shader, we must put 4x vec4s into a buffer, and set the attributes appropriately. It isn't possible to
	// do a mat4 attribute; but we can do 4x vec4s, which is the same thing.
	
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);

	glEnableVertexAttribArray(0);  // don't forget to go back to enabling VAO 0.
}