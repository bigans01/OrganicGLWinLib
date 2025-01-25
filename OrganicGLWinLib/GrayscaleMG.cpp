#include "stdafx.h"
#include "GrayscaleMG.h"

void GrayscaleMG::initializeMGear()
{
	std::cout << "GrayscaleMG: attempting instantiation of shader." << std::endl;

	// fetch the constructed program data, store it
	auto programData = OrganicShaderLoader::createBasicShaderProgram("graphics/mGearShaders/GrayscaleMG.vertexshader", "graphics/mGearShaders/GrayscaleMG.fragmentshader");
	mGearProgramID = programData.programId;
	grayscaleVertexShaderId = programData.vertexShaderId;
	grayscaleFragmentShaderId = programData.fragmentShaderId;
}

void GrayscaleMG::render()
{

}

void GrayscaleMG::cleanupMGear()
{
	// delete shaders, and then the program itself
	std::cout << "GrayscaleMG: cleaning up resources..." << std::endl;
	glDeleteShader(grayscaleVertexShaderId);
	glDeleteShader(grayscaleFragmentShaderId);
	glDeleteProgram(mGearProgramID);
}