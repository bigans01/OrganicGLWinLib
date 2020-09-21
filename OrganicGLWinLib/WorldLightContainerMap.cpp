#include "stdafx.h"
#include "WorldLightContainerMap.h"

void WorldLightContainerMap::insertLightIntoStringedContainer(std::string in_stringContainerName, int in_lightID, WorldLight in_worldLight)
{
	lightMap[in_stringContainerName].insertLight(in_lightID, in_worldLight);
}