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

		case MAPIObjectType::TEXTURE:
		{
			auto bindingNameFinder = textureResourceMap.find(in_bindingName);
			if (bindingNameFinder != textureResourceMap.end())
			{
				wasFound = true;
			}
			break;
		}

		case MAPIObjectType::FBO:
		{
			auto fboNameFinder = fboResourceMap.find(in_bindingName);
			if (fboNameFinder != fboResourceMap.end())
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
	int wasSuccessful = 0;	// this will get to 1 upon success
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
				attemptMetadata.insertString("MAPIObjectManager created new buffer, " + bindingNameToCheck + " bound with ID: " + std::to_string(bufferResourceMap[in_bindingToInsert.getBindingName()].getBufferId()));
			}
			// if the above statement didn't work, it already existed, and we didn't insert. So return false.
			else
			{ 
				attemptMetadata.insertString("MAPIObjectManager creation of new buffer, " + bindingNameToCheck + " failed (already exists)");
			}

			break;
		}

		case MAPIObjectType::TEXTURE:
		{
			auto bindingNameToCheck = in_bindingToInsert.getBindingName();
			auto nameFindIter = textureResourceMap.find(bindingNameToCheck);
			if (nameFindIter == textureResourceMap.end())
			{
				// Generating a texture is also a simple operation (glGenTextures); there should really be no error checking required here.
				MTextureBinding newTextureBinding(in_bindingToInsert);
				textureResourceMap[in_bindingToInsert.getBindingName()] = newTextureBinding;
				wasSuccessful = 1;
				attemptMetadata.insertString("MAPIObjectManager created new texture, " + bindingNameToCheck + " bound with ID: " + std::to_string(textureResourceMap[in_bindingToInsert.getBindingName()].getTextureId()));
			}
			else
			{
				attemptMetadata.insertString("MAPIObjectManager creation of new texture, " + bindingNameToCheck + " failed (already exists)");
			}

			break;
		}

		case MAPIObjectType::FBO:
		{
			auto bindingNameTocheck = in_bindingToInsert.getBindingName();
			auto nameFindIter = fboResourceMap.find(bindingNameTocheck);
			if (nameFindIter == fboResourceMap.end())
			{
				// Generating a FBO object is a simple call in OpenGL; various attachments such as textures and other settintgs are outside
				// of the scope of this function
				MFBOBinding newFBOBinding(in_bindingToInsert);
				fboResourceMap[in_bindingToInsert.getBindingName()] = newFBOBinding;
				wasSuccessful = 1;
				attemptMetadata.insertString("MAPIObjectManager created new frame buffer object (FBO), " + bindingNameTocheck + " bound with ID: " + std::to_string(fboResourceMap[in_bindingToInsert.getBindingName()].getFBOId()));
			}
			else
			{
				attemptMetadata.insertString("MAPIObjectManager creation of new frame buffer object (FBO), " + bindingNameTocheck + " failed (already exists)");
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
			// Create the MAPIObjectData for a BUFFER and use it in the call to attemptBindingInsert.
			// Should require no additional metadata in the Message.
			std::string bufferName = in_objectRequest.getBindingRequestName();
			MAPIObjectData newBufferBinding(MAPIObjectType::BUFFER, bufferName, Message(MessageType::NOVAL));
			handleAttemptData = attemptBindingInsert(newBufferBinding);
			break;
		}

		case MAPIObjectType::TEXTURE:
		{
			// Create the MAPIObjectData for a TEXTURE and use it in the call to attemptBindingInsert.
			// Should require no additional metadata in the Message.
			std::string textureName = in_objectRequest.getBindingRequestName();
			MAPIObjectData newTextureBinding(MAPIObjectType::TEXTURE, textureName, Message(MessageType::NOVAL));
			handleAttemptData = attemptBindingInsert(newTextureBinding);
			break;
		}

		case MAPIObjectType::FBO:
		{
			// Create the MAPIObjectData for a FBO and use it in the call to attemptBindingInsert.
			// Should require no additional metadata in the Message.
			std::string fboName = in_objectRequest.getBindingRequestName();
			MAPIObjectData newFBOBinding(MAPIObjectType::FBO, fboName, Message(MessageType::NOVAL));
			handleAttemptData = attemptBindingInsert(newFBOBinding);
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
				returnBinding = bufferResourceMap[in_bindingName].getBufferId();
			}
			break;
		}

		case MAPIObjectType::TEXTURE:
		{
			auto bindingNameFinder = textureResourceMap.find(in_bindingName);
			if (bindingNameFinder != textureResourceMap.end())	// it was found
			{
				returnBinding = textureResourceMap[in_bindingName].getTextureId();
			}
			break;
		}

		case MAPIObjectType::FBO:
		{
			auto bindingNameFinder = fboResourceMap.find(in_bindingName);
			if (bindingNameFinder != fboResourceMap.end())	// it was found
			{
				returnBinding = fboResourceMap[in_bindingName].getFBOId();
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

	// cleanup texture bindings
	for (auto& currentTextureResource : textureResourceMap)
	{
		currentTextureResource.second.deleteTextureResource();
	}

	// cleanup FBO bindings
	for (auto& currentFBOResource : fboResourceMap)
	{
		currentFBOResource.second.deleteFBOResource();
	}
}