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
	glfwSetKeyCallback(window, keyCallbackWrapper);

	// NEW ---> setup IMGui
	OrganicGLWinUtils::IMGuiInit(window);

	// TEMPORARY: Just create the localized highlighting gear for now, to ensure the proposed localized logic works.
	// create the simple terrain gear
	createProgram("LocalizedHighlighterGearT1");
	insertLocalizedHighlighterGear(0, programLookup["LocalizedHighlighterGearT1"]);
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

}

void SMLocalizedLightingComputeV1::setupTextureAtlases()
{

}

void SMLocalizedLightingComputeV1::runAllShadersAndSwap()
{

}

void SMLocalizedLightingComputeV1::runAllShadersNoSwap()
{
	updateUniformRegistry();
	sendGearUniforms();
	runGearTrain();		// For now: call just LocalizedHighlighterGearT1::render().
}

void SMLocalizedLightingComputeV1::multiDrawTerrain(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, int in_numberOfCollections)
{

}

void SMLocalizedLightingComputeV1::printDataForGears()
{

}

void SMLocalizedLightingComputeV1::insertCollectionGLData(TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef)
{

}

void SMLocalizedLightingComputeV1::flagCollectionGLDataForRemoval(EnclaveKeyDef::EnclaveKey in_keyForRemoval)
{

}