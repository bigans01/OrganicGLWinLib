#include "stdafx.h"
#include "PosYFaceResolver.h"

void PosYFaceResolver::setupBorderLineRangesAndDimLoc()
{
	std::cout << "(PosYFaceResolver): building one dimensional lines..." << std::endl;
	yLocation = dimensionalLimit;	// since we're at POS_Y, this value would be 1, 4 or 32

	// There will be two "X" and two "Z" instances of OneDimLine, for POS_Y.
	// Scan the border lines to determine the appropriate OneDimLine
	auto borderLinesBegin = sPolyPtr->borderLines.begin();
	auto borderLinesEnd = sPolyPtr->borderLines.end();
	for (; borderLinesBegin != borderLinesEnd; borderLinesBegin++)
	{
		int borderLineID = borderLinesBegin->first;

		// check for building ZDimLine at NEG_X
		if
		(
			(borderLinesBegin->second.pointA.x == 0)
			&&
			(borderLinesBegin->second.pointB.x == 0)
		)
		{
			std::cout << "(PosYFaceResolver): found a ZDimLine, at NEG_X" << std::endl;
			singleDimLines[borderLineID].reset(new ZDimLine);
			singleDimLines[borderLineID]->setOneDimLimit(dimensionalLimit);
			singleDimLines[borderLineID]->setStaticDims(0.0f, yLocation);	// X would be 0 (from negative X), and Y (yLocation) would be (1, 4, or 32) 
		}

		// check for buidling ZDimLine at POS_X
		else if
		(
			(borderLinesBegin->second.pointA.x == dimensionalLimit)
			&&
			(borderLinesBegin->second.pointB.x == dimensionalLimit)
		)
		{
			std::cout << "(PosYFaceResolver): found a ZDimLine, at POS_X" << std::endl;
			singleDimLines[borderLineID].reset(new ZDimLine);
			singleDimLines[borderLineID]->setOneDimLimit(dimensionalLimit);
			singleDimLines[borderLineID]->setStaticDims(dimensionalLimit, yLocation);	// X and Y would both be 1, 4 or 32.
		}

		// check for building XDimLine at NEG_Z
		else if
		(
			(borderLinesBegin->second.pointA.z == 0)
			&&
			(borderLinesBegin->second.pointB.z == 0)
		)
		{
			std::cout << "(PosYFaceResolver): found a XDimLine, at NEG_Z" << std::endl;
			singleDimLines[borderLineID].reset(new XDimLine);
			singleDimLines[borderLineID]->setOneDimLimit(dimensionalLimit);
			singleDimLines[borderLineID]->setStaticDims(yLocation, 0.0f);	// Y would be (1, 4, or 32), and Z would be 0 (from negative Z)
		}

		// check for building XDimLine at POS_Z
		else if
		(
			(borderLinesBegin->second.pointA.z == dimensionalLimit)
			&&
			(borderLinesBegin->second.pointB.z == dimensionalLimit)
		)
		{
			std::cout << "(PosYFaceResolver): found a XDimLine, at POS_Z" << std::endl;
			singleDimLines[borderLineID].reset(new XDimLine);
			singleDimLines[borderLineID]->setOneDimLimit(dimensionalLimit);
			singleDimLines[borderLineID]->setStaticDims(yLocation, dimensionalLimit);	// Y and Z of the XDimLine would be 1, 4, or 32
		}
	}
}

void PosYFaceResolver::runResolutionAlgorithm()
{
	// logic for determining resolution would go here; return a malformed version just for testing.
	bool wasResolutionFound = attemptSolveByInvalidCount();
	if (wasResolutionFound == false)
	{
		produceMalformedMitigation();
	}
}

bool PosYFaceResolver::attemptSolveByInvalidCount()
{
	bool wasResolved = false;
	int numberOfInvalids = int(invalidsCopy.sequenceMap.size());
	if (numberOfInvalids == 1)
	{
		std::cout << "(PosYFaceResolver): found 1 invalid CleaveSequence. " << std::endl;
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
			std::cout << "(PosYFaceResolver): Fracturing successful." << std::endl;
			wasResolved = true;

			SPolySupergroup resolvedGroup = fracturer.sPolySG;
			//resolvedGroup.printSPolys();

			// set the resolution to be the result of the fracturer, as long as that was valid
			if (fracturer.getFractureValidity())
			{
				std::cout << "(PosYFaceResolver): fracturing was valid, setting this as the resolution." << std::endl;
				resolution = std::move(fracturer.sPolySG);
			}
		}
	}
	else if (numberOfInvalids == 2)
	{
		std::cout << "(PosYFaceResolver): found 2 invalid CleaveSequences. " << std::endl;

	}

	return wasResolved;
}

void PosYFaceResolver::produceMalformedMitigation()
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
}