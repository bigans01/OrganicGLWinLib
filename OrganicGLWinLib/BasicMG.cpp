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

void BasicMG::conductMAPIObjectScan(bool in_fullscanFlag, std::vector<MAPIObjectUpdate>* in_updateVectorRef)
{ 
	// If the full scan flag is set, we will need to check all MAPI objects to see if there's anything we want.
	if (in_fullscanFlag)
	{
		// NOTICE: Be sure to clear out the container(s) used by this MGearBase-derived class completely, 
		// before inserting based off a full scan (it is safer to start from scratch, as bindings for any deleted buffers
		// will still be lurking in the binding maps (before the first tick) )
		//
		// This needs to be fixed at a later date to somtehing cleaner (noted on 5/2/2025)


		std::cout << "BasicMG: conducting scan of usable objects (FULL_SCAN)" << std::endl;
		std::vector<MAPIObjectBinding> fetchedBindings = mGearObjectManagerRef->fetchAllBindings();
		for (auto& currentBinding : fetchedBindings)
		{
			// ...
			// Do whatever we want here, (ignore a buffer, accept a buffer, put it's value into a map...)
			// ...

			currentBinding.printBindingData();
		}
	}
	// ...otherwise, check whatever was added in the in_updateVectorRef, which contains updates for this tick, but gets reset
	// and cleared after the tick ends.
	else
	{
		for (auto& currentObjectUpdate : *in_updateVectorRef)
		{
			currentObjectUpdate.printUpdateData();
		}
	}
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