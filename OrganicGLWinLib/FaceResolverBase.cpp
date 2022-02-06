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