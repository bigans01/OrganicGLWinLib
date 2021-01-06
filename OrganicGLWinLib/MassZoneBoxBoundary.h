#pragma once

#ifndef MASSZONEBOXBOUNDARY_H
#define MASSZONEBOXBOUNDARY_H

#include <map>
#include "MassZoneBoxBoundaryState.h"
#include <glm/glm.hpp>
#include <iostream>

class MassZoneBoxBoundary
{
	public:
		MassZoneBoxBoundary() {};
		MassZoneBoxBoundary(glm::vec3 in_corner1, glm::vec3 in_corner2, glm::vec3 in_corner3, glm::vec3 in_corner4)
		{
			insertCornerPoint(in_corner1);
			insertCornerPoint(in_corner2);
			insertCornerPoint(in_corner3);
			insertCornerPoint(in_corner4);
		};
		MassZoneBoxBoundaryState boundaryCurrentState = MassZoneBoxBoundaryState::INACTIVE;	// the boundary always starts as inactive.
		std::map<int, glm::vec3> cornerPointMap;

		void printPoints()
		{
			auto cornerPointMapBegin = cornerPointMap.begin();
			auto cornerPointMapEnd = cornerPointMap.end();
			for (; cornerPointMapBegin != cornerPointMapEnd; cornerPointMapBegin++)
			{
				std::cout << "[" << cornerPointMapBegin->first << "]: " << cornerPointMapBegin->second.x << ", "
																<< cornerPointMapBegin->second.y << ", "
																<< cornerPointMapBegin->second.z << std::endl;
			}
		}

	
	private:
		void insertCornerPoint(glm::vec3 in_cornerPoint)
		{
			int cornerPointMapIndex = cornerPointMap.size();
			cornerPointMap[cornerPointMapIndex] = in_cornerPoint;
		}
};

#endif
