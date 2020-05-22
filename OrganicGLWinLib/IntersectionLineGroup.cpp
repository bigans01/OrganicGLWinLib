#include "stdafx.h"
#include "IntersectionLineGroup.h"

void IntersectionLineGroup::addIntersectionLine(IntersectionLine in_intersectionLine)
{
	lineMap[numberOfIntersectionLines++] = in_intersectionLine;
	//std::cout << "Intersection line border value is: " << in_intersectionLine.numberOfBorderLines << std::endl;
}

IntersectionLine IntersectionLineGroup::mergeLines()
{
	IntersectionLine returnLine;
	int totalNumberOfLines = lineMap.size();										// get the total number of lines
	int totalNumberOfBorderLineIntercepts = findNumberOfBorderLineIntercepts();		// get the total number of intercepts

	//std::cout << "! total number of lines: " << totalNumberOfLines << std::endl;
	//std::cout << "! total number of border line intercepts: " << totalNumberOfBorderLineIntercepts << std::endl;

	// CASE 1: check for "A_SLICE" or "B_SLICE" types
	if
		(
		(totalNumberOfLines == 2)
			&&
			(totalNumberOfBorderLineIntercepts == 2)
			)
	{
		returnLine.pointA = lineMap[0].pointA;
		returnLine.isPointAOnBorder = 1;
		returnLine.pointABorder = lineMap[0].pointABorder;

		returnLine.pointB = lineMap[1].pointA;
		returnLine.isPointBOnBorder = 1;
		returnLine.pointBBorder = lineMap[1].pointABorder;

		returnLine.numberOfBorderLines = 2;		// this will be equal to 2
		returnLine.intersectedSecondaryID = lineMap[0].intersectedSecondaryID;	// we can get the intersected secondary from either line
		returnLine.numberOfPoints = 2;

		//returnLine.
		//std::cout << "!!! Merged 2 lines into a SLICE! " << std::endl;
		//std::cout << "CASE 1 HIT. " << std::endl;
	}

	// CASE 2: check for "PARTIAL_BOUND" types
	else if
		(
		(totalNumberOfLines == 1)
			&&
			(totalNumberOfBorderLineIntercepts == 1)
			)
	{
		returnLine.pointA = lineMap[0].pointA;
		returnLine.isPointAOnBorder = 1;
		returnLine.pointABorder = lineMap[0].pointABorder;

		returnLine.numberOfBorderLines = 1;
		returnLine.intersectedSecondaryID = lineMap[0].intersectedSecondaryID;
		returnLine.numberOfPoints = 1;
		//std::cout << "CASE 2-A HIT. " << std::endl;
		//std::cout << "Return line border value: " << returnLine.pointABorder << std::endl;
	}
	else if
		(
		(totalNumberOfLines == 2)
			&&
			(totalNumberOfBorderLineIntercepts == 1)
			)
	{
		// get the border line from the map, get it's point A
		IntersectionLine* borderLine = findSingleBorderLine();	// grab a reference to the found line
		IntersectionLine* nonBorderLine = findSingleNonBorderLine();
		returnLine.pointA = borderLine->pointA;
		returnLine.pointB = nonBorderLine->pointA;
		returnLine.isPointAOnBorder = 1;
		returnLine.pointABorder = borderLine->pointABorder;
		returnLine.numberOfBorderLines = 1;
		returnLine.intersectedSecondaryID = borderLine->intersectedSecondaryID;
		returnLine.numberOfPoints = 2;
		//std::cout << "CASE 2-B HIT. " << std::endl;
	}
	else if
		(totalNumberOfLines == 1)
	{
		//std::cout << "WHOA!! ONLY ONE!! " << std::endl;
		returnLine.pointA = lineMap[0].pointA;
		returnLine.pointABorder = lineMap[0].pointABorder;
		returnLine.numberOfBorderLines = 0;
		returnLine.intersectedSecondaryID = lineMap[0].intersectedSecondaryID;
		returnLine.numberOfPoints = 1;
		//std::cout << "( " << returnLine.pointA.x << ", " << returnLine.pointA.y << ", " << returnLine.pointA.z << ") " << std::endl;
		//std::cout << "!!#!! border is: " << returnLine.pointABorder << std::endl;
	}

	return returnLine;
}

int IntersectionLineGroup::findNumberOfBorderLineIntercepts()
{
	int numberOfIntercepts = 0;
	auto mapIter = lineMap.begin();
	auto mapEnd = lineMap.end();
	//std::cout << "Looping through " << lineMap.size() << " line(s)" << std::endl;
	for (mapIter; mapIter != mapEnd; mapIter++)
	{
		if (mapIter->second.numberOfBorderLines == 1)
		{
			numberOfIntercepts++;
		}
		//std::cout << "Line looped. " << std::endl;
	}
	//std::cout << "Num intercepts: " << numberOfIntercepts << std::endl;
	return numberOfIntercepts;
}

void IntersectionLineGroup::reset()
{
	numberOfIntersectionLines = 0;		// reset the number of intersection lines
	lineMap.clear();					// clear the map
}

IntersectionLine* IntersectionLineGroup::findSingleBorderLine()
{
	IntersectionLine* returnPtr = &lineMap[0];		// make the compiler happy
	auto mapIter = lineMap.begin();
	auto mapEnd = lineMap.end();
	for (mapIter; mapIter != mapEnd; mapIter++)
	{
		if (mapIter->second.numberOfBorderLines == 1)		// grab the one with the border line
		{
			returnPtr = &mapIter->second;
		}
	}
	return returnPtr;
}

IntersectionLine* IntersectionLineGroup::findSingleNonBorderLine()
{
	IntersectionLine* returnPtr = &lineMap[0];		// make the compiler happy
	auto mapIter = lineMap.begin();
	auto mapEnd = lineMap.end();
	for (mapIter; mapIter != mapEnd; mapIter++)
	{
		if (mapIter->second.numberOfBorderLines == 0)		// grab the one with no border line
		{
			returnPtr = &mapIter->second;
		}
	}
	return returnPtr;
}