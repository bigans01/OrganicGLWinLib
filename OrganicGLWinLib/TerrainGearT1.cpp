#include "stdafx.h"
#include "TerrainGearT1.h"

void TerrainGearT1::initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef)
{
	width = in_width;
	height = in_height;
	window = in_windowRef;
	programID = in_programID;

	// get the uniforms; program must have been compiled before this
	mvpHandle = glGetUniformLocation(programID, "MVP");
	textureUniform = glGetUniformLocation(programID, "terrainTexture");
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

	std::string deferredDrawRequest = "deferred";
	multiDrawArrayJobRequests.push_back(deferredDrawRequest);

}
void TerrainGearT1::render()
{
	useProgram();	// switch to this shader's program.
	//glActiveTexture(GL_TEXTURE0);
	runPass1();
	glFlush();
	runPass2();
}

void TerrainGearT1::runPass1()
{
	glBindFramebuffer(GL_FRAMEBUFFER, deferredFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// remember, GL clear sets the depth buffer values to 1.0f, meaning they are the furthest away (closest to screen is 0.0f)
	glEnable(GL_DEPTH_TEST);
	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &pass1index);		// set appropriate variables for pass #1
	glBindVertexArray(terrainVaoID);								// bind to the terrain VAO
	setPass1Matrices();
	GLMultiDrawArrayJob jobToUse = getMultiDrawArrayJob("deferred");
	//glMultiDrawArrays(GL_TRIANGLES, in_startArray, in_vertexCount, in_numberOfCollections);		// draw the terrain
	glMultiDrawArrays(GL_TRIANGLES, jobToUse.multiStartIndices.get(), jobToUse.multiVertexCount.get(), jobToUse.drawCount);
	//std::cout << "Draw count is: " << jobToUse.drawCount << std::endl;
	glFinish();
}

void TerrainGearT1::runPass2()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);

	// TESTING ONLY -- allows depth values to be copied over!! (comment out above line of glDisbale(GL_DEPTH_TEST))
	/*
	glBindFramebuffer(GL_READ_FRAMEBUFFER, deferredFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height,
		GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	*/

	setPass2Matrices();
	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &pass2index);		// set appropriate variables for pass #1
	glBindVertexArray(quadVaoID);
	glDrawArrays(GL_TRIANGLES, 0, 6);		// draw the quad
}

void TerrainGearT1::setPass1Matrices()
{
	GLuint mvpUniform = glGetUniformLocation(programID, "MVP");	// find the MVP uniform
	glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, &gearUniformRegistry.getMat4("MVP")[0][0]);		// set the uniform

	//glm::mat4 mv = view * model;
	GLuint mvUniform = glGetUniformLocation(programID, "ModelViewMatrix");
	glUniformMatrix4fv(mvUniform, 1, GL_FALSE, &gearUniformRegistry.getMat4("ModelViewMatrix")[0][0]);

	glUniform3fv(worldPosUniform, 1, &gearUniformRegistry.getVec3("worldPosUniform")[0]);
	glUniform1f(atlasWidthUniform, gearUniformRegistry.getFloat("atlasTextureWidth"));
	glUniform1f(atlasTileWidthUniform, gearUniformRegistry.getFloat("atlasTileTextureWidth"));

	//std::cout << "!!!!!! ATLAS VALUES ARE: ############################ " << std::endl;
	//std::cout << "Atlas width: " << gearUniformRegistry.getFloat("atlasWidthUniform") << std::endl;
}

void TerrainGearT1::setPass2Matrices()
{
	glm::mat4 temp_Proj = glm::mat4(1.0);
	glm::mat4 temp_View = glm::mat4(1.0);
	glm::mat4 temp_Model = glm::mat4(1.0);

	glm::mat4 temp_MVP = temp_Proj * temp_View * temp_Model;
	GLuint mvpUniform = glGetUniformLocation(programID, "MVP");			// find the MVP uniform
	glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, &temp_MVP[0][0]);		// set the uniform

	glm::mat4 temp_MV = temp_View * temp_Model;
	GLuint modelViewHandle = glGetUniformLocation(programID, "ModelViewMatrix");
	glUniformMatrix4fv(modelViewHandle, 1, GL_FALSE, &temp_MV[0][0]);

	glUniform3fv(worldPosUniform, 1, &gearUniformRegistry.getVec3("worldPosUniform")[0]);
	glUniform1f(atlasWidthUniform, gearUniformRegistry.getFloat("atlasTextureWidth"));
	glUniform1f(atlasTileWidthUniform, gearUniformRegistry.getFloat("atlasTileTextureWidth"));
}


void TerrainGearT1::passGLuintValue(std::string in_identifier, GLuint in_gluInt)
{
	if (in_identifier == "terrain_main")
	{
		registerNewPersistentBuffer(in_identifier, in_gluInt);	// register the "terrain_main" PERSISTENT buffer
		terrainBufferID = in_gluInt;		// set the main terrain buffer ID
	}
	else if (in_identifier == "terrain_swap")
	{
		registerNewPersistentBuffer(in_identifier, in_gluInt);	// register the "terrain_swap" PERSISTENT buffer
		terrainSwapID = in_gluInt;			// set the swap terrain buffer ID
	}
	else if (in_identifier == "render_quad_buffer")
	{
		registerNewBuffer(in_identifier, in_gluInt);			// register the "render_quad_buffer" NON-PERSISTENT buffer
		quadBufferID = in_gluInt;
		setUpRenderQuad();			// prepare the render quad
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

void TerrainGearT1::executeGearFunction(std::string in_identifier)
{
	if (in_identifier == "setup_terrain_VAO")
	{
		setupTerrainVAO();
	}
	else if (in_identifier == "acquire_subroutine_indices")
	{
		acquireSubroutineIndices();
	}
}

void TerrainGearT1::setUpRenderQuad()
{
	OrganicGLWinUtils::createAndBindVertexArray(&quadVaoID);	// create/bind the VAO to quadVaoID
	glBindBuffer(GL_ARRAY_BUFFER, quadBufferID);	// bind

	GLfloat quadData[] =
	{												// first 3 floats = position, 4th and 5th = texture coords. 
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,	0.0f, 0.0f,			// 1st point, lower left
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,	0.0f, 0.0f,			// 2nd point, lower right
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,				// 3rd point, upper right
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,	0.0f, 0.0f,			// 4th point, lower left
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,				// 5th point, upper right
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f				// 6th point, upper left
	};

	glBufferData(GL_ARRAY_BUFFER, 6 * 7 * sizeof(float), quadData, GL_STATIC_DRAW);		// populate the data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, (void*)20);
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);			// may not need to reset here, but lets do it anyway
}

void TerrainGearT1::setupTerrainVAO()
{
	OrganicGLWinUtils::createAndBindVertexArray(&terrainVaoID);	// create/bind the VAO to quadVaoID
	glBindBuffer(GL_ARRAY_BUFFER, terrainBufferID);	// bind to the immutable buffer before setting the attribs

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, (void*)20);
	glEnableVertexAttribArray(2);
}

void TerrainGearT1::acquireSubroutineIndices()
{
	pass1index = glGetSubroutineIndex(programID, GL_FRAGMENT_SHADER, "pass1");
	pass2index = glGetSubroutineIndex(programID, GL_FRAGMENT_SHADER, "pass2");
}

// uniform sending definitions
void TerrainGearT1::sendUniformMat4(std::string in_uniformVariableName, glm::mat4 in_mat4)
{
	if (in_uniformVariableName == "mv")
	{
		mv = in_mat4;
	}
}

void TerrainGearT1::printData()
{
	std::cout << "Atlas width: " << gearUniformRegistry.getFloat("atlasTextureWidth") << std::endl;
	std::cout << "Atlas tile width: " << gearUniformRegistry.getFloat("atlasTileTextureWidth") << std::endl;
}