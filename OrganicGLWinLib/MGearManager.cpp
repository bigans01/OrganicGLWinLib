#include "stdafx.h"
#include "MGearManager.h"

void MGearManager::initializeMGearManager(MAPIObjectManager* in_msbObjectManagerRef,
	GLUniformRegistry* in_msbUniformRegistrRef)
{
	msbObjectManagerRef = in_msbObjectManagerRef;
	msbUniformRegistryRef = in_msbUniformRegistrRef;
}

bool MGearManager::insertMGear(std::string in_mGearName, std::unique_ptr<MGearBase> in_newMGear)
{
	bool wasMGearInserted = false;
	auto mGearFinder = mGearMap.find(in_mGearName);
	if (mGearFinder == mGearMap.end())
	{
		mGearMap[in_mGearName] = std::move(in_newMGear);
		mGearMap[in_mGearName].get()->setMGearPointers(msbObjectManagerRef, msbUniformRegistryRef);
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
	for (auto& currentMGear : mGearMap)
	{
		currentMGear.second->render();
	}
}