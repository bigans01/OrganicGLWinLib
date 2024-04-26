#include "stdafx.h"
#include "SMDeferredLightingComputeV2.h"

// virtual functions (defined, inherited)
void SMDeferredLightingComputeV2::initialize(int in_windowWidth, int in_windowHeight, int in_immutableBufferSize)
{
	// set coordinate mode
	machineCoordMode = GPUCoordinateMode::COORDINATE_MODE_ABSOLUTE;
	setMachineCoordModeDependentSettings();

	// since this is a compute shader, we must make sure the compute "tiles" are setup correctly
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
	OrganicGLWinUtils::setClearColor(0.49f, 0.0f, 0.47f, 0.0f);	// background color


	// enable depth dest
	glEnable(GL_DEPTH_TEST);

	// set keyboard input callback function.
	glfwSetWindowUserPointer(window, this);	// testing only.

	// input callbacks
	glfwSetKeyCallback(window, keyCallbackWrapper);	// keyboard
	glfwSetScrollCallback(window, mouseScrollCallbackWrapper);			// mouse scroll
	glfwSetMouseButtonCallback(window, mouseButtonCallbackWrapper);		// mouse buttons

	// NEW ---> setup IMGui
	OrganicGLWinUtils::IMGuiInit(window);

	// create the panel, then add buttons to panel.
	buttonPanelContainer.insertNewPanel("panel1", 10, 10, 200, 100);
	buttonPanelContainer.insertButtonIntoPanel("panel1", "button1");
	buttonPanelContainer.insertButtonIntoPanel("panel1", "toggleBlockTargetHighlight");
	buttonPanelContainer.insertButtonIntoPanel("panel1", "toggleCurrentEnclaveHighlighting");
	buttonPanelContainer.insertButtonIntoPanel("panel1", "showSolidBlocks");

	// create a test panel for input.
	inputPanelContainer.insertNewPanel("test_input", 10, 330, 200, 130);
	inputPanelContainer.insertInputIntoPanel("test_input", "input_area1");

	//testPanel.insertNewSliderFloat("slider1", &globalAmbienceMultiplier, 0.0f, 1.0f);
	sliderPanelContainer.insertNewPanel("adjustable uniforms", 10, 150, 200, 100);
	sliderPanelContainer.insertSliderFloatIntoPanel("adjustable uniforms", "world light", &globalAmbienceMultiplier, 0.0f, 1.0f);


	// ############################ SSBO for WorldLights set up
	insertNewBuffer("world_lights_ssbo");



	// ########################################################################## Terrain Gear (Lighting Compute) set up
	// Gear 0: Remember, the TerrainLightingComputeGearBP gear doesn't use the two huge buffers; it manages it's own buffers for each blueprint draw call.
	// The actual renderable data for each blueprint gets stored in this gear, via the calls to insertCollectionGLData.
	createProgram("TerrainLightingComputeGearBP");
	insertNewFBO("deferred_FBO");
	setupDeferredFBO();

	insertTerrainGear(0, programLookup["TerrainLightingComputeGearBP"]);

	insertNewBuffer("compute_quad_buffer");								// quad buffer used for compute shaders.
	createComputeImage(GL_TEXTURE16, "computeRead", 1);					// image unit 1, "read"
	createComputeImage(GL_TEXTURE11, "computeWrite", 0);				// create on texture unit 11, bind to image unit 0


	// ########################################################################## Compute ComputeCopyRBGFromTextureToImage set up
	// Gear 1: The ComputeCopyRBGFromTextureToImageGearT1 gear will read from the color buffer that was output to, 
	// by Gear 0, and feed this data into the "computeRead" image unit, so that it may be operated on by the next gear.
	createComputeProgram("ComputeCopyRBGFromTextureToImageGearT1");
	insertComputeTransferGear(1, programLookup["ComputeCopyRBGFromTextureToImageGearT1"]);



	// ########################################################################## Compute Gear set up
	// Gear 2: The DeferredLightingComputeGearT1 compute shader is an experimental shader, that is designed to experiment with
	// rendering lights against tiles defined in compute shaders. This gear will apply lighting to the color buffer that was transffered-into by Gear 1.
	createComputeProgram("DeferredLightingComputeGearT1");
	insertComputeGear(2, programLookup["DeferredLightingComputeGearT1"]);


	// ########################################################################## Compute results gear set up
	// Gear 3: Transfer the resulting lighting data that was stored into the image, into the main FBO.
	createProgram("DeferredComputeResultsGearT1");
	insertComputeResultsGear(3, programLookup["DeferredComputeResultsGearT1"]);


	// ########################################################################## Highlighter Gear set up
	// Gear 4: Render any solid highlightable objects (block target, current blueprint borders, etc)
	createProgram("HighlighterGearT1");
	insertNewBuffer("highlighter_buffer");
	insertHighlighterGear(4, programLookup["HighlighterGearT1"]);

	// ########################################################################## Instanced Highlighter Gear set up
	// Gear 5: Draw any instanced rendering models (currently unused)
	createProgram("InstancedHighlighterGearT1");
	insertNewBuffer("mesh_buffer");
	insertNewBuffer("matrices_buffer");
	insertInstancedHighlighterGear(5, programLookup["InstancedHighlighterGearT1"]);

	// ########################################################################## Wave highlighter gear set up
	// Gear 6: display wave highlights (i.e, current ORE highlight, constituted highlights of an ORE)
	createProgram("WaveHighlighterGearT1");
	insertWaveHighlighterGear(6, programLookup["WaveHighlighterGearT1"]);

	std::cout << "Program IDs: " << std::endl;
	for (auto& currentProgramValue : programLookup)
	{
		std::cout << currentProgramValue.first << " -> " << currentProgramValue.second << std::endl;
	}

	std::cout << "Gears: " << std::endl;
	for (auto& currentGearID : gearTrain)
	{
		std::cout << currentGearID.first << std::endl;
	}


	std::cout << "++++++++ !! SMDeferredLightingComputeV2 complete. " << std::endl;
	int someVal = 4;
	std::cin >> someVal;
}

void SMDeferredLightingComputeV2::setupDeferredFBO()
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

	// Below: we need some variant that uses float data, for storing normals: GL_RGB32F can do the trick, or possibly even GLRGB16F...
	// note that the second parameter, GL_RGB32F, used to actually be GL_RGB8; using this format was incorrect, as only normals with positive 
	// values in the x/y/z dimension could be stored. Experiment with GL_RGBA16F when able, as GL_RGB32F may be overkill.
	//
	// See article: https://learnopengl.com/Advanced-Lighting/Deferred-Shading
	createGBufText(GL_TEXTURE14, GL_RGBA16F, getTextureLValueRef("normalTex"));		// g buffer for color = unit 2 (colorTex)
																					

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

void SMDeferredLightingComputeV2::createGBufText(GLenum texUnit, GLenum  format, GLuint &texid)
{
	glActiveTexture(texUnit);
	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void SMDeferredLightingComputeV2::createComputeImage(GLenum texUnit, std::string in_imageName, int in_imageUnit)
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

void SMDeferredLightingComputeV2::setupTextureAtlases()
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

void SMDeferredLightingComputeV2::runAllShadersAndSwap()
{
	// RESERVED FOR LATER USER
	updateWaveUniforms();
	updateUniformRegistry();	// update all necessary uniforms in the registry, before they are re-sent to each gear
	sendGearUniforms();	// send any other special uniform requests to each gear. 
	sendDrawJobs();		// send each draw job to the gear(s) that requested them.
	runGearTrain();	  // run the draw/rendering for each gear
	swapAndPoll();		// swap the buffers, poll for events
}

void SMDeferredLightingComputeV2::runAllShadersNoSwap()
{
	updateWaveUniforms();
	updateUniformRegistry();	// update all necessary uniforms in the registry, before they are re-sent to each gear
	sendGearUniforms();	// send any other special uniform requests to each gear. 
	sendDrawJobs();		// send each draw job to the gear(s) that requested them.
	runGearTrain();	  // run the draw/rendering for each gear
}

void SMDeferredLightingComputeV2::insertTerrainGear(int in_gearID, GLuint in_programID)
{
	gearTrain[in_gearID] = std::unique_ptr<Gear>(new TerrainLightingComputeGearBP());
	gearTrain[in_gearID]->initializeMachineShader(width, height, in_programID, window, this);
	gearTrain[in_gearID]->passGLuintValue("deferred_FBO", getFBOID("deferred_FBO"));					// pass the deferred FBO
}

void SMDeferredLightingComputeV2::insertComputeTransferGear(int in_gearID, GLuint in_programID)
{
	gearTrain[in_gearID] = std::unique_ptr<Gear>(new ComputeCopyRBGFromTextureToImageGearT1());
	gearTrain[in_gearID]->initializeMachineShader(width, height, in_programID, window, this);
	gearTrain[in_gearID]->passGLuintValue("compute_quad_buffer", getBufferID("compute_quad_buffer"));
	gearTrain[in_gearID]->passGLuintValue("deferred_FBO", getFBOID("deferred_FBO"));
}

void SMDeferredLightingComputeV2::insertComputeGear(int in_gearID, GLuint in_programID)
{
	gearTrain[in_gearID] = std::unique_ptr<Gear>(new DeferredLightingComputeGearT1());
	gearTrain[in_gearID]->initializeMachineShader(width, height, in_programID, window, this);
	gearTrain[in_gearID]->passGLuintValue("original_image_unit_1_texture_ID", getTextureID("computeWrite"));
	gearTrain[in_gearID]->passGLuintValue("original_image_unit_2_texture_ID", getTextureID("computeRead"));
	gearTrain[in_gearID]->passGLuintValue("world_lights_ssbo_buffer_ID", getBufferID("world_lights_ssbo"));

	std::cout << "!!! Compute Lights gear (Lighting Compute) inserted. " << std::endl;
}

void SMDeferredLightingComputeV2::insertComputeResultsGear(int in_gearID, GLuint in_programID)
{
	gearTrain[in_gearID] = std::unique_ptr<Gear>(new DeferredComputeResultsGearT1());
	gearTrain[in_gearID]->initializeMachineShader(width, height, in_programID, window, this);
	gearTrain[in_gearID]->passGLuintValue("compute_quad_buffer", getBufferID("compute_quad_buffer"));
	gearTrain[in_gearID]->passGLuintValue("deferred_FBO", getFBOID("deferred_FBO"));
}

void SMDeferredLightingComputeV2::insertHighlighterGear(int in_gearID, GLuint in_programID)
{
	gearTrain[in_gearID] = std::unique_ptr<Gear>(new HighlighterGearT1());
	gearTrain[in_gearID]->initializeMachineShader(width, height, in_programID, window, this);
	gearTrain[in_gearID]->passGLuintValue("highlighter_buffer", getBufferID("highlighter_buffer"));		// pass the main terrain buffer
	gearTrain[in_gearID]->executeGearFunction("setup_terrain_highlighter_VAO");
}

void SMDeferredLightingComputeV2::insertInstancedHighlighterGear(int in_gearID, GLuint in_programID)
{
	gearTrain[in_gearID] = std::unique_ptr<Gear>(new InstancedHighlighterGearT1());
	gearTrain[in_gearID]->initializeMachineShader(width, height, in_programID, window, this);
	gearTrain[in_gearID]->passGLuintValue("mesh_buffer", getBufferID("mesh_buffer"));
	gearTrain[in_gearID]->passGLuintValue("matrices_buffer", getBufferID("matrices_buffer"));
	gearTrain[in_gearID]->executeGearFunction("setup_instancing_buffers_and_VAO");
}

void SMDeferredLightingComputeV2::insertWaveHighlighterGear(int in_gearID, GLuint in_programID)
{
	gearTrain[in_gearID] = std::unique_ptr<Gear>(new WaveHighlighterGearT1());
	gearTrain[in_gearID]->initializeMachineShader(width, height, in_programID, window, this);
}

void SMDeferredLightingComputeV2::multiDrawTerrain(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, int in_numberOfCollections)
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

void SMDeferredLightingComputeV2::printDataForGears()
{
	updateUniformRegistry();
	sendGearUniforms();
	sendDrawJobs();
	gearTrain[0]->printData();
}

void SMDeferredLightingComputeV2::updateUniformRegistry()
{
	// screen width/height uniforms, adjusted to compute group dimensions
	uniformRegistry.insertInt("screenWidth", width);
	uniformRegistry.insertInt("screenHeight", height);

	// global ambience mutliplier
	//globalAmbienceMultiplier = 1.0f;
	uniformRegistry.insertFloat("globalAmbienceMultiplier", globalAmbienceMultiplier);
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

void SMDeferredLightingComputeV2::insertCollectionGLData(TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef)
{
	std::cout << "!!~~ Called insertCollectionGLData. | Array size is: " << in_arraySize << std::endl;
	insertCollectionGLDataIntoGearBuffer("TerrainLightingComputeGearBP", "default", in_jobResults, in_arraySize, in_arrayRef);
}


void SMDeferredLightingComputeV2::insertCollectionGLDataIntoGearBuffer(std::string in_gearName, std::string in_gearBufferName, TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef)
{
	std::cout << "!!!! Found gear to insert data into..." << std::endl;

	int targetGearToInsert = 0;
	std::cout << "Gear ID: " << targetGearToInsert << std::endl;

	gearTrain[targetGearToInsert]->sendTerrainDataToGear(in_jobResults, in_arraySize, in_arrayRef);

}

void SMDeferredLightingComputeV2::flagCollectionGLDataForRemoval(EnclaveKeyDef::EnclaveKey in_keyForRemoval)
{
	int targetGearToModify = 0;
	gearTrain[targetGearToModify]->removeTerrainDataFromGear(in_keyForRemoval);
}