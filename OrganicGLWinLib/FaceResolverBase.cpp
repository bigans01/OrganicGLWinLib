#include "stdafx.h"
#include "FaceResolverBase.h"

void FaceResolverBase::initialize(SPoly* in_sPolyPtr,
	InvalidCleaveSequences in_invalids,
	MassZoneBoxType in_boxType,
	MassZoneBoxBoundaryOrientation in_faceOrientation)
{
	sPolyPtr = in_sPolyPtr;
	invalidsCopy = in_invalids;
	boxType = in_boxType;
	faceOrientation = in_faceOrientation;

	// the dimensional limit can be determined by simply looking at the box type
	switch (boxType)
	{
	case MassZoneBoxType::BLOCK:
	{
		dimensionalLimit = 1.0f; break;
	}
	case MassZoneBoxType::ENCLAVE:
	{
		dimensionalLimit = 4.0f; break;
	}
	case MassZoneBoxType::COLLECTION:
	{
		dimensionalLimit = 32.0f; break;
	}
	}
}

void FaceResolverBase::debugPrintOneDimLines()	// for debug: print the values of the one dim lines
{
	std::cout << "(FaceResolverBase): printing one dim lines and verifying corresponding border lines..." << std::endl;
	auto singleDimLinesBegin = singleDimLines.begin();
	auto singleDimLinesEnd = singleDimLines.end();
	for (; singleDimLinesBegin != singleDimLinesEnd; singleDimLinesBegin++)
	{
		std::cout << ">   Dim Line at [" << singleDimLinesBegin->first << "]" << std::endl;
		std::cout << "> Metadata: " << std::endl;
		singleDimLinesBegin->second->printMetaData();
		std::cout << "> Corresponding border line: " << std::endl;
		std::cout << "Point A: " << sPolyPtr->borderLines[singleDimLinesBegin->first].pointA.x << ", "
			<< sPolyPtr->borderLines[singleDimLinesBegin->first].pointA.y << ", "
			<< sPolyPtr->borderLines[singleDimLinesBegin->first].pointA.z
			<< "| Point B: " << sPolyPtr->borderLines[singleDimLinesBegin->first].pointB.x << ", "
			<< sPolyPtr->borderLines[singleDimLinesBegin->first].pointB.y << ", "
			<< sPolyPtr->borderLines[singleDimLinesBegin->first].pointB.z << std::endl;
	}
}

SPolySupergroup FaceResolverBase::fetchResolution()
{
	return resolution;
}

bool FaceResolverBase::compareCorrectionCandidatesAgainstSequence(int in_invalidCleaveSequenceID, 
																std::vector<CSCorrectionCandidate> in_candidates, 
																CleaveSequence* in_invalidPtr)
{
	bool wasRepairSuccessful = false;
	// We will need a map of std::vector<CSCorrectionCandidate> vectors (this can be refactored later)

	// Step 1: Load into the map, so we can see how many border line hits each CategorizeDLine ID has.
	std::map<int, std::vector<CSCorrectionCandidate>> organizedCandidates;
	auto candidateVectorBegin = in_candidates.begin();
	auto candidateVectorEnd = in_candidates.end();
	for (; candidateVectorBegin != candidateVectorEnd; candidateVectorBegin++)
	{
		organizedCandidates[candidateVectorBegin->lineID].push_back(*candidateVectorBegin);	// push back the current candidate into it's respective key;
																							// the key value must equal the index of the CategorizedLine as it
																							// is held within passed-in CleaveSequence (in_invalidPtr), which should be stored in the lineID value
																							// of CSCorrectionCandidate.
	}

	// Step 2: The size of each std::vector in the organizedCandidates reflects the number of points in a CategorizedLine 
	//		   technically fall on the border; in most cases, this should only be 1. We can cross-reference this count 
	//		   against the CategorizedLines
	auto organizedBegin = organizedCandidates.begin();
	auto organizedEnd = organizedCandidates.end();
	bool isCleaveSequenceNowValid = false;
	for (; organizedBegin != organizedEnd; organizedBegin++)
	{
		// Get the size of the vector of CSCorrectionCandidates for the current line
		std::cout << "Analyzing size of CSCorrectionCandiate vector for the CategorizedLine having ID " << organizedBegin->first <<  "..." << std::endl;
		int currentHitCount = organizedBegin->second.size();
		std::cout << "Hit count is: " << currentHitCount << std::endl;

		// 1 Hit, always a PARTIAL_BOUND.
		if (currentHitCount == 1)
		{
			auto onlyCorrectionCandidate = organizedBegin->second.begin();
			int targetCategorizedLineID = onlyCorrectionCandidate->lineID;
			std::cout << "Target line ID is: " << targetCategorizedLineID << std::endl;

			// check if the numberOfBorderLines in the target CategorizedLine matches the currentHitCount;
			// if it isn't we need to alter that CategorizedLine to be a PARTIAL_BOUND, as that is technically
			// the only ending bounding line type that this CategorizedLine could possibly be with 1 hit.
			if (currentHitCount != in_invalidPtr->cleavingLines[targetCategorizedLineID].line.numberOfBorderLines)
			{
				std::cout << "!! Notice, the number of points that should hit a border line (based on OneLineDim checks) in this CategorizedLine's IntersectionLine " << std::endl;
				std::cout << "   does not match the numberOfBorderLines; this line needs to be corrected." << std::endl;

				// get a point to the CategorizedLine to alter
				
				auto categorizedLinePtr = &in_invalidPtr->cleavingLines[targetCategorizedLineID];
				if (onlyCorrectionCandidate->matchedType == IRPointType::POINT_A)
				{
					categorizedLinePtr->line.pointABorder = onlyCorrectionCandidate->dimLineID;
					categorizedLinePtr->line.isPointAOnBorder = 1;
					categorizedLinePtr->line.numberOfBorderLines = 1;
					categorizedLinePtr->type = IntersectionType::PARTIAL_BOUND;
					categorizedLinePtr->determineCyclingDirection(sPolyPtr->borderLines, PolyDebugLevel::NONE);
					sPolyPtr->borderLines[onlyCorrectionCandidate->dimLineID].intersectRecorder.insertNewRecord(in_invalidCleaveSequenceID, targetCategorizedLineID, categorizedLinePtr);
					isCleaveSequenceNowValid = true;
				}
				else if (onlyCorrectionCandidate->matchedType == IRPointType::POINT_B)
				{
					// NOTE: we shouldn't have to call to swapToA(), as this would have been done during SPoly::organizeCurrentCleaveLines()
					std::cout << "(FaceResolverBase): Matched against IRPointType::POINT_B" << std::endl;
					std::cout << "(FaceResolverBase): Number of border lines in this line: " << categorizedLinePtr->line.numberOfBorderLines << std::endl;
					categorizedLinePtr->line.pointBBorder = onlyCorrectionCandidate->dimLineID;
					categorizedLinePtr->line.isPointBOnBorder = 1;
					categorizedLinePtr->line.numberOfBorderLines = 1;
					categorizedLinePtr->type = IntersectionType::PARTIAL_BOUND;
					categorizedLinePtr->determineCyclingDirection(sPolyPtr->borderLines, PolyDebugLevel::NONE);
					sPolyPtr->borderLines[onlyCorrectionCandidate->dimLineID].intersectRecorder.insertNewRecord(in_invalidCleaveSequenceID, targetCategorizedLineID, categorizedLinePtr);
					isCleaveSequenceNowValid = true;
				}
				std::cout << "!!! Reformed CleaveSequence stats are: " << std::endl;
				in_invalidPtr->printCategorizedLines();
			}
		}

		// 2 Hits: A_SLICE, INTERCEPTS_POINT_PRECISE, A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE (rarer, but could still happen;
		// not developed yet)
	}

	// Step 3: If the CleaveSequence was repaired (isCleaveSequenceNowValid has a true value), then move it into the SPoly's CleaveMap.
	if (isCleaveSequenceNowValid == true)
	{
		moveFixedCleaveSequenceIntoSPoly(in_invalidCleaveSequenceID);
		wasRepairSuccessful = true;
	}

	return wasRepairSuccessful;
}

void FaceResolverBase::moveFixedCleaveSequenceIntoSPoly(int in_invalidCleaveSequenceID)
{
	std::cout << "(FaceResolverBase): Moving the following repaired CleaveSequence into the SPoly: " << std::endl;
	invalidsCopy.sequenceMap[in_invalidCleaveSequenceID].printCategorizedLines();

	sPolyPtr->insertCleaveSequenceAtIndex(in_invalidCleaveSequenceID, invalidsCopy.sequenceMap[in_invalidCleaveSequenceID]);
}