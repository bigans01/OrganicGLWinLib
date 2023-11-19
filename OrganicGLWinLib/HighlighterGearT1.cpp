#include "stdafx.h"
#include "HighlighterGearT1.h"
#include "ShaderMachineBase.h"

void HighlighterGearT1::initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef, ShaderMachineBase* in_shaderMachineBasePtr)
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


	// set up the draw call data
	std::string deferredDrawRequest = "highlighter_draw_job";
	multiDrawArrayJobRequests.push_back(deferredDrawRequest);

}
void HighlighterGearT1::render()
{
	useProgram();

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// remember, GL clear sets the depth buffer values to 1.0f, meaning they are the furthest away (closest to screen is 0.0f)
	//glDisable(GL_DEPTH_TEST);

	//glClear(GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(highlighterVAO);								// bind to the terrain VAO, before drawing
	GLuint mvpUniform = glGetUniformLocation(programID, "MVP");		// find the MVP uniform
	glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, &gearUniformRegistry.getMat4("MVP")[0][0]);		// set the uniform
	GLMultiDrawArrayJob jobToUse = getMultiDrawArrayJob("highlighter_draw_job");	// fetch the draw job
	//std::cout << "(HighlighterGearT1): jobToUse draw count is: " << jobToUse.drawCount << " | size of draw array job map is: " << gearMultiDrawArrayJobMap.size() << std::endl;
	glMultiDrawArrays(GL_TRIANGLES, jobToUse.multiStartIndices.get(), jobToUse.multiVertexCount.get(), jobToUse.drawCount);	// render with draw job data

	//std::cout << "Rendering highlights..." << std::endl;
	//std::cout << "(HighlighterGearT1): testing machine access proxy; there are " << accessProxy.getMachineGearRef()->size() << std::endl;

	//GLint* vertexOut = jobToUse.multiStartIndices.get();
	//std::cout << "Vertices start: " << vertexOut[0] << std::endl;

	//GLint* numberOut = jobToUse.multiVertexCount.get();
	//std::cout << "Number of vertices: " << numberOut[0] << std::endl;
}

void HighlighterGearT1::passGLuintValue(std::string in_identifier, GLuint in_gluInt)
{
	if (in_identifier == "highlighter_buffer")
	{
		registerNewBuffer(in_identifier, in_gluInt);
	}
}

void HighlighterGearT1::executeGearFunction(std::string in_identifier)
{
	if (in_identifier == "setup_terrain_highlighter_VAO")
	{
		setupHighlighterVAO();
	}
}

void HighlighterGearT1::setupHighlighterVAO()
{
	OrganicGLWinUtils::createAndBindVertexArray(&highlighterVAO);	// create/bind the highlighter VAO
	glBindBuffer(GL_ARRAY_BUFFER, getBufferID("highlighter_buffer"));	// bind to the highlighter buffer

	
	//glVertexAttribPointer(
	//	0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	//	3,                  // size
	//	GL_FLOAT,           // type
	//	GL_FALSE,           // normalized?
	//	0,                  // stride = 0 (tightly packed); bytes offset between consecutive generic vertex attributes is 0.
	//	(void*)0            /* array buffer offset. Number following (void*) indicates offset point to begin reading from in the pointed-to buffer, measured in bytes;
	//						For instance, if the data begins at byte 10000, you would put (void*)10000 in the array you are reading.
	//						*/
	//);
	

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)0);		// First attribute: a vec3 representing the point data, before it is translated by MVP.
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)12);    // Second attribute: a vec3 representing the output color.

	glEnableVertexAttribArray(0);
}