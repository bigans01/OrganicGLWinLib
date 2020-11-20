#include "stdafx.h"
#include "IntersectionLineGroup.h"

void IntersectionLineGroup::addIntersectionLine(IntersectionLine in_intersectionLine)
{
	lineMap[numberOfIntersectionLines++] = in_intersectionLine;
	//std::cout << "Intersection line border value is: " << in_intersectionLine.numberOfBorderLines << std::endl;
}

bool IntersectionLineGroup::performInvalidCheck()
{
	bool isInvalidPossible = false;
	if (lineMap.size() == 2)
	{
		std::cout << "####~~~~~~~~ Line map size is 2 (performInvalidCheck); printing lines: " << std::endl;
		for (int x = 0; x < 2; x++)
		{
			std::cout << "[" << x << "], point A: " << lineMap[x].pointA.x << ", " << lineMap[x].pointA.y << ", " << lineMap[x].pointA.z
				<< " | point B: " << lineMap[x].pointB.x << ", " << lineMap[x].pointB.y << ", " << lineMap[x].pointB.z << std::endl;
		}

		//if (lineMap[0].pointB == lineMap[0].pointA)
		if (lineMap[0].pointA == lineMap[1].pointA)
		{
			std::cout << "NOTE! invalid is possible..." << std::endl;
			isInvalidPossible = true;
		}

	}
	return isInvalidPossible;
}

IntersectionLine IntersectionLineGroup::mergeLines()
{
	IntersectionLine returnLine;
	int totalNumberOfLines = lineMap.size();										// get the total number of lines
	int totalNumberOfBorderLineIntercepts = findNumberOfBorderLineIntercepts();		// get the total number of intercepts

	bool invalidPossibilityFlag = performInvalidCheck();

	std::cout << "! total number of lines: " << totalNumberOfLines << std::endl;
	std::cout << "! total number of border line intercepts: " << totalNumberOfBorderLineIntercepts << std::endl;

	//std::cout << "! Line 0 is: " << std::endl;
	//std::cout << "## Point A: " << lineMap[0].pointA.x << ", " << lineMap[0].pointA.y << ", " << lineMap[0].pointA.z << std::endl;
	//std::cout << "## Point B: " << lineMap[0].pointB.x << ", " << lineMap[0].pointB.y << ", " << lineMap[0].pointB.z << std::endl;

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

		//std::cout << "+++++ Line 0: " << std::endl;
		//std::cout << "point A: " << lineMap[0].pointA.x << ", " << lineMap[0].pointA.y << ", " << lineMap[0].pointA.z << std::endl;
		//std::cout << "point B: " << lineMap[0].pointB.x << ", " << lineMap[0].pointB.y << ", " << lineMap[0].pointB.z << std::endl;

		//std::cout << "+++++ Line 1: " << std::endl;
		//std::cout << "point A: " << lineMap[1].pointA.x << ", " << lineMap[1].pointA.y << ", " << lineMap[1].pointA.z << std::endl;
		//std::cout << "point B: " << lineMap[1].pointB.x << ", " << lineMap[1].pointB.y << ", " << lineMap[1].pointB.z << std::endl;

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
		//std::cout << ":: (CASE 2-A) point A of this one Line is: " << lineMap[0].pointA.x << ", " << lineMap[0].pointA.y << ", " << lineMap[0].pointA.z << std::endl;
		//std::cout << ":: (CASE 2-A) point B of this one Line is: " << lineMap[0].pointB.x << ", " << lineMap[0].pointB.y << ", " << lineMap[0].pointB.z << std::endl;
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

		//std::cout << "Non-border line points: A-> " << nonBorderLine->pointA.x << ", " << nonBorderLine->pointA.y << ", " << nonBorderLine->pointA.z << std::endl;
		//std::cout << "Non-border line points: B-> " << nonBorderLine->pointB.x << ", " << nonBorderLine->pointB.y << ", " << nonBorderLine->pointB.z << std::endl;

		//std::cout << "Border line points:     A-> " << borderLine->pointA.x << ", " << borderLine->pointA.y << ", " << borderLine->pointA.z << std::endl;
		//std::cout << "Border line points:     B-> " << borderLine->pointB.x << ", " << borderLine->pointB.y << ", " << borderLine->pointB.z << std::endl;

		//std::cout << "CASE 2-B HIT. " << std::endl;
	}
	else if
		(totalNumberOfLines == 1)
	{
		//std::cout << "WHOA!! ONLY ONE!! " << std::endl;
		//std::cout << ":: point A of this one Line is: " << lineMap[0].pointA.x << ", " << lineMap[0].pointA.y << ", " << lineMap[0].pointA.z << std::endl;
		//std::cout << ":: point B of this one Line is: " << lineMap[0].pointB.x << ", " << lineMap[0].pointB.y << ", " << lineMap[0].pointB.z << std::endl;
		returnLine.pointA = lineMap[0].pointA;
		returnLine.pointABorder = lineMap[0].pointABorder;
		returnLine.numberOfBorderLines = 0;
		returnLine.intersectedSecondaryID = lineMap[0].intersectedSecondaryID;
		returnLine.numberOfPoints = 1;
		//std::cout << "( " << returnLine.pointA.x << ", " << returnLine.pointA.y << ", " << returnLine.pointA.z << ") " << std::endl;
		//std::cout << "!!#!! border is: " << returnLine.pointABorder << std::endl;
	}
	else
	{
		//std::cout << "!!!! WARNING, No if/else case found!!! " << std::endl;
		//std::cout << "Total number of lines: " << totalNumberOfLines << std::endl;
		//std::cout << "Total number of intercepts: " << totalNumberOfBorderLineIntercepts << std::endl;
	}

	//std::cout << "#### Return line stats ---> " << std::endl;
	//std::cout << returnLine.pointA.x << ", " << returnLine.pointA.y << ", " << returnLine.pointA.z << std::endl;
	//std::cout << returnLine.pointB.x << ", " << returnLine.pointB.y << ", " << returnLine.pointB.z << std::endl;

	if 
	(
		(returnLine.pointA == returnLine.pointB)

		&&

		//!((returnLine.pointA.x == 0) && (returnLine.pointA.y == 0) && (returnLine.pointA.z == 0))
		(totalNumberOfLines > 0)

		&&
		(invalidPossibilityFlag == true)

		//&&

		//!((returnLine.pointB.x == 0) && (returnLine.pointB.y == 0) && (returnLine.pointB.z == 0))
	)
	{
		int someVal = 3;
		std::cout << "++++++++++ point check halting, points are same! " << std::endl;
		//std::cout << returnLine.pointA.x << ", " << returnLine.pointA.y << ", " << returnLine.pointA.z << std::endl;
		//std::cout << returnLine.pointB.x << ", " << returnLine.pointB.y << ", " << returnLine.pointB.z << std::endl;
		//std::cin >> someVal;
		returnLine.lineValidity = IntersectionLineValidity::INVALID;	// flag it as invalid.
	}
	if (totalNumberOfLines == 0)
	{
		//std::cout << "++++++++++ invalid, due to 0 lines! " << std::endl;
		//returnLine.lineValidity = IntersectionLineValidity::INVALID;	// flag it as invalid.
	}

	std::cout << "#### Return line stats ---> " << std::endl;
	std::cout << returnLine.pointA.x << ", " << returnLine.pointA.y << ", " << returnLine.pointA.z << std::endl;
	std::cout << returnLine.pointB.x << ", " << returnLine.pointB.y << ", " << returnLine.pointB.z << std::endl;
	if (returnLine.lineValidity == IntersectionLineValidity::VALID)
	{
		std::cout << "#### Line is VALID" << std::endl;
	}
	else
	{
		std::cout << "#### Line is INVALID" << std::endl;
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