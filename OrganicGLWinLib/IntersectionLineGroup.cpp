#include "stdafx.h"
#include "IntersectionLineGroup.h"

IntersectionLineGroup::IntersectionLineGroup(PolyDebugLevel in_polyDebugLevel)
{
	lineGroupLogger.setDebugLevel(in_polyDebugLevel);
}

void IntersectionLineGroup::setFusionAnalyzerSPolyRef(SPoly* in_sPolyRef)
{
	returnLine.completedAnalysis.setSPolyRef(in_sPolyRef);
}

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
		//std::cout << "####~~~~~~~~ Line map size is 2 (performInvalidCheck); printing lines: " << std::endl;
		for (int x = 0; x < 2; x++)
		{
			//std::cout << "[" << x << "], point A: " << lineMap[x].pointA.x << ", " << lineMap[x].pointA.y << ", " << lineMap[x].pointA.z
			//	<< " | point B: " << lineMap[x].pointB.x << ", " << lineMap[x].pointB.y << ", " << lineMap[x].pointB.z << std::endl;
		}

		//if (lineMap[0].pointB == lineMap[0].pointA)
		if (lineMap[0].pointA == lineMap[1].pointA)
		{
			//std::cout << "NOTE! invalid is possible..." << std::endl;
			isInvalidPossible = true;
		}

	}
	return isInvalidPossible;
}

FusedIntersectionLine IntersectionLineGroup::mergeLines()
{
	//FusedIntersectionLine returnLine;
	returnLine.completedAnalysis.setMapRefAndRunAnalysis(&lineMap);
	int totalNumberOfLines = lineMap.size();										// get the total number of lines
	int totalNumberOfBorderLineIntercepts = findNumberOfBorderLineIntercepts();		// get the total number of intercepts

	bool invalidPossibilityFlag = performInvalidCheck();

	std::cout << "! total number of lines: " << totalNumberOfLines << std::endl;
	std::cout << "! total number of border line intercepts: " << totalNumberOfBorderLineIntercepts << std::endl;
	std::cout << "! Line 0 is: " << std::endl;
	std::cout << "## Point A: " << lineMap[0].pointA.x << ", " << lineMap[0].pointA.y << ", " << lineMap[0].pointA.z << std::endl;
	std::cout << "## Point B: " << lineMap[0].pointB.x << ", " << lineMap[0].pointB.y << ", " << lineMap[0].pointB.z << std::endl;

	//lineGroupLogger.log("! total number of lines: ", totalNumberOfLines, "\n");
	//lineGroupLogger.log("! total number of border line intercepts: ", totalNumberOfBorderLineIntercepts, "\n");
	//lineGroupLogger.log("! Line 0 is: ", "\n");
	//lineGroupLogger.log("## Point A: ", lineMap[0].pointA.x, ", ", lineMap[0].pointA.y, ", ", lineMap[0].pointA.z, "\n");
	//lineGroupLogger.log("## Point B: ", lineMap[0].pointB.x, ", ", lineMap[0].pointB.y, ", ", lineMap[0].pointB.z, "\n");

	if (lineMap.size() == 3)	// three lines requires special logic checks.
	{
		IntersectionLine lineArray[3];	// there should only be 3 lines possible to work with.
		int lineArrayIndex = 0;
		glm::vec3 acquiredPointA;
		glm::vec3 acquiredPointB;

		auto linesBegin = lineMap.begin();
		auto linesEnd = lineMap.end();
		for (; linesBegin != linesEnd; linesBegin++)
		{
			std::cout << "Line " << linesBegin->first << ", point A: " << linesBegin->second.pointA.x << ", " << linesBegin->second.pointA.y << ", " << linesBegin->second.pointA.z
				<< " | point B: " << linesBegin->second.pointB.x << ", " << linesBegin->second.pointB.y << ", " << linesBegin->second.pointB.z 
				<< " | is A border: " << linesBegin->second.isPointAOnBorder 
				<< " | A border valeu: " << linesBegin->second.pointABorder << std::endl;
			//lineGroupLogger.log("Line ", linesBegin->first, ", point A: ", linesBegin->second.pointA.x, ", ", linesBegin->second.pointA.y, ", ", linesBegin->second.pointA.z,
			//	" | point B: ", linesBegin->second.pointB.x, ", ", linesBegin->second.pointB.y, ", ", linesBegin->second.pointB.z,
			//	" | is A border: ", linesBegin->second.isPointAOnBorder,
			//	" | A border value: ", linesBegin->second.pointABorder, "\n");


			if (linesBegin->second.numberOfBorderLines == 1)
			{
				//std::cout << "!! Inserting into array, the line at " << linesBegin->first << ". " << std::endl;
				lineGroupLogger.log("!! Inserting into array, the line at ", linesBegin->first, ". ", "\n");
				//lineArray[lineArrayIndex++] = linesBegin->second;
				acquiredPointA = linesBegin->second.pointA;

			}
			else if (linesBegin->second.numberOfBorderLines == 0)
			{
				acquiredPointB = linesBegin->second.pointA;
			}
		}

		returnLine.deprecatedLine.numberOfBorderLines = 1;			// when there are 2 lines in the map that are border lines, set this to 1. When there are 3, set this to 2.
		returnLine.deprecatedLine.numberOfPoints = 2;
		returnLine.deprecatedLine.pointA = acquiredPointA;
		//returnLine.deprecatedLine.isPointAOnBorder = 1;
		//returnLine.deprecatedLine.pointABorder = lineArray[0].pointABorder;

		returnLine.deprecatedLine.pointB = acquiredPointB;
		//returnLine.deprecatedLine.isPointBOnBorder = 1;
		//returnLine.deprecatedLine.pointBBorder = lineArray[1].pointABorder;

		
		std::cout << "#### Return line stats ---> " << std::endl;
		std::cout << returnLine.deprecatedLine.pointA.x << ", " << returnLine.deprecatedLine.pointA.y << ", " << returnLine.deprecatedLine.pointA.z << std::endl;
		std::cout << returnLine.deprecatedLine.pointB.x << ", " << returnLine.deprecatedLine.pointB.y << ", " << returnLine.deprecatedLine.pointB.z << std::endl;
		std::cout << "#### Triple line branch complete, continue? " << std::endl;
		std::cout << "#### Return line Number of border lines: --> " << returnLine.deprecatedLine.numberOfBorderLines << std::endl;
		int someVal = 3;
		std::cin >> someVal;
		

		//lineGroupLogger.log("#### Return line stats ---> ", "\n");
		//lineGroupLogger.log(returnLine.deprecatedLine.pointA.x, ", ", returnLine.deprecatedLine.pointA.y, ", ", returnLine.deprecatedLine.pointA.z, "\n");
		//lineGroupLogger.log(returnLine.deprecatedLine.pointB.x, ", ", returnLine.deprecatedLine.pointB.y, ", ", returnLine.deprecatedLine.pointB.z, "\n");
		//lineGroupLogger.waitForDebugInput();

		
		
	}
	else   // as long as there aren't 3 lines in the map, do this.
	{
		// CASE 1: check for "A_SLICE" or "B_SLICE" types
		if
			(
			(totalNumberOfLines == 2)
				&&
				(totalNumberOfBorderLineIntercepts == 2)
				)
		{
			returnLine.deprecatedLine.pointA = lineMap[0].pointA;
			returnLine.deprecatedLine.isPointAOnBorder = 1;
			returnLine.deprecatedLine.pointABorder = lineMap[0].pointABorder;

			returnLine.deprecatedLine.pointB = lineMap[1].pointA;
			returnLine.deprecatedLine.isPointBOnBorder = 1;
			returnLine.deprecatedLine.pointBBorder = lineMap[1].pointABorder;

			returnLine.deprecatedLine.numberOfBorderLines = 2;		// this will be equal to 2
			//returnLine.deprecatedLine.intersectedSecondaryID = lineMap[0].intersectedSecondaryID;	// we can get the intersected secondary from either line
			returnLine.deprecatedLine.numberOfPoints = 2;

			//returnLine.deprecatedLine.
			/*
			std::cout << "!!! Merged 2 lines into a SLICE! " << std::endl;

			std::cout << "+++++ Line 0: " << std::endl;
			std::cout << "point A: " << lineMap[0].pointA.x << ", " << lineMap[0].pointA.y << ", " << lineMap[0].pointA.z << std::endl;
			std::cout << "point B: " << lineMap[0].pointB.x << ", " << lineMap[0].pointB.y << ", " << lineMap[0].pointB.z << std::endl;

			std::cout << "+++++ Line 1: " << std::endl;
			std::cout << "point A: " << lineMap[1].pointA.x << ", " << lineMap[1].pointA.y << ", " << lineMap[1].pointA.z << std::endl;
			std::cout << "point B: " << lineMap[1].pointB.x << ", " << lineMap[1].pointB.y << ", " << lineMap[1].pointB.z << std::endl;

			std::cout << "CASE 1 HIT. " << std::endl;
			int someVal = 3;
			std::cin >> someVal;
			*/
		}

		// CASE 2: check for "PARTIAL_BOUND" types
		else if
			(
			(totalNumberOfLines == 1)
				&&
				(totalNumberOfBorderLineIntercepts == 1)
				)
		{
			returnLine.deprecatedLine.pointA = lineMap[0].pointA;
			returnLine.deprecatedLine.isPointAOnBorder = 1;
			returnLine.deprecatedLine.pointABorder = lineMap[0].pointABorder;

			returnLine.deprecatedLine.numberOfBorderLines = 1;
			//returnLine.deprecatedLine.intersectedSecondaryID = lineMap[0].intersectedSecondaryID;
			returnLine.deprecatedLine.numberOfPoints = 1;
			//std::cout << "CASE 2-A HIT. " << std::endl;
			//std::cout << ":: (CASE 2-A) point A of this one Line is: " << lineMap[0].pointA.x << ", " << lineMap[0].pointA.y << ", " << lineMap[0].pointA.z << std::endl;
			//std::cout << ":: (CASE 2-A) point B of this one Line is: " << lineMap[0].pointB.x << ", " << lineMap[0].pointB.y << ", " << lineMap[0].pointB.z << std::endl;
			//std::cout << "Return line border value: " << returnLine.deprecatedLine.pointABorder << std::endl;
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
			returnLine.deprecatedLine.pointA = borderLine->pointA;
			returnLine.deprecatedLine.pointB = nonBorderLine->pointA;
			returnLine.deprecatedLine.isPointAOnBorder = 1;
			returnLine.deprecatedLine.pointABorder = borderLine->pointABorder;
			returnLine.deprecatedLine.numberOfBorderLines = 1;
			//returnLine.deprecatedLine.intersectedSecondaryID = borderLine->intersectedSecondaryID;
			returnLine.deprecatedLine.numberOfPoints = 2;

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
			returnLine.deprecatedLine.pointA = lineMap[0].pointA;
			returnLine.deprecatedLine.pointABorder = lineMap[0].pointABorder;
			returnLine.deprecatedLine.numberOfBorderLines = 0;
			//returnLine.deprecatedLine.intersectedSecondaryID = lineMap[0].intersectedSecondaryID;
			returnLine.deprecatedLine.numberOfPoints = 1;
			//std::cout << "( " << returnLine.deprecatedLine.pointA.x << ", " << returnLine.deprecatedLine.pointA.y << ", " << returnLine.deprecatedLine.pointA.z << ") " << std::endl;
			//std::cout << "!!#!! border is: " << returnLine.deprecatedLine.pointABorder << std::endl;
		}
		else
		{
			//std::cout << "!!!! WARNING, No if/else case found!!! " << std::endl;
			//std::cout << "Total number of lines: " << totalNumberOfLines << std::endl;
			//std::cout << "Total number of intercepts: " << totalNumberOfBorderLineIntercepts << std::endl;
		}

		//std::cout << "#### Return line stats ---> " << std::endl;
		//std::cout << returnLine.deprecatedLine.pointA.x << ", " << returnLine.deprecatedLine.pointA.y << ", " << returnLine.deprecatedLine.pointA.z << std::endl;
		//std::cout << returnLine.deprecatedLine.pointB.x << ", " << returnLine.deprecatedLine.pointB.y << ", " << returnLine.deprecatedLine.pointB.z << std::endl;

		if
			(
			(returnLine.deprecatedLine.pointA == returnLine.deprecatedLine.pointB)

				&&

				//!((returnLine.deprecatedLine.pointA.x == 0) && (returnLine.deprecatedLine.pointA.y == 0) && (returnLine.deprecatedLine.pointA.z == 0))
				(totalNumberOfLines > 0)

				&&
				(invalidPossibilityFlag == true)

				//&&

				//!((returnLine.deprecatedLine.pointB.x == 0) && (returnLine.deprecatedLine.pointB.y == 0) && (returnLine.deprecatedLine.pointB.z == 0))
				)
		{
			int someVal = 3;
			//std::cout << "++++++++++ point check halting, points are same! " << std::endl;
			//std::cout << returnLine.deprecatedLine.pointA.x << ", " << returnLine.deprecatedLine.pointA.y << ", " << returnLine.deprecatedLine.pointA.z << std::endl;
			//std::cout << returnLine.deprecatedLine.pointB.x << ", " << returnLine.deprecatedLine.pointB.y << ", " << returnLine.deprecatedLine.pointB.z << std::endl;
			//std::cin >> someVal;
			returnLine.deprecatedLine.lineValidity = IntersectionLineValidity::INVALID;	// flag it as invalid.
		}
		if (totalNumberOfLines == 0)
		{
			//std::cout << "++++++++++ invalid, due to 0 lines! " << std::endl;
			//returnLine.deprecatedLine.lineValidity = IntersectionLineValidity::INVALID;	// flag it as invalid.
		}

		//std::cout << "#### Return line stats ---> " << std::endl;
		//std::cout << returnLine.deprecatedLine.pointA.x << ", " << returnLine.deprecatedLine.pointA.y << ", " << returnLine.deprecatedLine.pointA.z << std::endl;
		//std::cout << returnLine.deprecatedLine.pointB.x << ", " << returnLine.deprecatedLine.pointB.y << ", " << returnLine.deprecatedLine.pointB.z << std::endl;
		if (returnLine.deprecatedLine.lineValidity == IntersectionLineValidity::VALID)
		{
			//std::cout << "#### Line is VALID" << std::endl;
		}
		else
		{
			//std::cout << "#### Line is INVALID" << std::endl;
		}
	}


	//std::cout << "#### (2) Return line Number of border lines: --> " << returnLine.deprecatedLine.numberOfBorderLines << std::endl;
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

void IntersectionLineGroup::insertFusionCandidateIntoAnalyzer(int in_lineIndex, FusionCandidate in_fusionCandidate, IntersectionResult in_intersectResult)
{
	returnLine.completedAnalysis.insertFusionCandidate(in_lineIndex, in_fusionCandidate, in_intersectResult);
}

void IntersectionLineGroup::reset()
{
	//FusedIntersectionLine newLineReplacement;
	//returnLine = newLineReplacement;
	IntersectionLine blankIntersectionLine;
	returnLine.deprecatedLine = blankIntersectionLine;
	numberOfIntersectionLines = 0;		// reset the number of intersection lines
	lineMap.clear();					// clear the map
	returnLine.completedAnalysis.clearCandidateData();
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