#include "stdafx.h"
#include "TerrainComputeGearT1.h"

void TerrainComputeGearT1::initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef)
{
	width = in_width;
	height = in_height;
	window = in_windowRef;
	programID = in_programID;

	// get the uniforms; program must have been compiled before this
	mvpHandle = glGetUniformLocation(programID, "MVP");
	//textureUniform = glGetUniformLocation(programID, "terrainTexture");
	worldPosUniform = glGetUniformLocation(programID, "worldPosition");
	atlasWidthUniform = glGetUniformLocation(programID, "atlasTextureWidth");
	atlasTileWidthUniform = glGetUniformLocation(programID, "atlasTileTextureWidth");

	// set up the uniform requests
	GLUniformRequest reqMVP(GLDataType::MAT4, "MVP");
	uniformRequests.push_back(reqMVP);

	GLUniformRequest modelViewMatrix(GLDataType::MAT4, "ModelViewMatrix");
	uniformRequests.push_back(modelViewMatrix);

	GLUniformRequest worldPosition(GLDataType::VEC3, "worldPosition");
	uniformRequests.push_back(worldPosition);

	GLUniformRequest atlasTextureWidth(GLDataType::FLOAT, "atlasTextureWidth");
	uniformRequests.push_back(atlasTextureWidth);

	GLUniformRequest atlasTileTextureWidth(GLDataType::FLOAT, "atlasTileTextureWidth");
	uniformRequests.push_back(atlasTileTextureWidth);

	std::string deferredDrawRequest = "terrain";
	multiDrawArrayJobRequests.push_back(deferredDrawRequest);

}
void TerrainComputeGearT1::render()
{
	useProgram();	
	writeToGBuffers();
	glFlush();
}

void TerrainComputeGearT1::writeToGBuffers()
{
	glBindFramebuffer(GL_FRAMEBUFFER, deferredFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// remember, GL clear sets the depth buffer values to 1.0f, meaning they are the furthest away (closest to screen is 0.0f)
	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(terrainVaoID);								// bind to the terrain VAO
	setMatrices();
	GLMultiDrawArrayJob jobToUse = getMultiDrawArrayJob("terrain");
	glMultiDrawArrays(GL_TRIANGLES, jobToUse.multiStartIndices.get(), jobToUse.multiVertexCount.get(), jobToUse.drawCount);
	glFinish();
}

void TerrainComputeGearT1::setMatrices()
{
	GLuint mvpUniform = glGetUniformLocation(programID, "MVP");	// find the MVP uniform
	glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, &gearUniformRegistry.getMat4("MVP")[0][0]);		// set the uniform

	//glm::mat4 mv = view * model;
	GLuint mvUniform = glGetUniformLocation(programID, "ModelViewMatrix");
	glUniformMatrix4fv(mvUniform, 1, GL_FALSE, &gearUniformRegistry.getMat4("ModelViewMatrix")[0][0]);

	//glUniform3fv(worldPosUniform, 1, &gearUniformRegistry.getVec3("worldPosUniform")[0]);
	glUniform3fv(worldPosUniform, 1, &gearUniformRegistry.getVec3("worldPosition")[0]);
	glUniform1f(atlasWidthUniform, gearUniformRegistry.getFloat("atlasTextureWidth"));
	glUniform1f(atlasTileWidthUniform, gearUniformRegistry.getFloat("atlasTileTextureWidth"));
}

void TerrainComputeGearT1::passGLuintValue(std::string in_identifier, GLuint in_gluInt)
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
	else if (in_identifier == "render_quad_buffer")
	{
		registerNewBuffer(in_identifier, in_gluInt);			// register the "render_quad_buffer" NON-PERSISTENT buffer
		//quadBufferID = in_gluInt;
	}
	else if (in_identifier == "deferred_FBO")
	{
		registerNewFBO(in_identifier, in_gluInt);
		deferredFBO = in_gluInt;
	}
	else if (in_identifier == "terrainAtlas")
	{
		registerNewTexture(in_identifier, in_gluInt);
	}
}

void TerrainComputeGearT1::executeGearFunction(std::string in_identifier)
{
	if (in_identifier == "setup_terrain_VAO")
	{
		setupTerrainVAO();
	}

	/*
	else if (in_identifier == "acquire_subroutine_indices")
	{
		acquireSubroutineIndices();
	}
	*/
}

void TerrainComputeGearT1::setupTerrainVAO()
{
	OrganicGLWinUtils::createAndBindVertexArray(&terrainVaoID);	// create/bind the VAO to terrainVaoID
	glBindBuffer(GL_ARRAY_BUFFER, getPersistentBufferID("terrain_main"));	// bind to the immutable buffer before setting the attribs

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, (void*)20);
	glEnableVertexAttribArray(2);
}

/*
void TerrainComputeGearT1::acquireSubroutineIndices()
{
	pass1index = glGetSubroutineIndex(programID, GL_FRAGMENT_SHADER, "pass1");
	pass2index = glGetSubroutineIndex(programID, GL_FRAGMENT_SHADER, "pass2");
}
*/


void TerrainComputeGearT1::printData()
{
	std::cout << "Atlas width: " << gearUniformRegistry.getFloat("atlasTextureWidth") << std::endl;
	std::cout << "Atlas tile width: " << gearUniformRegistry.getFloat("atlasTileTextureWidth") << std::endl;
}