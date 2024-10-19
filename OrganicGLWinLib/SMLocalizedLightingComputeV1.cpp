#include "stdafx.h"
#include "SMLocalizedLightingComputeV1.h"

void SMLocalizedLightingComputeV1::initialize(int in_windowWidth, int in_windowHeight, int in_immutableBufferSize)
{
	// set coordinate mode
	machineCoordMode = GPUCoordinateMode::COORDINATE_MODE_LOCAL;
	setMachineCoordModeDependentSettings();

	// basic initialization;
	// For the time being, this is a simple shader that should examine how localized translations 
	// are rendered; this will be changed to take compute shaders into account at a later date.
	width = in_windowWidth;
	height = in_windowHeight;

	// set shader specific VAO values
	vaoAttribMode = 1;
	vaoAttribByteSize = 24;

	OrganicGLWinUtils::initializeLibraryAndSetHints();				// initialization
	window = OrganicGLWinUtils::createGLFWWindow(width, height);	// create the GLFW window
	OrganicGLWinUtils::checkWindowValidity(window);			// CHECK FOR DEFERRED?
	OrganicGLWinUtils::makeContextCurrent(window);
	OrganicGLWinUtils::initializeGlew();
	OrganicGLWinUtils::setBasicStates();					// CHECK FOR DEFERRED?
	OrganicGLWinUtils::setGLFWInputMode(window);
	OrganicGLWinUtils::setClearColor(.11f, .13f, .15f, 0.0f);	// background color

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


	// Stuff for first gear (LocalizedGBufferComputeLoaderGear)
	// !! create program here 
	createProgram("LocalizedGBufferComputeLoaderGear");
	insertNewFBO("deferred_FBO");
	setupDeferredFBO();
	insertTerrainGear(0, programLookup["LocalizedGBufferComputeLoaderGear"]);

	// TEMPORARY: Just create the localized highlighting gear for now, to ensure the proposed localized logic works.
	// create the simple terrain gear
	createProgram("LocalizedHighlighterGearT1");
	insertLocalizedHighlighterGear(1, programLookup["LocalizedHighlighterGearT1"]);










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


	std::cout << "++++++++ !! SMLocalizedLightingComputeV1 complete. " << std::endl;
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

void SMLocalizedLightingComputeV1::insertTerrainGear(int in_gearID, GLuint in_programID)
{
	gearTrain[in_gearID] = std::unique_ptr<Gear>(new LocalizedGBufferComputeLoaderGear());
	gearTrain[in_gearID]->initializeMachineShader(width, height, in_programID, window, this);
	gearTrain[in_gearID]->passGLuintValue("deferred_FBO", getFBOID("deferred_FBO"));					// pass the deferred FBO
}

void SMLocalizedLightingComputeV1::insertLocalizedHighlighterGear(int in_gearID, GLuint in_programID)
{
	gearTrain[in_gearID] = std::unique_ptr<Gear>(new LocalizedHighlighterGearT1());
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
	//globalAmbienceMultiplier = 1.0f;
	uniformRegistry.insertFloat("globalAmbienceMultiplier", globalAmbienceMultiplier);

	//glm::mat4 currentMV = view;
	//uniformRegistry.insertMat4("ModelViewMatrix", currentMV); // update the MV
	uniformRegistry.insertVec3("worldPosition", position);	// update the world position uniform

	// insert the normal matrix for lighting

	//glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(view)));

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