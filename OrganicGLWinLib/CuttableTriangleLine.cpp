#include "stdafx.h"
#include "CuttableTriangleLine.h"

void CuttableTriangleLine::printCuttingIntersections()
{
	if (cuttableIntersectionManager.numberOfRecords() > 0)
	{
		cuttableIntersectionManager.printRecordMap();
	}
}

void CuttableTriangleLine::insertNonIntersectingCuttingLinePoint(int in_cuttingLineID, glm::vec3 in_point)
{
	cuttingLineNonIntersectingPoints[in_cuttingLineID] = in_point;
}


glm::vec3 CuttableTriangleLine::fetchNextPointBasedOnCyclingDirection(CyclingDirection in_cyclingDirection)
{
	if (in_cyclingDirection == CyclingDirection::FORWARD)
	{
		return pointB;
	}
	else if (in_cyclingDirection == CyclingDirection::REVERSE)
	{
		return pointA;
	}
}


glm::vec3 CuttableTriangleLine::getOtherEndpoint(glm::vec3 in_currentEndpoint)
{
	if (in_currentEndpoint == pointA)
	{
		return pointB;
	}
	else if (in_currentEndpoint == pointB)
	{
		return pointA;
	}
}

IntersectionFindingAttempt CuttableTriangleLine::getNonIntersectingPountForCuttingLine(int in_cuttingLineID, glm::vec3 in_currentLeadingPoint)
{
	//return cuttingLineNonIntersectingPoints[in_cuttingLineID];
	IntersectionFindingAttempt returnAttempt;
	auto doesIndexExist = cuttingLineNonIntersectingPoints.find(in_cuttingLineID);
	if (doesIndexExist != cuttingLineNonIntersectingPoints.end()) // it was found; return what we found.
	{
		//returnValue = doesIndexExist->second;
		returnAttempt.foundPoint = doesIndexExist->second;
		returnAttempt.wasFound = true;
	}
	else if (doesIndexExist == cuttingLineNonIntersectingPoints.end())
	{
		std::cout << "!!!!! Notice, couldn't find entry with that ID..." << std::endl;

		std::cout << "Current leading point is: " << in_currentLeadingPoint.x << ", " << in_currentLeadingPoint.y << ", " << in_currentLeadingPoint.z << std::endl;
		std::cout << "Point A is: " << pointA.x << ", " << pointA.y << ", " << pointA.z << std::endl;
		std::cout << "Point B is: " << pointB.x << ", " << pointB.y << ", " << pointB.z << std::endl;

		// since we didn't find the index, we are going to have to just grab the other endpoint of the CuttableTriangleLine.
		// this function assumes that the value of in_currentLeadingPoint is one of the endpoints of the CuttableTriangleLine.
		returnAttempt.foundPoint = getOtherEndpoint(in_currentLeadingPoint);
	}
	return returnAttempt;
}

glm::vec3 CuttableTriangleLine::getNonIntersectingPountForCuttingLine(int in_cuttingLineID)
{
	return cuttingLineNonIntersectingPoints[in_cuttingLineID];
}