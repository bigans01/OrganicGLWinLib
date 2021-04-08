#include "stdafx.h"
#include "CategorizedLinePartialBoundMerger.h"
#include "CleaveSequenceFactory.h"

void CategorizedLinePartialBoundMerger::runMerging()
{
	//std::cout << "Running merge for PARTIAL_BOUND. " << std::endl;
	mergeMachineLogger.log("(CategorizedLinePartialBoundMerger) Running merge for PARTIAL_BOUND. ", "\n");
	//std::cout << "(CategorizedLinePartialBoundMerger) executing..." << std::endl;

	// get the first PARTIAL_BOUND line.
	auto firstPartialBoundIterator = mergablePartialBoundsMap.begin();
	int firstPartialBoundIndex = firstPartialBoundIterator->first;
	CategorizedLine firstPartialBound = fetchAndRemovePartialBoundMergeCandidate(firstPartialBoundIndex);

	glm::vec3 pointToSearch = firstPartialBound.line.pointB;

	// fetch each NON_BOUND line (if any exist); the value of mostRecentNonBound is equal to whatever nonbound was fetched in the current tick of the while loop.
	CategorizedLine mostRecentNonBound;
	while (mergableNonboundCount > 0)
	{
		//std::cout << "!! (CategorizedLinePartialBoundMerger) Searching for point: " << pointToSearch.x << ", " << pointToSearch.y << ", " << pointToSearch.z << std::endl;
		mergeMachineLogger.log("(CategorizedLinePartialBoundMerger) !! Searching for point: ", pointToSearch.x, ", ", pointToSearch.y, ", ", pointToSearch.z, "\n");
		CategorizedLineSearchResult result = checkForNextNonboundMergeCandidate(pointToSearch);
		if (result.wasFound == true)
		{
			//std::cout << "!! Point A of the found non-bound is: " << result.returnLine.line.pointA.x << ", " << result.returnLine.line.pointA.y << ", " << result.returnLine.line.pointA.z << std::endl;
			//std::cout << "!! Point B of the found non-bound is: " << result.returnLine.line.pointB.x << ", " << result.returnLine.line.pointB.y << ", " << result.returnLine.line.pointB.z << std::endl;
			mergeMachineLogger.log("(CategorizedLinePartialBoundMerger) !! Point A of the found non-bound is: ", result.returnLine.line.pointA.x, ", ", result.returnLine.line.pointA.y, ", ", result.returnLine.line.pointA.z, "\n");
			mergeMachineLogger.log("(CategorizedLinePartialBoundMerger) !! Point B of the found non-bound is: ", result.returnLine.line.pointB.x, ", ", result.returnLine.line.pointB.y, ", ", result.returnLine.line.pointB.z, "\n");
			pointToSearch = result.returnLine.line.pointB;
		}
		else if (result.wasFound == false)
		{
			// Organic_Logic_Guidelines code: MERGE-0

			// in the exceptionally rare event that there is a non-bound point that is "slightly off" (discovered on 3/24/2021), we need a way to handle it;
			// all CategorizedLines produced by a single SPoly MUST have linking points; if they do not, we need to force link them.
			// See error showcase, CategorizedLinePartialBoundMerger-001.
			mergeMachineLogger.log("(CategorizedLinePartialBoundMerger): No matching point found from mergable pool, performing special logic.", "\n");
			result = getClosestNonBoundMergeCandidate(pointToSearch);
			mergeMachineLogger.log("(CategorizedLinePartialBoundMerger): Closest line, point A: ", result.returnLine.line.pointA.x, ", ", result.returnLine.line.pointA.y, ", ", result.returnLine.line.pointA.z, 
																					" | point B ", result.returnLine.line.pointB.x, ", ", result.returnLine.line.pointB.y, ", ", result.returnLine.line.pointB.z, "\n");
			pointToSearch = result.returnLine.line.pointB;
		}
		mergeMachineLogger.waitForDebugInput();
	}

	// set up the mergedLineResult.
	mergedLineResult.type = IntersectionType::PARTIAL_BOUND;

	// remember, point A of the partial bound is what's on the border.
	mergedLineResult.line.pointA = firstPartialBound.line.pointA;
	mergedLineResult.line.isPointAOnBorder = firstPartialBound.line.isPointAOnBorder;
	mergedLineResult.line.pointABorder = firstPartialBound.line.pointABorder;

	mergedLineResult.line.pointB = pointToSearch;


	mergedLineResult.emptyNormal = firstPartialBound.emptyNormal;

	mergedLineResult.parentPoly = firstPartialBound.parentPoly;
	mergedLineResult.line.numberOfBorderLines = 1;


	//int mergeFlag = 0;
	//std::cin >> mergeFlag;
	mergeMachineLogger.waitForDebugInput();

}