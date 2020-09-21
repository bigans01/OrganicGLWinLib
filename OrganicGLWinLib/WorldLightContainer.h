#pragma once

#ifndef WORLDLIGHTCONTAINER_H
#define WORLDLIGHTCONTAINER_H

#include <map>
#include "WorldLight.h"

class WorldLightContainer
{
	public:
		std::map<int, WorldLight> worldLights;
		void insertLight(int in_lightID, WorldLight in_worldLight)
		{
			worldLights[in_lightID] = in_worldLight;
		}
		void updateLightPosition(int in_lightID, glm::vec3 in_position)
		{
			worldLights[in_lightID].updatePosition(in_position);
		}
};

#endif
