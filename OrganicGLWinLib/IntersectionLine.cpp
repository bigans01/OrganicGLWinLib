#include "stdafx.h"
#include "IntersectionLine.h"

void IntersectionLine::addIntersectionResult(IntersectionResult in_result)
{
	//std::cout << "!!!! Add intersection result, point A: " << in_result.intersectedPoint.x << ", " << in_result.intersectedPoint.y << ", " << in_result.intersectedPoint.z << std::endl;

	int currentNoOfPoints = numberOfPoints;
	if (currentNoOfPoints == 0)		// insert into point A
	{
		pointA = in_result.intersectedPoint;
		isPointAOnBorder = in_result.wasIntersectOnBorderLine;
		if (isPointAOnBorder == 1)	// if this intersect was caused by a border line, increment the value appropriately.
		{
			numberOfBorderLines++;
		}
		pointABorder = in_result.borderLineID;
		numberOfPoints++;
	}

	if (currentNoOfPoints == 1)		// insert into point B
	{
		pointB = in_result.intersectedPoint;
		isPointBOnBorder = in_result.wasIntersectOnBorderLine;
		if (isPointBOnBorder == 1) // if this intersect was caused by a border line, increment the value appropriately.
		{
			numberOfBorderLines++;
		}
		pointBBorder = in_result.borderLineID;
		numberOfPoints++;
	}
}

void IntersectionLine::swapBorderToA()
{
	if (isPointBOnBorder == 1)	// only swap borders if B is the one that has a border
	{
		glm::vec3 tempPointA = pointA;				// save values of A, since it's being overwrriten
		int tempPointAOnBorder = isPointAOnBorder;
		int tempPointABorder = pointABorder;

		pointA = pointB;							// overwrite A with B
		isPointAOnBorder = isPointBOnBorder;
		pointABorder = pointBBorder;

		pointB = tempPointA;
		isPointBOnBorder = tempPointAOnBorder;
		pointBBorder = tempPointABorder;
	}
}

void IntersectionLine::swapBorderToB()
{
	if (isPointAOnBorder == 1)
	{
		glm::vec3 tempPointB = pointB;
		int tempPointBOnBorder = isPointBOnBorder;
		int tempPointBBorder = pointBBorder;

		pointB = pointA;
		isPointBOnBorder = isPointAOnBorder;
		pointBBorder = pointABorder;

		pointA = tempPointB;
		isPointAOnBorder = tempPointBOnBorder;
		pointABorder = tempPointBBorder;
	}
}

void IntersectionLine::swapToA()
{
	std::cout << ":::>>> Swapping to A...." << std::endl;

	glm::vec3 tempPointA = pointA;				// save values of A, since it's being overwrriten
	int tempPointAOnBorder = isPointAOnBorder;
	int tempPointABorder = pointABorder;

	pointA = pointB;							// overwrite A with B
	isPointAOnBorder = isPointBOnBorder;
	pointABorder = pointBBorder;

	pointB = tempPointA;
	isPointBOnBorder = tempPointAOnBorder;
	pointBBorder = tempPointABorder;

	wasSwapped = true;
}

int IntersectionLine::getBorderLineIDFromSingularBorderLineCount()
{
	int result = 0;		// may or may not be 0 in the end, but starts as 0 
	if (isPointAOnBorder == 1)
	{
		result = pointABorder;
	}
	else if (isPointBOnBorder == 1)
	{
		//std::cout << "!!! Point B is on border! " << std::endl;
		result = pointBBorder;
		//std::cout << "!!! Result will be: " << result << std::endl;
	}
	return result;
}

BorderLineIDPair IntersectionLine::getBorderLineIDPair()
{
	BorderLineIDPair pair(pointABorder, pointBBorder);
	return pair;
}

BorderLinePointPair IntersectionLine::getBorderLinePointPair()
{
	// remember, point A in pointPair should always be the point that hits the border line.

	BorderLinePointPair pointPair;
	if (numberOfPoints < 2)				
	{
		if (isPointAOnBorder == 1)
		{
			pointPair.pointA = pointA;
			pointPair.pointB = pointB;
		}
		else if (isPointBOnBorder == 1)
		{
			pointPair.pointA = pointB;
			pointPair.pointB = pointA;
		}
	}
	else if (numberOfPoints == 2)			// for when one intersection line hits two different border lines (a straight line, for example)
	{
		pointPair.pointA = pointA;
		pointPair.pointB = pointB;
	}
	return pointPair;
}

glm::vec3 IntersectionLine::getBorderPointFromSingularBorderLineCount()
{
	glm::vec3 borderPoint;
	if (isPointAOnBorder == 1)
	{
		borderPoint = pointA;
	}
	else if (isPointBOnBorder == 1)
	{
		borderPoint = pointB;
	}
	return borderPoint;
}

glm::vec3 IntersectionLine::getNonBorderPointFromSingularBorderLineCount()
{
	glm::vec3 nonBorderPoint;
	if (isPointAOnBorder == 1)
	{
		nonBorderPoint = pointB;
	}
	else if (isPointBOnBorder == 1)
	{
		nonBorderPoint = pointA;
	}
	return nonBorderPoint;
}

glm::vec3 IntersectionLine::fetchNextPointBasedOnCyclingDirection(CyclingDirection in_direction)
{
	/*
	glm::vec3 returnVec;
	if (in_direction == CyclingDirection::FORWARD)
	{
		std::cout << ">>> Fetchin point for CyclingDirection::FORWARD" << std::endl;
		glm::vec3 returnPoint;
		if (wasSwapped == true)
		{
			//returnPoint = pointA;
			returnPoint = pointB;
		}
		else
		{
			returnPoint = pointB;
			//returnPoint = pointA;
		}

		return returnPoint;
	}
	else if (in_direction == CyclingDirection::REVERSE)
	{
		std::cout << ">>> Fetchin point for CyclingDirection::REVERSE" << std::endl;

		glm::vec3 returnPoint;
		if (wasSwapped == true)
		{
			returnPoint = pointB;
		}
		else
		{
			//returnPoint = pointA;
			returnPoint = pointB;
		}

		return returnPoint;
	}
	*/
	return pointB;
}