#include "stdafx.h"
#include "MShaderCatalog.h"

bool MShaderCatalog::insertMShader(std::string in_shaderName, std::unique_ptr<MShaderBase> in_newShaderInstancePtr)
{
	bool wasInserted = false;
	auto currentShaderFinder = shaderMap.find(in_shaderName);
	if (currentShaderFinder == shaderMap.end())	// it was not found
	{
		std::cout << "!! Inserting new shader..." << std::endl;
		shaderMap[in_shaderName] = std::move(in_newShaderInstancePtr);
		std::cout << "!! Inserting new shader, MOVE OK" << std::endl;
		wasInserted = true;
	}
	return wasInserted;
}

bool MShaderCatalog::isShaderInMap(std::string in_shaderName)
{
	bool shaderFound = false;
	auto shaderFinder = shaderMap.find(in_shaderName);
	if (shaderFinder != shaderMap.end())	// it was found
	{
		shaderFound = true;
	}
	return shaderFound;
}

MShaderBase* MShaderCatalog::getShaderRef(std::string in_shaderName)
{
	MShaderBase* returnPtr = nullptr;
	auto shaderFinder = shaderMap.find(in_shaderName);
	if (shaderFinder != shaderMap.end())	// it was found
	{
		returnPtr = shaderMap[in_shaderName].get();
	}
	return returnPtr;
}