#include "stdafx.h"
#include "CategorizedLineASliceMerger.h"
#include "CleaveSequenceFactory.h"

void CategorizedLineASliceMerger::runMerging()
{
//	std::cout << "(CategorizedLineASliceMerger) Running merge for A_SLICE. " << std::endl;
	mergeMachineLogger.log("(CategorizedLineASliceMerger) Running merge for A_SLICE. ", "\n");

	// get the first PARTIAL_BOUND line.
	auto firstPartialBoundIterator = mergablePartialBoundsMap.begin();
	int firstPartialBoundIndex = firstPartialBoundIterator->first;
	CategorizedLine firstPartialBound = fetchAndRemovePartialBoundMergeCandidate(firstPartialBoundIndex);

	// get the second PARTIAL_BOUND line.
	auto secondPartialBoundIterator = mergablePartialBoundsMap.begin();
	int secondPartialBoundIndex = secondPartialBoundIterator->first;
	CategorizedLine secondPartialBound = fetchAndRemovePartialBoundMergeCandidate(secondPartialBoundIndex);

	// test: print the point B, and point B borders of each categorized line.
	// remember: point A of a partial bound line is what is on the border.
	//std::cout << "First categorized line: " << std::endl;
	//std::cout << "Point A: " << firstPartialBound.line.pointA.x << ", " << firstPartialBound.line.pointA.y << ", " << firstPartialBound.line.pointA.z << std::endl;
	//std::cout << "Is Point A on border: " << firstPartialBound.line.isPointAOnBorder << std::endl;
	//std::cout << "Point A border value: " << firstPartialBound.line.pointABorder << std::endl;

	mergeMachineLogger.log("(CategorizedLineASliceMerger) ::: First categorized line: ", "\n");
	mergeMachineLogger.log("(CategorizedLineASliceMerger) Point A: ", firstPartialBound.line.pointA.x, ", ", firstPartialBound.line.pointA.y, ", ", firstPartialBound.line.pointA.z, "\n");
	mergeMachineLogger.log("(CategorizedLineASliceMerger) Is Point A on border: ", firstPartialBound.line.isPointAOnBorder, "\n");
	mergeMachineLogger.log("(CategorizedLineASliceMerger) Point A border value: ", firstPartialBound.line.pointABorder, "\n");


	//std::cout << "Second categorized line: " << std::endl;
	//std::cout << "Point A: " << secondPartialBound.line.pointA.x << ", " << secondPartialBound.line.pointA.y << ", " << secondPartialBound.line.pointA.z << std::endl;
	//std::cout << "Is point A on border: " << secondPartialBound.line.isPointAOnBorder << std::endl;
	//std::cout << "Point A border value: " << secondPartialBound.line.pointABorder << std::endl;

	mergeMachineLogger.log("(CategorizedLineASliceMerger) ::: Second categorized line: ", "\n");
	mergeMachineLogger.log("(CategorizedLineASliceMerger) Point A: ", secondPartialBound.line.pointA.x, ", ", secondPartialBound.line.pointA.y, ", ", secondPartialBound.line.pointA.z, "\n");
	mergeMachineLogger.log("(CategorizedLineASliceMerger) Is Point A on border: ", secondPartialBound.line.isPointAOnBorder, "\n");
	mergeMachineLogger.log("(CategorizedLineASliceMerger) Point A border value: ", secondPartialBound.line.pointABorder, "\n");

	// set up the mergedLineResult.
	mergedLineResult.type = IntersectionType::A_SLICE;

	mergedLineResult.line.pointA = firstPartialBound.line.pointA;
	mergedLineResult.line.isPointAOnBorder = firstPartialBound.line.isPointAOnBorder;
	mergedLineResult.line.pointABorder = firstPartialBound.line.pointABorder;

	mergedLineResult.line.pointB = secondPartialBound.line.pointA;
	mergedLineResult.line.isPointBOnBorder = secondPartialBound.line.isPointAOnBorder;
	mergedLineResult.line.pointBBorder = secondPartialBound.line.pointABorder;

	mergedLineResult.emptyNormal = firstPartialBound.emptyNormal;

	mergedLineResult.parentPoly = firstPartialBound.parentPoly;
	mergedLineResult.line.numberOfBorderLines = 2;


	//int mergeFlag = 3;
	//std::cin >> mergeFlag;
	mergeMachineLogger.waitForDebugInput();
}
