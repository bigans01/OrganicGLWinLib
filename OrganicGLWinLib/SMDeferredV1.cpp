#include "stdafx.h"
#include "SMDeferredV1.h"

void SMDeferredV1::initialize(int in_windowWidth, int in_windowHeight, int in_immutableBufferSize)
{
	// set coordinate mode
	machineCoordMode = GPUCoordinateMode::COORDINATE_MODE_ABSOLUTE;
	setMachineCoordModeDependentSettings();

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
	OrganicGLWinUtils::setClearColor(0.0f, 0.0f, 0.7f, 0.0f);	// background color

	// enable depth dest
	glEnable(GL_DEPTH_TEST);

	// set keyboard input callback function.
	glfwSetWindowUserPointer(window, this);	// testing only.
	glfwSetKeyCallback(window, keyCallbackWrapper);

	// NEW ---> setup IMGui
	OrganicGLWinUtils::IMGuiInit(window);

	std::cout << "============SMDeferredV1, START initializing..." << std::endl;

	// ########################################################################## Terrain Gear set up
	// create the programs
	createProgram("TerrainGearT1");

	// setup the immutable buffers, x2
	terrainBufferSize = in_immutableBufferSize * 1000000;			// setup the immutable buffers, x2
	insertNewPersistentBuffer("terrain_main", terrainBufferSize);		// main terrain buffer
	insertNewPersistentBuffer("terrain_swap", terrainBufferSize);		// terrain swap buffer

	// create the deferred multiDrawCallJob
	//insertNewMultiDrawArrayJob("terrain");

	// set up the render quad buffer
	std::cout << "!!! Attemtpting render quad buffer setup. " << std::endl;
	insertNewBuffer("render_quad_buffer");
	std::cout << "!!! DONE with render quad buffer setup. " << std::endl;

	// create the deferred FBO; set it up
	insertNewFBO("deferred_FBO");
	setupDeferredFBO();
	//setupAlternativeDepthTexture();


	// other things to set up before inserting the terrain gear...
	// ...
	// ...

	// create the terrain gear
	std::cout << "!!! Attempting insert of TerrainGearT1" << std::endl;

	// ########################################################################## terrain Gear set up
	// Gear 0: This basic Gear that is supposed to deferred rendering isn't working on some GPUs; it causes a crash on an Nvidia 4070
	insertTerrainGear(0, programLookup["TerrainGearT1"]);		// create the terrain shader (always the first shader); set the gear's program to be mode 4
	std::cout << "!!! DONE Attempting insert of TerrainGearT1" << std::endl;

	// ########################################################################## Highlighter Gear set up
	// Gear 1: Render any solid highlightable objects (block target, current blueprint borders, etc)
	//createMode0Program("Mode0");
	//createMode0Program("HighlighterGearT1");
	createProgram("HighlighterGearT1");
	insertNewBuffer("highlighter_buffer");
	//insertNewMultiDrawArrayJob("highlighter_draw_job");
	insertHighlighterGear(1, programLookup["HighlighterGearT1"]);

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::cout << "============SMDeferredV1, DONE initializing..." << std::endl;
	int someVal = 3;
	std::cin >> someVal;
}

void SMDeferredV1::setupTextureAtlases()
{
	// register the atlas values; these should not need to change, and are not updated in calls to updateUniformRegistry -- thus, they only need to be registered once.
	uniformRegistry.insertFloat("atlasTextureWidth", 1.0f);		// the "1.0f"s will get reset in the call to OrganicGLWinUtils::setupTextureAtlasJPEG
	uniformRegistry.insertFloat("atlasTileTextureWidth", 1.0f);
	insertNewTexture("terrainAtlas");
	//OrganicGLWinUtils::setupTextureAtlasJPEG(getTextureRef("terrainAtlas"), in_atlasMapRef, in_atlasPropertiesGLRef, uniformRegistry.getFloatRef("atlasTileTextureWidth"), uniformRegistry.getFloatRef("atlasTextureWidth"));
	//in_atlasMapRef->buildAtlas("Core", getTextureRef("terrainAtlas"), uniformRegistry.getFloatRef("atlasTileTextureWidth"), uniformRegistry.getFloatRef("atlasTextureWidth"));
	insertAndBuildNewAtlas("Core", getTextureRef("terrainAtlas"), uniformRegistry.getFloatRef("atlasTileTextureWidth"), uniformRegistry.getFloatRef("atlasTextureWidth"));

	//AtlasMap test1;
	//AtlasMap test2;
	//test1 = test2;

	std::cout << "Atlas set up.................printing values: " << std::endl;
	std::cout << "Atlas texture width: " << uniformRegistry.getFloat("atlasTextureWidth");
	std::cout << "Atlas Tile texture width: "<< uniformRegistry.getFloat("atlasTileTextureWidth");

	// send the value of the texture atlas ID to the 0 gear
	gearTrain[0]->passGLuintValue("terrainAtlas", getTextureID("terrainAtlas"));
}

void SMDeferredV1::runAllShadersAndSwap()
{
	// RESERVED FOR LATER USER
	updateUniformRegistry();	// update all necessary uniforms in the registry, before they are re-sent to each gear
	sendGearUniforms();	// send any other special uniform requests to each gear. 
	sendDrawJobs();		// send each draw job to the gear(s) that requested them.
	runGearTrain();	  // run the draw/rendering for each gear
	swapAndPoll();		// swap the buffers, poll for events
}

void SMDeferredV1::runAllShadersNoSwap()
{
	updateUniformRegistry();	// update all necessary uniforms in the registry, before they are re-sent to each gear
	sendGearUniforms();	// send any other special uniform requests to each gear. 
	sendDrawJobs();		// send each draw job to the gear(s) that requested them.
	runGearTrain();	  // run the draw/rendering for each gear

	//std::cout << "test: uniform for atlasTextureWidth: " << uniformRegistry.getFloat("atlasTextureWidth") << std::endl;
	//std::cout << "test: uniform for atlasTileTextureWidth: " << uniformRegistry.getFloat("atlasTileTextureWidth") << std::endl;
}

void SMDeferredV1::insertTerrainGear(int in_gearID, GLuint in_programID)
{
	//int currentSize = gearTrain.size();
	gearTrain[in_gearID] = std::unique_ptr<Gear>(new TerrainGearT1());

	std::cout << "Attempting to initialize SMDeferredV1..." << std::endl;
	gearTrain[in_gearID]->initializeMachineShader(width, height, in_programID, window, this);
	std::cout << "DONE Attempting to initialize SMDeferredV1..." << std::endl;

	gearTrain[in_gearID]->passGLuintValue("terrain_main", getPersistentBufferID("terrain_main"));		// pass the main terrain buffer
	gearTrain[in_gearID]->passGLuintValue("terrain_swap", getPersistentBufferID("terrain_swap"));		// pass the swap terrain buffer
	gearTrain[in_gearID]->passGLuintValue("render_quad_buffer", getBufferID("render_quad_buffer"));		// pass the render quad buffer
	gearTrain[in_gearID]->passGLuintValue("deferred_FBO", getFBOID("deferred_FBO"));					// pass the deferred FBO

	std::cout << "Attempting to execute gear function, setup_terrain_VAO. " << std::endl;
	gearTrain[in_gearID]->executeGearFunction("setup_terrain_VAO");
	std::cout << "DONE Attempting to execute gear function, setup_terrain_VAO. " << std::endl;

	std::cout << "Attempting to execute gear function, acquire_subroutine_indices. " << std::endl;
	gearTrain[in_gearID]->executeGearFunction("acquire_subroutine_indices");
	std::cout << "DONE Attempting to execute gear function, acquire_subroutine_indices. " << std::endl;

	std::cout << "!!! Terrain gear inserted. " << std::endl;
}

void SMDeferredV1::insertHighlighterGear(int in_gearID, GLuint in_programID)
{
	gearTrain[in_gearID] = std::unique_ptr<Gear>(new HighlighterGearT1());
	gearTrain[in_gearID]->initializeMachineShader(width, height, in_programID, window, this);
	gearTrain[in_gearID]->passGLuintValue("highlighter_buffer", getBufferID("highlighter_buffer"));		// pass the main terrain buffer
	gearTrain[in_gearID]->executeGearFunction("setup_terrain_highlighter_VAO");
}

void SMDeferredV1::multiDrawTerrain(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, int in_numberOfCollections)
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

void SMDeferredV1::printDataForGears()
{
	updateUniformRegistry();
	sendGearUniforms();
	sendDrawJobs();		// send each draw job to the gear(s) that requested them.
	gearTrain[0]->printData();

	//GLMultiDrawArrayJob drawJobA = getMultiDrawArrayJob("deferred");

}

void SMDeferredV1::updateUniformRegistry()
{
	// update the MVP

	/*
	glm::vec3 dummyposition;
	view = glm::lookAt(
		dummyposition,
		dummyposition + direction,
		up
	);
	*/

	MVP = projection * view * model;
	uniformRegistry.insertMat4("MVP", MVP);

	glm::mat4 currentMV = view * model;
	uniformRegistry.insertMat4("ModelViewMatrix", currentMV); // update the MV
	uniformRegistry.insertVec3("worldPosition", position);	// update the world position uniform

	//std::cout << "position is: " << position.x << ", " << position.y << ", " << position.z << std::endl;
}

void SMDeferredV1::setupDeferredFBO()
{
	// set up the deferred FBO
	GLuint depthBuf, posTex, colorTex;
	glBindFramebuffer(GL_FRAMEBUFFER, getFBOID("deferred_FBO"));
	//OrganicGLWinUtils::setClearColor(70.0f, 0.0f, 0.0f, 0.0f);	// this is not FBO specific, just overwrites whatever was there before (for instance, the blue color).

	// depth buffer
	glGenRenderbuffers(1, &depthBuf);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

	// create textures for position and color (bindings 1 and 2, respectively).
	// unit 0 is reserved for original texture (albedo) lookup
	createGBufText(GL_TEXTURE1, GL_RGB32F, posTex);		// g buffer for position = unit 1
	createGBufText(GL_TEXTURE2, GL_RGB8, colorTex);		// g buffer for color = unit 2

	// attach textures to the frame buffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);

	//GLuint depthTexture;
	// D-1
	/*
	glActiveTexture(GL_TEXTURE3);
	glGenTextures(1, &depthBuf);
	glBindTexture(GL_TEXTURE_2D, depthBuf);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT24, width, height, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthBuf, 0);
	*/


	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, posTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, colorTex, 0);

	GLenum drawBuffers[] = { GL_NONE, GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };		// output locations, under the "outputs" section of the fragment shader;
																						// indicates relationship between the framebuffer's components, and the fragment shader's output locations.
	glDrawBuffers(3, drawBuffers);

	// check the buffer status
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);				// reset back to the default OpenGL FBO
}

void SMDeferredV1::createGBufText(GLenum texUnit, GLenum  format, GLuint &texid)
{
	glActiveTexture(texUnit);
	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void SMDeferredV1::insertCollectionGLData(TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef)
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

void SMDeferredV1::removeUnusedReplaceables()
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

void SMDeferredV1::flagCollectionGLDataForRemoval(EnclaveKeyDef::EnclaveKey in_keyForRemoval)
{
	terrainMemoryTracker.jobFlagAsReplaceable(in_keyForRemoval);
}