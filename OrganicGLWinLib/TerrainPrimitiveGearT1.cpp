#include "stdafx.h"
#include "TerrainPrimitiveGearT1.h"
#include "ShaderMachineBase.h"

void TerrainPrimitiveGearT1::initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef, ShaderMachineBase* in_shaderMachineBasePtr)
{
	width = in_width;
	height = in_height;
	window = in_windowRef;
	programID = in_programID;
	accessProxy.setMachinePtr(in_shaderMachineBasePtr);

	mvpHandle = glGetUniformLocation(programID, "MVP");

	// set up the uniform requests
	GLUniformRequest reqMVP(GLDataType::MAT4, "MVP");
	uniformRequests.push_back(reqMVP);

	std::string forwardDrawRequest = "terrain";
	multiDrawArrayJobRequests.push_back(forwardDrawRequest);
}

void TerrainPrimitiveGearT1::render()
{
	useProgram();
	renderTerrain();
	glFlush();
}

void TerrainPrimitiveGearT1::passGLuintValue(std::string in_identifier, GLuint in_gluInt)
{
	if (in_identifier == "terrain_main")
	{
		registerNewPersistentBuffer(in_identifier, in_gluInt);	// register the "terrain_main" PERSISTENT buffer
		//terrainBufferID = in_gluInt;		// set the main terrain buffer ID
	}
	else if (in_identifier == "terrain_swap")
	{
		registerNewPersistentBuffer(in_identifier, in_gluInt);	// register the "terrain_swap" PERSISTENT buffer
		//terrainSwapID = in_gluInt;			// set the swap terrain buffer ID
	}
}

void TerrainPrimitiveGearT1::executeGearFunction(std::string in_identifier)
{
	if (in_identifier == "setup_terrain_VAO")
	{
		setupTerrainVAO();
	}
}

void TerrainPrimitiveGearT1::printData()
{
	std::cout << "Atlas width: " << gearUniformRegistry.getFloat("atlasTextureWidth") << std::endl;
	std::cout << "Atlas tile width: " << gearUniformRegistry.getFloat("atlasTileTextureWidth") << std::endl;
}

void TerrainPrimitiveGearT1::setupTerrainVAO()
{
	OrganicGLWinUtils::createAndBindVertexArray(&terrainVaoID);	// create/bind the VAO to quadVaoID
	glBindBuffer(GL_ARRAY_BUFFER, getPersistentBufferID("terrain_main"));	// bind to the immutable buffer before setting the attribs
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride = 0 (tightly packed); bytes offset between consecutive generic vertex attributes is 0.
		(void*)0            /* array buffer offset. Number following (void*) indicates offset point to begin reading from in the pointed-to buffer, measured in bytes;
							For instance, if the data begins at byte 10000, you would put (void*)10000 in the array you are reading.
							*/
	);
}

void TerrainPrimitiveGearT1::renderTerrain()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // bind to default frame buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// remember, GL clear sets the depth buffer values to 1.0f, meaning they are the furthest away (closest to screen is 0.0f)
	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(terrainVaoID);
	setMatrices();
	GLMultiDrawArrayJob jobToUse = getMultiDrawArrayJob("terrain");
	//glMultiDrawArrays(GL_TRIANGLES, in_startArray, in_vertexCount, in_numberOfCollections);		// draw the terrain
	glMultiDrawArrays(GL_TRIANGLES, jobToUse.multiStartIndices.get(), jobToUse.multiVertexCount.get(), jobToUse.drawCount);
}

void TerrainPrimitiveGearT1::setMatrices()
{
	GLuint mvpUniform = glGetUniformLocation(programID, "MVP");	// find the MVP uniform
	glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, &gearUniformRegistry.getMat4("MVP")[0][0]);		// set the uniform
}