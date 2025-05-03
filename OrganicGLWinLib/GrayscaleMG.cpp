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

void GrayscaleMG::setObjectBindings()
{

}

void GrayscaleMG::conductMAPIObjectScan(bool in_fullscanFlag, std::vector<MAPIObjectUpdate>* in_updateVectorRef)
{
	// If the full scan flag is set, we will need to check all MAPI objects to see if there's anything we want.
	if (in_fullscanFlag)
	{
		// NOTICE: Be sure to clear out the container(s) used by this MGearBase-derived class completely, 
		// before inserting based off a full scan (it is safer to start from scratch, as bindings for any deleted buffers
		// will still be lurking in the binding maps (before the first tick) )
		//
		// This needs to be fixed at a later date to somtehing cleaner (noted on 5/2/2025)

		std::cout << "GrayscaleMG: conducting scan of usable objects (FULL_SCAN)" << std::endl;
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

void GrayscaleMG::render()
{
	// switch to this program, then
	// use MAPIObjectManager::fetchBinding to bind to appropriate resources (i.e, glBindBuffer(mGearObjectManagerRef->fetchBinding(...)); )
	switchToMGearProgram();
}

void GrayscaleMG::cleanupMGear()
{
	// delete shaders, and then the program itself
	std::cout << "GrayscaleMG: cleaning up resources..." << std::endl;
	glDeleteShader(grayscaleVertexShaderId);
	glDeleteShader(grayscaleFragmentShaderId);
	glDeleteProgram(mGearProgramID);
}