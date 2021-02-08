#pragma once

#ifndef TWODINTERSECTIONRECORDMANAGER_H
#define TWODINTERSECTIONRECORDMANAGER_H

#include <map>
#include <glm/glm.hpp>

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
	private:
		std::map<int, glm::vec3> recordMap;	// the key of the map = the ID of the line in the triangle that performed the intersection; value = intersecting point.

};

#endif

