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
			singleDimLines[borderLineID]->setOneDimLimit(xLocation);
			singleDimLines[borderLineID]->setStaticDims(xLocation, 0.0f);	// X would be (1, 4, or 32), and Y would be 0, since we
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
			singleDimLines[borderLineID]->setOneDimLimit(xLocation);
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
			singleDimLines[borderLineID]->setOneDimLimit(xLocation);
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
			singleDimLines[borderLineID]->setOneDimLimit(xLocation);
			singleDimLines[borderLineID]->setStaticDims(xLocation, dimensionalLimit);	// X would be whatever the xLocation is, and Z would be the limit.
		}
	}
}