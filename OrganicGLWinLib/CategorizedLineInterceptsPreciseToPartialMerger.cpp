#include "stdafx.h"
#include "CategorizedLineInterceptsPreciseToPartialMerger.h"
#include "CleaveSequenceFactory.h"

void CategorizedLineInterceptsPreciseToPartialMerger::runMerging()
{
	std::cout << "(CategorizedLineInterceptsPreciseToPartialMerger) Running merge for A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE. " << std::endl;
	mergeMachineLogger.log("(CategorizedLineInterceptsPreciseToPartialMerger) Running merge for A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE. ", "\n");

	// get the first INTERCEPTS_POINT_PRECISE line.
	auto firstASliceSingleInterceptsPointPreciseIterator = mergableInterceptsPointPreciseMap.begin();
	int firstASliceSingleInterceptsPointPreciseIndex = firstASliceSingleInterceptsPointPreciseIterator->first;
	std::cout << "firstASliceSingleInterceptsPointPreciseIndex value: " << firstASliceSingleInterceptsPointPreciseIndex << std::endl;

	CategorizedLine firstASliceSingleInterceptsPointPrecise = fetchAndRemoveInterceptsPointPreciseMergeCandidate(firstASliceSingleInterceptsPointPreciseIndex);

	glm::vec3 pointToSearch = firstASliceSingleInterceptsPointPrecise.line.pointB;

	// fetch each NON_BOUND line (if any exist); the value of mostRecentNonBound is equal to whatever nonbound was fetched in the current tick of the while loop.
	CategorizedLine mostRecentNonBound;
	while (mergableNonboundCount > 0)
	{
		//std::cout << "!! Searching for point: " << pointToSearch.x << ", " << pointToSearch.y << ", " << pointToSearch.z << std::endl;
		mergeMachineLogger.log("(CategorizedLineInterceptsPreciseToPartialMerger) !! Searching for point: ", pointToSearch.x, ", ", pointToSearch.y, ", ", pointToSearch.z, "\n");

		CategorizedLineSearchResult result = checkForNextNonboundMergeCandidate(pointToSearch);
		if (result.wasFound == true)
		{
			//std::cout << "!! Point A of the found non-bound is: " << result.returnLine.line.pointA.x << ", " << result.returnLine.line.pointA.y << ", " << result.returnLine.line.pointA.z << std::endl;
			//std::cout << "!! Point B of the found non-bound is: " << result.returnLine.line.pointB.x << ", " << result.returnLine.line.pointB.y << ", " << result.returnLine.line.pointB.z << std::endl;
			mergeMachineLogger.log("(CategorizedLineInterceptsPreciseToPartialMerger) !! Point A of the found non-bound is: ", result.returnLine.line.pointA.x, ", ", result.returnLine.line.pointA.y, ", ", result.returnLine.line.pointA.z, "\n");
			mergeMachineLogger.log("(CategorizedLineInterceptsPreciseToPartialMerger) !! Point B of the found non-bound is: ", result.returnLine.line.pointB.x, ", ", result.returnLine.line.pointB.y, ", ", result.returnLine.line.pointB.z, "\n");

			pointToSearch = result.returnLine.line.pointB;
		}
		else if (result.wasFound == false)
		{
			// Organic_Logic_Guidelines code: MERGE-0

			// in the exceptionally rare event that there is a non-bound point that is "slightly off" (discovered on 3/24/2021), we need a way to handle it;
			// all CategorizedLines produced by a single SPoly MUST have linking points; if they do not, we need to force link them.
			// See error showcase, CategorizedLinePartialBoundMerger-001.
			mergeMachineLogger.log("(CategorizedLineInterceptsPreciseToPartialMerger): No matching point found from mergable pool, performing special logic.", "\n");
			result = getClosestNonBoundMergeCandidate(pointToSearch);
			mergeMachineLogger.log("(CategorizedLineInterceptsPreciseToPartialMerger): Closest line, point A: ", result.returnLine.line.pointA.x, ", ", result.returnLine.line.pointA.y, ", ", result.returnLine.line.pointA.z,
				" | point B ", result.returnLine.line.pointB.x, ", ", result.returnLine.line.pointB.y, ", ", result.returnLine.line.pointB.z, "\n");
			pointToSearch = result.returnLine.line.pointB;
		}
		//int stopValSpecial = 3;
		//std::cin >> stopValSpecial;
		mergeMachineLogger.log("(CategorizedLineInterceptsPreciseToPartialMerger) While loop complete; enter number to continue.", "\n");
		mergeMachineLogger.waitForDebugInput();
	}

	// get the partial bound line that remains
	auto targetLineIterator = mergablePartialBoundsMap.begin();
	CategorizedLine fetchedPartialBound = fetchAndRemovePartialBoundMergeCandidate(targetLineIterator->first);
	pointToSearch = fetchedPartialBound.line.pointA;

	// set up the mergedLineResult; it is literally the same as the only INTERCEPTS_POINT_PRECISE, only except point B of the new line should be the latest value of the point to search.
	mergedLineResult = firstASliceSingleInterceptsPointPrecise;
	mergedLineResult.type = IntersectionType::A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE;
	mergedLineResult.line.isPointAOnBorder = 1;
	mergedLineResult.line.pointB = pointToSearch;
	mergedLineResult.line.isPointBOnBorder = 1;
	mergedLineResult.line.pointBBorder = fetchedPartialBound.line.pointABorder;
	mergedLineResult.line.numberOfBorderLines = 2;

	//std::cout << "pointA: " << mergedLineResult.line.pointA.x << ", " << mergedLineResult.line.pointA.y << ", " << mergedLineResult.line.pointA.z << std::endl;
	//std::cout << "pointB: " << mergedLineResult.line.pointB.x << ", " << mergedLineResult.line.pointB.y << ", " << mergedLineResult.line.pointB.z << std::endl;


	//std::cout << "+++++++++++++++++++++++++++++++++++++++++++ test stop...." << std::endl;

	//int preciseStop;
	//std::cin >> preciseStop;
}