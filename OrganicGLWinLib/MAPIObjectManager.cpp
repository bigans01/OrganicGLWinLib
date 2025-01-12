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

Message MAPIObjectManager::attemptBindingInsert(MAPIObjectData in_bindingToInsert)
{
	Message attemptMetadata(MessageType::MSHADER_INFO);
	int wasSuccessful = 0;
	switch (in_bindingToInsert.getBindingType())
	{
		case MAPIObjectType::BUFFER:
		{
			auto bindingNameToCheck = in_bindingToInsert.getBindingName();
			auto nameFindIter = bufferResourceMap.find(bindingNameToCheck);
			if (nameFindIter == bufferResourceMap.end()) // it was not found
			{
				// Because MBufferBinding makes a call to glGenBuffers with an argument of 1 buffer,
				// there should in theory never be an error that is generated when attempting to create a buffer.
				// So if it gets created an inserted, just automatically assume it was succesful (i.e, no special
				// error checking is required.)
				MBufferBinding newBufferBinding(in_bindingToInsert);
				bufferResourceMap[in_bindingToInsert.getBindingName()] = newBufferBinding;
				wasSuccessful = 1;
				attemptMetadata.insertString("MAPIObjectManager created new buffer, " + bindingNameToCheck);
			}
			// if the above statement didn't work, it already existed, and we didn't insert. So return false.
			else
			{ 
				attemptMetadata.insertString("MAPIObjectManager creation of new buffer, " + bindingNameToCheck + " failed (already exists)");
			}

			break;
		}
	}

	attemptMetadata.insertInt(wasSuccessful);
	return attemptMetadata;
}

Message MAPIObjectManager::handleMAPIObjectRequest(MAPIObjectRequest in_objectRequest)
{
	Message handleAttemptData;

	switch (in_objectRequest.getBindingRequestType())
	{
		case MAPIObjectType::BUFFER:
		{
			// check if the buffer exists already. If it does not exist, 
			// create the MAPIObjectData for a BUFFER and use it in the call to attemptBindingInsert.
			// Should require no additional metadata in the Message.
			std::string bufferName = in_objectRequest.getBindingRequestName();
			MAPIObjectData newBufferBinding(MAPIObjectType::BUFFER, bufferName, Message(MessageType::NOVAL));
			handleAttemptData = attemptBindingInsert(newBufferBinding);
			break;
		}
	}

	return handleAttemptData;
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