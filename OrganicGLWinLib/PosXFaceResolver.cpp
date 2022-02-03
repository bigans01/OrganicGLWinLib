#include "stdafx.h"
#include "PosXFaceResolver.h"

void PosXFaceResolver::setupBorderLineRangesAndDimLoc()
{
	std::cout << "(PosXFaceResolver): building one dimensional lines..." << std::endl;
	xLocation = dimensionalLimit;	// since we're at POS_X, this value would be 1, 4 or 32

	// There will be exactly two "Y" and two "Z" instances of OneDimLine, for POS_X.
	// Scan the border lines to determine the appropriate OneDimLine
	auto borderLinesBegin = sPolyPtr->borderLines.begin();
	auto borderLinesEnd = sPolyPtr->borderLines.end();
	for (; borderLinesBegin != borderLinesEnd; borderLinesBegin++)
	{
		// check for neg Y
		if
		(
			(borderLinesBegin->second.pointA.y == 0)
			&&
			(borderLinesBegin->second.pointB.y == 0)
		)
		{
			std::cout << "(PosXFaceResolver): found NEG_Y OneDimLine." << std::endl;
		}

		// check for pos Y
		else if
		(
			(borderLinesBegin->second.pointA.y == dimensionalLimit)
			&&
			(borderLinesBegin->second.pointB.y == dimensionalLimit)
		)
		{
			std::cout << "(PosXFaceResolver): found POS_Y OneDimLine." << std::endl;
		}

		// check for neg Z
		else if
		(
			(borderLinesBegin->second.pointA.z == 0)
			&&
			(borderLinesBegin->second.pointB.z == 0)
		)
		{
			std::cout << "(PosXFaceResolver): found NEG_Z OneDimLine." << std::endl;
		}

		// check for pos Z
		else if
		(
			(borderLinesBegin->second.pointA.z == dimensionalLimit)
			&&
			(borderLinesBegin->second.pointB.z == dimensionalLimit)
		)
		{
			std::cout << "(PosXFaceResolver): found POS_Z OneDimLine." << std::endl;
		}
	}
}