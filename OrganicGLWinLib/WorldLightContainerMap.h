#pragma once

#ifndef WORLDLIGHTCONTAINERMAP_H
#define WORLDLIGHTCONTAINERMAP_H

#include <unordered_map>
#include <mutex>
#include "WorldLightContainer.h"


class WorldLightContainerMap
{
	public:
		WorldLightContainerMap() {};
		WorldLightContainerMap& operator=(const WorldLightContainerMap& containerMap_b)
		{
			lightDataSize = containerMap_b.lightDataSize;			// copy light data
			if (lightDataSize > 0)
			{
				lightData.reset(new float[lightDataSize]);
				for (int x = 0; x < lightDataSize; x++)
				{
					lightData[x] = containerMap_b.lightData[x];

				}
			}

			lightMap = containerMap_b.lightMap;						// copy the world light map
			return *this;
		}
		float lightDataSize = 0;			// the size of the lightData array (see below)
		std::unique_ptr<float[]> lightData;	// the smart pointer holding light data.
		std::unordered_map<std::string, WorldLightContainer> lightMap;
		void insertLightIntoStringedContainer(std::string in_stringContainerName, int in_lightID, WorldLight in_worldLight);
		void generateLightDataFromAllContainers();	// uses all light containers to generate the data for lightData.
};

#endif
