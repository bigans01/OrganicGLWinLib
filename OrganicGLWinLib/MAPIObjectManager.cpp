#include "stdafx.h"
#include "MAPIObjectManager.h"

MAPIObjectManager::~MAPIObjectManager()
{
	cleanupResources();
}

bool MAPIObjectManager::doesBindingExist(MAPIObjectMetadata in_objectMeta)
{
	bool wasFound = false;

	switch (in_objectMeta.fetchMdMapKeyType())
	{
		// For typical string-based binding searching
		case MAPIObjectMapKeyType::STRING_KEYTYPE:
		{
			switch (in_objectMeta.fetchMdObjectType())
			{
				case MAPIObjectType::BUFFER:
				{
					auto bindingNameFinder = bufferResourceMap.find(in_objectMeta.fetchMdName());
					if (bindingNameFinder != bufferResourceMap.end())	// it was found
					{
						wasFound = true;
					}
					break;
				}

				case MAPIObjectType::TEXTURE:
				{
					auto bindingNameFinder = textureResourceMap.find(in_objectMeta.fetchMdName());
					if (bindingNameFinder != textureResourceMap.end())
					{
						wasFound = true;
					}
					break;
				}

				case MAPIObjectType::FBO:
				{
					auto fboNameFinder = fboResourceMap.find(in_objectMeta.fetchMdName());
					if (fboNameFinder != fboResourceMap.end())
					{
						wasFound = true;
					}
					break;
				}
			}
			break;
		}

		// For EnclaveKey-based  binding searching
		case MAPIObjectMapKeyType::ENCLAVE_KEYTYPE:
		{
			std::string threeDKeyMapName = in_objectMeta.fetchMdKeyMapName();
			switch (in_objectMeta.fetchMdObjectType())
			{
				case MAPIObjectType::BUFFER:
				{
					auto threeDKeyMapNameFinder = stringedThreeDBufferMaps.find(threeDKeyMapName);
					if (threeDKeyMapNameFinder != stringedThreeDBufferMaps.end())
					{
						// it exists
						if (threeDKeyMapNameFinder->second.doesThreeDBindingExist(in_objectMeta.fetchMdKeyValue()))
						{
							wasFound = true;
						}
					}
					break;
				}
			}
			break;
		}

	}
	return wasFound;
}

bool MAPIObjectManager::deleteBinding(MAPIObjectMetadata in_objectMeta)
{ 
	bool wasBindingDeleted = false;
	switch (in_objectMeta.fetchMdMapKeyType())
	{
		// For typical string-based binding searching
		case MAPIObjectMapKeyType::STRING_KEYTYPE:
		{
			switch (in_objectMeta.fetchMdObjectType())
			{
				case MAPIObjectType::BUFFER:
				{
					auto bindingNameFinder = bufferResourceMap.find(in_objectMeta.fetchMdName());
					if (bindingNameFinder != bufferResourceMap.end())	// it was found
					{
						// Remember, the buffer resource must be deleted first, before the erase.
						bufferResourceMap[in_objectMeta.fetchMdName()].deleteBufferResource();
						bufferResourceMap.erase(in_objectMeta.fetchMdName());
						wasBindingDeleted = true;
					}
					break;
				}

				case MAPIObjectType::TEXTURE:
				{
					auto bindingNameFinder = textureResourceMap.find(in_objectMeta.fetchMdName());
					if (bindingNameFinder != textureResourceMap.end())
					{
						// Remember, the texture resource must be deleted first, before the erase.
						textureResourceMap[in_objectMeta.fetchMdName()].deleteTextureResource();
						textureResourceMap.erase(in_objectMeta.fetchMdName());
						wasBindingDeleted = true;
					}
					break;
				}

				case MAPIObjectType::FBO:
				{
					auto fboNameFinder = fboResourceMap.find(in_objectMeta.fetchMdName());
					if (fboNameFinder != fboResourceMap.end())
					{
						fboResourceMap[in_objectMeta.fetchMdName()].deleteFBOResource();
						fboResourceMap.erase(in_objectMeta.fetchMdName());
						wasBindingDeleted = true;
					}
					break;
				}
			}
			break;
		}

		// For EnclaveKey-based  binding searching
		case MAPIObjectMapKeyType::ENCLAVE_KEYTYPE:
		{
			std::string threeDKeyMapName = in_objectMeta.fetchMdKeyMapName();
			switch (in_objectMeta.fetchMdObjectType())
			{
				case MAPIObjectType::BUFFER:
				{
					auto threeDKeyMapNameFinder = stringedThreeDBufferMaps.find(threeDKeyMapName);
					if (threeDKeyMapNameFinder != stringedThreeDBufferMaps.end())
					{
						// it exists
						if (threeDKeyMapNameFinder->second.doesThreeDBindingExist(in_objectMeta.fetchMdKeyValue()))
						{
							threeDKeyMapNameFinder->second.deleteThreeDBinding(in_objectMeta.fetchMdKeyValue());
							wasBindingDeleted = true;
						}

						// if a binding was deleted, check the size of the 3d map. if it's 0, just delete it to free up memory.
						if (wasBindingDeleted && threeDKeyMapNameFinder->second.isMapEmpty())
						{
							stringedThreeDBufferMaps.erase(threeDKeyMapName);
						}
					}
					break;
				}
			}
			break;
		}

	}

	return wasBindingDeleted;
}

std::vector<MAPIObjectBinding> MAPIObjectManager::fetchAllBindings()
{ 
	std::vector<MAPIObjectBinding> bindingVector;

	// Check stringed buffers
	for (auto& currentBufferResource : bufferResourceMap)
	{
		bindingVector.push_back(MAPIObjectBinding(MAPIObjectType::BUFFER,
								currentBufferResource.second.getStringedDataBindingName(),
								currentBufferResource.second.getBufferId()));
	}

	// Check stringed textures
	for (auto& currentTextureResource : textureResourceMap)
	{
		bindingVector.push_back(MAPIObjectBinding(MAPIObjectType::TEXTURE,
			currentTextureResource.second.getStringedDataBindingName(),
			currentTextureResource.second.getTextureId()));
	}

	// Check stringed FBOs
	for (auto& currentFBOResource : fboResourceMap)
	{
		bindingVector.push_back(MAPIObjectBinding(MAPIObjectType::TEXTURE,
			currentFBOResource.second.getStringedDataBindingName(),
			currentFBOResource.second.getFBOId()));
	}

	// Check 3d-keyed buffers
	for (auto& currentStringedKeyedmap : stringedThreeDBufferMaps)
	{
		for (auto& current3dKeyedElement : currentStringedKeyedmap.second.threeDBindings)
		{
			bindingVector.push_back(MAPIObjectBinding(MAPIObjectType::BUFFER,
													current3dKeyedElement.first,
													currentStringedKeyedmap.first, 
													current3dKeyedElement.second.getThreeDKeyedBufferId()));
		}
	}


	return bindingVector;
}


Message MAPIObjectManager::handleMAPIObjectRequest(MAPIObjectRequest in_objectRequest)
{
	Message handleAttemptData;

	switch (in_objectRequest.getBindingMapKeyType())
	{
		// For typical string-based requests
		case MAPIObjectMapKeyType::STRING_KEYTYPE:
		{
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
		    break;
		}

		// For EnclaveKey-based requests
		case MAPIObjectMapKeyType::ENCLAVE_KEYTYPE:
		{
			switch (in_objectRequest.getBindingRequestType())
			{
				case MAPIObjectType::BUFFER:
				{
					// Create the MAPIObjectData for a BUFFER and use it in the call to attemptBindingInsert.
					// Should require no additional metadata in the Message; we will need to get the EnclaveKey for the map,
					// and the string-keyed value for that map we are inserting into.
					std::string keyedMapName = in_objectRequest.getBindingRequestKeyMapName();
					EnclaveKeyDef::EnclaveKey keyedMapKey = in_objectRequest.getBindingEnclaveKey();
					MAPIObjectData newBufferBinding(MAPIObjectType::BUFFER, keyedMapKey, keyedMapName, Message(MessageType::NOVAL));
					handleAttemptData = attemptBindingInsert(newBufferBinding);
					break;
				}

				case MAPIObjectType::TEXTURE:
				{
					// Create the MAPIObjectData for a TEXTURE and use it in the call to attemptBindingInsert.
					// Should require no additional metadata in the Message; we will need to get the EnclaveKey for the map,
					// and the string-keyed value for that map we are inserting into.
					std::string keyedMapName = in_objectRequest.getBindingRequestKeyMapName();
					EnclaveKeyDef::EnclaveKey keyedMapKey = in_objectRequest.getBindingEnclaveKey();
					MAPIObjectData newTextureBinding(MAPIObjectType::TEXTURE, keyedMapKey, keyedMapName, Message(MessageType::NOVAL));
					handleAttemptData = attemptBindingInsert(newTextureBinding);
					break;
				}

				case MAPIObjectType::FBO:
				{
					// Create the MAPIObjectData for a FBO and use it in the call to attemptBindingInsert.
					// Should require no additional metadata in the Message; we will need to get the EnclaveKey for the map,
					// and the string-keyed value for that map we are inserting into.
					std::string keyedMapName = in_objectRequest.getBindingRequestKeyMapName();
					EnclaveKeyDef::EnclaveKey keyedMapKey = in_objectRequest.getBindingEnclaveKey();
					MAPIObjectData newTextureBinding(MAPIObjectType::FBO, keyedMapKey, keyedMapName, Message(MessageType::NOVAL));
					handleAttemptData = attemptBindingInsert(newTextureBinding);
					break;
				}
			}
			break;
		}
	}
	return handleAttemptData;
}

Message MAPIObjectManager::attemptBindingInsert(MAPIObjectData in_bindingToInsert)
{
	Message attemptMetadata(MessageType::MSHADER_INFO);
	int wasSuccessful = 0;	// this will get to 1 upon success

	switch (in_bindingToInsert.getDataMapKeyType())
	{
		// For typical string-based binding inserts
		case MAPIObjectMapKeyType::STRING_KEYTYPE:
		{
			switch (in_bindingToInsert.getDataBindingType())
			{
				case MAPIObjectType::BUFFER:
				{
					auto bindingNameToCheck = in_bindingToInsert.getDataBindingName();
					auto nameFindIter = bufferResourceMap.find(bindingNameToCheck);
					if (nameFindIter == bufferResourceMap.end()) // it was not found
					{
						// Because StringedMBufferBinding makes a call to glGenBuffers with an argument of 1 buffer,
						// there should in theory never be an error that is generated when attempting to create a buffer.
						// So if it gets created an inserted, just automatically assume it was succesful (i.e, no special
						// error checking is required.)
						StringedMBufferBinding newBufferBinding(in_bindingToInsert);
						bufferResourceMap[in_bindingToInsert.getDataBindingName()] = newBufferBinding;
						wasSuccessful = 1;
						attemptMetadata.insertString("MAPIObjectManager created new buffer, " + bindingNameToCheck + " bound with ID: " + std::to_string(bufferResourceMap[in_bindingToInsert.getDataBindingName()].getBufferId()));
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
					auto bindingNameToCheck = in_bindingToInsert.getDataBindingName();
					auto nameFindIter = textureResourceMap.find(bindingNameToCheck);
					if (nameFindIter == textureResourceMap.end())
					{
						// Generating a texture is also a simple operation (glGenTextures); there should really be no error checking required here.
						StringedMTextureBinding newTextureBinding(in_bindingToInsert);
						textureResourceMap[in_bindingToInsert.getDataBindingName()] = newTextureBinding;
						wasSuccessful = 1;
						attemptMetadata.insertString("MAPIObjectManager created new texture, " + bindingNameToCheck + " bound with ID: " + std::to_string(textureResourceMap[in_bindingToInsert.getDataBindingName()].getTextureId()));
					}
					else
					{
						attemptMetadata.insertString("MAPIObjectManager creation of new texture, " + bindingNameToCheck + " failed (already exists)");
					}

					break;
				}

				case MAPIObjectType::FBO:
				{
					auto bindingNameTocheck = in_bindingToInsert.getDataBindingName();
					auto nameFindIter = fboResourceMap.find(bindingNameTocheck);
					if (nameFindIter == fboResourceMap.end())
					{
						// Generating a FBO object is a simple call in OpenGL; various attachments such as textures and other settintgs are outside
						// of the scope of this function
						StringedMFBOBinding newFBOBinding(in_bindingToInsert);
						fboResourceMap[in_bindingToInsert.getDataBindingName()] = newFBOBinding;
						wasSuccessful = 1;
						attemptMetadata.insertString("MAPIObjectManager created new frame buffer object (FBO), " + bindingNameTocheck + " bound with ID: " + std::to_string(fboResourceMap[in_bindingToInsert.getDataBindingName()].getFBOId()));
					}
					else
					{
						attemptMetadata.insertString("MAPIObjectManager creation of new frame buffer object (FBO), " + bindingNameTocheck + " failed (already exists)");
					}

					break;
				}
			}
			break;
		}

		// For EnclaveKey-based binding inserts
		case MAPIObjectMapKeyType::ENCLAVE_KEYTYPE:
		{
			switch (in_bindingToInsert.getDataBindingType())
			{
				case MAPIObjectType::BUFFER:
				{
					auto threeDMapName = in_bindingToInsert.getDataBindingThreeDKeyMapName();
					auto threeDMapKeyValue = in_bindingToInsert.getDataBindingThreeDKey();

					// If there is no key/value pair for the given 3d key, insert the requested value
					if (!stringedThreeDBufferMaps[threeDMapName].doesThreeDBindingExist(threeDMapKeyValue))
					{
						ThreeDKeyedMBufferBinding newThreeDBufferBinding(in_bindingToInsert);
						stringedThreeDBufferMaps[threeDMapName].insertThreeDBinding(threeDMapKeyValue, newThreeDBufferBinding);
						wasSuccessful = 1;
						attemptMetadata.insertString("MAPIObjectManager created new 3d keyed buffer binding, (\"" + threeDMapName + "\", " + threeDMapKeyValue.getKeyString() 
							                        + "), bound with ID: " + std::to_string(stringedThreeDBufferMaps[threeDMapName].fetchThreeDBindingId(threeDMapKeyValue)));
					}
					//auto threeDMapNameFinder = stringedThreeDBufferMaps.find(threeDMapNameToCheck);
					//if (threeDMapNameFinder)
					else
					{
						attemptMetadata.insertString("MAPIObjectManager creation of new 3d keyed buffer binding, (\"" + threeDMapName + "\", " + threeDMapKeyValue.getKeyString() + "), failed (already exists) ");
					}

					break;
				}

				case MAPIObjectType::TEXTURE:
				{
					break;
				}

				case MAPIObjectType::FBO:
				{
					break;
				}
			}
			break;
		}
	}

	attemptMetadata.insertInt(wasSuccessful);
	return attemptMetadata;
}

int MAPIObjectManager::fetchBinding(MAPIObjectMetadata in_objectMeta)
{
	int returnBinding = -1;	// if this is still -1 when we return, we're in trouble (the resource wasn't found)

	switch (in_objectMeta.fetchMdMapKeyType())
	{
		// For typical string-based searches
		case MAPIObjectMapKeyType::STRING_KEYTYPE:
		{

			switch (in_objectMeta.fetchMdObjectType())
			{
				case MAPIObjectType::BUFFER:
				{
					auto bindingNameFinder = bufferResourceMap.find(in_objectMeta.fetchMdName());
					if (bindingNameFinder != bufferResourceMap.end())	// it was found
					{
						//returnBinding = bufferResourceMap[in_objectMeta.fetchMdName()].getBufferId();
						returnBinding = bufferResourceMap[in_objectMeta.fetchMdName()].getBufferId();
					}
					break;
				}

				case MAPIObjectType::TEXTURE:
				{
					auto bindingNameFinder = textureResourceMap.find(in_objectMeta.fetchMdName());
					if (bindingNameFinder != textureResourceMap.end())	// it was found
					{
						returnBinding = textureResourceMap[in_objectMeta.fetchMdName()].getTextureId();
					}
					break;
				}

				case MAPIObjectType::FBO:
				{
					auto bindingNameFinder = fboResourceMap.find(in_objectMeta.fetchMdName());
					if (bindingNameFinder != fboResourceMap.end())	// it was found
					{
						returnBinding = fboResourceMap[in_objectMeta.fetchMdName()].getFBOId();
					}
					break;
				}
			}
			break;
		}

		// For EnclaveKey-based searches
		case MAPIObjectMapKeyType::ENCLAVE_KEYTYPE:
		{
			switch (in_objectMeta.fetchMdObjectType())
			{
				case MAPIObjectType::BUFFER:
				{
					auto threeDMapNameFinder = stringedThreeDBufferMaps.find(in_objectMeta.fetchMdKeyMapName());
					if (threeDMapNameFinder != stringedThreeDBufferMaps.end()) // it was found, now check for the key
					{
						if (threeDMapNameFinder->second.doesThreeDBindingExist(in_objectMeta.fetchMdKeyValue())) // it was found!
						{
							returnBinding = threeDMapNameFinder->second.fetchThreeDBindingId(in_objectMeta.fetchMdKeyValue());
						}
					}
					break;
				}
			}
			break;
		}
	}
	return returnBinding;
}

void MAPIObjectManager::cleanupResources()
{
	// cleanup stringed buffer bindings
	for (auto& currentBufferResource : bufferResourceMap)
	{
		currentBufferResource.second.deleteBufferResource();
	}

	// cleanup stringed texture bindings
	for (auto& currentTextureResource : textureResourceMap)
	{
		currentTextureResource.second.deleteTextureResource();
	}

	// cleanup stringed FBO bindings
	for (auto& currentFBOResource : fboResourceMap)
	{
		currentFBOResource.second.deleteFBOResource();
	}

	// cleanup keyed buffer bindings
	for (auto& currentStringedKeyedMap : stringedThreeDBufferMaps)
	{
		for (auto& currentKeyBinding : currentStringedKeyedMap.second.threeDBindings)
		{
			currentKeyBinding.second.deleteThreeDBufferResource();
		}
	}
}