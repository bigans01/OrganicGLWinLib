#include "stdafx.h"
#include "BasicMG.h"

void BasicMG::initializeMGear()
{
	std::cout << "BasicMG: attempting instantiation of shader." << std::endl;

	// fetch the constructed program data, store it
	auto programData = OrganicShaderLoader::createBasicShaderProgram("graphics/mGearShaders/BasicMG.vertexshader", "graphics/mGearShaders/BasicMG.fragmentshader");
	mGearProgramID = programData.programId;
	basicVertexShaderId = programData.vertexShaderId;
	basicFragmentShaderId = programData.fragmentShaderId;
}

std::vector<MAPIObjectMetadata> BasicMG::fetchRequiredObjects()
{
	std::vector<MAPIObjectMetadata> returnVector;
	returnVector.push_back(MAPIObjectMetadata(MAPIObjectType::BUFFER, "somebufferhuh"));




	return returnVector;
}

void BasicMG::setObjectBindings()
{ 

}

void BasicMG::render()
{
	// switch to this program, then
	// use MAPIObjectManager::fetchBinding to bind to appropriate resources (i.e, glBindBuffer(mGearObjectManagerRef->fetchBinding(...)); )
	switchToMGearProgram();
}

void BasicMG::cleanupMGear()
{ 
	// delete shaders, and then the program itself
	std::cout << "BasicMG: cleaning up resources..." << std::endl;
	glDeleteShader(basicVertexShaderId);
	glDeleteShader(basicFragmentShaderId);
	glDeleteProgram(mGearProgramID);
}