#include "stdafx.h"
#include "MAPIObjectManager.h"

MAPIObjectManager::~MAPIObjectManager()
{
	cleanupResources();
}

bool MAPIObjectManager::doesBindingExist(MAPIObjectType in_bindingType, std::string in_bindingName)
{
	bool wasFound = false;

	switch (in_bindingType)
	{
		case MAPIObjectType::BUFFER:
		{
			auto bindingNameFinder = bufferResourceMap.find(in_bindingName);
			if (bindingNameFinder != bufferResourceMap.end())	// it was found
			{
				wasFound = true;
			}
			break;
		}
	}

	return wasFound;
}

bool MAPIObjectManager::attemptBindingInsert(MAPIObjectData in_bindingToInsert)
{
	bool wasInsertSuccessful = false;
	switch (in_bindingToInsert.getBindingType())
	{
		case MAPIObjectType::BUFFER:
		{
			auto bindingNameToCheck = in_bindingToInsert.getBindingName();
			auto nameFindIter = bufferResourceMap.find(bindingNameToCheck);
			if (nameFindIter == bufferResourceMap.end()) // it was not found
			{
				MBufferBinding newBufferBinding(in_bindingToInsert);
				bufferResourceMap[in_bindingToInsert.getBindingName()] = newBufferBinding;
				wasInsertSuccessful = true;
			}
			// if the above statement didn't work, it already existed, and we didn't insert.

			break;
		}
	}
	return wasInsertSuccessful;
}

int MAPIObjectManager::fetchBinding(MAPIObjectType in_bindingType, std::string in_bindingName)
{
	int returnBinding = -1;	// if this is still -1 when we return, we're in trouble (the resource wasn't found)

	switch (in_bindingType)
	{
		case MAPIObjectType::BUFFER:
		{
			auto bindingNameFinder = bufferResourceMap.find(in_bindingName);
			if (bindingNameFinder != bufferResourceMap.end())	// it was found
			{
				returnBinding = bufferResourceMap[in_bindingName].bufferId;
			}
			break;
		}
	}

	return returnBinding;
}

void MAPIObjectManager::cleanupResources()
{
	// cleanup buffer bindings
	for (auto& currentBufferResource : bufferResourceMap)
	{
		currentBufferResource.second.deleteBufferResource();
	}
}