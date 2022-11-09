#pragma once

#ifndef TWODINTERSECTIONRECORDMANAGER_H
#define TWODINTERSECTIONRECORDMANAGER_H


class TwoDIntersectionRecordManager
{
	public:
		void insertRecord(int in_idOfTheIntersectingLine, glm::vec3 in_intersectingPoint);
		void eraseRecord(int in_idToErase);
		void clearRecordMap();
		glm::vec3 getPointForRecord(int in_idToGetPointFor);
		bool doesRecordExist(int in_idToGetPointFor);
		void printRecordMap();
		int numberOfRecords();
		int getIdOfClosestPoint(glm::vec3 in_pointToCompareAgainst, OperableIntSet in_linesToExclude);
		int getIdOfClosestPoint(glm::vec3 in_pointToCompareAgainst);
		bool doesPointExist(glm::vec3 in_point);
		bool arePointsEqual();	// would be used to invalidate a SLICE attempt
	private:
		friend class CuttableTriangle;
		friend class CuttingTriangle;
		std::map<int, glm::vec3> recordMap;	// the key of the map = the ID of the line in the triangle that performed the intersection; value = intersecting point.

};

#endif

