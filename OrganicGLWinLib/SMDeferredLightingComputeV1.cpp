#include "stdafx.h"
#include "SMDeferredLightingComputeV1.h"

// virtual functions (defined, inherited)
void SMDeferredLightingComputeV1::initialize(int in_windowWidth, int in_windowHeight, int in_immutableBufferSize)
{
	// basic initialization
	//width = in_windowWidth;
	//height = in_windowHeight;
	ComputeResolution resolution(in_windowWidth, in_windowHeight, 16, 16);// use compute-adjusted coordinates, for a group size of 16
	width = resolution.computeScreenWidth;
	height = resolution.computeScreenHeight;

	std::cout << "#### Width adjusted from: " << in_windowWidth << " to -> " << width << std::endl;
	std::cout << "#### Height adjusted from: " << in_windowHeight << " to -> " << height << std::endl;

	// set shader specific VAO values
	vaoAttribMode = 4;
	vaoAttribByteSize = 40;

	OrganicGLWinUtils::initializeLibraryAndSetHints();				// initialization
	window = OrganicGLWinUtils::createGLFWWindow(width, height);	// create the GLFW windowgf
	OrganicGLWinUtils::checkWindowValidity(window);			// CHECK FOR DEFERRED?
	OrganicGLWinUtils::makeContextCurrent(window);
	OrganicGLWinUtils::initializeGlew();
	OrganicGLWinUtils::setBasicStates();					// CHECK FOR DEFERRED?
	OrganicGLWinUtils::setGLFWInputMode(window);
	//OrganicGLWinUtils::setClearColor(0.33f, 0.01f, 0.23f, 0.0f);	// background color
	OrganicGLWinUtils::setClearColor(0.0f, 0.0f, 0.0f, 0.0f);	// background color


	// enable depth dest
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// set keyboard input callback function.
	glfwSetWindowUserPointer(window, this);	// testing only.

	// input callbacks
	glfwSetKeyCallback(window, keyCallBackWrapper);	// keyboard
	glfwSetScrollCallback(window, mouseScrollCallBackWrapper);			// mouse scroll
	glfwSetMouseButtonCallback(window, mouseButtonCallBackWrapper);		// mouse buttons

	// NEW ---> setup IMGui
	OrganicGLWinUtils::IMGuiInit(window);

	// NEW ---> setup IMGui buttons
	//testButtonPanel.insertNewButton("button1");
	//testButtonPanel.insertNewButton("button2");

	// create the panel, then add buttons to panel.
	buttonPanelContainer.insertNewPanel("panel1", 10, 10, 200, 100);
	buttonPanelContainer.insertButtonIntoPanel("panel1", "button1");
	buttonPanelContainer.insertButtonIntoPanel("panel1", "toggleBlockTargetHighlight");
	buttonPanelContainer.insertButtonIntoPanel("panel1", "toggleCurrentEnclaveHighlighting");

	// create a test panel for input.
	inputPanelContainer.insertNewPanel("test_input", 10, 330, 200, 130);
	inputPanelContainer.insertInputIntoPanel("test_input", "input_area1");

	//testPanel.insertNewSliderFloat("slider1", &globalAmbienceMultiplier, 0.0f, 1.0f);
	sliderPanelContainer.insertNewPanel("adjustable uniforms", 10, 150, 200, 100);
	sliderPanelContainer.insertSliderFloatIntoPanel("adjustable uniforms", "world light", &globalAmbienceMultiplier, 0.0f, 1.0f);


	// ############################ SSBO for WorldLights set up
	insertNewBuffer("world_lights_ssbo");



	// ########################################################################## Terrain Gear (Lighting Compute) set up
	createProgram("TerrainLightingComputeGearT1");
	terrainBufferSize = in_immutableBufferSize * 1000000;			// setup the immutable buffers, x2
	insertNewPersistentBuffer("terrain_main", terrainBufferSize);		// main terrain buffer
	insertNewPersistentBuffer("terrain_swap", terrainBufferSize);		// terrain swap buffer
	insertNewFBO("deferred_FBO");
	setupDeferredFBO();
	insertTerrainGear(0, programLookup["TerrainLightingComputeGearT1"]);		// create the terrain shader (always the first shader); set the gear's program to be mode 4


	insertNewBuffer("compute_quad_buffer");								// quad buffer used for compute shaders.
	createComputeImage(GL_TEXTURE11, "computeWrite", 0);				// create on texture unit 11, bind to image unit 0
	createComputeImage(GL_TEXTURE31, "computeRead", 1);					// image unit 1, "read"
	//createComputeImage(GL_TEXTURE11, "computeWrite", 0);				// create on texture unit 11, bind to image unit 0


	// ########################################################################## Compute ComputeCopyRBGFromTextureToImage set up
	createComputeProgram("ComputeCopyRBGFromTextureToImageGearT1");
	insertComputeTransferGear(1, programLookup["ComputeCopyRBGFromTextureToImageGearT1"]);



	// ########################################################################## Compute Gear set up
	createComputeProgram("DeferredLightingComputeGearT1");
	//createComputeImage(GL_TEXTURE11, "computeWrite", 0);				// create on texture unit 11, bind to image unit 0
	insertComputeGear(2, programLookup["DeferredLightingComputeGearT1"]);

	// ########################################################################## Compute results gear set up
	createProgram("DeferredComputeResultsGearT1");
	insertComputeResultsGear(3, programLookup["DeferredComputeResultsGearT1"]);

	// ########################################################################## Highlighter Gear set up
	createProgram("HighlighterGearT1");
	insertNewBuffer("highlighter_buffer");
	//insertNewMultiDrawArrayJob("highlighter_draw_job");
	insertHighlighterGear(4, programLookup["HighlighterGearT1"]);

	// ########################################################################## Instanced Highlighter Gear set up
	createProgram("InstancedHighlighterGearT1");
	insertNewBuffer("mesh_buffer");
	insertNewBuffer("matrices_buffer");
	insertInstancedHighlighterGear(5, programLookup["InstancedHighlighterGearT1"]);

	// ########################################################################## Wave highlighter gear set up
	createProgram("WaveHighlighterGearT1");
	insertWaveHighlighterGear(6, programLookup["WaveHighlighterGearT1"]);



	std::cout << "++++++++ !! SMDeferredLightingComputeV1 complete. " << std::endl;
	int someVal = 4;
	std::cin >> someVal;
}

void SMDeferredLightingComputeV1::setupDeferredFBO()
{
	
	insertNewTexture("depthBuf");
	insertNewTexture("posTex");
	insertNewTexture("ambientTex");		// for lighting
	insertNewTexture("diffuseTex");
	insertNewTexture("specularTex");	// ""
	insertNewTexture("normalTex");
	insertNewTexture("colorTex");		// will be used by compute shader, to read color data into an image

	glBindFramebuffer(GL_FRAMEBUFFER, getFBOID("deferred_FBO"));


	// create textures for position and color (bindings 1 and 2, respectively).
	// unit 0 is reserved for original texture (albedo) lookup
	createGBufText(GL_TEXTURE1, GL_RGB32F, getTextureLValueRef("posTex"));		// g buffer for position = unit 1 (posTex)
	createGBufText(GL_TEXTURE2, GL_RGB8, getTextureLValueRef("ambientTex"));		// g buffer for light ambient = unit 1 (ambientTex)
	createGBufText(GL_TEXTURE3, GL_RGB8, getTextureLValueRef("diffuseTex"));		// g buffer for light diffuse = unit 2 (diffuseTex)
	createGBufText(GL_TEXTURE4, GL_RGB8, getTextureLValueRef("specularTex"));		// g buffer for light specular = unit 3 (specularTex)
	createGBufText(GL_TEXTURE13, GL_RGB8, getTextureLValueRef("colorTex"));		// g buffer for color = unit 2 (colorTex)
	createGBufText(GL_TEXTURE14, GL_RGB8, getTextureLValueRef("normalTex"));		// g buffer for color = unit 2 (colorTex)

	std::cout << "!!!!!!!!!!! COMPUTE_V1: Depth buf value (pre-assign) is: " << getTextureID("depthBuf") << std::endl;
	glActiveTexture(GL_TEXTURE6);
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
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, getTextureID("ambientTex"), 0);	// posTex			(location = 1), in fragment shader
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, getTextureID("diffuseTex"), 0);	// diffuseTex	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, getTextureID("specularTex"), 0);	// specularTex	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, getTextureID("colorTex"), 0);	// colorTex     (location = 2), in fragment shader
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_2D, getTextureID("normalTex"), 0);	// colorTex     (location = 2), in fragment shader
	GLenum drawBuffers[] = { GL_NONE, GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5 };
	glDrawBuffers(7, drawBuffers);

	// check the buffer status
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)

	{
		std::cout << "Framebuffer not complete!" << std::endl;
		int someVal = 3;
		std::cin >> someVal;
	}
		
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//std::cout << "Test of framebuffer has completed. " << std::endl;
	//int someVal2 = 5;
	//std::cin >> someVal2;
}

void SMDeferredLightingComputeV1::createGBufText(GLenum texUnit, GLenum  format, GLuint &texid)
{
	glActiveTexture(texUnit);
	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void SMDeferredLightingComputeV1::createComputeImage(GLenum texUnit, std::string in_imageName, int in_imageUnit)
{
	int tex_w = width;
	int tex_h = height;
	insertNewTexture(in_imageName);
	glGenTextures(1, getTextureRef(in_imageName));
	glActiveTexture(texUnit);						// compute will read from texture unit 4, to get the image.
	glBindTexture(GL_TEXTURE_2D, getTextureID(in_imageName));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, tex_w, tex_h, 0, GL_RGBA, GL_FLOAT,
		NULL);
	glBindImageTexture(in_imageUnit, getTextureID(in_imageName), 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);	// bind to image unit 0, for this texture (it can now be sampled in the compute shader)
	std::cout << "!!!!!!! Compute Image Texture ID is: " << getTextureID(in_imageName) << std::endl;	// should be 4
}

void SMDeferredLightingComputeV1::setupTextureAtlases()
{
	uniformRegistry.insertFloat("atlasTextureWidth", 1.0f);		// the "1.0f"s will get reset in the call to OrganicGLWinUtils::setupTextureAtlasJPEG
	uniformRegistry.insertFloat("atlasTileTextureWidth", 1.0f);

	insertNewTexture("terrainAtlas");
	insertNewTexture("ambientAtlas");
	insertNewTexture("diffuseAtlas");
	insertNewTexture("specularAtlas");

	insertAndBuildNewAtlasToSpecifiedTextureChannel(GL_TEXTURE7, "Core", getTextureRef("terrainAtlas"), uniformRegistry.getFloatRef("atlasTileTextureWidth"), uniformRegistry.getFloatRef("atlasTextureWidth"));
	insertAndBuildNewAtlasToSpecifiedTextureChannel(GL_TEXTURE8, "Ambient", getTextureRef("ambientAtlas"), uniformRegistry.getFloatRef("atlasTileTextureWidth"), uniformRegistry.getFloatRef("atlasTextureWidth"));
	insertAndBuildNewAtlasToSpecifiedTextureChannel(GL_TEXTURE9, "Diffuse", getTextureRef("diffuseAtlas"), uniformRegistry.getFloatRef("atlasTileTextureWidth"), uniformRegistry.getFloatRef("atlasTextureWidth"));
	insertAndBuildNewAtlasToSpecifiedTextureChannel(GL_TEXTURE10, "Specular", getTextureRef("specularAtlas"), uniformRegistry.getFloatRef("atlasTileTextureWidth"), uniformRegistry.getFloatRef("atlasTextureWidth"));

	std::cout << "Atlas set up.................printing values: " << std::endl;
	std::cout << "Atlas texture width: " << uniformRegistry.getFloat("atlasTextureWidth");
	std::cout << "Atlas Tile texture width: " << uniformRegistry.getFloat("atlasTileTextureWidth");

}

void SMDeferredLightingComputeV1::runAllShadersAndSwap()
{
	// RESERVED FOR LATER USER
	updateWaveUniforms();
	updateUniformRegistry();	// update all necessary uniforms in the registry, before they are re-sent to each gear
	sendGearUniforms();	// send any other special uniform requests to each gear. 
	sendDrawJobs();		// send each draw job to the gear(s) that requested them.
	runGearTrain();	  // run the draw/rendering for each gear
	swapAndPoll();		// swap the buffers, poll for events
}

void SMDeferredLightingComputeV1::runAllShadersNoSwap()
{
	updateWaveUniforms();
	updateUniformRegistry();	// update all necessary uniforms in the registry, before they are re-sent to each gear
	sendGearUniforms();	// send any other special uniform requests to each gear. 
	sendDrawJobs();		// send each draw job to the gear(s) that requested them.
	runGearTrain();	  // run the draw/rendering for each gear
}

void SMDeferredLightingComputeV1::shutdownGL()
{

}

void SMDeferredLightingComputeV1::insertTerrainGear(int in_gearID, GLuint in_programID)
{
	
	gearTrain[in_gearID] = std::unique_ptr<Gear>(new TerrainLightingComputeGearT1());
	gearTrain[in_gearID]->initializeMachineShader(width, height, in_programID, window, this);
	gearTrain[in_gearID]->passGLuintValue("terrain_main", getPersistentBufferID("terrain_main"));		// pass the main terrain buffer
	gearTrain[in_gearID]->passGLuintValue("terrain_swap", getPersistentBufferID("terrain_swap"));		// pass the swap terrain buffer
	gearTrain[in_gearID]->passGLuintValue("deferred_FBO", getFBOID("deferred_FBO"));					// pass the deferred FBO
	gearTrain[in_gearID]->executeGearFunction("setup_terrain_VAO");

	std::cout << "!!! Terrain gear (Lighting Compute) inserted. " << std::endl;
}

void SMDeferredLightingComputeV1::insertComputeTransferGear(int in_gearID, GLuint in_programID)
{
	gearTrain[in_gearID] = std::unique_ptr<Gear>(new ComputeCopyRBGFromTextureToImageGearT1());
	gearTrain[in_gearID]->initializeMachineShader(width, height, in_programID, window, this);
	gearTrain[in_gearID]->passGLuintValue("compute_quad_buffer", getBufferID("compute_quad_buffer"));
	gearTrain[in_gearID]->passGLuintValue("deferred_FBO", getFBOID("deferred_FBO"));
}

void SMDeferredLightingComputeV1::insertComputeGear(int in_gearID, GLuint in_programID)
{
	gearTrain[in_gearID] = std::unique_ptr<Gear>(new DeferredLightingComputeGearT1());
	gearTrain[in_gearID]->initializeMachineShader(width, height, in_programID, window, this);
	gearTrain[in_gearID]->passGLuintValue("original_image_unit_1_texture_ID", getTextureID("computeWrite"));
	gearTrain[in_gearID]->passGLuintValue("original_image_unit_2_texture_ID", getTextureID("computeRead"));
	gearTrain[in_gearID]->passGLuintValue("world_lights_ssbo_buffer_ID", getBufferID("world_lights_ssbo"));

	std::cout << "!!! Compute Lights gear (Lighting Compute) inserted. " << std::endl;
}

void SMDeferredLightingComputeV1::insertComputeResultsGear(int in_gearID, GLuint in_programID)
{
	gearTrain[in_gearID] = std::unique_ptr<Gear>(new DeferredComputeResultsGearT1());
	gearTrain[in_gearID]->initializeMachineShader(width, height, in_programID, window, this);
	gearTrain[in_gearID]->passGLuintValue("compute_quad_buffer", getBufferID("compute_quad_buffer"));
	gearTrain[in_gearID]->passGLuintValue("deferred_FBO", getFBOID("deferred_FBO"));
}

void SMDeferredLightingComputeV1::insertHighlighterGear(int in_gearID, GLuint in_programID)
{
	gearTrain[in_gearID] = std::unique_ptr<Gear>(new HighlighterGearT1());
	gearTrain[in_gearID]->initializeMachineShader(width, height, in_programID, window, this);
	gearTrain[in_gearID]->passGLuintValue("highlighter_buffer", getBufferID("highlighter_buffer"));		// pass the main terrain buffer
	gearTrain[in_gearID]->executeGearFunction("setup_terrain_highlighter_VAO");
}

void SMDeferredLightingComputeV1::insertInstancedHighlighterGear(int in_gearID, GLuint in_programID)
{
	gearTrain[in_gearID] = std::unique_ptr<Gear>(new InstancedHighlighterGearT1());
	gearTrain[in_gearID]->initializeMachineShader(width, height, in_programID, window, this);
	gearTrain[in_gearID]->passGLuintValue("mesh_buffer", getBufferID("mesh_buffer"));
	gearTrain[in_gearID]->passGLuintValue("matrices_buffer", getBufferID("matrices_buffer"));
	gearTrain[in_gearID]->executeGearFunction("setup_instancing_buffers_and_VAO");
}

void SMDeferredLightingComputeV1::insertWaveHighlighterGear(int in_gearID, GLuint in_programID)
{
	gearTrain[in_gearID] = std::unique_ptr<Gear>(new WaveHighlighterGearT1());
	gearTrain[in_gearID]->initializeMachineShader(width, height, in_programID, window, this);
}

void SMDeferredLightingComputeV1::multiDrawTerrain(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, int in_numberOfCollections)
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

void SMDeferredLightingComputeV1::printDataForGears()
{
	updateUniformRegistry();
	sendGearUniforms();
	sendDrawJobs();
	gearTrain[0]->printData();
}

void SMDeferredLightingComputeV1::updateUniformRegistry()
{
	// screen width/height uniforms, adjusted to compute group dimensions
	uniformRegistry.insertInt("screenWidth", width);
	uniformRegistry.insertInt("screenHeight", height);

	// global ambience mutliplier
	//globalAmbienceMultiplier = 1.0f;
	uniformRegistry.insertFloat("globalAmbienceMultiplier", globalAmbienceMultiplier);

	// update the MVP; model is not needed here, as terrain is already translated to world space.
	MVP = projection * view;
	uniformRegistry.insertMat4("MVP", MVP);

	glm::mat4 currentMV = view;
	uniformRegistry.insertMat4("ModelViewMatrix", currentMV); // update the MV
	uniformRegistry.insertVec3("worldPosition", position);	// update the world position uniform

	// insert the normal matrix for lighting

	//glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(view)));

	model = glm::mat4(1.0);
	glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));

	uniformRegistry.insertMat3("NormalMatrix", normalMatrix);
}

void SMDeferredLightingComputeV1::insertCollectionGLData(TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef)
{
	//std::cout << "!! Called insertCollectionGLData. | Array size is: " << in_arraySize << std::endl;
	TerrainMemoryMoveMeta currentMeta = terrainMemoryTracker.checkForMemoryMovements(in_jobResults);		// check if there are any memory movements required
	if (currentMeta.containsMovement == 1)
	{
		//std::cout << "!! Branch 1 hit. " << std::endl;

		auto copystart = std::chrono::high_resolution_clock::now();
		//std::cout << "Notice,  currentMeta contains movement. " << std::endl;
		//OrganicGLWinUtils::copyToBuffer(getTerrainBufferRef(), getTerrainSwapRef(), currentMeta.byteOffset, currentMeta.byteSize, 0);
		int writeBackOffset = terrainMemoryTracker.insertNewCollection(in_jobResults);
		//OrganicGLWinUtils::copyToBuffer(getTerrainSwapRef(), getTerrainBufferRef(), 0, currentMeta.byteSize, writeBackOffset);
		auto copyend = std::chrono::high_resolution_clock::now();

		OrganicGLWinUtils::moveForCopy(getTerrainBufferRef(), currentMeta.byteOffset, currentMeta.byteSize, writeBackOffset);

		glBindBuffer(GL_ARRAY_BUFFER, *getTerrainBufferRef());

		int targetOffset = terrainMemoryTracker.getCollectionOffset(in_jobResults.collectionKey);
		//RenderCollection* tempRenderCollectionRef = organicSystemPtr->renderCollMap.getRenderCollectionRef(in_jobResults.collectionKey);

		auto buffersubstart = std::chrono::high_resolution_clock::now();
		glBufferSubData(GL_ARRAY_BUFFER, targetOffset, in_arraySize, in_arrayRef);
		auto buffersubend = std::chrono::high_resolution_clock::now();

		//auto trueend = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> copyelapsed = copyend - copystart;
		std::chrono::duration<double> buffersubelapsed = buffersubend - buffersubstart;
		//std::cout << ">> !! Branch 1: InsertCollectionGLData copy swap time: " << copyelapsed.count() << std::endl;
		//std::cout << ">> !! Branch 1: InsertCollectionGLData buffer sub time: " << buffersubelapsed.count() << std::endl;
		//std::cout << ">> Size of swapped data was: " << currentMeta.byteSize << std::endl;
		
		/*
		if (copyelapsed.count() > 0.016)
		{
			std::cout << ">> !! Branch 1: InsertCollectionGLData copy swap time: " << copyelapsed.count() << std::endl;
			std::cout << ">> Size of swapped data was: " << currentMeta.byteSize << std::endl;
		}
		if (buffersubelapsed.count() > 0.016)
		{
			std::cout << ">> !! Branch 1: InsertCollectionGLData buffer sub time: " << buffersubelapsed.count() << std::endl;
			std::cout << ">> Size of swapped data was: " << currentMeta.byteSize << std::endl;
		}
		*/
		
	}
	else if (currentMeta.containsMovement == 0)
	{
		//std::cout << "!! Branch 2 hit. " << std::endl;
		auto truestart = std::chrono::high_resolution_clock::now();

		terrainMemoryTracker.insertNewCollection(in_jobResults);
		glBindBuffer(GL_ARRAY_BUFFER, *getTerrainBufferRef());
		int targetOffset = terrainMemoryTracker.getCollectionOffset(in_jobResults.collectionKey);
		//RenderCollection* tempRenderCollectionRef = organicSystemPtr->renderCollMap.getRenderCollectionRef(in_jobResults.collectionKey);
		glBufferSubData(GL_ARRAY_BUFFER, targetOffset, in_arraySize, in_arrayRef);

		auto trueend = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> trueelapsed = trueend - truestart;
		//std::cout << ">> !! Branch 2: InsertCollectionGLData time: " << trueelapsed.count() << std::endl;
		/*
		if (trueelapsed.count() > 0.016)
		{
			std::cout << ">> !! Branch 2: InsertCollectionGLData time: " << trueelapsed.count() << std::endl;
		}
		*/
	}
}

void SMDeferredLightingComputeV1::removeUnusedReplaceables()
{
	if (terrainMemoryTracker.getNumberOfUnusedReplaceables() != 0)
	{
		int numberOfTicks = terrainMemoryTracker.getNumberOfUnusedReplaceables();
		for (int x = 0; x < numberOfTicks; x++)
		{

			TerrainMemoryMoveMeta currentMeta = terrainMemoryTracker.removeUnusedReplaceablesAndShift();
			//OrganicGLWinUtils::copyToBuffer(getTerrainBufferRef(), getTerrainSwapRef(), currentMeta.byteOffset, currentMeta.byteSize, 0);
			int writeBackOffset = currentMeta.copyBackOffset;
			//OrganicGLWinUtils::copyToBuffer(getTerrainSwapRef(), getTerrainBufferRef(), 0, currentMeta.byteSize, writeBackOffset);
			//std::cout << std::endl;
			OrganicGLWinUtils::moveForCopy(getTerrainBufferRef(), currentMeta.byteOffset, currentMeta.byteSize, writeBackOffset);
			terrainMemoryTracker.outputAllElements();

		}
	}
	else
	{
		//std::cout << "No unused replaceables to remove..." << std::endl;
	}
}

void SMDeferredLightingComputeV1::insertWorldLight(std::string in_stringedContainerName, int in_lightID, WorldLight in_worldLight)
{

}