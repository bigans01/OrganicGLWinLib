#include "stdafx.h"
#include "CategorizedLineInterceptsPointPreciseMerger.h"
#include "CleaveSequenceFactory.h"

void CategorizedLineInterceptsPointPreciseMerger::runMerging()
{
	std::cout << "Running merge for INTERCEPTS_POINT_PRECISE. " << std::endl;

	// get the first INTERCEPTS_POINT_PRECISE line.
	auto firstInterceptsPointPreciseIterator = mergableInterceptsPointPreciseMap.begin();
	int firstInterceptsPointPreciseIndex = firstInterceptsPointPreciseIterator->first;
	CategorizedLine firstInterceptsPointPrecise = fetchAndRemoveInterceptsPointPreciseMergeCandidate(firstInterceptsPointPreciseIndex);

	glm::vec3 pointToSearch = firstInterceptsPointPrecise.line.pointB;

	// fetch each NON_BOUND line (if any exist); the value of mostRecentNonBound is equal to whatever nonbound was fetched in the current tick of the while loop.
	CategorizedLine mostRecentNonBound;
	while (mergableNonboundCount > 0)
	{
		std::cout << "!! Searching for point: " << pointToSearch.x << ", " << pointToSearch.y << ", " << pointToSearch.z << std::endl;
		CategorizedLineSearchResult result = checkForNextNonboundMergeCandidate(pointToSearch);
		if (result.wasFound == true)
		{
			std::cout << "!! Point A of the found non-bound is: " << result.returnLine.line.pointA.x << ", " << result.returnLine.line.pointA.y << ", " << result.returnLine.line.pointA.z << std::endl;
			std::cout << "!! Point B of the found non-bound is: " << result.returnLine.line.pointB.x << ", " << result.returnLine.line.pointB.y << ", " << result.returnLine.line.pointB.z << std::endl;
			pointToSearch = result.returnLine.line.pointB;
		}

		int stopValSpecial = 3;
		std::cin >> stopValSpecial;
	}

	// set up the mergedLineResult; it is literally the same as the only INTERCEPTS_POINT_PRECISE, only except point B of the new line should be the latest value of the point to search.
	mergedLineResult = firstInterceptsPointPrecise;
	mergedLineResult.line.pointB = pointToSearch;

	int preciseStop;
	std::cin >> preciseStop;
}