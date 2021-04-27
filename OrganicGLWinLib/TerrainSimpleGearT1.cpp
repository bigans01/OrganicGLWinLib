#include "stdafx.h"
#include "TerrainSimpleGearT1.h"
#include "ShaderMachineBase.h"

void TerrainSimpleGearT1::initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef, ShaderMachineBase* in_shaderMachineBasePtr)
{
	width = in_width;
	height = in_height;
	window = in_windowRef;
	programID = in_programID;
	gearMachinePtr = in_shaderMachineBasePtr;

	mvpHandle = glGetUniformLocation(programID, "MVP");

	// set up the uniform requests
	GLUniformRequest reqMVP(GLDataType::MAT4, "MVP");
	uniformRequests.push_back(reqMVP);

	std::string forwardDrawRequest = "terrain";
	multiDrawArrayJobRequests.push_back(forwardDrawRequest);
}

void TerrainSimpleGearT1::render()
{
	useProgram();
	renderTerrain();
}

void TerrainSimpleGearT1::renderTerrain()
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

void TerrainSimpleGearT1::setMatrices()
{
	GLuint mvpUniform = glGetUniformLocation(programID, "MVP");	// find the MVP uniform
	glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, &gearUniformRegistry.getMat4("MVP")[0][0]);		// set the uniform

	//glm::mat4 mv = view * model;
	//GLuint mvUniform = glGetUniformLocation(programID, "ModelViewMatrix");
	//glUniformMatrix4fv(mvUniform, 1, GL_FALSE, &gearUniformRegistry.getMat4("ModelViewMatrix")[0][0]);

	//glUniform3fv(worldPosUniform, 1, &gearUniformRegistry.getVec3("worldPosUniform")[0]);
	//glUniform1f(atlasWidthUniform, gearUniformRegistry.getFloat("atlasTextureWidth"));
	//glUniform1f(atlasTileWidthUniform, gearUniformRegistry.getFloat("atlasTileTextureWidth"));
}

void TerrainSimpleGearT1::passGLuintValue(std::string in_identifier, GLuint in_gluInt)
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

void TerrainSimpleGearT1::executeGearFunction(std::string in_identifier)
{
	if (in_identifier == "setup_terrain_VAO")
	{
		setupTerrainVAO();
	}
}



void TerrainSimpleGearT1::setupTerrainVAO()
{
	OrganicGLWinUtils::createAndBindVertexArray(&terrainVaoID);	// create/bind the VAO to quadVaoID
	glBindBuffer(GL_ARRAY_BUFFER, getPersistentBufferID("terrain_main"));	// bind to the immutable buffer before setting the attribs
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)12);
	glEnableVertexAttribArray(1);
}

void TerrainSimpleGearT1::printData()
{
	std::cout << "Atlas width: " << gearUniformRegistry.getFloat("atlasTextureWidth") << std::endl;
	std::cout << "Atlas tile width: " << gearUniformRegistry.getFloat("atlasTileTextureWidth") << std::endl;
}

void TerrainSimpleGearT1::interpretMessage(Message in_message)
{

}

