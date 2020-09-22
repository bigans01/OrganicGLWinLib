#include "stdafx.h"
#include "WorldLightContainerMap.h"

void WorldLightContainerMap::insertLightIntoStringedContainer(std::string in_stringContainerName, int in_lightID, WorldLight in_worldLight)
{
	lightMap[in_stringContainerName].insertLight(in_lightID, in_worldLight);
}

void WorldLightContainerMap::generateLightDataFromAllContainers()
{
	// cycle through each container, to get the total number of floats required per container.
	int totalFloatCount = 0;	// the total number of floats that would need to go into the smart pointer.
	auto lightMapBegin = lightMap.begin();
	auto lightMapEnd = lightMap.end();
	for (; lightMapBegin != lightMapEnd; lightMapBegin++)	
	{
		totalFloatCount += (lightMapBegin->second.getCombinedWorldLightBytes()) / 4;	// number of floats is equivalent to the byte size / 4.
	}

	// re-create the smart pointer array with the new totalFloatCount
	lightDataSize = totalFloatCount;
	lightData.reset(new float[lightDataSize]);

	// re-assign iterators; load individual light data from each WorldLightContainer
	int currentFloatIndex = 0;
	lightMapBegin = lightMap.begin();
	lightMapEnd = lightMap.end();
	for (; lightMapBegin != lightMapEnd; lightMapBegin++)
	{
		//totalFloatCount += (lightMapBegin->second.getCombinedWorldLightBytes()) / 4;	// number of floats is equivalent to the byte size / 4.
		auto currentLightsBegin = lightMapBegin->second.worldLights.begin();
		auto currentLightsEnd = lightMapBegin->second.worldLights.end();
		for (; currentLightsBegin != currentLightsEnd; currentLightsBegin++)
		{
			lightData[currentFloatIndex++] = currentLightsBegin->second.position.x;
			lightData[currentFloatIndex++] = currentLightsBegin->second.position.y;
			lightData[currentFloatIndex++] = currentLightsBegin->second.position.z;
			lightData[currentFloatIndex++] = currentLightsBegin->second.color.x;
			lightData[currentFloatIndex++] = currentLightsBegin->second.color.y;
			lightData[currentFloatIndex++] = currentLightsBegin->second.color.z;
			lightData[currentFloatIndex++] = currentLightsBegin->second.linear;
			lightData[currentFloatIndex++] = currentLightsBegin->second.quadratic;
			lightData[currentFloatIndex++] = currentLightsBegin->second.radius;
		}
	}
}