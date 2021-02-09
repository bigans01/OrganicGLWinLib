#pragma once

#ifndef TWODINTERSECTIONRECORDMANAGER_H
#define TWODINTERSECTIONRECORDMANAGER_H

#include <map>
#include <glm/glm.hpp>
#include <iostream>

class TwoDIntersectionRecordManager
{
	public:
		void insertRecord(int in_idOfTheIntersectingLine, glm::vec3 in_intersectingPoint)
		{
			recordMap[in_idOfTheIntersectingLine] = in_intersectingPoint;
		}
		void eraseRecord(int in_idToErase)
		{
			recordMap.erase(in_idToErase);
		}
		glm::vec3 getPointForRecord(int in_idToGetPointFor)
		{
			return recordMap[in_idToGetPointFor];
		}
		void printRecordMap()
		{
			auto recordMapBegin = recordMap.begin();
			auto recordMapEnd = recordMap.end();
			for (; recordMapBegin != recordMapEnd; recordMapBegin++)
			{
				std::cout << "[" << recordMapBegin->first << "]" 
					      << " point: " << recordMapBegin->second.x << ", " << recordMapBegin->second.y << ", " << recordMapBegin->second.z << std::endl;
			}
		}
		int numberOfRecords() 
		{ 
			return recordMap.size(); 
		};
	private:
		friend class CuttableTriangle;
		std::map<int, glm::vec3> recordMap;	// the key of the map = the ID of the line in the triangle that performed the intersection; value = intersecting point.

};

#endif

