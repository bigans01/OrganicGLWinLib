#include "stdafx.h"
#include "MGearManager.h"

void MGearManager::initializeMGearManager(MAPIObjectManager* in_msbObjectManagerRef,
	GLUniformRegistry* in_msbUniformRegistrRef)
{
	msbObjectManagerRef = in_msbObjectManagerRef;
	msbUniformRegistryRef = in_msbUniformRegistrRef;
}

void MGearManager::cleanupMGears()
{
	for (auto& currentMGear : mGearMap)
	{
		currentMGear.second->cleanupMGear();
	}
}

void MGearManager::createRequiredGearObjects()
{
	for (auto& currentMGear : mGearMap)
	{
		currentMGear.second->setObjectBindings();
	}
}

void MGearManager::sendScanSignalToGears(bool in_fullscanFlag, std::vector<MAPIObjectUpdate>* in_updateVectorRef)
{
	for (auto& currentMGear : mGearMap)
	{
		currentMGear.second->conductMAPIObjectScan(in_fullscanFlag, in_updateVectorRef);
	}
}

bool MGearManager::insertMGear(int in_gearSequenceLocation, std::string in_mGearName, std::unique_ptr<MGearBase> in_newMGear)
{
	bool wasMGearInserted = false;
	auto mGearFinder = mGearMap.find(in_mGearName);
	if (mGearFinder == mGearMap.end())
	{
		mGearMap[in_mGearName] = std::move(in_newMGear);
		mGearMap[in_mGearName].get()->setMGearPointers(msbObjectManagerRef, msbUniformRegistryRef);
		mGearMap[in_mGearName].get()->initializeMGear();

		gearSequence[in_gearSequenceLocation] = in_mGearName;
	}

	return wasMGearInserted;
}

bool MGearManager::isMGearInManager(std::string in_mGearName)
{
	bool doesGearExist = false;
	auto mGearFinder = mGearMap.find(in_mGearName);
	if (mGearFinder == mGearMap.end())
	{
		doesGearExist = true;
	}

	return doesGearExist;
}

MGearBase* MGearManager::getMGearRef(std::string in_mGearName)
{
	MGearBase* returnMGearPtr = nullptr;
	auto mGearFinder = mGearMap.find(in_mGearName);
	if (mGearFinder == mGearMap.end())
	{
		returnMGearPtr = mGearMap[in_mGearName].get();
	}

	return returnMGearPtr;
}

void MGearManager::renderMGears()
{
	for (auto& currentSequencedGear : gearSequence)
	{
		auto gearName = currentSequencedGear.second;
		mGearMap[gearName]->render();
	}
}

bool MGearManager::verifyRequiredGearObjects(std::queue<Message>* in_messageQueueRef)
{
	bool allObjectsFound = true;	// assume true; set to false the moment a single object isn't found.

	for (auto& currentMGear : mGearMap)
	{
		in_messageQueueRef->push(Message(MessageType::MSHADER_INFO, "Checking required objects for named gear, " + currentMGear.first));

		// First, check required MAPI objects.
		auto currentMGearObjectVector = currentMGear.second->fetchRequiredMAPIObjects();
		for (auto& currentRequiredObject : currentMGearObjectVector)
		{
			switch (currentRequiredObject.fetchMdMapKeyType())
			{
				// For typical string-based MGear objects
				case MAPIObjectMapKeyType::STRING_KEYTYPE:
				{
					std::string mapiObjectNameString = "";
					switch (currentRequiredObject.fetchMdObjectType())
					{
						case MAPIObjectType::BUFFER: { mapiObjectNameString = "BUFFER"; break; }
						case MAPIObjectType::FBO: { mapiObjectNameString = "FBO"; break; }
						case MAPIObjectType::TEXTURE: { mapiObjectNameString = "TEXTURE"; break; }
						case MAPIObjectType::VAO: { mapiObjectNameString = "VAO"; break; }
					}

					std::string objectLoggingString = "Checking for " + mapiObjectNameString + " with the name " + currentRequiredObject.fetchMdName() + ": ";

					bool didBindingExist = msbObjectManagerRef->doesBindingExist(currentRequiredObject);
					if (didBindingExist)
					{
						objectLoggingString += "Success";
					}
					else
					{
						objectLoggingString += "Failure";
						allObjectsFound = false;
					}
					in_messageQueueRef->push(Message(MessageType::MSHADER_INFO, objectLoggingString));
				}
			}
		}

		// Next, check required uniforms
		auto currentMGearRequiredUniformVector = currentMGear.second->fetchRequiredGLUniforms();
		for (auto& currentRequiredUniform : currentMGearRequiredUniformVector)
		{
			switch (currentRequiredUniform.dataType)
			{
				case GLDataType::VEC2:
				{
					std::string uniformLoggingString = "Checking for vec2 uniform with name " + currentRequiredUniform.uniformName + ": ";

					if (msbUniformRegistryRef->doesVec2Exist(currentRequiredUniform.uniformName))
					{
						uniformLoggingString += "Success";
					}
					else
					{
						uniformLoggingString += "Failure";
						allObjectsFound = false;
					}
					in_messageQueueRef->push(Message(MessageType::MSHADER_INFO, uniformLoggingString));
					break;
				}

				case GLDataType::VEC3:
				{
					std::string uniformLoggingString = "Checking for vec3 uniform with name " + currentRequiredUniform.uniformName + ": ";

					if (msbUniformRegistryRef->doesVec3Exist(currentRequiredUniform.uniformName))
					{
						uniformLoggingString += "Success";
					}
					else
					{
						uniformLoggingString += "Failure";
						allObjectsFound = false;
					}
					in_messageQueueRef->push(Message(MessageType::MSHADER_INFO, uniformLoggingString));
					break;
				}

				case GLDataType::MAT3:
				{
					std::string uniformLoggingString = "Checking for mat3 uniform with name " + currentRequiredUniform.uniformName + ": ";

					if (msbUniformRegistryRef->doesMat3Exist(currentRequiredUniform.uniformName))
					{
						uniformLoggingString += "Success";
					}
					else
					{
						uniformLoggingString += "Failure";
						allObjectsFound = false;
					}
					in_messageQueueRef->push(Message(MessageType::MSHADER_INFO, uniformLoggingString));
					break;
				}

				case GLDataType::MAT4:
				{
					std::string uniformLoggingString = "Checking for mat4 uniform with name " + currentRequiredUniform.uniformName + ": ";

					if (msbUniformRegistryRef->doesMat4Exist(currentRequiredUniform.uniformName))
					{
						uniformLoggingString += "Success";
					}
					else
					{
						uniformLoggingString += "Failure";
						allObjectsFound = false;
					}
					in_messageQueueRef->push(Message(MessageType::MSHADER_INFO, uniformLoggingString));
					break;
				}

				case GLDataType::FLOAT:
				{
					std::string uniformLoggingString = "Checking for float uniform with name " + currentRequiredUniform.uniformName + ": ";

					if (msbUniformRegistryRef->doesFloatExist(currentRequiredUniform.uniformName))
					{
						uniformLoggingString += "Success";
					}
					else
					{
						uniformLoggingString += "Failure";
						allObjectsFound = false;
					}
					in_messageQueueRef->push(Message(MessageType::MSHADER_INFO, uniformLoggingString));
					break;
				}

				case GLDataType::INT:
				{
					std::string uniformLoggingString = "Checking for int uniform with name " + currentRequiredUniform.uniformName + ": ";

					if (msbUniformRegistryRef->doesIntExist(currentRequiredUniform.uniformName))
					{
						uniformLoggingString += "Success";
					}
					else
					{
						uniformLoggingString += "Failure";
						allObjectsFound = false;
					}
					in_messageQueueRef->push(Message(MessageType::MSHADER_INFO, uniformLoggingString));
					break;
				}

				case GLDataType::KEY3D:
				{
					std::string uniformLoggingString = "Checking for EnclaveKeyDef::EnclaveKey uniform with name " + currentRequiredUniform.uniformName + ": ";

					if (msbUniformRegistryRef->does3DKeyExist(currentRequiredUniform.uniformName))
					{
						uniformLoggingString += "Success";
					}
					else
					{
						uniformLoggingString += "Failure";
						allObjectsFound = false;
					}
					in_messageQueueRef->push(Message(MessageType::MSHADER_INFO, uniformLoggingString));
					break;
				}
			}
		}
	}
	return allObjectsFound;
}