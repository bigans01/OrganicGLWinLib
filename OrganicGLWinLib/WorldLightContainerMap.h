#pragma once

#ifndef WORLDLIGHTCONTAINERMAP_H
#define WORLDLIGHTCONTAINERMAP_H

#include <unordered_map>
#include "WorldLightContainer.h"

class WorldLightContainerMap
{
	public:
		std::unordered_map<std::string, WorldLightContainer> lightMap;
		void insertLightIntoStringedContainer(std::string in_stringContainerName, int in_lightID, WorldLight in_worldLight);
};

#endif
