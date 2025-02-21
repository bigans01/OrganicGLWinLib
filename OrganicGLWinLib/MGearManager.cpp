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

		auto currentMGearObjectVector = currentMGear.second->fetchRequiredObjects();
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
	}
	return allObjectsFound;
}