#include "stdafx.h"
#include "InstancedHighlighterGearT1.h"

void InstancedHighlighterGearT1::initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef)
{
	width = in_width;
	height = in_height;
	window = in_windowRef;
	programID = in_programID;

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

	// new code for instancing goes here
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

void InstancedHighlighterGearT1::printData()
{

}

void InstancedHighlighterGearT1::setupInstancingBuffersAndVAO()
{
	OrganicGLWinUtils::createAndBindVertexArray(&instancingVAO);	// create/bind the instancing VAO

	// bind to the first buffer (the mesh itself)
	// call vertex attrib arrays for this buffer, and enable them

	// bind to the first buffer (the matrices for each instance)
	// call vertex attrib arrays for this buffer, and enable them


	// don't forget to go back to enabling VAO 0.
}