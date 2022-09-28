#include "stdafx.h"
#include "PosZFaceResolver.h"

void PosZFaceResolver::setupBorderLineRangesAndDimLoc()
{
	std::cout << "(PosZFaceResolver): dimensionalLimit value is: " << dimensionalLimit << std::endl;
	std::cout << "(PosZFaceResolver): points of sPolyPtr are: " << std::endl;
	sPolyPtr->printPoints();

	zLocation = dimensionalLimit;	// since we're at POS_Z, this value would be 1, 4 or 32

	// There will be two "X" and two "Y" instances of OneDimLine, for POS_Z.
	auto borderLinesBegin = sPolyPtr->borderLines.begin();
	auto borderLinesEnd = sPolyPtr->borderLines.end();
	for (; borderLinesBegin != borderLinesEnd; borderLinesBegin++)
	{
		int borderLineID = borderLinesBegin->first;

		// check for building XDimLine at NEG_Y
		if
			(
			(borderLinesBegin->second.pointA.y == 0)
				&&
				(borderLinesBegin->second.pointB.y == 0)
				)
		{
			singleDimLines[borderLineID].reset(new XDimLine);
			singleDimLines[borderLineID]->setOneDimLimit(dimensionalLimit);
			singleDimLines[borderLineID]->setStaticDims(0.0f, zLocation);	// Y would be 0, since we are at the bottom of the limit; and Z would be (1, 4, or 32), 
		}

		// check for building XDimLine at POS_Y
		if
			(
			(borderLinesBegin->second.pointA.y == dimensionalLimit)
				&&
				(borderLinesBegin->second.pointB.y == dimensionalLimit)
				)
		{
			singleDimLines[borderLineID].reset(new XDimLine);
			singleDimLines[borderLineID]->setOneDimLimit(dimensionalLimit);
			singleDimLines[borderLineID]->setStaticDims(dimensionalLimit, zLocation);	// Y and Z would both be 1, 4 or 32.
		}

		// check for building YDimLine at NEG_X
		if
			(
			(borderLinesBegin->second.pointA.x == 0)
				&&
				(borderLinesBegin->second.pointB.x == 0)
				)
		{
			singleDimLines[borderLineID].reset(new YDimLine);
			singleDimLines[borderLineID]->setOneDimLimit(dimensionalLimit);
			singleDimLines[borderLineID]->setStaticDims(0.0f, zLocation);
		}

		// check for building YDimLine at POS_X
		if
			(
			(borderLinesBegin->second.pointA.x == dimensionalLimit)
				&&
				(borderLinesBegin->second.pointB.x == dimensionalLimit)
				)
		{
			singleDimLines[borderLineID].reset(new YDimLine);
			singleDimLines[borderLineID]->setOneDimLimit(dimensionalLimit);
			singleDimLines[borderLineID]->setStaticDims(dimensionalLimit, zLocation);
		}

	}
}

void PosZFaceResolver::runResolutionAlgorithm()
{
	// logic for determining resolution would go here; return a malformed version just for testing.
	bool wasResolutionFound = attemptSolveByInvalidCount();
	if (wasResolutionFound == false)
	{
		produceMalformedMitigation();
	}
}

bool PosZFaceResolver::attemptSolveByInvalidCount()
{
	bool wasResolved = false;
	int numberOfInvalids = int(invalidsCopy.sequenceMap.size());
	if (numberOfInvalids == 1)
	{
		std::cout << "(PosZFaceResolver): found 1 invalid CleaveSequence. " << std::endl;
		auto invalidSequenceID = invalidsCopy.sequenceMap.begin()->first;

		// be sure to determine the CyclingDirections for the sequence.
		invalidsCopy.sequenceMap[invalidSequenceID].printCategorizedLines();

		bool resolved = checkCleaveSequenceLinesAgainstDimLines(invalidSequenceID, &(invalidsCopy.sequenceMap[invalidSequenceID]));
		if (resolved == true)
		{
			SPolyMorphTracker morphTracker;

			// Remember, POS_Z means that the SPolyFracturer shouldn't have to do anything.
			SPolyFracturer fracturer(0, 
									sPolyPtr, 
									&morphTracker, 
									SPolyFracturerOptionEnum::NO_ROTATE_TO_Z, 
									PolyDebugLevel::NONE,
									resolverRecorderRef, 
									resolverBoundaryOrientation);
			std::cout << "(PosZFaceResolver): Fracturing successful." << std::endl;
			wasResolved = true;

			SPolySupergroup resolvedGroup = fracturer.sPolySG;
			//resolvedGroup.printSPolys();

			// set the resolution to be the result of the fracturer, as long as that was valid
			if (fracturer.getFractureValidity())
			{
				std::cout << "(PosZFaceResolver): fracturing was valid, setting this as the resolution." << std::endl;
				resolution = std::move(fracturer.sPolySG);
			}
		}
	}
	else if (numberOfInvalids == 2)
	{
		std::cout << "(PosZFaceResolver): found 2 invalid CleaveSequences. " << std::endl;

	}
	else if (numberOfInvalids == 0)
	{
		std::cout << "(PosZFaceResolver): Found no invalid CleaveSequences. " << std::endl;
	}
	return wasResolved;
}

void PosZFaceResolver::produceMalformedMitigation()
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

	std::cout << "(PosZFaceResolver): +++++++++++++ Printing out produced SPolys. " << std::endl;
	resolution.printSPolys();
}