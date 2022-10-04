#include "stdafx.h"
#include "NegZFaceResolver.h"

void NegZFaceResolver::setupBorderLineRangesAndDimLoc()
{
	// xLocation never gets modified, unlike POS_Z.
	// There will be two "X" and two "Y" instances of OneDimLine, for NEG_Z.
	// Scan the border lines to determine the appropriate OneDimLine

	std::cout << "(NegZFaceResolver): dimensionalLimit value is: " << dimensionalLimit << std::endl;

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

void NegZFaceResolver::runResolutionAlgorithm()
{
	// logic for determining resolution would go here; return a malformed version just for testing.
	bool wasResolutionFound = attemptSolveByInvalidCount();
	if (wasResolutionFound == false)
	{
		produceMalformedMitigation();
	}
}

bool NegZFaceResolver::attemptSolveByInvalidCount()
{
	bool wasResolved = false;
	int numberOfInvalids = int(invalidsCopy.sequenceMap.size());
	if (numberOfInvalids == 1)
	{
		std::cout << "(NegZFaceResolver): found 1 invalid CleaveSequence. " << std::endl;
		auto invalidSequenceID = invalidsCopy.sequenceMap.begin()->first;

		// be sure to determine the CyclingDirections for the sequence.
		invalidsCopy.sequenceMap[invalidSequenceID].printCategorizedLines();

		bool resolved = checkCleaveSequenceLinesAgainstDimLines(invalidSequenceID, &(invalidsCopy.sequenceMap[invalidSequenceID]));
		if (resolved == true)
		{
			SPolyMorphTracker morphTracker;

			// Remember, NEG_Z means that the SPolyFracturer shouldn't have to do anything.
			SPolyFracturer fracturer(0, 
									sPolyPtr, 
									&morphTracker, 
									SPolyFracturerOptionEnum::NO_ROTATE_TO_Z, 
									PolyDebugLevel::NONE,
									resolverRecorderRef, 
									resolverBoundaryOrientation);
			std::cout << "(NegZFaceResolver): Fracturing successful." << std::endl;
			wasResolved = true;

			SPolySupergroup resolvedGroup = fracturer.sPolySG;
			//resolvedGroup.printSPolys();

			// set the resolution to be the result of the fracturer, as long as that was valid
			if (fracturer.getFractureValidity())
			{
				std::cout << "(NegZFaceResolver): fracturing was valid, setting this as the resolution." << std::endl;
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
		std::cout << "(NegZFaceResolver): found 2 invalid CleaveSequences. " << std::endl;

	}
	else if (numberOfInvalids == 0)
	{
		std::cout << "(NegZFaceResolver): Found no invalid CleaveSequences. " << std::endl;
	}
	return wasResolved;
}

void NegZFaceResolver::produceMalformedMitigation()
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