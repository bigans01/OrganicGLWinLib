#include "stdafx.h"
#include "TwoDIntersectionRecordManager.h"

void TwoDIntersectionRecordManager::insertRecord(int in_idOfTheIntersectingLine, glm::vec3 in_intersectingPoint)
{
	recordMap[in_idOfTheIntersectingLine] = in_intersectingPoint;
}

void TwoDIntersectionRecordManager::eraseRecord(int in_idToErase)
{
	recordMap.erase(in_idToErase);
}

void TwoDIntersectionRecordManager::clearRecordMap()
{
	recordMap.clear();
}

glm::vec3 TwoDIntersectionRecordManager::getPointForRecord(int in_idToGetPointFor)
{
	auto doesExistWithinMap = recordMap.find(in_idToGetPointFor);
	if (doesExistWithinMap == recordMap.end())
	{
		std::cout << "(TwoDIntersectionRecordManager): WARNING, no record found for ID. " << std::endl;
	}
	return recordMap[in_idToGetPointFor];
}

bool TwoDIntersectionRecordManager::doesRecordExist(int in_idToGetPointFor)
{
	bool returnValue = false;
	/*
	auto recordFinder = recordMap.find(in_idToGetPointFor);
	if (recordFinder != recordMap.end())
	{
		returnValue = true;
	}
	*/

	if (auto finder = recordMap.find(in_idToGetPointFor); finder != recordMap.end())	// new in C++ 17. Should be retroactively applied, to other similiar
																						// functions, when time permits.
	{
		returnValue = true;
	}
	return returnValue;
}

void TwoDIntersectionRecordManager::printRecordMap()
{
	auto recordMapBegin = recordMap.begin();
	auto recordMapEnd = recordMap.end();
	for (; recordMapBegin != recordMapEnd; recordMapBegin++)
	{
		std::cout << "[" << recordMapBegin->first << "]"
			<< " point: " << recordMapBegin->second.x << ", " << recordMapBegin->second.y << ", " << recordMapBegin->second.z << std::endl;
	}
}

int TwoDIntersectionRecordManager::numberOfRecords()
{
	return int(recordMap.size());
};

int TwoDIntersectionRecordManager::getIdOfClosestPoint(glm::vec3 in_pointToCompareAgainst, OperableIntSet in_linesToExclude)
{
	float distance = 10000.0f;
	int shortestID = 0;
	auto recordMapBegin = recordMap.begin();
	auto recordMapEnd = recordMap.end();
	for (; recordMapBegin != recordMapEnd; recordMapBegin++)
	{
		auto isExcludableFound = in_linesToExclude.intSet.find(recordMapBegin->first);
		if (isExcludableFound == in_linesToExclude.intSet.end())	// it wasn't found; OK to analyze this.
		{
			if (glm::distance(in_pointToCompareAgainst, recordMapBegin->second) < distance)
			{
				shortestID = recordMapBegin->first;
				distance = glm::distance(in_pointToCompareAgainst, recordMapBegin->second);
			};
		}
	}
	return shortestID;
};

int TwoDIntersectionRecordManager::getIdOfClosestPoint(glm::vec3 in_pointToCompareAgainst)
{
	float distance = 10000.0f;
	int shortestID = 0;
	auto recordMapBegin = recordMap.begin();
	auto recordMapEnd = recordMap.end();
	for (; recordMapBegin != recordMapEnd; recordMapBegin++)
	{
		//auto isExcludableFound = in_linesToExclude.intSet.find(recordMapBegin->first);
		//if (isExcludableFound == in_linesToExclude.intSet.end())	// it wasn't found; OK to analyze this.
		//{
			if (glm::distance(in_pointToCompareAgainst, recordMapBegin->second) < distance)
			{
				shortestID = recordMapBegin->first;
				distance = glm::distance(in_pointToCompareAgainst, recordMapBegin->second);
			};
		//}
	}
	return shortestID;
};

bool TwoDIntersectionRecordManager::doesPointExist(glm::vec3 in_point)
{
	bool wasFound = false;
	auto recordMapBegin = recordMap.begin();
	auto recordMapEnd = recordMap.end();
	for (; recordMapBegin != recordMapEnd; recordMapBegin++)
	{
		if (recordMapBegin->second == in_point)
		{
			wasFound = true;
		}
	}
	return wasFound;
}

bool TwoDIntersectionRecordManager::arePointsEqual() // would be used to invalidate a SLICE attempt
{
	bool arePointsEqual = false;
	auto firstPoint = recordMap.begin();
	auto secondPoint = recordMap.rbegin();
	if (firstPoint->second == secondPoint->second)
	{
		arePointsEqual = true;
	}
	return arePointsEqual;
}
