#include "stdafx.h"
#include "PosXFaceResolver.h"

void PosXFaceResolver::setupBorderLineRangesAndDimLoc()
{
	std::cout << "(PosXFaceResolver): building one dimensional lines..." << std::endl;
	xLocation = dimensionalLimit;	// since we're at POS_X, this value would be 1, 4 or 32; for NEG_X, this would still be at 0.

	// There will be exactly two "Y" and two "Z" instances of OneDimLine, for POS_X.
	// Scan the border lines to determine the appropriate OneDimLine
	auto borderLinesBegin = sPolyPtr->borderLines.begin();
	auto borderLinesEnd = sPolyPtr->borderLines.end();
	for (; borderLinesBegin != borderLinesEnd; borderLinesBegin++)
	{
		int borderLineID = borderLinesBegin->first;

		// check for building ZDimLine at NEG_Y
		if
		(
			(borderLinesBegin->second.pointA.y == 0)
			&&
			(borderLinesBegin->second.pointB.y == 0)
		)
		{
			std::cout << "(PosXFaceResolver): found a ZDimLine, at NEG_Y" << std::endl;
			singleDimLines[borderLineID].reset(new ZDimLine);
			singleDimLines[borderLineID]->setOneDimLimit(dimensionalLimit);
			singleDimLines[borderLineID]->setStaticDims(xLocation, 0.0f);	// X would be (1, 4, or 32), and Y would be 0 (from negative Y), since we
																					// are at the bottom of the limit.
		}

		// check for building ZDimLine at POS_Y
		else if
		(
			(borderLinesBegin->second.pointA.y == dimensionalLimit)
			&&
			(borderLinesBegin->second.pointB.y == dimensionalLimit)
		)
		{
			std::cout << "(PosXFaceResolver): found a ZDimLine, at POS_Y " << std::endl;
			singleDimLines[borderLineID].reset(new ZDimLine);
			singleDimLines[borderLineID]->setOneDimLimit(dimensionalLimit);
			singleDimLines[borderLineID]->setStaticDims(xLocation, dimensionalLimit);	// X and Y would both be 1, 4 or 32.
		}

		// check for building YDimLine at NEG_Z
		else if
		(
			(borderLinesBegin->second.pointA.z == 0)
			&&
			(borderLinesBegin->second.pointB.z == 0)
		)
		{
			std::cout << "(PosXFaceResolver): found a YDimLine, at NEG_Z" << std::endl;
			singleDimLines[borderLineID].reset(new YDimLine);
			singleDimLines[borderLineID]->setOneDimLimit(dimensionalLimit);
			singleDimLines[borderLineID]->setStaticDims(xLocation, 0.0f);	// X would be (1, 4, or 32), and Z would be 0, since we
																			// are at the bottom of the limit
		}

		// check for building YDimLine at POS_Z
		else if
		(
			(borderLinesBegin->second.pointA.z == dimensionalLimit)
			&&
			(borderLinesBegin->second.pointB.z == dimensionalLimit)
		)
		{
			std::cout << "(PosXFaceResolver): found a YDimLine, at POS_Z" << std::endl;
			singleDimLines[borderLineID].reset(new YDimLine);
			singleDimLines[borderLineID]->setOneDimLimit(dimensionalLimit);
			singleDimLines[borderLineID]->setStaticDims(xLocation, dimensionalLimit);	// X would be whatever the xLocation is, and Z would be the limit.
		}
	}
}

void PosXFaceResolver::runResolutionAlgorithm()
{
	// logic for determining resolution would go here; return a malformed version just for testing.
	bool wasResolutionFound = attemptSolveByInvalidCount();
	if (wasResolutionFound == false)
	{
		produceMalformedMitigation();
	}
}

bool PosXFaceResolver::attemptSolveByInvalidCount()
{
	bool wasResolved = false;
	int numberOfInvalids = int(invalidsCopy.sequenceMap.size());
	if (numberOfInvalids == 1)
	{
		std::cout << "(PosXFaceResolver): found 1 invalid CleaveSequence. " << std::endl;
		auto invalidSequenceID = invalidsCopy.sequenceMap.begin()->first;

		// be sure to determine the CyclingDirections for the sequence.
		invalidsCopy.sequenceMap[invalidSequenceID].printCategorizedLines();

		bool resolved = checkCleaveSequenceLinesAgainstDimLines(invalidSequenceID, &(invalidsCopy.sequenceMap[invalidSequenceID]));
		if (resolved == true)
		{
			SPolyMorphTracker morphTracker;
			SPolyFracturer fracturer(0, 
									sPolyPtr, 
									&morphTracker, 
									SPolyFracturerOptionEnum::ROTATE_TO_Z, 
									PolyDebugLevel::NONE,
									resolverRecorderRef, 
									resolverBoundaryOrientation);
			std::cout << "(PosXFaceResolver): Fracturing successful." << std::endl;
			wasResolved = true;

			SPolySupergroup resolvedGroup = fracturer.sPolySG;
			//resolvedGroup.printSPolys();

			// set the resolution to be the result of the fracturer, as long as that was valid
			if (fracturer.getFractureValidity())
			{
				std::cout << "(PosXFaceResolver): fracturing was valid, setting this as the resolution." << std::endl;
				resolution = std::move(fracturer.sPolySG);

				// remember, we must manually set the empty normal, orientations, and scab parent flag --
				// as it is the responsibility of the Resolver class to do this.
				resolution.setEmptyNormalInAllSPolys(resolverEmptyNormal);
				resolution.setBoundaryOrientationInAllSPolys(resolverBoundaryOrientation);
				resolution.setScabParentFlagInAllSPolys();
			}
		}
	}
	else if (numberOfInvalids == 2)
	{
		std::cout << "(PosXFaceResolver): found 2 invalid CleaveSequences. " << std::endl;

	}

	return wasResolved;
}

void PosXFaceResolver::produceMalformedMitigation()
{
	// This below code block is just a test; until this logic is fleshed out.
	glm::vec3 point0 = sPolyPtr->borderLines[0].pointA;
	glm::vec3 point1 = sPolyPtr->borderLines[1].pointA;
	glm::vec3 point2 = sPolyPtr->borderLines[2].pointA;
	glm::vec3 point3 = sPolyPtr->borderLines[3].pointA;
	
	STriangle triangleA(point0, point1, point2);
	STriangle triangleB(point0, point2, point3);

	SPoly malformedMitigationPoly;
	malformedMitigationPoly.addTriangle(triangleA);
	malformedMitigationPoly.addTriangle(triangleB);
	malformedMitigationPoly.setMode(SPolyMode::MALFORMED_MITIGATION);	// when we can't solve and produce the "last resort" result,
																		// we must flag the SPoly as MALFORMED_MITIGATION, so that it 
																		// isn't counted in the call to MassZoneBox::generateTouchedBoxFacesList
	resolution.insertSPoly(malformedMitigationPoly);

	// remember, we must manually set the empty normal, orientations, and scab parent flag --
	// as it is the responsibility of the Resolver class to do this.
	resolution.setEmptyNormalInAllSPolys(resolverEmptyNormal);
	resolution.setBoundaryOrientationInAllSPolys(resolverBoundaryOrientation);
	resolution.setScabParentFlagInAllSPolys();
}