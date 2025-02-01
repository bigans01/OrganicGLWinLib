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

	std::cout << "GrayscaleMG: checking current required variables: " << std::endl;
	auto screenWidth = mGearUniformRegistryRef->getInt("screen_pixel_width");
	auto screenHeight = mGearUniformRegistryRef->getInt("screen_pixel_height");
	auto computeGroupDim = mGearUniformRegistryRef->getInt("screen_compute_group_dim");
	auto computeColumns = mGearUniformRegistryRef->getInt("screen_compute_columns");
	auto computeRows = mGearUniformRegistryRef->getInt("screen_compute_rows");
	std::cout << "screenWidth: " << screenWidth << std::endl;
	std::cout << "screenHeight: " << screenHeight << std::endl;;
	std::cout << "computeGroupDim: " << computeGroupDim << std::endl;
	std::cout << "computeColumns: " << computeColumns << std::endl;
	std::cout << "computeRows: " << computeRows << std::endl;
}

std::vector<MAPIObjectMetadata> GrayscaleMG::fetchRequiredObjects()
{
	std::vector<MAPIObjectMetadata> returnVector;
	returnVector.push_back(MAPIObjectMetadata(MAPIObjectType::BUFFER, "testbuffer"));
	returnVector.push_back(MAPIObjectMetadata(MAPIObjectType::TEXTURE, "testTexture"));
	returnVector.push_back(MAPIObjectMetadata(MAPIObjectType::FBO, "testFBO"));



	return returnVector;
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