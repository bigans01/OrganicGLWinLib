#include "stdafx.h"
#include "SMDeferredComputeV1.h"

void SMDeferredComputeV1::initialize(int in_windowWidth, int in_windowHeight, int in_immutableBufferSize)
{
	// basic initialization
	//width = in_windowWidth;
	//height = in_windowHeight;
	ComputeResolution resolution(in_windowWidth, in_windowHeight, 16, 16);// use compute-adjusted coordinates, for a group size of 16
	width = resolution.computeScreenWidth;
	height = resolution.computeScreenHeight;

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


	// ########################################################################## Terrain Gear (Compute) set up
	createProgram("TerrainComputeGearT1");
	terrainBufferSize = in_immutableBufferSize * 1000000;			// setup the immutable buffers, x2
	insertNewPersistentBuffer("terrain_main", terrainBufferSize);		// main terrain buffer
	insertNewPersistentBuffer("terrain_swap", terrainBufferSize);		// terrain swap buffer
	insertNewBuffer("render_quad_buffer");							// set up the render quad buffer
	insertNewFBO("deferred_FBO");									// create the deferred FBO; set it up
	setupDeferredFBO();
	//insertNewMultiDrawArrayJob("terrain");
	insertTerrainGear(0, programLookup["TerrainComputeGearT1"]);		// create the terrain shader (always the first shader); set the gear's program to be mode 4

	// ########################################################################## Compute Gear set up
	createComputeProgram("DeferredComputeGearT1");
	createComputeImage("computeWrite");			// the name of the texture that contains the image the compute shader will write to
	insertComputeGear(1, programLookup["DeferredComputeGearT1"]);

	// ########################################################################## Compute results gear set up
	createProgram("DeferredComputeResultsGearT1");
	insertNewBuffer("compute_quad_buffer");
	insertComputeResultsGear(2, programLookup["DeferredComputeResultsGearT1"]);

	// ########################################################################## Highlighter Gear set up
	createProgram("HighlighterGearT1");
	insertNewBuffer("highlighter_buffer");
	//insertNewMultiDrawArrayJob("highlighter_draw_job");
	insertHighlighterGear(3, programLookup["HighlighterGearT1"]);

	createProgram("InstancedHighlighterGearT1");
	insertNewBuffer("mesh_buffer");
	insertNewBuffer("matrices_buffer");
	insertInstancedHighlighterGear(4, programLookup["InstancedHighlighterGearT1"]);


}

void SMDeferredComputeV1::setupTextureAtlases()
{
	// register the atlas values; these should not need to change, and are not updated in calls to updateUniformRegistry -- thus, they only need to be registered once.
	uniformRegistry.insertFloat("atlasTextureWidth", 1.0f);		// the "1.0f"s will get reset in the call to OrganicGLWinUtils::setupTextureAtlasJPEG
	uniformRegistry.insertFloat("atlasTileTextureWidth", 1.0f);
	insertNewTexture("terrainAtlas");
	//OrganicGLWinUtils::setupTextureAtlasJPEG(getTextureRef("terrainAtlas"), in_atlasMapRef, in_atlasPropertiesGLRef, uniformRegistry.getFloatRef("atlasTileTextureWidth"), uniformRegistry.getFloatRef("atlasTextureWidth"));
	//in_atlasMapRef->buildAtlas("Core", getTextureRef("terrainAtlas"), uniformRegistry.getFloatRef("atlasTileTextureWidth"), uniformRegistry.getFloatRef("atlasTextureWidth"));
	insertAndBuildNewAtlas("Core", getTextureRef("terrainAtlas"), uniformRegistry.getFloatRef("atlasTileTextureWidth"), uniformRegistry.getFloatRef("atlasTextureWidth"));

	std::cout << "Atlas set up.................printing values: " << std::endl;
	std::cout << "Atlas texture width: " << uniformRegistry.getFloat("atlasTextureWidth");
	std::cout << "Atlas Tile texture width: " << uniformRegistry.getFloat("atlasTileTextureWidth");

	// send the value of the texture atlas ID to the 0 gear
	gearTrain[0]->passGLuintValue("terrainAtlas", getTextureID("terrainAtlas"));
}

void SMDeferredComputeV1::runAllShadersAndSwap()
{
	// RESERVED FOR LATER USER
	updateUniformRegistry();	// update all necessary uniforms in the registry, before they are re-sent to each gear
	sendGearUniforms();	// send any other special uniform requests to each gear. 
	sendDrawJobs();		// send each draw job to the gear(s) that requested them.
	runGearTrain();	  // run the draw/rendering for each gear
	swapAndPoll();		// swap the buffers, poll for events
}

void SMDeferredComputeV1::runAllShadersNoSwap()
{
	updateUniformRegistry();	// update all necessary uniforms in the registry, before they are re-sent to each gear
	sendGearUniforms();	// send any other special uniform requests to each gear. 
	sendDrawJobs();		// send each draw job to the gear(s) that requested them.
	runGearTrain();	  // run the draw/rendering for each gear
}

void SMDeferredComputeV1::shutdownGL()
{

}

void SMDeferredComputeV1::insertTerrainGear(int in_gearID, GLuint in_programID)
{
	//int currentSize = gearTrain.size();
	gearTrain[in_gearID] = std::unique_ptr<Gear>(new TerrainComputeGearT1());
	gearTrain[in_gearID]->initializeMachineShader(width, height, in_programID, window);
	gearTrain[in_gearID]->passGLuintValue("terrain_main", getPersistentBufferID("terrain_main"));		// pass the main terrain buffer
	gearTrain[in_gearID]->passGLuintValue("terrain_swap", getPersistentBufferID("terrain_swap"));		// pass the swap terrain buffer
	gearTrain[in_gearID]->passGLuintValue("render_quad_buffer", getBufferID("render_quad_buffer"));		// pass the render quad buffer
	gearTrain[in_gearID]->passGLuintValue("deferred_FBO", getFBOID("deferred_FBO"));					// pass the deferred FBO
	gearTrain[in_gearID]->executeGearFunction("setup_terrain_VAO");
	//gearTrain[in_gearID]->executeGearFunction("acquire_subroutine_indices");

	std::cout << "!!! Terrain gear (Compute) inserted. " << std::endl;
}

void SMDeferredComputeV1::insertHighlighterGear(int in_gearID, GLuint in_programID)
{
	gearTrain[in_gearID] = std::unique_ptr<Gear>(new HighlighterGearT1());
	gearTrain[in_gearID]->initializeMachineShader(width, height, in_programID, window);
	gearTrain[in_gearID]->passGLuintValue("highlighter_buffer", getBufferID("highlighter_buffer"));		// pass the main terrain buffer
	gearTrain[in_gearID]->executeGearFunction("setup_terrain_highlighter_VAO");
}

void SMDeferredComputeV1::insertComputeGear(int in_gearID, GLuint in_programID)
{
	gearTrain[in_gearID] = std::unique_ptr<Gear>(new DeferredComputeGearT1());
	gearTrain[in_gearID]->initializeMachineShader(width, height, in_programID, window);
}

void SMDeferredComputeV1::insertComputeResultsGear(int in_gearID, GLuint in_programID)
{
	gearTrain[in_gearID] = std::unique_ptr<Gear>(new DeferredComputeResultsGearT1());
	gearTrain[in_gearID]->initializeMachineShader(width, height, in_programID, window);
	gearTrain[in_gearID]->passGLuintValue("compute_quad_buffer", getBufferID("compute_quad_buffer"));
	gearTrain[in_gearID]->passGLuintValue("deferred_FBO", getFBOID("deferred_FBO"));
}

void SMDeferredComputeV1::insertInstancedHighlighterGear(int in_gearID, GLuint in_programID)
{
	gearTrain[in_gearID] = std::unique_ptr<Gear>(new InstancedHighlighterGearT1());
	gearTrain[in_gearID]->initializeMachineShader(width, height, in_programID, window);
	gearTrain[in_gearID]->passGLuintValue("mesh_buffer", getBufferID("mesh_buffer"));
	gearTrain[in_gearID]->passGLuintValue("matrices_buffer", getBufferID("matrices_buffer"));
	gearTrain[in_gearID]->executeGearFunction("setup_instancing_buffers_and_VAO");
}

void SMDeferredComputeV1::multiDrawTerrain(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, int in_numberOfCollections)
{
	// clear the FBOs here;
	// -each Gear's uniforms need to be appropriately set before glUseProgram is called for that gear.
	// -draw jobs are also sent, where necessary
	// -gear train is run
	// -buffers are swapped at end, to render overall results to screen.

	updateUniformRegistry();	// update all necessary uniforms in the registry, before they are re-sent to each gear
	sendGearUniforms();	// send any other special uniform requests to each gear. 
	sendDrawJobs();		// send each draw job to the gear(s) that requested them.
	runGearTrain();	  // run the draw/rendering for each gear
	swapAndPoll();		// swap the buffers, poll for events
}

void SMDeferredComputeV1::printDataForGears()
{
	updateUniformRegistry();
	sendGearUniforms();
	sendDrawJobs();	
	gearTrain[0]->printData();
}

void SMDeferredComputeV1::updateUniformRegistry()
{
	// screen width/height uniforms, adjusted to compute group dimensions
	uniformRegistry.insertInt("screenWidth", width);
	uniformRegistry.insertInt("screenHeight", height);

	// update the MVP; model is not needed here, as terrain is already translated to world space.
	MVP = projection * view;
	uniformRegistry.insertMat4("MVP", MVP);

	glm::mat4 currentMV = view;
	uniformRegistry.insertMat4("ModelViewMatrix", currentMV); // update the MV
	uniformRegistry.insertVec3("worldPosition", position);	// update the world position uniform
}

void SMDeferredComputeV1::setupDeferredFBO()
{
	// set up the deferred FBO
	GLuint depthBuf, posTex, colorTex;
	depthBuf = 0;
	insertNewTexture("depthBuf");
	insertNewTexture("posTex");
	insertNewTexture("colorTex");		// will be used by compute shader, to read color data into an image

	glBindFramebuffer(GL_FRAMEBUFFER, getFBOID("deferred_FBO"));


	// create textures for position and color (bindings 1 and 2, respectively).
	// unit 0 is reserved for original texture (albedo) lookup
	createGBufText(GL_TEXTURE1, GL_RGB32F, getTextureLValueRef("posTex"));		// g buffer for position = unit 1 (posTex)
	createGBufText(GL_TEXTURE2, GL_RGB8, getTextureLValueRef("colorTex"));		// g buffer for color = unit 2 (colorTex)

	std::cout << "!!!!!!!!!!! COMPUTE_V1: Depth buf value (pre-assign) is: " << getTextureID("depthBuf") << std::endl;
	glActiveTexture(GL_TEXTURE3);
	glGenTextures(1, getTextureRef("depthBuf"));
	glBindTexture(GL_TEXTURE_2D, getTextureID("depthBuf"));
	std::cout << "!!!!!!!!!!! COMPUTE_V1: Position buf value is: " << getTextureID("posTex") << std::endl;
	std::cout << "!!!!!!!!!!! COMPUTE_V1: Color buf value is: " << getTextureID("colorTex") << std::endl;
	std::cout << "!!!!!!!!!!! COMPUTE_V1: Depth buf value is: " << getTextureID("depthBuf") << std::endl;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, getTextureID("depthBuf"), 0);



	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, getTextureID("posTex"), 0);	// posTex			(location = 1), in fragment shader
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, getTextureID("colorTex"), 0);	// colorTex     (location = 2), in fragment shader

	GLenum drawBuffers[] = { GL_NONE, GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(3, drawBuffers);

	// check the buffer status
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);				// reset back to the default OpenGL FBO
}

void SMDeferredComputeV1::createComputeImage(std::string in_imageName)
{
	int tex_w = width;
	int tex_h = height;
	insertNewTexture(in_imageName);
	glGenTextures(1, getTextureRef(in_imageName));
	glActiveTexture(GL_TEXTURE4);						// compute will read from texture unit 4, to get the image.
	glBindTexture(GL_TEXTURE_2D, getTextureID(in_imageName));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, tex_w, tex_h, 0, GL_RGBA, GL_FLOAT,
		NULL);
	glBindImageTexture(0, getTextureID(in_imageName), 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);	// bind to image unit 0, for this texture (it can now be sampled in the compute shader)
	std::cout << "!!!!!!! Compute Image Texture ID is: " << getTextureID("computeWrite") << std::endl;	// should be 4
}

void SMDeferredComputeV1::createGBufText(GLenum texUnit, GLenum  format, GLuint &texid)
{
	glActiveTexture(texUnit);
	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void SMDeferredComputeV1::insertCollectionGLData(TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef)
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

void SMDeferredComputeV1::removeUnusedReplaceables()
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

void SMDeferredComputeV1::insertWorldLight(WorldLight in_worldLight)
{

}