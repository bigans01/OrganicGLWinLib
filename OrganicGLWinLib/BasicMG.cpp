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
	

	// optional: use doesBindingExist to check that a binding exists.
	bool testExistence = mGearObjectManagerRef->doesBindingExist(MAPIObjectMetadata(MAPIObjectType::BUFFER, EnclaveKeyDef::EnclaveKey(1, 1, 1), "3dTest"));
	if (testExistence)
	{
		//std::cout << "binding exists!!! " << std::endl;
	}
	
	GLuint testFetch = mGearObjectManagerRef->fetchBinding(MAPIObjectMetadata(MAPIObjectType::BUFFER, EnclaveKeyDef::EnclaveKey(1, 1, 1), "3dTest"));

	/*
	// Testing only (to make sure fetchBinding works), un comment when needed.
	if (testFetch != -1)
	{
		std::cout << "BasicMG: found buffer in mGearObjectManagerRef: " << testFetch << std::endl;
	}
	*/

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