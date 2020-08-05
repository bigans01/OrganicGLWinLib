#include "stdafx.h"
#include "SMDeferredV2.h"

void SMDeferredV2::initialize(int in_windowWidth, int in_windowHeight, int in_immutableBufferSize)
{
	// basic initialization
	width = in_windowWidth;
	height = in_windowHeight;

	// set shader specific VAO values
	vaoAttribMode = 3;
	vaoAttribByteSize = 28;

	OrganicGLWinUtils::initializeLibraryAndSetHints();				// initialization
	window = OrganicGLWinUtils::createGLFWWindow(width, height);	// create the GLFW window
	OrganicGLWinUtils::checkWindowValidity(window);			// CHECK FOR DEFERRED?
	OrganicGLWinUtils::makeContextCurrent(window);
	OrganicGLWinUtils::initializeGlew();
	OrganicGLWinUtils::setBasicStates();					// CHECK FOR DEFERRED?
	OrganicGLWinUtils::setGLFWInputMode(window);
	OrganicGLWinUtils::setClearColor(0.0f, 0.0f, 23.7f, 0.0f);	// background color

	// enable depth dest
	glEnable(GL_DEPTH_TEST);

	// NEW ---> setup IMGui
	OrganicGLWinUtils::IMGuiInit(window);


	// ########################################################################## Terrain Gear set up
	// create the programs
	//createMode4Program("Mode4");		// create the mode 4 program, name it mode 4. it MUST be created before the corresponding gear(s) is/are inserted.
	//createMode4Program("TerrainGearT1");
	createProgram("TerrainGearT2");

	// setup the immutable buffers, x2
	terrainBufferSize = in_immutableBufferSize * 1000000;			// setup the immutable buffers, x2
	insertNewPersistentBuffer("terrain_main", terrainBufferSize);		// main terrain buffer
	insertNewPersistentBuffer("terrain_swap", terrainBufferSize);		// terrain swap buffer

	// set up the render quad buffer
	insertNewBuffer("render_quad_buffer");

	// create the deferred FBO; set it up
	insertNewFBO("deferred_FBO");
	setupDeferredFBO();
	//setupAlternativeDepthTexture();

	// create the deferred multiDrawCallJob
	//insertNewMultiDrawArrayJob("terrain");

	// other things to set up before inserting the terrain gear...
	// ...
	// ...

	// create the terrain gear
	insertTerrainGear(0, programLookup["TerrainGearT2"]);		// create the terrain shader (always the first shader); set the gear's program to be mode 4

	// ########################################################################## Highlighter Gear set up
	//createMode0Program("Mode0");
	//createMode0Program("HighlighterGearT1");
	createProgram("HighlighterGearT1");
	insertNewBuffer("highlighter_buffer");
	//insertNewMultiDrawArrayJob("highlighter_draw_job");
	insertHighlighterGear(1, programLookup["HighlighterGearT1"]);

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SMDeferredV2::setupTextureAtlas(AtlasMap* in_atlasMapRef, AtlasPropertiesGL* in_atlasPropertiesGLRef)
{
	// register the atlas values; these should not need to change, and are not updated in calls to updateUniformRegistry -- thus, they only need to be registered once.
	uniformRegistry.insertFloat("atlasTextureWidth", 1.0f);		// the "1.0f"s will get reset in the call to OrganicGLWinUtils::setupTextureAtlasJPEG
	uniformRegistry.insertFloat("atlasTileTextureWidth", 1.0f);
	insertNewTexture("terrainAtlas");
	OrganicGLWinUtils::setupTextureAtlasJPEG(getTextureRef("terrainAtlas"), in_atlasMapRef, in_atlasPropertiesGLRef, uniformRegistry.getFloatRef("atlasTileTextureWidth"), uniformRegistry.getFloatRef("atlasTextureWidth"));

	std::cout << "Atlas set up.................printing values: " << std::endl;
	std::cout << "Atlas texture width: " << uniformRegistry.getFloat("atlasTextureWidth");
	std::cout << "Atlas Tile texture width: " << uniformRegistry.getFloat("atlasTileTextureWidth");

	// send the value of the texture atlas ID to the 0 gear
	gearTrain[0]->passGLuintValue("terrainAtlas", getTextureID("terrainAtlas"));
}

void SMDeferredV2::runAllShadersAndSwap()
{
	// RESERVED FOR LATER USER
	updateUniformRegistry();	// update all necessary uniforms in the registry, before they are re-sent to each gear
	sendGearUniforms();	// send any other special uniform requests to each gear. 
	sendDrawJobs();		// send each draw job to the gear(s) that requested them.
	runGearTrain();	  // run the draw/rendering for each gear
	swapAndPoll();		// swap the buffers, poll for events
}

void SMDeferredV2::runAllShadersNoSwap()
{
	// RESERVED FOR LATER USER
	updateUniformRegistry();	// update all necessary uniforms in the registry, before they are re-sent to each gear
	sendGearUniforms();	// send any other special uniform requests to each gear. 
	sendDrawJobs();		// send each draw job to the gear(s) that requested them.
	runGearTrain();	  // run the draw/rendering for each gear
}

void SMDeferredV2::shutdownGL()
{

}

void SMDeferredV2::insertTerrainGear(int in_gearID, GLuint in_programID)
{
	//int currentSize = gearTrain.size();
	gearTrain[in_gearID] = std::unique_ptr<Gear>(new TerrainGearT1());
	gearTrain[in_gearID]->initializeMachineShader(width, height, in_programID, window);
	gearTrain[in_gearID]->passGLuintValue("terrain_main", getPersistentBufferID("terrain_main"));		// pass the main terrain buffer
	gearTrain[in_gearID]->passGLuintValue("terrain_swap", getPersistentBufferID("terrain_swap"));		// pass the swap terrain buffer
	gearTrain[in_gearID]->passGLuintValue("render_quad_buffer", getBufferID("render_quad_buffer"));		// pass the render quad buffer
	gearTrain[in_gearID]->passGLuintValue("deferred_FBO", getFBOID("deferred_FBO"));					// pass the deferred FBO
	gearTrain[in_gearID]->executeGearFunction("setup_terrain_VAO");
	gearTrain[in_gearID]->executeGearFunction("acquire_subroutine_indices");

	std::cout << "!!! Terrain gear inserted. " << std::endl;
}

void SMDeferredV2::insertHighlighterGear(int in_gearID, GLuint in_programID)
{
	gearTrain[in_gearID] = std::unique_ptr<Gear>(new HighlighterGearT1());
	gearTrain[in_gearID]->initializeMachineShader(width, height, in_programID, window);
	gearTrain[in_gearID]->passGLuintValue("highlighter_buffer", getBufferID("highlighter_buffer"));		// pass the main terrain buffer
	gearTrain[in_gearID]->executeGearFunction("setup_terrain_highlighter_VAO");
}

void SMDeferredV2::multiDrawTerrain(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, int in_numberOfCollections)
{
	// clear the FBOs here;
	// -each Gear's uniforms need to be appropriately set before glUseProgram is called for that gear.

	updateUniformRegistry();	// update all necessary uniforms in the registry, before they are re-sent to each gear
	//updateMVPinGears(); // update the MVP uniforms in each gear
	sendGearUniforms();	// send any other special uniform requests to each gear. 
	sendDrawJobs();		// send each draw job to the gear(s) that requested them.
	runGearTrain();	  // run the draw/rendering for each gear
	swapAndPoll();		// swap the buffers, poll for events
}

void SMDeferredV2::printDataForGears()
{
	updateUniformRegistry();
	sendGearUniforms();
	sendDrawJobs();		// send each draw job to the gear(s) that requested them.
	gearTrain[0]->printData();

	//GLMultiDrawArrayJob drawJobA = getMultiDrawArrayJob("deferred");

}

void SMDeferredV2::updateUniformRegistry()
{
	// update the MVP
	MVP = projection * view * model;
	uniformRegistry.insertMat4("MVP", MVP);

	glm::mat4 currentMV = view * model;
	uniformRegistry.insertMat4("ModelViewMatrix", currentMV); // update the MV
	uniformRegistry.insertVec3("worldPosition", position);	// update the world position uniform
}

void SMDeferredV2::setupDeferredFBO()
{
	// set up the deferred FBO
	GLuint depthBuf, posTex, colorTex;
	depthBuf = 0;
	glBindFramebuffer(GL_FRAMEBUFFER, getFBOID("deferred_FBO"));


	// create textures for position and color (bindings 1 and 2, respectively).
	// unit 0 is reserved for original texture (albedo) lookup
	createGBufText(GL_TEXTURE1, GL_RGB32F, posTex);		// g buffer for position = unit 1
	createGBufText(GL_TEXTURE2, GL_RGB8, colorTex);		// g buffer for color = unit 2

	// attach textures to the frame buffer
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);

	//GLuint depthTexture;
	// D-1


	std::cout << "!!!!!!!!!!! Depth buf value (pre-assign) is: " << depthBuf << std::endl;
	glActiveTexture(GL_TEXTURE3);
	glGenTextures(1, &depthBuf);
	glBindTexture(GL_TEXTURE_2D, depthBuf);
	std::cout << "!!!!!!!!!!! Depth buf value is: " << depthBuf << std::endl;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthBuf, 0);



	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, posTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, colorTex, 0);

	GLenum drawBuffers[] = { GL_NONE, GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(3, drawBuffers);

	// check the buffer status
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);				// reset back to the default OpenGL FBO
}

void SMDeferredV2::createGBufText(GLenum texUnit, GLenum  format, GLuint &texid)
{
	glActiveTexture(texUnit);
	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void SMDeferredV2::insertCollectionGLData(TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef)
{
	TerrainMemoryMoveMeta currentMeta = terrainMemoryTracker.checkForMemoryMovements(in_jobResults);		// check if there are any memory movements required
	if (currentMeta.containsMovement == 1)
	{
		OrganicGLWinUtils::copyToBuffer(getTerrainBufferRef(), getTerrainSwapRef(), currentMeta.byteOffset, currentMeta.byteSize, 0);
		int writeBackOffset = terrainMemoryTracker.insertNewCollection(in_jobResults);
		OrganicGLWinUtils::copyToBuffer(getTerrainSwapRef(), getTerrainBufferRef(), 0, currentMeta.byteSize, writeBackOffset);

		glBindBuffer(GL_ARRAY_BUFFER, *getTerrainBufferRef());

		int targetOffset = terrainMemoryTracker.getCollectionOffset(in_jobResults.collectionKey);
		//RenderCollection* tempRenderCollectionRef = organicSystemPtr->renderCollMap.getRenderCollectionRef(in_jobResults.collectionKey);
		glBufferSubData(GL_ARRAY_BUFFER, targetOffset, in_arraySize, in_arrayRef);

	}
	else if (currentMeta.containsMovement == 0)
	{
		terrainMemoryTracker.insertNewCollection(in_jobResults);
		glBindBuffer(GL_ARRAY_BUFFER, *getTerrainBufferRef());
		int targetOffset = terrainMemoryTracker.getCollectionOffset(in_jobResults.collectionKey);
		//RenderCollection* tempRenderCollectionRef = organicSystemPtr->renderCollMap.getRenderCollectionRef(in_jobResults.collectionKey);
		glBufferSubData(GL_ARRAY_BUFFER, targetOffset, in_arraySize, in_arrayRef);
	}
}

void SMDeferredV2::removeUnusedReplaceables()
{
	if (terrainMemoryTracker.getNumberOfUnusedReplaceables() != 0)
	{
		int numberOfTicks = terrainMemoryTracker.getNumberOfUnusedReplaceables();
		for (int x = 0; x < numberOfTicks; x++)
		{

			TerrainMemoryMoveMeta currentMeta = terrainMemoryTracker.removeUnusedReplaceablesAndShift();
			OrganicGLWinUtils::copyToBuffer(getTerrainBufferRef(), getTerrainSwapRef(), currentMeta.byteOffset, currentMeta.byteSize, 0);
			int writeBackOffset = currentMeta.copyBackOffset;
			OrganicGLWinUtils::copyToBuffer(getTerrainSwapRef(), getTerrainBufferRef(), 0, currentMeta.byteSize, writeBackOffset);
			//std::cout << std::endl;
			terrainMemoryTracker.outputAllElements();

		}
	}
	else
	{
		//std::cout << "No unused replaceables to remove..." << std::endl;
	}
}