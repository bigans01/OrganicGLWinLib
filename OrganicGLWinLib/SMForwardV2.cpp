#include "stdafx.h"
#include "SMForwardV2.h"

void SMForwardV2::initialize(int in_windowWidth, int in_windowHeight, int in_immutableBufferSize)
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
	OrganicGLWinUtils::setClearColor(.23f, .37f, 23.7f, 0.0f);	// background color

	// enable depth dest
	glEnable(GL_DEPTH_TEST);

	// NEW ---> setup IMGui
	OrganicGLWinUtils::IMGuiInit(window);

	// create the forward terrain gear
	createProgram("TerrainForwardGearT1");

	// setup the immutable buffers, x2
	terrainBufferSize = in_immutableBufferSize * 1000000;			// setup the immutable buffers, x2
	insertNewPersistentBuffer("terrain_main", terrainBufferSize);		// main terrain buffer
	insertNewPersistentBuffer("terrain_swap", terrainBufferSize);		// terrain swap buffer

	// create the forward multiDrawCallJob
	insertNewMultiDrawArrayJob("terrain");

	// create the terrain gear
	insertTerrainGear(0, programLookup["TerrainForwardGearT1"]);		// create the terrain shader (always the first shader); set the gear's program to be mode 4

	// create the highlighter gear
	createProgram("HighlighterGearT1");
	insertNewBuffer("highlighter_buffer");
	insertNewMultiDrawArrayJob("highlighter_draw_job");
	insertHighlighterGear(1, programLookup["HighlighterGearT1"]);
}

void SMForwardV2::setupTextureAtlas(AtlasMap* in_atlasMapRef, AtlasPropertiesGL* in_atlasPropertiesGLRef)
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

void SMForwardV2::runAllShadersAndSwap()
{
	// RESERVED FOR LATER USER
	updateUniformRegistry();	// update all necessary uniforms in the registry, before they are re-sent to each gear
	sendGearUniforms();	// send any other special uniform requests to each gear. 
	sendDrawJobs();		// send each draw job to the gear(s) that requested them.
	runGearTrain();	  // run the draw/rendering for each gear
	swapAndPoll();		// swap the buffers, poll for events
}

void SMForwardV2::runAllShadersNoSwap()
{
	// RESERVED FOR LATER USER
	updateUniformRegistry();	// update all necessary uniforms in the registry, before they are re-sent to each gear
	sendGearUniforms();	// send any other special uniform requests to each gear. 
	sendDrawJobs();		// send each draw job to the gear(s) that requested them.
	runGearTrain();	  // run the draw/rendering for each gear
}

void SMForwardV2::shutdownGL()
{

}

void SMForwardV2::multiDrawTerrain(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, int in_numberOfCollections)
{
	updateUniformRegistry();	// update all necessary uniforms in the registry, before they are re-sent to each gear
	//updateMVPinGears(); // update the MVP uniforms in each gear
	sendGearUniforms();	// send any other special uniform requests to each gear. 
	sendDrawJobs();		// send each draw job to the gear(s) that requested them.
	runGearTrain();	  // run the draw/rendering for each gear
	swapAndPoll();		// swap the buffers, poll for events
}

void SMForwardV2::printDataForGears()
{
	updateUniformRegistry();
	sendGearUniforms();
	sendDrawJobs();		// send each draw job to the gear(s) that requested them.
	gearTrain[0]->printData();
}

void SMForwardV2::insertTerrainGear(int in_gearID, GLuint in_programID)
{
	gearTrain[in_gearID] = std::unique_ptr<Gear>(new TerrainForwardGearT1());
	gearTrain[in_gearID]->initializeMachineShader(width, height, in_programID, window);
	gearTrain[in_gearID]->passGLuintValue("terrain_main", getPersistentBufferID("terrain_main"));		// pass the main terrain buffer
	gearTrain[in_gearID]->passGLuintValue("terrain_swap", getPersistentBufferID("terrain_swap"));		// pass the swap terrain buffer
	gearTrain[in_gearID]->executeGearFunction("setup_terrain_VAO");
}

void SMForwardV2::insertHighlighterGear(int in_gearID, GLuint in_programID)
{
	gearTrain[in_gearID] = std::unique_ptr<Gear>(new HighlighterGearT1());
	gearTrain[in_gearID]->initializeMachineShader(width, height, in_programID, window);
	gearTrain[in_gearID]->passGLuintValue("highlighter_buffer", getBufferID("highlighter_buffer"));		// pass the main terrain buffer
	gearTrain[in_gearID]->executeGearFunction("setup_terrain_highlighter_VAO");
}

void SMForwardV2::updateUniformRegistry()
{
	// update the MVP
	MVP = projection * view * model;
	uniformRegistry.insertMat4("MVP", MVP);

	glm::mat4 currentMV = view * model;
	uniformRegistry.insertMat4("ModelViewMatrix", currentMV); // update the MV
	uniformRegistry.insertVec3("worldPosition", position);	// update the world position uniform
}