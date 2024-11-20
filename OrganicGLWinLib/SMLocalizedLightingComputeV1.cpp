#include "stdafx.h"
#include "SMLocalizedLightingComputeV1.h"

void SMLocalizedLightingComputeV1::initialize(int in_windowWidth, int in_windowHeight, int in_immutableBufferSize)
{
	// set coordinate mode
	machineCoordMode = GPUCoordinateMode::COORDINATE_MODE_LOCAL;
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
	OrganicGLWinUtils::setClearColor(0.33f, 0.01f, 0.73f, 0.0f);	// background color

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





	// ########################################################################## Terrain Gear (Lighting Compute) set up
	// Gear 0: Remember, the TerrainLightingComputeGearBP gear doesn't use the two huge buffers; it manages it's own buffers for each blueprint draw call.
	// The actual renderable data for each blueprint gets stored in this gear, via the calls to insertCollectionGLData.
	createProgram("LocalizedGBufferComputeLoaderGear");
	insertNewFBO("deferred_FBO");
	setupDeferredFBO();
	insertTerrainGear(0, programLookup["LocalizedGBufferComputeLoaderGear"]);


	// ########################################################################## Compute ComputeCopyRBGFromTextureToImage set up
	// Gear 1: The ComputeCopyRBGFromTextureToImageGearT1 gear will read from the color buffer that was output to, 
	// by Gear 0, and feed this data into the "computeRead" image unit, so that it may be operated on by the next gear.
	//
	// This specific compute gear will assume that the color data built when the previous gear was run, will be found in GL_TEXTURE13;
	// This is required for the following line to work in the compute shader:
	//
	// layout(binding = 13) uniform sampler2D colorGData;	// for reading from the color texture
	createComputeProgram("ComputeCopyRBGFromTextureToImageGearT1");
	insertNewBuffer("compute_quad_buffer");								// quad buffer used for compute shaders.
	createComputeImage(GL_TEXTURE16, "computeRead", 1);					// image unit 1, "read"
	createComputeImage(GL_TEXTURE11, "computeWrite", 0);				// create on texture unit 11, bind to image unit 0
	insertComputeTransferGear(1, programLookup["ComputeCopyRBGFromTextureToImageGearT1"]);



	// ########################################################################## Compute Gear set up
	// Gear 2: The DeferredLightingComputeGearT1 compute shader is an experimental shader, that is designed to experiment with
	// rendering lights against tiles defined in compute shaders. This gear will apply lighting to the color buffer that was transffered-into by Gear 1.
	createComputeProgram("DeferredLightingComputeGearT1");
	insertNewBuffer("world_lights_ssbo");	// SSBO for WorldLights set up
	insertComputeGear(2, programLookup["DeferredLightingComputeGearT1"]);


	// ########################################################################## Compute results gear set up
	// Gear 3: Transfer the resulting lighting data that was stored into the image, into the main FBO.
	createProgram("DeferredComputeResultsGearT1");
	insertComputeResultsGear(3, programLookup["DeferredComputeResultsGearT1"]);

	
	// TEMPORARY: Just create the localized highlighting gear for now, to ensure the proposed localized logic works.
	// create the simple terrain gear
	createProgram("LocalizedHighlighterGearT1");
	insertNewBuffer("highlighter_buffer");
	insertLocalizedHighlighterGear(4, programLookup["LocalizedHighlighterGearT1"]);
	

	// ########################################################################## Instanced Highlighter Gear set up
	// Gear 5: Draw any instanced rendering models (currently unused)
	createProgram("InstancedHighlighterGearT1");
	insertNewBuffer("mesh_buffer");
	insertNewBuffer("matrices_buffer");
	//insertInstancedHighlighterGear(5, programLookup["InstancedHighlighterGearT1"]);

	// ########################################################################## Wave highlighter gear set up
	// Gear 6: display wave highlights (i.e, current ORE highlight, constituted highlights of an ORE)
	createProgram("LocalizedWaveHighlighterGearT1");
	insertWaveHighlighterGear(6, programLookup["LocalizedWaveHighlighterGearT1"]);

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

void SMLocalizedLightingComputeV1::setupDeferredFBO()
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

	// bind back to default frame buffer when done
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	std::cout << "Test of framebuffer has completed; value of deferred_FBO is: " << getFBOID("deferred_FBO") << std::endl;
	int someVal2 = 5;
	std::cin >> someVal2;
}

void SMLocalizedLightingComputeV1::createGBufText(GLenum texUnit, GLenum  format, GLuint &texid)
{
	glActiveTexture(texUnit);
	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void SMLocalizedLightingComputeV1::createComputeImage(GLenum texUnit, std::string in_imageName, int in_imageUnit)
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
	std::cout << "!!!!!!! SMLocalizedLightingComputeV1: Compute Image Texture ID is: " << getTextureID(in_imageName) << std::endl;	
}

void SMLocalizedLightingComputeV1::insertTerrainGear(int in_gearID, GLuint in_programID)
{
	gearTrain[in_gearID] = std::unique_ptr<Gear>(new LocalizedGBufferComputeLoaderGear());
	gearTrain[in_gearID]->initializeMachineShader(width, height, in_programID, window, this);
	gearTrain[in_gearID]->passGLuintValue("deferred_FBO", getFBOID("deferred_FBO"));					// pass the deferred FBO
}

void SMLocalizedLightingComputeV1::insertComputeTransferGear(int in_gearID, GLuint in_programID)
{
	gearTrain[in_gearID] = std::unique_ptr<Gear>(new ComputeCopyRBGFromTextureToImageGearT1());
	gearTrain[in_gearID]->initializeMachineShader(width, height, in_programID, window, this);
	gearTrain[in_gearID]->passGLuintValue("compute_quad_buffer", getBufferID("compute_quad_buffer"));
}

void SMLocalizedLightingComputeV1::insertComputeGear(int in_gearID, GLuint in_programID)
{
	gearTrain[in_gearID] = std::unique_ptr<Gear>(new DeferredLightingComputeGearT1());
	gearTrain[in_gearID]->initializeMachineShader(width, height, in_programID, window, this);
	gearTrain[in_gearID]->passGLuintValue("original_image_unit_1_texture_ID", getTextureID("computeWrite"));
	gearTrain[in_gearID]->passGLuintValue("original_image_unit_2_texture_ID", getTextureID("computeRead"));
	gearTrain[in_gearID]->passGLuintValue("world_lights_ssbo_buffer_ID", getBufferID("world_lights_ssbo"));

	std::cout << "!!! Compute Lights gear (Lighting Compute) inserted. " << std::endl;
}

void SMLocalizedLightingComputeV1::insertComputeResultsGear(int in_gearID, GLuint in_programID)
{
	gearTrain[in_gearID] = std::unique_ptr<Gear>(new DeferredComputeResultsGearT1());
	gearTrain[in_gearID]->initializeMachineShader(width, height, in_programID, window, this);
	gearTrain[in_gearID]->passGLuintValue("compute_quad_buffer", getBufferID("compute_quad_buffer"));
	gearTrain[in_gearID]->passGLuintValue("deferred_FBO", getFBOID("deferred_FBO"));
}

void SMLocalizedLightingComputeV1::insertLocalizedHighlighterGear(int in_gearID, GLuint in_programID)
{
	gearTrain[in_gearID] = std::unique_ptr<Gear>(new LocalizedHighlighterGearT1());
	gearTrain[in_gearID]->initializeMachineShader(width, height, in_programID, window, this);
}

void SMLocalizedLightingComputeV1::insertWaveHighlighterGear(int in_gearID, GLuint in_programID)
{
	gearTrain[in_gearID] = std::unique_ptr<Gear>(new LocalizedWaveHighlighterGearT1());
	gearTrain[in_gearID]->initializeMachineShader(width, height, in_programID, window, this);
}

void SMLocalizedLightingComputeV1::updateUniformRegistry()
{	
	// Assuming runMatrixAndDeltaLocalComputations was called before this, the value of MVP should be ok.
	//
	// The values of "worldCoordBPLocalCameraKey" and "worldCoordBPLocalCameraCoord" should have been updated 
	// in that same function, and their values already input into uniformRegistry.
	uniformRegistry.insertMat4("MVP", MVP);

	// screen width/height uniforms, adjusted to compute group dimensions
	uniformRegistry.insertInt("screenWidth", width);
	uniformRegistry.insertInt("screenHeight", height);

	// global ambience mutliplier
	uniformRegistry.insertFloat("globalAmbienceMultiplier", globalAmbienceMultiplier);

	//glm::mat4 currentMV = view;
	//uniformRegistry.insertMat4("ModelViewMatrix", currentMV); // update the MV
	uniformRegistry.insertVec3("worldPosition", position);	// update the world position uniform

	// insert the normal matrix for lighting
	model = glm::mat4(1.0);
	glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));

	uniformRegistry.insertMat3("NormalMatrix", normalMatrix);
}

void SMLocalizedLightingComputeV1::setupTextureAtlases()
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

	std::cout << "SMLocalizedLightingComputeV1: Atlas set up.................printing values: " << std::endl;
	std::cout << "SMLocalizedLightingComputeV1: Atlas texture width: " << uniformRegistry.getFloat("atlasTextureWidth");
	std::cout << "SMLocalizedLightingComputeV1: Atlas Tile texture width: " << uniformRegistry.getFloat("atlasTileTextureWidth");
}

void SMLocalizedLightingComputeV1::runAllShadersNoSwap()
{
	updateUniformRegistry();
	sendGearUniforms();
	runGearTrain();		// For now: call just LocalizedHighlighterGearT1::render().
}

void SMLocalizedLightingComputeV1::printDataForGears()
{

}

void SMLocalizedLightingComputeV1::insertCollectionGLData(TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef)
{	
	GearFindResult gearFinder = findGear("LocalizedGBufferComputeLoaderGear");
	if (gearFinder.wasResultFound)
	{
		std::cout << "SMLocalizedLightingComputeV1 instance found gear to insert terrain data into, at gear index: " << gearFinder.foundGearIndex << std::endl;
		gearTrain[gearFinder.foundGearIndex]->sendTerrainDataToGear(in_jobResults, in_arraySize, in_arrayRef);
	}
}

void SMLocalizedLightingComputeV1::flagCollectionGLDataForRemoval(EnclaveKeyDef::EnclaveKey in_keyForRemoval)
{
	GearFindResult gearFinder = findGear("LocalizedGBufferComputeLoaderGear");
	if (gearFinder.wasResultFound)
	{
		std::cout << "SMLocalizedLightingComputeV1 instance found gear to remove terrain rendering for: " << gearFinder.foundGearIndex << std::endl;
		gearTrain[gearFinder.foundGearIndex]->removeTerrainDataFromGear(in_keyForRemoval);
	}
}